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
#include "database.h"

class TaskAddWindow : public QWidget {
Q_OBJECT

public:
    explicit TaskAddWindow(QWidget *parent = nullptr);

signals:
    void taskAdded(const QString &person, const QString &title, const QString &description, const QString &dueDate, const QString &startTime, const QString &time);

private:
    Database database;
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
