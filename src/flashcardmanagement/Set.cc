#include "Set.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

Set::Set(std::string name): name_(std::move(name)) {}

Set::~Set() = default;

std::string Set::getName() const {
    return name_;
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

void Set::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (file.is_open()) {
        for (const auto& card : flashcards_) {
            file << card->getQuestion() << "," << card->getAnswer() << std::endl;
        }
        file.close();
        std::cout << "Zapisano zestaw do pliku: " << filename << std::endl;
    } else {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << filename << std::endl;
    }
}

Set readFromFile(const std::string& filename, const std::string& setName) {
    Set set(setName);
    std::ifstream file(filename);

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
        std::cout << "Wczytano zestaw z pliku: " << filename << std::endl;
    } else {
        std::cerr << "Nie można otworzyć pliku do odczytu: " << filename << std::endl;
    }

    return set;
}