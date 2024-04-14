#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int base() {
   char * sql;
   
   try {
      connection C("dbname = postgres user = admin password = admin \
      hostaddr = 127.0.0.1 port = 5433");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }

      /* Create SQL statement */
      sql = "SELECT * from app_user";

      /* Create a non-transactional object. */
      nontransaction N(C);
      
      /* Execute SQL query */
      result R( N.exec( sql ));
      
      /* List down all the records */
      for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
         cout << "id = " << c[0].as<int>() << endl;
         cout << "Username = " << c[1].as<string>() << endl;
         cout << "Password = " << c[2].as<string>() << endl;
         cout << "Gender = " << c[3].as<string>() << endl;
      }
      cout << "Operation done successfully" << endl;
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}