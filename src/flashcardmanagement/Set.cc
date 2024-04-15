#include "Set.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <filesystem>
#include "db_connection/connect_db.cc"
#include <pqxx/pqxx>

Set::Set(std::string name): name_(std::move(name)) {}

Set::~Set() = default;

std::string Set::getName() const {
    return name_;
}

void Set::setName(const std::string& name) {
    name_ = name;
}

void Set::addCard(std::shared_ptr<Flashcard> card) {
    flashcards_.push_back(card);
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

void Set::saveToDB() const {
    try {        
        pqxx::connection conn = connect_to_database();
        pqxx::work txn(conn); 
        std::string insert_set = "INSERT INTO set (name) VALUES ($1)";
        txn.exec_params(insert_set, name_);

        pqxx::result result = txn.exec("SELECT lastval()");
        int set_id = result[0][0].as<int>();
        
        std::string insert_flashcard = "INSERT INTO flashcard (set_id, question, answer) VALUES ($1, $2, $3)";
        
        for (const auto& card : flashcards_) {
            txn.exec_params(insert_flashcard, set_id, card->getQuestion(), card->getAnswer());
        }

        txn.commit();
        std::cout << "Set saved successfully to database." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

Set readFromFile(const std::string& filename, const std::string& setName) {
    Set set(setName);
    std::ifstream file("sets/" + filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string question, answer;
            if (std::getline(iss, question, ',') && std::getline(iss, answer)) {
                std::shared_ptr<Flashcard> card = std::make_shared<Flashcard>(question, answer);
                set.addCard(card);
            }
        }
        file.close();
        std::cout << "Wczytano zestaw z pliku: sets/" << filename << std::endl;
    } else {
        std::cerr << "Nie można otworzyć pliku do odczytu: sets/" << filename << std::endl;
    }

    return set;
}

Set getSetByName(const std::string& setName) {
    try {        
        pqxx::connection conn = connect_to_database();
        std::string sql = "SELECT flashcard.question, flashcard.answer \
                           FROM flashcard JOIN set \
                           ON flashcard.set_id = set.id \
                           WHERE set.name = $1;";
        pqxx::nontransaction N(conn);
        pqxx::result R(N.exec_params(sql, setName));
        Set set(setName);
        
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            std::shared_ptr<Flashcard> card = std::make_shared<Flashcard>(c[0].as<std::string>(), c[1].as<std::string>());
            set.addCard(card);
        }
        return set;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
