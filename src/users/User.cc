#include "User.h"
#include <iostream>

User::User(std::string username):
    username_(username) {}

std::string User::getUsername() {
    return username_;
}

void User::setUsername(std::string new_username) {
    username_ = new_username;
}