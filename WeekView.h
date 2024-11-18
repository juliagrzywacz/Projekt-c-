#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include "TaskWindow.h"
#include "database.h"

class WeekView : public QWidget {
Q_OBJECT

public:
    explicit WeekView(QWidget *parent = nullptr);

private slots:
    void showPreviousWeek();
    void showNextWeek();
    void openCalendar();
    void onDateSelected(const QDate &date);
    void showTaskAddWindow(int row, int col);

private:
    void updateCalendar();

   // Database database;

    QGridLayout *layout;
    QLabel *dayLabels[7];
    QDate currentWeekStartDate;
    QPushButton *dateRangeButton;
    QCalendarWidget *calendarWidget;

    TaskAddWindow *taskAddWindow;  // Okno dodawania zadań
};

#endif // WEEKVIEW_H
