#include "TaskWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

TaskAddWindow::TaskAddWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Dodaj zadanie");
    setFixedSize(300, 400);
    this->setStyleSheet("background-color: lightgray;");
    // Ustawienie layoutu
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Pole tekstowe dla osoby odpowiedzialnej
    taskPersonEdit = new QLineEdit(this);
    taskPersonEdit->setPlaceholderText("Wpisz osobę odpowiedzialną");

    // Pole tekstowe dla tytułu
    taskTitleEdit = new QLineEdit(this);
    taskTitleEdit->setPlaceholderText("Wpisz tytuł zadania");

    // Pole tekstowe dla opisu
    taskDescriptionEdit = new QLineEdit(this);
    taskDescriptionEdit->setPlaceholderText("Wpisz opis zadania");

    // Widget do wyboru daty
    taskDueDateEdit = new QDateEdit(this);
    if (!taskDueDateEdit) {
        qDebug() << "taskDueDateEdit nie został poprawnie zainicjalizowany!";
    }
    taskDueDateEdit->setDisplayFormat("yyyy-MM-dd");
    taskDueDateEdit->setCalendarPopup(true);

    // Widget do wyboru czasu rozpoczecia zadania
    taskStartTimeEdit = new QTimeEdit(this);  // Domyślnie ustawia bieżący czas
    taskStartTimeEdit->setDisplayFormat("HH:mm");

    // Widget do wyboru czasu
    taskTimeEdit = new QTimeEdit(this);  // Domyślnie ustawia bieżący czas
    taskTimeEdit->setDisplayFormat("HH:mm");

    // Przycisk zapisz
    saveButton = new QPushButton("Zapisz", this);
    connect(saveButton, &QPushButton::clicked, this, &TaskAddWindow::saveTask);

    // Przycisk anuluj
    cancelButton = new QPushButton("Anuluj", this);
    connect(cancelButton, &QPushButton::clicked, this, &TaskAddWindow::close);

    // Dodanie widgetów do layoutu
    layout->addWidget(new QLabel("Osoba odpowiedzialna za:"));
    layout->addWidget(taskPersonEdit);
    layout->addWidget(new QLabel("Tytuł zadania:"));
    layout->addWidget(taskTitleEdit);
    layout->addWidget(new QLabel("Opis zadania:"));
    layout->addWidget(taskDescriptionEdit);
    layout->addWidget(new QLabel("Termin zadania:"));
    layout->addWidget(taskDueDateEdit);
    layout->addWidget(new QLabel("Godzina rozpoczęcia zadania:"));
    layout->addWidget(taskStartTimeEdit);
    layout->addWidget(new QLabel("Czas  trwania zadania:"));
    layout->addWidget(taskTimeEdit);

    // Dodanie przycisków
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    setLayout(layout);
}

void TaskAddWindow::setInitialDateTime(const QDate &date, const QTime &time) {
    taskStartTimeEdit->setTime(time);
    if (taskDueDateEdit) {
        taskDueDateEdit->setDate(date);  // Ustawienie daty
    } else {
        qDebug() << "taskDueDateEdit nie jest zainicjowane!";
    }
}

void TaskAddWindow::saveTask() {
    QString person = taskPersonEdit->text();
    QString title = taskTitleEdit->text();
    QString description = taskDescriptionEdit->text();
    QString dueDate = taskDueDateEdit->date().toString("yyyy-MM-dd");
    qDebug() << dueDate;
    QString startTime = taskStartTimeEdit->time().toString("HH:mm");
    QString time = taskTimeEdit->time().toString("HH:mm");

    if (title.isEmpty() || person.isEmpty() || taskDueDateEdit->date().isNull() || time.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Nie mogą być puste.");
        return;
    }
    this->database.addTask(person, title, description, dueDate, startTime, time);
    emit taskAdded(person, title, description, dueDate, startTime, time);  // Wysyłanie sygnału

    QMessageBox::information(this, "Sukces", "Zadanie zostało dodane.");
    close();
}