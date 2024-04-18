#pragma once
#include <pqxx/pqxx>

std::unique_ptr<pqxx::connection> connectToDatabase();
void findUsers();
