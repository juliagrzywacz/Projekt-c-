#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>

class TaskAddWindow : public QWidget {
Q_OBJECT

public:
    explicit TaskAddWindow(QWidget *parent = nullptr);

signals:
    void taskAdded(const QString &title, const QString &description);

private:
    QLineEdit *taskPersonEdit;
    QLineEdit *taskTitleEdit;
    QLineEdit *taskDescriptionEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;

  //  QSqlDatabase db;  // Dodajemy bazę danych

private slots:
    void saveTask();
};

#endif // TASKWINDOW_H
