#pragma once
#include <pqxx/pqxx>

std::unique_ptr<pqxx::connection> connect_to_database();
void findUsers();
