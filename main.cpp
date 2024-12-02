#include <QApplication>
#include <QMainWindow>
#include "WeekView.h"
#include "database.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow mainWindow;

    // Inicjalizacja bazy danych
    initializeDatabase();

    // Tworzenie widoku tygodniowego
    Database database;  // Tworzymy instancję bazy danych
    WeekView *weekView = new WeekView(database);

    // Debugowanie: sprawdzenie czy widok tygodniowy jest stworzony
    if (weekView) {
        qDebug() << "Widok tygodniowy stworzony.";
    } else {
        qDebug() << "Nie udało się stworzyć widoku tygodniowego.";
    }

    // Ustawienie go jako centralny widget w oknie głównym
    mainWindow.setCentralWidget(weekView);

    mainWindow.setWindowTitle("Kalendarz Tygodniowy");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return a.exec();
}
