#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include "database.h"

class TaskAddWindow : public QWidget {
Q_OBJECT

public:
    explicit TaskAddWindow(QWidget *parent = nullptr);

signals:
    void taskAdded(const QString &person, const QString &title, const QString &description);

private:
    Database database;
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
