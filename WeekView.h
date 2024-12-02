#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QMap>
#include <QPair>
#include "TaskWindow.h"
#include "database.h"

class WeekView : public QWidget {
Q_OBJECT

public:
    explicit WeekView(Database &db, QWidget *parent = nullptr);

private slots:
    void showPreviousWeek();
    void showNextWeek();
    void openCalendar();
    void onDateSelected(const QDate &date);
    void showTaskAddWindow(const QDate &date, const QTime &time);

private:
    Database &database;
    void updateCalendar();
    int calculateRowForTime(const QDate &date, const QPair<QString, QString> &task);
    QGridLayout *layout;
    QLabel *dayLabels[7];
    QDate currentWeekStartDate;
    QPushButton *dateRangeButton;
    QCalendarWidget *calendarWidget;
    QMap<QPushButton*, QPair<QDate, QTime>> cellDateTimeMap;

    TaskAddWindow *taskAddWindow;  // Okno dodawania zadań
};

#endif // WEEKVIEW_H
