#include "Users_sets.h"
#include <pqxx/pqxx>
#include <iostream>

#include "../db_connection/connect_db.h"

void saveUsersSetToDb(int setId, int userId) {
    try {        
        auto conn = connectToDatabase();
        pqxx::work txn(*conn); 
        std::string insertUsersSet = "INSERT INTO users_sets (user_id, set_id) VALUES ($1, $2)";
        std::string insertUserFlashcard = "INSERT INTO user_flashcard (flashcard_id, users_sets_id) VALUES ($1, $2)";
        txn.exec_params(insertUsersSet, userId, setId);

        pqxx::result result = txn.exec("SELECT lastval()");
        int usersSetId = result[0][0].as<int>();
        std::vector<int> flashcardIds = getFlashcardIds(setId);

        for (auto flashcardId : flashcardIds) {
          txn.exec_params(insertUserFlashcard, flashcardId, usersSetId);
        }

        txn.commit();
        std::cout << "User set saved successfully to database." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

bool checkIsSetSubscribed(int setId, int userId) {
try {
    auto conn = connectToDatabase();
    std::string query = "SELECT * FROM users_sets WHERE user_id = $1 AND set_id = $2;";
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