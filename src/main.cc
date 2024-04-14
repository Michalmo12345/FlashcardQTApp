#include "qt_ui/mainwindow.h"
#include "flashcardmanagement/Set.h"
#include "db_connection/connect_db.cpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Set set("zestaw A");
    Flashcard* cardA = new Flashcard("Ile masz lat?", "11");
    Flashcard* cardB = new Flashcard("Jak masz na imie?", "ABCD");
    Flashcard* cardC = new Flashcard("Ile to 2x2?", "4");
    set.addCard(cardA);
    set.addCard(cardB);
    set.addCard(cardC);
    auto randomCard = set.giveRandomCard();
    randomCard->showQuestion();
    randomCard->showAnswer();
    set.saveToFile("pliczek_1");
    // Set set = readFromFile("pliczek_1", "Zestaw1");
    // auto randomCard = set.giveRandomCard();
    // randomCard->showQuestion();
    // randomCard->showAnswer();
    return a.exec();
}