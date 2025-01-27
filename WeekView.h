#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QCalendarWidget>
#include <QMap>
#include <random>
#include <cstdlib>
#include <QDebug>
#include <QTextEdit>
#include <QScrollArea>
#include <QDir>
#include "TaskWindow.h"
#include "database.h"
#include "TaskEditWindow.h"


class WeekView : public QWidget {
Q_OBJECT

public:
    explicit WeekView(Database &db, QWidget *parent = nullptr);
    ~WeekView();

private slots:
    void showPreviousWeek();
    void showNextWeek();
    void openCalendar();
    void onDateSelected(const QDate &date);
    void showTaskAddWindow(const QDate &date, const QTime &time);
    void showTaskEditWindow(int taskId);
    void openAddTaskWindowWithCurrentDateTime();

private:
    Database &database;
    void updateCalendar();
    int calculateRowForTime(const QDate &date, const QPair<QString, QString> &task);
    void displayTasksForWeek();
    QString getColorForPerson(const QString &person);

    QGridLayout *layout;
    QLabel *dayLabels[7];
    QDate currentWeekStartDate;
    QPushButton *dateRangeButton;
    QCalendarWidget *calendarWidget;
    QMap<QWidget*, QPair<QDate, QTime>> cellDateTimeMap;
    QMap<QString, QString> personColorMap;
    QStringList pastelColors;
    QStringList days;

    TaskAddWindow *taskAddWindow;
};

#endif // WEEKVIEW_H