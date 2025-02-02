#include "WeekView.h"


WeekView::WeekView(Database &db, QWidget *parent) : QWidget(parent), taskAddWindow(nullptr), database(db) {
    pastelColors = {
            "#FF6F61", "#FF9F80", "#FFEC73", "#66FF66", "#FF8C00",
            "#FF46B1", "#FDE74C", "#FF385C", "#FF6347", "#FFD700"
    };
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    setMinimumSize(800, 600); // Minimalny rozmiar
    setMaximumSize(1920, 1080); // Maksymalny rozmiar

    currentWeekStartDate = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);

    QPushButton *addTaskButton = new QPushButton(this);
    QIcon icon(":/icons/add-circle-bold.png");
    addTaskButton->setIcon(icon);
    addTaskButton->setIconSize(QSize(40, 40));
    addTaskButton->setFixedSize(55 , 55);

    addTaskButton->setStyleSheet("QPushButton { background-color: lightblue; color: DarkSlateGrey; border-radius: 5px; } QPushButton:hover { background-color: DodgerBlue; }");
    addTaskButton->show();

    mainLayout->addWidget(addTaskButton, 0, Qt::AlignCenter);
    connect(addTaskButton, &QPushButton::clicked, this, &WeekView::openAddTaskWindowWithCurrentDateTime);

    QPushButton *prevWeekButton = new QPushButton("Poprzedni tydzień", this);
    prevWeekButton->setFixedSize(200, 40);
    QFont font = prevWeekButton->font();
    font.setBold(true);
    font.setPointSize(12);
    prevWeekButton->setFont(font);
    prevWeekButton->setStyleSheet("QPushButton { background-color: lightblue; color: DarkSlateGrey; margin-right: 20px; border-radius: 5px; } QPushButton:hover { background-color: DodgerBlue; }");

    QPushButton *nextWeekButton = new QPushButton("Następny tydzień", this);
    nextWeekButton->setFixedSize(200, 40);
    nextWeekButton->setFont(font);
    nextWeekButton->setStyleSheet("QPushButton { background-color: lightblue; color: DarkSlateGrey; margin-left: 20px; border-radius: 5px; } QPushButton:hover { background-color: DodgerBlue; }");

    connect(prevWeekButton, &QPushButton::clicked, this, &WeekView::showPreviousWeek);
    connect(nextWeekButton, &QPushButton::clicked, this, &WeekView::showNextWeek);

    dateRangeButton = new QPushButton(this);
    connect(dateRangeButton, &QPushButton::clicked, this, &WeekView::openCalendar);
    dateRangeButton->setFixedSize(200 , 40);
    dateRangeButton->setFont(font);
    dateRangeButton->setStyleSheet("QPushButton { background-color: lightblue; color: DarkSlateGrey; border-radius: 5px; } QPushButton:hover { background-color: DodgerBlue; }");

    calendarWidget = new QCalendarWidget(this);
    calendarWidget->setWindowFlags(Qt::Popup);
    connect(calendarWidget, &QCalendarWidget::clicked, this, &WeekView::onDateSelected);
    calendarWidget->hide();

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(prevWeekButton);
    topLayout->addWidget(dateRangeButton);
    topLayout->addWidget(nextWeekButton);
    topLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(topLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0,0,0,0);
    layout = new QGridLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    days = {"Poniedziałek", "Wtorek", "Środa", "Czwartek", "Piątek", "Sobota", "Niedziela"};

    // Tworzenie nagłówków dni
    for (int i = 0; i < 7; ++i) {
        dayLabels[i] = new QLabel(this);
        dayLabels[i]->setAlignment(Qt::AlignCenter | Qt::AlignTop);
        dayLabels[i]->setStyleSheet("padding: 0px; border-bottom: 3px solid #555555; background-color: lightblue; color: DarkSlateGrey;");

        QFont font = dayLabels[i]->font();
        font.setPointSize(12);
        font.setBold(true);
        dayLabels[i]->setFont(font);
        dayLabels[i]->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        layout->addWidget(dayLabels[i], 1, i + 1);
    }

    QStringList hours;
    for (int hour = 6; hour < 22; ++hour) {
        for (int minute = 0; minute < 60; minute += 15) {
            hours.append(QString("%1:%2").arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0')));
        }
    }

    // Nagłówki godzin
    for (int row = 0; row < hours.size(); ++row) {
        QLabel *timeLabel = new QLabel(this);
        timeLabel->setAlignment(Qt::AlignRight);

        if (row % 2 == 0) {
            timeLabel->setText(hours[row]);
            timeLabel->setStyleSheet("border-right: 3px solid #555555; padding: 0px; background-color: lightblue; color: DarkSlateGrey;");
            if (row % 4 == 0) {
                timeLabel->setStyleSheet("border-top: 3px solid #555555; border-right: 3px solid #555555; padding: 0px; background-color: lightblue; color: DarkSlateGrey;");
            }
            QFont font = timeLabel->font();
            font.setPointSize(12);
            font.setBold(true);
            timeLabel->setFont(font);
            layout->setRowMinimumHeight(row + 2, 20);
        } else {
            timeLabel->setStyleSheet("border-right: 3px solid #555555; padding: 0px; background-color: lightblue;");
            layout->setRowMinimumHeight(row + 2, 5);
        }
        layout->addWidget(timeLabel, row + 2, 0);
    }

    layout->addWidget(addTaskButton, 1, 0);

    // Tworzenie komórek siatki
    for (int row = 0; row < hours.size(); ++row) {

        for (int col = 0; col < days.size(); ++col) {
            QTextEdit *cell = new QTextEdit(this);
            cell->setReadOnly(true);
            cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            cell->setMinimumSize(0, 0);
            cell->setMaximumHeight(50); //Maksymalna wysokość
            cell->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // Pasek przewijania

            QTextCharFormat format;
            format.setForeground(Qt::black);
            QTextBlockFormat blockFormat;
            blockFormat.setAlignment(Qt::AlignCenter);

            QTextCursor cursor = cell->textCursor();
            cursor.select(QTextCursor::Document);
            cursor.setCharFormat(format);
            cursor.setBlockFormat(blockFormat);
            cursor.clearSelection();

            layout->addWidget(cell, row + 2, col + 1);

            QDate cellDate = currentWeekStartDate.addDays(col);
            int hour = 6 + row / 4;
            int minute = (row % 4) * 15;
            QTime cellTime(hour, minute);

            cellDateTimeMap[cell] = qMakePair(cellDate, cellTime);

            connect(cell, &QTextEdit::selectionChanged, this, [this, cell]() {
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

    layout->setRowMinimumHeight(1, 55);
    centralLayout->addLayout(layout);
    centralWidget->setLayout(centralLayout);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(centralWidget);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
    updateCalendar();
}

WeekView::~WeekView() {
    for (int i = 0; i < 7; ++i) {
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
    //personColorMap.clear();
    for (int row = 2; row < layout->rowCount(); ++row) {
        for (int col = 1; col < layout->columnCount(); ++col) {
            QLayoutItem *item = layout->itemAtPosition(row, col);
            if (item) {
                QWidget *widget = item->widget();
                if (widget) {
                    QTextEdit *cell = dynamic_cast<QTextEdit*>(widget);
                    if (cell) {
                        cell->clear();
                        cell->setProperty("taskId", QVariant());

                        if (row % 2 == 0) {
                            cell->setStyleSheet("background-color: #888888; border-top: none; border-bottom: none; border-right: 1px solid #bbbbbb;");
                        } else {
                            cell->setStyleSheet("background-color: #aaaaaa; border-top: none; border-bottom: none; border-right: 1px solid #bbbbbb;");
                        }

                        if (col == 6) {
                            if (row % 2 == 0) {
                                cell->setStyleSheet("padding: 0px; background-color: LightSkyBlue; border: none;");
                            } else {
                                cell->setStyleSheet("padding: 0px; background-color: LightBlue; border: none;");
                            }
                        }

                        if (col == 7) {
                            if (row % 2 == 0) {
                                cell->setStyleSheet("padding: 0px; background-color: PaleTurquoise; border: none;");
                            } else {
                                cell->setStyleSheet("padding: 0px; background-color: LightCyan; border: none;");
                            }
                        }

                    }
                }
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
        int rowsToSpan = durationInMinutes / 15; // 15 minut na komórkę

        for (int i = 0; i < rowsToSpan; ++i) {
            int currentRow = startRow + i;
            if(currentRow >= layout->rowCount()) break;

            QLayoutItem *item = layout->itemAtPosition(currentRow, dayIndex);
            if (item) {
                QWidget *widget = item->widget();
                if (widget) {
                    QTextEdit *cell = dynamic_cast<QTextEdit*>(widget);
                    if (cell) {
                        QString taskText = (i == 0) ? (task.person + "\n" + task.title) : "";
                        cell->setPlainText(taskText);

                        QTextCharFormat format;
                        format.setForeground(Qt::black);
                        QTextBlockFormat blockFormat;
                        blockFormat.setAlignment(Qt::AlignCenter);
                        QTextCursor cursor = cell->textCursor();
                        cursor.select(QTextCursor::Document);
                        cursor.setCharFormat(format);
                        cursor.setBlockFormat(blockFormat);
                        cursor.clearSelection();

                        if (task.completed == 1) {
                            cell->setStyleSheet("background: #B0B0B0; border: none;");
                        } else {
                            QString personColor = getColorForPerson(task.person);
                            cell->setStyleSheet(QString("background-color: %1; border: 1px %1;").arg(personColor));
                        }
                        cell->setProperty("taskId", task.id);
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

    int row = (hour - 6) * 4 + (minute / 15);
    return row + 2;
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