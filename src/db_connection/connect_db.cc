#include "connect_db.h"
#include <iostream>
#include <pqxx/pqxx>
#include <memory>

std::unique_ptr<pqxx::connection> connectToDatabase() {
    try {
        auto C = std::make_unique<pqxx::connection>("dbname = postgres user = admin password = admin hostaddr = 127.0.0.1 port = 5433");
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
         std::cout << "Password = " << c[2].as<std::string>() << std::endl;
         std::cout << "Gender = " << c[3].as<std::string>() << std::endl;
      }

      std::cout << "Operation done successfully" << std::endl;
   } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
   }
}
