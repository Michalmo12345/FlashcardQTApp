#include "User.h"

#include <iostream>
#include <pqxx/pqxx>

#include "../db_connection/connect_db.h"
User::User(std::string username) : username_(username) {}

User::~User() = default;

std::string User::getUsername() { return username_; }

void User::setUsername(std::string new_username) { username_ = new_username; }

void User::saveToDb() {
  try {
    auto conn = connectToDatabase();
    pqxx::work txn(*conn);
    std::string query = "INSERT INTO app_user (username) VALUES ($1);";
    txn.exec_params(query, username_);
    txn.commit();
    std::cout << "Dodano użytkownika: " << username_ << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void User::updateInDb(const std::string &oldUsername) {
  try {
    auto conn = connectToDatabase();
    pqxx::work txn(*conn);
    std::string query =
        "UPDATE app_user SET username = $1 WHERE username = $2;";
    txn.exec_params(query, oldUsername,
                    username_);  // username_ is the new username, oldUsername
                                   // is the current username
    txn.commit();
    std::cout << "Updated user from: " << oldUsername << " to: " << username_
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void User::deleteFromDb() {
  try {
    auto conn = connectToDatabase();
    pqxx::work txn(*conn);
    std::string query = "DELETE FROM app_user WHERE username = $1;";
    txn.exec_params(query, username_);  // Securely pass the username parameter
    txn.commit();
    std::cout << "Deleted user: " << username_ << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

bool checkUsernameInDb(const std::string &username) {
  try {
    auto conn = connectToDatabase();
    std::string query = "SELECT FROM app_user WHERE username = $1;";
    pqxx::nontransaction N(*conn);
    pqxx::result R(N.exec_params(query, username));
    if (R.empty()) {
      return true;
    }
    return false;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
}

int getUserId(const std::string &username) {
  try {
    auto conn = connectToDatabase();
    std::string query = "SELECT id FROM app_user WHERE username = $1;";
    pqxx::nontransaction N(*conn);
    pqxx::result R(N.exec_params(query, username));
    if (!R.empty()) {
        pqxx::row row = R[0];
        int id = row["id"].as<int>();
        std::cout << "User ID: " << id << std::endl;
        return id;
    } else {
        std::cout << "No user found with username: " << username << std::endl;
        return -1;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return -1;
  }
}