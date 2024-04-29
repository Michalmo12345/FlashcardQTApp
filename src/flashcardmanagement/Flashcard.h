#pragma once
#include <iostream>
#include <cmath>
#include <string>

class Flashcard {
    private:
        std::string question;
        std::string answer;
        unsigned int repetitions; //number of repetitions
        unsigned int interval; //interval between repetitions
        double EFactor; //easiness factor
    public:
        Flashcard(std::string question, std::string answer);
        double calculateEFactor(double EFactor, int quality);
        unsigned int calculateInterval(unsigned int repetitions, double EFactor);
        void update(unsigned int quality); 
        double getEFactor();
        unsigned int getRepetitions();
        unsigned int getInterval();
        std::string getQuestion();
        std::string getAnswer();
};