#include "Users_sets.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <sstream>

#include "../db_connection/connect_db.h"
#include "Set.h"

void saveUsersSetToDb(int setId, int userId) {
  try {
    auto conn = connectToDatabase();
    pqxx::work txn(*conn);
    std::string insertUsersSet =
        "INSERT INTO users_sets (user_id, set_id) VALUES ($1, $2)";
    std::string insertUserFlashcard =
        "INSERT INTO user_flashcard (flashcard_id, users_sets_id) VALUES ($1, "
        "$2)";
    txn.exec_params(insertUsersSet, userId, setId);

    pqxx::result result = txn.exec("SELECT lastval()");
    int usersSetId = result[0][0].as<int>();
    std::vector<int> flashcardIds = getFlashcardIds(setId);

    for (auto flashcardId : flashcardIds) {
      txn.exec_params(insertUserFlashcard, flashcardId, usersSetId);
    }

    txn.commit();
    QMessageBox::information(nullptr, "Sukces",
                             "Pomyślnie zapisano zestaw do ulubionych.");
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void deleteUserSetFromDB(int setId, int userId) {
  try {
    auto conn = connectToDatabase();
    pqxx::work txn(*conn);
    std::string userSetIdSql =
        "SELECT id FROM users_sets WHERE set_id = $1 and user_id = $2";
    std::string deleteUserFlaschardSql =
        "DELETE FROM user_flashcard WHERE users_sets_id = $1";
    std::string deleteUserSetSql = "DELETE FROM users_sets WHERE id = $1";
    pqxx::result result = txn.exec_params(userSetIdSql, setId, userId);

    int usersSetId = result[0][0].as<int>();
    txn.exec_params(deleteUserFlaschardSql, usersSetId);

    txn.exec_params(deleteUserSetSql, usersSetId);

    txn.commit();
    QMessageBox::information(nullptr, "Sukces",
                             "Usunięto z obserwowanych zestawów.");
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

std::unique_ptr<Set> getUserSetByName(const std::string &setName,
                                      const std::string &userName) {
  try {
    auto conn = connectToDatabase();
    std::string sql =
        "SELECT f.question, f.answer, f.question_file_name, f.answer_file_name, uf.EFactor, uf.Interval, uf.Repetitions, uf.LastReview, uf.NextReview, uf.id, f.id \
         FROM app_user JOIN users_sets \
         ON app_user.id = users_sets.user_id JOIN set \
         ON set.id = users_sets.set_id JOIN user_flashcard AS uf \
         ON uf.Users_sets_id = users_sets.id JOIN flashcard as f \
         ON f.id = uf.flashcard_id \
         WHERE set.name = $1 and app_user.username = $2";

    std::string question_file_sql =
        "SELECT question_file \
                           FROM flashcard \
                           WHERE flashcard.id = $1;";

    std::string answer_file_sql =
        "SELECT answer_file \
                           FROM flashcard \
                           WHERE flashcard.id= $1;";

    pqxx::nontransaction N(*conn);
    pqxx::result R(N.exec_params(sql, setName, userName));
    std::unique_ptr<Set> set = std::make_unique<Set>(setName);

    std::string setPath = "flashcardFiles/" + setName;

    if (!std::filesystem::exists("flashcardFiles/")) {
      std::filesystem::create_directory("flashcardFiles/");
    }

    if (!std::filesystem::exists(setPath)) {
      std::filesystem::create_directory(setPath);
    }

    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
      std::shared_ptr<Flashcard> card = std::make_shared<Flashcard>(
          c[0].as<std::string>(), c[1].as<std::string>(),
          c[2].as<std::string>(), c[3].as<std::string>(), c[4].as<double>(),
          c[5].as<unsigned int>(), c[6].as<unsigned int>(),
          parseTimestamp(c[7].as<std::string>()),
          parseTimestamp(c[8].as<std::string>()), c[9].as<int>());
      if (card->getQuestionFile() != "") {
        card->setQuestionFile(setPath + "/" + card->getQuestionFile());
        if (!std::filesystem::exists(card->getQuestionFile())) {
          downloadFileFromDatabase(N, card->getQuestionFile(), c[10].as<int>(),
                                   question_file_sql);
        }
      }
      if (card->getAnswerFile() != "") {
        card->setAnswerFile(setPath + "/" + card->getAnswerFile());
        if (!std::filesystem::exists(card->getAnswerFile())) {
          downloadFileFromDatabase(N, card->getAnswerFile(), c[10].as<int>(),
                                   answer_file_sql);
        }
      }
      set->addCard(card);
    }
    return set;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return nullptr;
  }
}

bool checkIsSetSubscribed(int setId, int userId) {
  try {
    auto conn = connectToDatabase();
    std::string query =
        "SELECT * FROM users_sets WHERE user_id = $1 AND set_id = $2;";
    pqxx::nontransaction N(*conn);
    pqxx::result R(N.exec_params(query, userId, setId));
    if (R.empty()) {
      return true;
    }
    return false;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
}

std::chrono::system_clock::time_point parseTimestamp(
    const std::string &timestamp) {
  std::tm tm = {};
  std::istringstream ss(timestamp);
  ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
  if (ss.fail()) {
    throw std::runtime_error("Failed to parse timestamp");
  }
  std::time_t time_t_value = std::mktime(&tm);
  return std::chrono::system_clock::from_time_t(time_t_value);
}