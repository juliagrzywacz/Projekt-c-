#include "TaskWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

TaskAddWindow::TaskAddWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Dodaj zadanie");
    setFixedSize(300, 400);

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
    taskDueDateEdit = new QDateEdit(QDate::currentDate(), this);  // Domyślnie ustawia dzisiejszą datę
    taskDueDateEdit->setDisplayFormat("yyyy-MM-dd");
    taskDueDateEdit->setCalendarPopup(true);

    // Widget do wyboru czasu
    taskTimeEdit = new QTimeEdit(QTime::currentTime(), this);  // Domyślnie ustawia bieżący czas
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
    layout->addWidget(new QLabel("Czas zadania:"));
    layout->addWidget(taskTimeEdit);

    // Dodanie przycisków
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    setLayout(layout);
}

void TaskAddWindow::saveTask() {
    QString person = taskPersonEdit->text();
    QString title = taskTitleEdit->text();
    QString description = taskDescriptionEdit->text();
    QString dueDate = taskDueDateEdit->date().toString("yyyy-MM-dd");
    QString time = taskTimeEdit->time().toString("HH:mm");

    if (title.isEmpty() || person.isEmpty() || taskDueDateEdit->date().isNull() || time.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Nie mogą być puste.");
        return;
    }
    this->database.addTask(person, title, description, dueDate, time);
    emit taskAdded(person, title, description, dueDate, time);  // Wysyłanie sygnału

    QMessageBox::information(this, "Sukces", "Zadanie zostało dodane.");
    close();
}
