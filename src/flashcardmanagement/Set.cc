#include "Set.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <filesystem>
#include "../db_connection/connect_db.cc"
#include "users/User.h"
#include <pqxx/pqxx>
#include <chrono>
#include <ctime>
#include <iomanip>

Set::Set(std::string name): name_(std::move(name)) {}

Set::Set(std::string name, std::string creationDate, std::string creatorUsername): name_(std::move(name)), creationDate_(std::move(creationDate)), creatorUsername_(std::move(creatorUsername)) {}

Set::~Set() = default;

std::string Set::getName() const {
    return name_;
}

std::string Set::getCreationDate() const {
    return creationDate_;
}

std::string Set::getCreatorUsername() const {
    return creatorUsername_;
}

void Set::setName(const std::string& name) {
    name_ = name;
}

void Set::addCard(std::shared_ptr<Flashcard> card) {
    flashcards_.push_back(card);
}


std::shared_ptr<Flashcard> Set::getCard(size_t index) {
    if (index >= flashcards_.size()) {
        return nullptr;
    }
    return flashcards_[index];
}
std::shared_ptr<Flashcard> Set::giveRandomCard(){
    if (flashcards_.empty()) {
        return nullptr;
    }
    srand(time(nullptr));
    return flashcards_[rand() % flashcards_.size()];
}

void Set::saveToFile() const {
    std::string dir = "sets";
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directory(dir);
    }

    std::string path = dir + "/" + name_;
    int counter = 0;
    while (std::filesystem::exists(path)) {
        ++counter;
        path = dir + "/" + name_ + "_" + std::to_string(counter);
    }

    path = path + ".txt";
    std::ofstream file(path);
    if (file.is_open()) {
        for (const auto& card : flashcards_) {
            file << card->getQuestion() << "," << card->getAnswer() << std::endl;
        }
        file.close();
        std::cout << "Zapisano zestaw do pliku: " << path << std::endl;
    } else {
        std::cerr << "Wystąpił błąd przy zapisie: " << path << std::endl;
    }
}

void Set::saveToDB(const std::string& username) const {
    try {        
        auto conn = connectToDatabase();
        pqxx::work txn(*conn); 
        std::string insert_set = "INSERT INTO set (name, creation_date, creator_id) VALUES ($1, $2, $3)";
        txn.exec_params(insert_set, name_, getCurrentDate(), getUserId(username));

        pqxx::result result = txn.exec("SELECT lastval()");
        int set_id = result[0][0].as<int>();
        
        std::string insertFlashcard = "INSERT INTO flashcard (set_id, question, answer, question_file_name, answer_file_name) VALUES ($1, $2, $3, $4, $5)";
        std::string insertFlashcardBothFiles = "INSERT INTO flashcard (set_id, question, answer, question_file, question_file_name, answer_file, answer_file_name) VALUES ($1, $2, $3, $4, $5, $6, $7)";
        std::string insertFlashcardQuestionFile = "INSERT INTO flashcard (set_id, question, answer, question_file, question_file_name, answer_file_name) VALUES ($1, $2, $3, $4, $5, $6)";
        std::string insertFlashcardAnswerFile = "INSERT INTO flashcard (set_id, question, answer, question_file_name, answer_file, answer_file_name) VALUES ($1, $2, $3, $4, $5, $6)";
        
        for (const auto& card : flashcards_) {
            auto questionData = getBinaryString(card->getQuestionFile());
            auto answerData = getBinaryString(card->getAnswerFile());
            if (questionData && answerData) {
                txn.exec_params(insertFlashcardBothFiles, set_id, card->getQuestion(), card->getAnswer(), *questionData, trimFromLastSlash(card->getQuestionFile()), *answerData, trimFromLastSlash(card->getAnswerFile()));
            }
            else if (questionData && !answerData) {
                txn.exec_params(insertFlashcardQuestionFile, set_id, card->getQuestion(), card->getAnswer(), *questionData, trimFromLastSlash(card->getQuestionFile()), trimFromLastSlash(card->getAnswerFile()));
            }
            else if (!questionData && answerData) {
                txn.exec_params(insertFlashcardAnswerFile, set_id, card->getQuestion(), card->getAnswer(), trimFromLastSlash(card->getQuestionFile()), *answerData, trimFromLastSlash(card->getAnswerFile()));
            }
            else {
                txn.exec_params(insertFlashcard, set_id, card->getQuestion(), card->getAnswer(), trimFromLastSlash(card->getQuestionFile()), trimFromLastSlash(card->getAnswerFile()));
            }
        }

        txn.commit();
        std::cout << "Set saved successfully to database." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

std::unique_ptr<Set> readFromFile(const std::string& filename, const std::string& setName) {
    std::unique_ptr<Set> set = std::make_unique<Set>(setName);
    std::ifstream file("sets/" + filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string question, answer;
            if (std::getline(iss, question, ',') && std::getline(iss, answer)) {
                std::shared_ptr<Flashcard> card = std::make_shared<Flashcard>(question, answer);
                set->addCard(card);
            }
        }
        file.close();
        std::cout << "Wczytano zestaw z pliku: sets/" << filename << std::endl;
    } else {
        std::cerr << "Nie można otworzyć pliku do odczytu: sets/" << filename << std::endl;
    }

    return set;
}

std::unique_ptr<Set> getSetByName(const std::string& setName) {
    try {
        std::string sql = "SELECT flashcard.question, flashcard.answer, flashcard.question_file_name, flashcard.answer_file_name, flashcard.id \
                           FROM flashcard JOIN set \
                           ON flashcard.set_id = set.id \
                           WHERE set.name = $1;";
        std::string question_file_sql = "SELECT question_file \
                           FROM flashcard \
                           WHERE flashcard.id = $1;";
        std::string answer_file_sql = "SELECT answer_file \
                           FROM flashcard \
                           WHERE flashcard.id= $1;";

        auto set = getSetInfo(setName);
        auto conn = connectToDatabase();
        pqxx::nontransaction N(*conn);
        std::string setPath = "flashcardFiles/" + setName;

        if (!std::filesystem::exists(setPath)) {
            std::filesystem::create_directory(setPath);
        }
        
        pqxx::result R(N.exec_params(sql, setName));
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            std::shared_ptr<Flashcard> card = std::make_shared<Flashcard>(c[0].as<std::string>(), c[1].as<std::string>(), c[2].as<std::string>(), c[3].as<std::string>());
            if (card->getQuestionFile() != "") {
                card->setQuestionFile(setPath+"/"+card->getQuestionFile());
                if (!std::filesystem::exists(card->getQuestionFile())) {
                    downloadFileFromDatabase(N, card->getQuestionFile(), c[4].as<int>(), question_file_sql);
                }
            }
            if (card->getAnswerFile() != ""){
                card->setAnswerFile(setPath+"/"+card->getAnswerFile());
                if (!std::filesystem::exists(card->getAnswerFile())) {
                    downloadFileFromDatabase(N, card->getAnswerFile(), c[4].as<int>(), answer_file_sql);
                }
            }
            set->addCard(card);
        }
        return set;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

int getSetId(const std::string& setName) {
    std::string set_id = "SELECT id \
                           FROM set \
                           WHERE name = $1;";
    auto conn = connectToDatabase();
    pqxx::nontransaction N(*conn);
    pqxx::result R(N.exec_params(set_id, setName));
    return R[0][0].as<int>();
}

std::unique_ptr<Set> getSetInfo(const std::string& setName) {
    std::string set_info = "SELECT set.creation_date, app_user.username \
                           FROM set JOIN app_user \
                           ON set.creator_id = app_user.id \
                           WHERE set.name = $1;";

    auto conn = connectToDatabase();
    pqxx::nontransaction N(*conn);
    pqxx::result R(N.exec_params(set_info, setName));
    std::unique_ptr<Set> set = std::make_unique<Set>(setName, R[0][0].as<std::string>(), R[0][1].as<std::string>());
    return set;
}


std::unique_ptr<pqxx::binarystring> getBinaryString(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return nullptr;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        return nullptr;
    }

    return std::make_unique<pqxx::binarystring>(buffer.data(), buffer.size());
}

std::string getFileType(const std::string& filePath) {
    std::string lastFour = filePath.substr(filePath.size() - 4);
    if (lastFour == ".png" || lastFour == ".jpg" || lastFour == ".bmp") {
        return "img";
    }
    else if (lastFour == ".mp4" || lastFour == ".avi" || lastFour == ".mkv") {
        return "video";
    }
    else {
        return "audio";        
    }
}

std::string trimFromLastSlash(const std::string& str) {
    size_t lastSlashPos = str.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        return str.substr(lastSlashPos + 1);
    }
    return str;
}

void downloadFileFromDatabase(pqxx::nontransaction& N, const std::string& fileName, int id, const std::string& querry) {
    pqxx::result result(N.exec_params(querry, id));
    if (!result.empty()) {
        pqxx::binarystring bytea_data(result[0][0]);

        // Utwórz strumień do zapisu danych do pliku
        std::ofstream file(fileName, std::ios::binary);
        // Zapisz dane bytea do pliku
        file.write(reinterpret_cast<const char*>(bytea_data.data()), bytea_data.size());
        // Zamknij plik
        file.close();

        std::cout << "Bytea data saved to file successfully." << std::endl;
    } else {
        std::cerr << "No data found." << std::endl;
    }
}

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d");
    return oss.str();
}

std::vector<int> getFlashcardIds(int setId) {
    try {
        std::string sql = "SELECT id \
                           FROM flashcard \
                           WHERE set_id = $1;";

        auto conn = connectToDatabase();
        pqxx::nontransaction N(*conn);
        std::vector<int> flashcardIds;
        pqxx::result R(N.exec_params(sql, setId));
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            flashcardIds.push_back(c[0].as<int>());
        }
        return flashcardIds;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return {};
    }
}