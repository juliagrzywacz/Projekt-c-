#include "WeekView.h"
#include "TaskWindow.h"
#include "database.h"
#include <QGridLayout>
#include <QDebug> // Dodane includowanie QDebug

WeekView::WeekView(Database &db, QWidget *parent) : QWidget(parent), taskAddWindow(nullptr), database(db) {
    pastelColors = {
            "#FFB3BA", "#FFDFBA", "#FFFFBA", "#BAFFC9", "#BAE1FF",
            "#D7BAFF", "#FFC4E1", "#C4FFF9", "#E2FFBA", "#FFE4BA"
    };
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    currentWeekStartDate = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);

    QPushButton *addTaskButton = new QPushButton("+", this);
    addTaskButton->setFixedSize(30, 30);
    mainLayout->addWidget(addTaskButton, 0, Qt::AlignLeft);

    connect(addTaskButton, &QPushButton::clicked, this, &WeekView::openAddTaskWindowWithCurrentDateTime);

    QPushButton *prevWeekButton = new QPushButton("Poprzedni tydzień", this);
    QPushButton *nextWeekButton = new QPushButton("Następny tydzień", this);
    connect(prevWeekButton, &QPushButton::clicked, this, &WeekView::showPreviousWeek);
    connect(nextWeekButton, &QPushButton::clicked, this, &WeekView::showNextWeek);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(addTaskButton);
    topLayout->addStretch();
    topLayout->addWidget(prevWeekButton);
    topLayout->addWidget(nextWeekButton);

    mainLayout->addLayout(topLayout);

    dateRangeButton = new QPushButton(this);
    connect(dateRangeButton, &QPushButton::clicked, this, &WeekView::openCalendar);

    calendarWidget = new QCalendarWidget(this);
    calendarWidget->setWindowFlags(Qt::Popup);
    connect(calendarWidget, &QCalendarWidget::clicked, this, &WeekView::onDateSelected);
    calendarWidget->hide();

    mainLayout->addWidget(dateRangeButton, 0, Qt::AlignCenter);

    layout = new QGridLayout();
    layout->setSpacing(0); // **USUNIĘCIE ODSTĘPÓW W LAYOUCIE**
    layout->setContentsMargins(20,0,20,0);
    days = {"Poniedziałek", "Wtorek", "Środa", "Czwartek", "Piątek", "Sobota", "Niedziela"};

    // tworzenie nagłówków dni
    for (int i = 0; i < 7; ++i) {
        dayLabels[i] = new QLabel(this);
        dayLabels[i]->setAlignment(Qt::AlignCenter);
        dayLabels[i]->setStyleSheet("padding: 0px;");
        layout->addWidget(dayLabels[i], 1, i + 1);
    }

    for (int i = 0; i < 7; ++i) {
        layout->addWidget(dayLabels[i], 1, i + 1); // Dodawanie do layoutu
    }

    QStringList hours;
    for (int hour = 6; hour < 22; ++hour) {
        for (int minute = 0; minute < 60; minute += 30) { // Zmiana na += 30
            hours.append(QString("%1:%2").arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0')));
        }
    }

    for (int row = 0; row < hours.size(); ++row) {
        QLabel *timeLabel = new QLabel(this);
        timeLabel->setAlignment(Qt::AlignCenter);
        timeLabel->setText(hours[row]);
        timeLabel->setStyleSheet("padding: 0px;");
        layout->addWidget(timeLabel, row + 2, 0);
    }

    // Tworzenie komórek siatki
    for (int row = 0; row < hours.size(); ++row) {
        for (int col = 0; col < days.size(); ++col) {
            QPushButton *cell = new QPushButton(this);
            cell->setText("");
            cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            cell->setStyleSheet("padding: 0px;"); // Usunięcie marginesów wewnętrznych
            cell->setMinimumSize(0, 0); // Ustawienie minimalnego rozmiaru na 0
            layout->addWidget(cell, row + 2, col + 1);

            QDate cellDate = currentWeekStartDate.addDays(col);
            int hour = 6 + row / 2; // Zmiana na / 2
            int minute = (row % 2) * 30; // Zmiana na * 30
            QTime cellTime(hour, minute);

            cellDateTimeMap[cell] = qMakePair(cellDate, cellTime);

            connect(cell, &QPushButton::clicked, this, [this, cell]() {
                QVariant taskIdVariant = cell->property("taskId");
                if (taskIdVariant.isValid()) {
                    int taskId = taskIdVariant.toInt();
                    showTaskEditWindow(taskId);
                } else {
                    QPair<QDate, QTime> dateTime = cellDateTimeMap[cell];
                    showTaskAddWindow(dateTime.first, dateTime.second);
                }
            });
        }
    }
    mainLayout->addLayout(layout);
    setLayout(mainLayout);

    updateCalendar();
}

WeekView::~WeekView() {
    for (int i = 0; i < 7; ++i) { // Poprawione usuwanie
        delete dayLabels[i];
    }
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
    calendarWidget->setSelectedDate(currentWeekStartDate);
    calendarWidget->move(dateRangeButton->mapToGlobal(QPoint(0, dateRangeButton->height())));
    calendarWidget->show();
}

void WeekView::onDateSelected(const QDate &date) {
    currentWeekStartDate = date.addDays(-date.dayOfWeek() + 1);
    updateCalendar();
    calendarWidget->hide();
}

void WeekView::updateCalendar() {
    personColorMap.clear();
    for (int row = 2; row < layout->rowCount(); ++row) {
        for (int col = 1; col < layout->columnCount(); ++col) {
            QPushButton *button = dynamic_cast<QPushButton*>(layout->itemAtPosition(row, col)->widget());
            if (button) {
                button->setText("");
                button->setStyleSheet("");
                button->setProperty("taskId", QVariant());
            }
        }
    }
    for (int i = 0; i < 7; ++i) {
        QDate dayDate = currentWeekStartDate.addDays(i);
        QString dayName = this->days[i];

        dayLabels[i]->setText(dayName + "\n" + dayDate.toString("yyyy-MM-dd"));
    }
    QDate endDate = currentWeekStartDate.addDays(6);
    dateRangeButton->setText(currentWeekStartDate.toString("dd.MM") + " - " + endDate.toString("dd.MM"));

    displayTasksForWeek();
}

void WeekView::showTaskAddWindow(const QDate &date, const QTime &time) {
    if (!taskAddWindow) {
        taskAddWindow = new TaskAddWindow(database, this);
        taskAddWindow->move(this->geometry().center() - taskAddWindow->rect().center());

        // Poprawione połączenie: używamy sygnału destroyed
        connect(taskAddWindow, &TaskAddWindow::destroyed, this, [this](){
            this->taskAddWindow = nullptr;
        });
    }

    connect(taskAddWindow, &TaskAddWindow::taskAdded, this, &WeekView::updateCalendar);

    taskAddWindow->setInitialDateTime(date, time);

    if (!taskAddWindow->isVisible()) {
        taskAddWindow->show();
    }

    taskAddWindow->raise();
    taskAddWindow->activateWindow();
}

void WeekView::showTaskEditWindow(int taskId) {
    TaskEditWindow *editWindow = new TaskEditWindow(database, taskId, this);
    connect(editWindow, &TaskEditWindow::taskUpdated, this, &WeekView::updateCalendar);
    connect(editWindow, &TaskEditWindow::taskDeleted, this, &WeekView::updateCalendar);
    editWindow->show();
}

void WeekView::displayTasksForWeek() {
    QList<Task> tasks = database.getTasksForWeek(currentWeekStartDate);

    for (const Task &task : tasks) {
        int dayIndex = task.dueDate.dayOfWeek();
        if (dayIndex < 1 || dayIndex > 7) {
            continue;
        }

        QTime startTime = database.getTaskStartTimeForDate(task.dueDate, task.title);
        if (!startTime.isValid()) {
            qDebug() << "Nieprawidłowa godzina dla zadania: " << task.title << " Data: " << task.dueDate;
            continue;
        }

        int startRow = calculateRowForTime(task.dueDate, QPair<QString, QString>(task.startTime.toString(), task.title));
        if (startRow == -1) continue;

        int durationInMinutes = task.time.hour() * 60 + task.time.minute();
        int rowsToSpan = durationInMinutes / 30; // 30 minut na komórkę

        for (int i = 0; i < rowsToSpan; ++i) {
            int currentRow = startRow + i;
            if(currentRow >= layout->rowCount()) break;

            QLayoutItem *item = layout->itemAtPosition(currentRow, dayIndex);
            if (item) {
                QWidget *widget = item->widget();
                if (widget) {
                    QPushButton *button = dynamic_cast<QPushButton*>(item->widget());
                    if (button) {
                        QString taskText = (i == 0) ? (task.person + "\n" + task.title) : "";
                        button->setText(taskText);

                        if (task.completed == 1) {
                            button->setStyleSheet("background-color: #f0f0f0;");
                        } else {
                            QString personColor = getColorForPerson(task.person);
                            button->setStyleSheet(QString("background-color: %1;").arg(personColor));
                        }
                        button->setProperty("taskId", task.id);
                    }
                }
            }
        }
    }
}

int WeekView::calculateRowForTime(const QDate &date, const QPair<QString, QString> &task) {
    QTime startTime = database.getTaskStartTimeForDate(date, task.second);
    if (!startTime.isValid()) {
        qDebug() << "Nieprawidłowa godzina dla zadania: " << task.second
                 << "Data: " << date
                 << "StartTime: " << startTime;
        return -1;
    }

    int hour = startTime.hour();
    int minute = startTime.minute();

    if (hour < 6 || hour >= 22) {
        qDebug() << "Zadanie poza zakresem godzin siatki: " << startTime;
        return -1; // Zadanie poza zakresem siatki
    }

    int row = (hour - 6) * 2 + (minute / 30); // Obliczanie wiersza dla 30-minutowych interwałów
    //qDebug() << "Obliczony wiersz: " << row;
    return row + 2; // +2 dla nagłówków
}

QString WeekView::getColorForPerson(const QString &person) {
    if (!personColorMap.contains(person)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, pastelColors.size() - 1);
        int colorIndex = dis(gen);
        personColorMap[person] = pastelColors[colorIndex];
    }
    return personColorMap[person];
}

void WeekView::openAddTaskWindowWithCurrentDateTime() {
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    showTaskAddWindow(currentDate, currentTime);
}