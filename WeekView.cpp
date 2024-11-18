#include "WeekView.h"
#include "TaskWindow.h"
#include "database.h"

WeekView::WeekView(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Ustawienie bieżącego tygodnia na poniedziałek aktualnego tygodnia
    currentWeekStartDate = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);

    // Przyciski do zmiany tygodnia
    QPushButton *prevWeekButton = new QPushButton("Poprzedni tydzień", this);
    QPushButton *nextWeekButton = new QPushButton("Następny tydzień", this);
    connect(prevWeekButton, &QPushButton::clicked, this, &WeekView::showPreviousWeek);
    connect(nextWeekButton, &QPushButton::clicked, this, &WeekView::showNextWeek);

    // Przycisk wyświetlający zakres dat dla bieżącego tygodnia
    dateRangeButton = new QPushButton(this);
    connect(dateRangeButton, &QPushButton::clicked, this, &WeekView::openCalendar);

    // Kalendarz do wyboru daty
    calendarWidget = new QCalendarWidget(this);
    calendarWidget->setWindowFlags(Qt::Popup);
    connect(calendarWidget, &QCalendarWidget::clicked, this, &WeekView::onDateSelected);
    calendarWidget->hide();  // Początkowo kalendarz jest ukryty

    // Ustawienie przycisków i zakresu dat w układzie
    mainLayout->addWidget(dateRangeButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(prevWeekButton);
    mainLayout->addWidget(nextWeekButton);

    // Layout dla dni tygodnia i tabeli
    layout = new QGridLayout();
    QStringList days = {"Poniedziałek", "Wtorek", "Środa", "Czwartek", "Piątek", "Sobota", "Niedziela"};

    // Dodanie godzin po lewej stronie tabeli
    QStringList hours = {"06:00 - 08:00", "08:00 - 10:00", "10:00 - 12:00", "12:00 - 14:00", "14:00 - 16:00",
                         "16:00 - 18:00", "18:00 - 20:00", "20:00 - 22:00"};

    for (int row = 0; row < hours.size(); ++row) {
        QLabel *timeLabel = new QLabel(this);
        timeLabel->setAlignment(Qt::AlignCenter);
        timeLabel->setText(hours[row]);  // Ustawienie godziny dla każdego wiersza
        layout->addWidget(timeLabel, row + 2, 0);  // Dodaj godzinę do lewej kolumny
    }

    // Dodanie dni tygodnia u góry tabeli
    for (int i = 0; i < days.size(); ++i) {
        dayLabels[i] = new QLabel(this);
        dayLabels[i]->setAlignment(Qt::AlignCenter);
        layout->addWidget(dayLabels[i], 1, i + 1); // Ustawienie nazw dni tygodnia
    }

    // Wypełnianie tabeli komórkami dla zadań
    for (int row = 0; row < hours.size(); ++row) {
        for (int col = 0; col < days.size(); ++col) {
            QPushButton *cell = new QPushButton(this);
            cell->setText("");  // Puste komórki na początku
            cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            layout->addWidget(cell, row + 2, col + 1);

            // Po kliknięciu otwórz okno dodawania zadania
            connect(cell, &QPushButton::clicked, this, [this, row, col]() {
                showTaskAddWindow(row, col);
            });
        }
    }

    mainLayout->addLayout(layout);
    setLayout(mainLayout);

    updateCalendar();
}

void WeekView::showPreviousWeek() {
    currentWeekStartDate = currentWeekStartDate.addDays(-7);
    updateCalendar();
}

void WeekView::showNextWeek() {
    currentWeekStartDate = currentWeekStartDate.addDays(7);
    updateCalendar();
}

void WeekView::openCalendar() {
    calendarWidget->setSelectedDate(currentWeekStartDate);  // Ustawienie kalendarza na bieżący tydzień
    calendarWidget->move(dateRangeButton->mapToGlobal(QPoint(0, dateRangeButton->height())));
    calendarWidget->show();
}

void WeekView::onDateSelected(const QDate &date) {
    currentWeekStartDate = date.addDays(-date.dayOfWeek() + 1);  // Ustawienie wybranego tygodnia
    updateCalendar();
    calendarWidget->hide();  // Ukrycie kalendarza po wyborze daty
}

void WeekView::updateCalendar() {
    // Aktualizacja etykiet dni tygodnia z datami
    for (int i = 0; i < 7; ++i) {
        QDate dayDate = currentWeekStartDate.addDays(i);
        dayLabels[i]->setText(dayDate.toString("dddd dd.MM"));
    }

    // Aktualizacja tekstu przycisku zakresu dat
    QDate endDate = currentWeekStartDate.addDays(6);
    dateRangeButton->setText(currentWeekStartDate.toString("dd.MM") + " - " + endDate.toString("dd.MM"));
}

void WeekView::showTaskAddWindow(int row, int col) {
    if (!taskAddWindow) {
        taskAddWindow = new TaskAddWindow(this);
        // Ustawienia pozycji okna (opcjonalnie)
        taskAddWindow->move(this->geometry().center() - taskAddWindow->rect().center());

        connect(taskAddWindow, &TaskAddWindow::taskAdded, this, [this](const QString &person, const QString &title, const QString &description) {

            //this->database.addTask(person, title, description);
            qDebug() << "Dodano zadanie: " << title << ", " << description;
        });
    }

    taskAddWindow->show();
    taskAddWindow->raise();
    taskAddWindow->activateWindow();
}
