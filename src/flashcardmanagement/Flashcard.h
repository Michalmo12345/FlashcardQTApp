#include <iostream>
#include <cmath>
#include <string>

class Flashcard {
    private:
        std::string question;
        std::string answer;
        int n; //number of repetitions
        int I; //interval between repetitions
        double EF; //easiness factor
    public:
        Flashcard(std::string question, std::string answer);
        void update(int q);//q is the integer from1-5 chosen by the user to determine knowledge of the flashcard
        void showQuestion();
        void showAnswer();
        
};