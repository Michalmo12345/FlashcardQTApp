#pragma once
#include <pqxx/pqxx>

pqxx::connection connect_to_database();
void findUsers();
