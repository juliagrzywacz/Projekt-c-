#include <QApplication>
#include <QMainWindow>
#include "WeekView.h"
#include "database.h"
#include <QScreen>
#include <QStyle>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Inicjalizacja bazy danych (przenieś to na początek!)
    initializeDatabase();
    Database database;

    // Tworzenie okna głównego
    QMainWindow mainWindow;

    // Tworzenie widoku tygodniowego i ustawienie go jako centralny widget
    WeekView *weekView = new WeekView(database);
    mainWindow.setCentralWidget(weekView);

    mainWindow.setWindowTitle("Kalendarz Tygodniowy");
    mainWindow.show();

    // **Centrowanie okna głównego (PO mainWindow.show())**
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