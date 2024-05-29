#pragma once
#include <memory>
#include <string>

void saveUsersSetToDb(int setId, int userId);
std::unique_ptr<Set> getUserSetByName(std::string const &setName,
                                      std::string const &userName);
bool checkIsSetSubscribed(int setId, int userId);
std::chrono::system_clock::time_point parseTimestamp(
    const std::string &timestamp);