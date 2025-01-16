#include <QApplication>
#include <QMainWindow>
#include "WeekView.h"
#include "database.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow mainWindow;

    // Inicjalizacja bazy danych
    initializeDatabase();
    Database database;  // Tworzymy instancję bazy danych

    // Tworzenie widoku tygodniowego
    WeekView *weekView = new WeekView(database);

    // Ustawienie go jako centralny widget w oknie głównym
    mainWindow.setCentralWidget(weekView);

    mainWindow.setWindowTitle("Kalendarz Tygodniowy");
    mainWindow.resize(800, 600);
    mainWindow.show();


    return a.exec();
}
