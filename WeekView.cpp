#include "WeekView.h"
#include "TaskWindow.h"
#include "database.h"
#include <QGridLayout>

WeekView::WeekView(Database &db, QWidget *parent) : QWidget(parent), taskAddWindow(nullptr), database(db)  {
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

    for (int row = 0; row < hours.size(); ++row) {
        for (int col = 0; col < days.size(); ++col) {
            QPushButton *cell = new QPushButton(this);
            cell->setText("");  // Puste komórki na początku
            cell->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            layout->addWidget(cell, row + 2, col + 1);

            // Oblicz datę i czas dla tej komórki
            QDate cellDate = currentWeekStartDate.addDays(col);  // Kolumna to dzień tygodnia
            QTime cellTime = QTime(6 + row * 2, 0);              // Wiersz to godzina (np. 06:00 + 2h na wiersz)

            // Zapisz datę i godzinę w mapie
            cellDateTimeMap[cell] = qMakePair(cellDate, cellTime);

            // Po kliknięciu otwórz okno dodawania zadania z odpowiednią datą i godziną
            connect(cell, &QPushButton::clicked, this, [this, cell]() {
                QPair<QDate, QTime> dateTime = cellDateTimeMap[cell];
                showTaskAddWindow(dateTime.first, dateTime.second);
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
    // Czyszczenie widoku zadań, żeby nie wyświetlały się z innych tygodni
    for (int row = 2; row < layout->rowCount(); ++row) {
        for (int col = 1; col < layout->columnCount(); ++col) {
            QPushButton *button = dynamic_cast<QPushButton*>(layout->itemAtPosition(row, col)->widget());
            if (button) {
                button->setText("");  // Usunięcie tekstu z przycisku (czyli czyszczenie zadań)

                // Oblicz datę i czas dla tej komórki
                QDate cellDate = currentWeekStartDate.addDays(col-1);  // Kolumna to dzień tygodnia
                QTime cellTime = QTime(6 + ((row - 2) * 2), 0);              // Wiersz to godzina (np. 06:00 + 2h na wiersz)

                // Zapisz datę i godzinę w mapie
                cellDateTimeMap[button] = qMakePair(cellDate, cellTime);
            }
        }
    }

    // Aktualizacja etykiet dni tygodnia z datami
    for (int i = 0; i < 7; ++i) {
        QDate dayDate = currentWeekStartDate.addDays(i);
        dayLabels[i]->setText(dayDate.toString("yyyy-MM-dd"));
    }

    // Aktualizacja tekstu przycisku zakresu dat
    QDate endDate = currentWeekStartDate.addDays(6);
    dateRangeButton->setText(currentWeekStartDate.toString("dd.MM") + " - " + endDate.toString("dd.MM"));

    // Wywołanie funkcji wyświetlającej zadania na dany tydzień
    displayTasksForWeek();
}

void WeekView::showTaskAddWindow(const QDate &date, const QTime &time) {
    if (!taskAddWindow) {
        taskAddWindow = new TaskAddWindow(this);
        // Ustawienia pozycji okna
        taskAddWindow->move(this->geometry().center() - taskAddWindow->rect().center());
    } else {
        qDebug() << "Okno TaskAddWindow już istnieje.";
    }

    connect(taskAddWindow, &TaskAddWindow::taskAdded, this, [this](const QString &person, const QString &title, const QString &description) {
        qDebug() << "Dodano zadanie: " << title << ", " << description;
    });

    connect(taskAddWindow, &TaskAddWindow::taskAdded, this, &WeekView::updateCalendar);

    // Sprawdzenie, czy taskAddWindow jest null lub nie
    if (!taskAddWindow) {
        qDebug() << "taskAddWindow jest nullptr";
        return;
    }

    taskAddWindow->setInitialDateTime(date, time);

    // Jeżeli okno nie jest widoczne, to je pokazujemy
    if (!taskAddWindow->isVisible()) {
        taskAddWindow->show();
    }

    taskAddWindow->raise();
    taskAddWindow->activateWindow();
}

void WeekView::displayTasksForWeek() {
    // Wyczyść poprzednie zadania z przycisków
    for (int row = 0; row < layout->rowCount() - 2; ++row) { // -2, aby ominąć nagłówki
        for (int col = 0; col < layout->columnCount() - 1; ++col) { // -1, aby ominąć godziny
            QPushButton *button = dynamic_cast<QPushButton*>(layout->itemAtPosition(row + 2, col + 1)->widget());
            if (button) {
                button->setText(""); // Czyść tekst w komórkach
                disconnect(button, nullptr, nullptr, nullptr); // Usuń wszystkie poprzednie połączenia

                // Przywrócenie akcji dla pustych okienek
                QDate cellDate = currentWeekStartDate.addDays(col);
                QTime cellTime = QTime(6 + row * 2, 0);
                connect(button, &QPushButton::clicked, this, [this, cellDate, cellTime]() {
                    showTaskAddWindow(cellDate, cellTime); // Otwórz okno dodawania zadania
                });
            }
        }
    }

    // Pobierz zadania dla bieżącego tygodnia
    QList<Task> tasks = database.getTasksForWeek(currentWeekStartDate);

    // Przypisz zadania do odpowiednich komórek
    for (const Task &task : tasks) {
        // Określenie, która komórka odpowiada za ten dzień i godzinę
        int dayIndex = currentWeekStartDate.daysTo(task.dueDate); // Kolumna na podstawie dnia
        if (dayIndex < 0 || dayIndex >= 7) {
            continue; // Pomijamy zadania, które nie pasują do tego tygodnia
        }

        // Określ wiersz w siatce na podstawie godziny zadania
        int row = calculateRowForTime(task.dueDate, QPair<QString, QString>(task.time.toString(), task.title));
        if (row < 0) {
            continue; // Pomijamy zadania poza zakresem godzin
        }

        // Znajdź odpowiedni przycisk w siatce
        QPushButton *button = dynamic_cast<QPushButton*>(layout->itemAtPosition(row + 2, dayIndex + 1)->widget());
        if (button) {
            // Dodaj informacje o osobie odpowiedzialnej i tytule zadania
            QString taskText = QString("%1\n%2") // Osoba i tytuł w dwóch liniach
                    .arg(task.person) // Imię osoby
                    .arg(task.title); // Tytuł zadania

            // Jeżeli już jest tekst, dodaj nową linię
            if (!button->text().isEmpty()) {
                taskText = button->text() + "\n" + taskText;
            }

            button->setText(taskText);

            // Dodaj opcję usuwania zadania po kliknięciu
            connect(button, &QPushButton::clicked, this, [this, task, button]() {
                QMessageBox::StandardButton reply = QMessageBox::question(
                        this, "Usuń zadanie",
                        QString("Czy na pewno chcesz usunąć to zadanie: \"%1\"?\nOsoba: %2")
                                .arg(task.title)
                                .arg(task.person),
                        QMessageBox::Yes | QMessageBox::No
                );

                if (reply == QMessageBox::Yes) {
                    // Usuń zadanie z bazy danych
                    if (database.removeTask(task.dueDate, task.title)) {
                        QMessageBox::information(this, "Sukces", "Zadanie zostało usunięte.");
                        button->setText(""); // Wyczyść tekst z komórki
                    } else {
                        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć zadania z bazy danych.");
                    }

                    // Odśwież widok kalendarza
                    updateCalendar();
                }
            });
        }
    }
}




// Funkcja pomocnicza do obliczenia wiersza siatki na podstawie godziny zadania
int WeekView::calculateRowForTime(const QDate &date, const QPair<QString, QString> &task) {
    // Pobierz godzinę rozpoczęcia z bazy danych
    QTime startTime = database.getTaskStartTimeForDate(date, task.second);
    if (!startTime.isValid()) {
        qDebug() << "Nieprawidłowa godzina dla zadania: " << task.second
                 << "Data: " << date
                 << "StartTime: " << startTime;
        return -1;
    }

    int hour = startTime.hour();
    if (hour < 6 || hour >= 22) {
        qDebug() << "Zadanie poza zakresem godzin siatki: " << startTime;
        return -1; // Zadanie poza zakresem siatki
    }

    // Mapowanie godzin na wiersze siatki
    int row = (hour - 6) / 2 + 2; // Dodaj +2, aby uwzględnić nagłówk
    return row;
}