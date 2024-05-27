#include "connect_db.h"
#include <iostream>
#include <pqxx/pqxx>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

// Funkcja do odczytu pliku CSV i zwrócenia mapy z kluczami i wartościami
std::unordered_map<std::string, std::string> parseCsv(const std::string &filePath) {
    std::unordered_map<std::string, std::string> config;
    std::ifstream configFile(filePath);
    if (!configFile) {
        throw std::runtime_error("Unable to open config file");
    }

    std::string line;
    // Pomijanie nagłówka
    std::getline(configFile, line);

    while (std::getline(configFile, line)) {
        std::istringstream lineStream(line);
        std::string key, value;
        if (std::getline(lineStream, key, ',') && std::getline(lineStream, value)) {
            config[key] = value;
        }
    }

    return config;
}

std::string convertCsvToConnectionString(const std::string &filePath) {
    std::unordered_map<std::string, std::string> config = parseCsv(filePath);

    std::ostringstream connStr;
    connStr << "dbname=" << config["database"]
            << " user=" << config["user"]
            << " password=" << config["password"]
            << " hostaddr=" << config["host"]
            << " port=" << config["port"];

    return connStr.str();
}

std::unique_ptr<pqxx::connection> connectToDatabase() {
    try {
        std::string connect_string = convertCsvToConnectionString("db_config.csv");

        // wersja do testowania na lokalnej bazie danych
        // auto C = std::make_unique<pqxx::connection>("dbname = postgres user = admin password = admin hostaddr = 127.0.0.1 port = 5433");
        
        std::cout << "db string: " << connect_string<< std::endl;
        auto C = std::make_unique<pqxx::connection>(connect_string);
        if (C->is_open()) {
            std::cout << "Opened database successfully: " << C->dbname() << std::endl;
        } else {
            std::cerr << "Can't open database" << std::endl;
            exit(1);
        }
        return C;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void findUsers() {
   try {        
      auto conn = connectToDatabase();
      std::string sql = "SELECT * from app_user";
      pqxx::nontransaction N(*conn);
      pqxx::result R(N.exec(sql));
      
      for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
         std::cout << "id = " << c[0].as<int>() << std::endl;
         std::cout << "Username = " << c[1].as<std::string>() << std::endl;
      }

      std::cout << "Operation done successfully" << std::endl;
   } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
   }
}
