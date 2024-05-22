#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Flashcard.h"
#include <memory>
#include <pqxx/pqxx>

class Set {
    private:
        std::vector<std::shared_ptr<Flashcard>> flashcards_;
        std::string name_;
    public:
        Set(std::string name);
        ~Set();
        std::string getName() const;
        void setName(const std::string& name);
        void addCard(std::shared_ptr<Flashcard> card);
        std::shared_ptr<Flashcard> getCard(size_t index);
        std::shared_ptr<Flashcard> giveRandomCard();
        void saveToFile() const;
        void saveToDB() const;
};

Set readFromFile(const std::string& filename, const std::string& setName);
Set getSetByName(const std::string& setName);
std::unique_ptr<pqxx::binarystring> getBinaryString(const std::string& filePath);
std::string getFileType(const std::string& filePath);
std::string trimFromLastSlash(const std::string& str);
void downloadFileFromDatabase(pqxx::nontransaction& N, const std::string& setName, const std::string& fileName, int id, const std::string& querry);
