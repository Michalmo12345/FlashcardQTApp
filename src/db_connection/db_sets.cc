#include "connect_db.h"
#include "db_sets.h"
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include <memory>
#include <filesystem>

std::vector<std::string> getSetNamesFromDb() {
    try {        
        auto conn = connectToDatabase();
        std::string sql = "SELECT name from set";
        pqxx::nontransaction N(*conn);
        pqxx::result R(N.exec(sql));
        std::vector<std::string> set_names;
        
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            set_names.push_back(c[0].as<std::string>());
        }
        return set_names;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return {};
    }
}

std::vector<std::string> getSubscribedSetNamesFromDb(int userId) {
    try {        
        auto conn = connectToDatabase();
        std::string sql = "SELECT set.name \
                           FROM set JOIN users_sets \
                           ON set.id = users_sets.set_id \
                           WHERE users_sets.user_id = $1";
        pqxx::nontransaction N(*conn);
        pqxx::result R(N.exec_params(sql, userId));
        std::vector<std::string> set_names;
        
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            set_names.push_back(c[0].as<std::string>());
        }
        return set_names;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return {};
    }
}

std::vector<std::string> getSetNamesFromFiles() {
    std::string dir = "sets";
    std::vector<std::string> setNames;
    try {
        if (!std::filesystem::exists(dir)) {
            return {};
        }

        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                std::string fileName = entry.path().filename().stem().string();
                setNames.push_back(fileName);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Wystąpił błąd podczas przetwarzania folderu: " << e.what() << std::endl;
    }

    return setNames;
}

