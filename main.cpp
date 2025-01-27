#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QStyle>
#include "WeekView.h"
#include "database.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Inicjalizacja bazy danych
    initializeDatabase();
    Database database;

    // Tworzenie okna głównego
    QMainWindow mainWindow;

    // Tworzenie widoku tygodniowego i ustawienie go jako centralny widget
    WeekView *weekView = new WeekView(database);
    mainWindow.setCentralWidget(weekView);

    mainWindow.setWindowTitle("Kalendarz Tygodniowy");
    mainWindow.show();

    //Centrowanie okna głównego
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        mainWindow.setGeometry(
                QStyle::alignedRect(
                        Qt::LeftToRight,
                        Qt::AlignCenter,
                        mainWindow.size(),
                        screenGeometry
                )
        );
    }

    return a.exec();
}