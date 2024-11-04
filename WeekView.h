#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QCalendarWidget>
#include <QVBoxLayout>

class WeekView : public QWidget {
Q_OBJECT

public:
    explicit WeekView(QWidget *parent = nullptr);

private slots:
    void showPreviousWeek();
    void showNextWeek();
    void openCalendar();
    void onDateSelected(const QDate &date);

private:
    void updateCalendar();

    QGridLayout *layout;
    QLabel *dayLabels[7];
    QDate currentWeekStartDate;
    QPushButton *dateRangeButton;
    QCalendarWidget *calendarWidget;
};

#endif // WEEKVIEW_H
