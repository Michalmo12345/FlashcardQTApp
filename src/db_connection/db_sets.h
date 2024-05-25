#pragma once
#include <vector>

std::vector<std::string> getSetNamesFromDb();
std::vector<std::string> getSubscribedSetNamesFromDb(int userId);
std::vector<std::string> getSetNamesFromFiles();  