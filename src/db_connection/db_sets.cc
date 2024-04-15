#include "connect_db.h"
#include "db_sets.h"
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include <memory>

std::vector<std::string> getSetNamesFromDb() {
    try {        
        pqxx::connection conn = connect_to_database();
        std::string sql = "SELECT name from set";
        pqxx::nontransaction N(conn);
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

