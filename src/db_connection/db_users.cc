#include "connect_db.h"
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include <memory>
#include <filesystem>


std::vector<std::string> getUsersFromDb() {
    try {
        auto conn = connectToDatabase();
        std::string sql = "SELECT * from app_user";
        pqxx::nontransaction N(*conn);
        pqxx::result R(N.exec(sql));
        std::vector<std::string> usernames;

        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            usernames.push_back(c[1].as<std::string>());
        }
        return usernames;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return {};
    }
}
