#pragma once
#include <iostream>
#include <cmath>
#include <string>

class Flashcard {
    private:
        std::string question;
        std::string answer;
        unsigned int n; //number of repetitions
        unsigned int interval; //interval between repetitions
        double EFactor; //easiness factor
    public:
        Flashcard(std::string question, std::string answer);
        double calculateEFactor(double Efactor, int quality);
        void update(); 
        double getEFactor();
        unsigned int getN();
        unsigned int getInterval();
        std::string getQuestion();
        std::string getAnswer();
};