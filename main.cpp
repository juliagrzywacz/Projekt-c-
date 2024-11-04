#include <QApplication>
#include <QMainWindow>
#include "WeekView.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow mainWindow;

    // Tworzymy widok tygodniowy i ustawiamy go jako centralny widget w oknie głównym
    WeekView *weekView = new WeekView;
    mainWindow.setCentralWidget(weekView);

    mainWindow.setWindowTitle("Kalendarz Tygodniowy");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return a.exec();
}
