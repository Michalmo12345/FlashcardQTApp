#pragma once
#include <iostream>
#include <cmath>
#include <string>

class Flashcard {
    private:
        std::string question;
        std::string answer;
        unsigned int n; //number of repetitions
        int interval; //interval between repetitions
        double EFactor; //easiness factor
    public:
        Flashcard(std::string question, std::string answer);
        void update(); 
        std::string getQuestion();
        std::string getAnswer();
};