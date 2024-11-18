#include <QApplication>
#include <QMainWindow>
#include "WeekView.h"
#include "database.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow mainWindow;

    // Inicjalizacja bazy danych
    initializeDatabase();

    // Tworzenie widoku tygodniowego i ustawienie go jako centralny widget w oknie głównym
    WeekView *weekView = new WeekView;
    mainWindow.setCentralWidget(weekView);

    mainWindow.setWindowTitle("Kalendarz Tygodniowy");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return a.exec();
}
