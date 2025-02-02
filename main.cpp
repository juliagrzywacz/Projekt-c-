#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QStyle>
#include <QIcon>
#include "WeekView.h"
#include "database.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Inicjalizacja bazy danych
    initializeDatabase();
    Database database;

    // Tworzenie okna głównego
    QMainWindow mainWindow;

    // Ustawienie ikony okna
    mainWindow.setWindowIcon(QIcon(":/icons/calendar.png"));


    // Tworzenie widoku tygodniowego i ustawienie go jako centralny widget
    WeekView *weekView = new WeekView(database);
    mainWindow.setCentralWidget(weekView);

    mainWindow.setWindowTitle("Roomie");
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