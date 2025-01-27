#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QDateEdit>
#include <QTimeEdit>
#include <QApplication>
#include <QScreen>
#include <QFormLayout>
#include "database.h"
#include <QSqlError>
#include <QMessageBox>

class TaskAddWindow : public QWidget {
Q_OBJECT

public:
    explicit TaskAddWindow(Database& database, QWidget *parent = nullptr);
    void setInitialDateTime(const QDate &date, const QTime &time);

signals:
    void taskAdded();

private:
    Database& database;
    QLineEdit *taskPersonEdit;
    QLineEdit *taskTitleEdit;
    QLineEdit *taskDescriptionEdit;
    QDateEdit *taskDueDateEdit;
    QTimeEdit *taskStartTimeEdit;
    QTimeEdit *taskTimeEdit;
    QPushButton *saveButton;
    QPushButton *cancelButton;

private slots:
    void saveTask();
};

#endif // TASKWINDOW_H