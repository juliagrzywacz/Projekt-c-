#include <QApplication>
#include <QWidget>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.resize(250, 150);
    window.setWindowTitle("My Qt App");
    window.show();

    qDebug() << "Window should be displayed";

    return app.exec();
}
