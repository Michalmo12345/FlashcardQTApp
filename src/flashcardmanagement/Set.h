#pragma once
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "Flashcard.h"

class Set {
 private:
  std::vector<std::shared_ptr<Flashcard>> flashcards_;
  std::string name_;
  std::string creationDate_;
  std::string creatorUsername_;

 public:
  Set(std::string name);
  Set(std::string name, std::string creationDate, std::string creatorUsername);
  ~Set();
  std::string getName() const;
  std::string getCreationDate() const;
  std::string getCreatorUsername() const;
  void setName(const std::string& name);
  void addCard(std::shared_ptr<Flashcard> card);
  std::shared_ptr<Flashcard> getCard(size_t index);
  std::shared_ptr<Flashcard> giveRandomCard();
  void saveToFile() const;
  void saveToDB(const std::string& username) const;
  std::vector<std::shared_ptr<Flashcard>> getFlashcards() const;
};

std::unique_ptr<Set> readFromFile(const std::string& filename,
                                  const std::string& setName);
std::unique_ptr<Set> getSetByName(const std::string& setName);
std::unique_ptr<pqxx::binarystring> getBinaryString(
    const std::string& filePath);
std::string getFileType(const std::string& filePath);
std::string trimFromLastSlash(const std::string& str);
void downloadFileFromDatabase(pqxx::nontransaction& N,
                              const std::string& fileName, int id,
                              const std::string& querry);
std::string getCurrentDate();
std::unique_ptr<Set> getSetInfo(const std::string& setName);
int getSetId(const std::string& setName);
std::vector<int> getFlashcardIds(int setId);
