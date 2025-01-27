#ifndef TASKEDITWINDOW_H
#define TASKEDITWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCheckBox>
#include <QScreen>
#include "database.h"

class TaskEditWindow : public QWidget {
Q_OBJECT

public:
    explicit TaskEditWindow(Database& database, int taskId, QWidget *parent = nullptr);
    void setTaskDetails(int taskId);

    signals:
        void taskUpdated(int taskId);
        void taskDeleted(int taskId);


private slots:
    void saveTask();
    void deleteTask();

private:
    Database& database;
    int taskId;
    QLineEdit *taskPersonEdit;
    QLineEdit *taskTitleEdit;
    QLineEdit *taskDescriptionEdit;
    QDateEdit *taskDueDateEdit;
    QTimeEdit *taskStartTimeEdit;
    QTimeEdit *taskTimeEdit;
    QPushButton *saveButton;
    QPushButton *deleteButton;
    QPushButton *cancelButton;
    QCheckBox *completedCheckbox;
};

#endif // TASKEDITWINDOW_H
