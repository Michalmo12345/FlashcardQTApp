#pragma once
#include <iostream>
#include <string>
#include <vector>


class User {
    private:
        std::string username_;
    public:
        User(std::string username);
        std::string getUsername();
        void setUsername(std::string username);
};