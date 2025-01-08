#include "TaskEditWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

TaskEditWindow::TaskEditWindow(Database& db, int taskId, QWidget *parent)
        : QWidget(parent), database(db), taskId(taskId) {

    setWindowTitle("Edytuj zadanie");
    setFixedSize(300, 400);
    this->setStyleSheet("background-color: lightgray;");

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
    taskDueDateEdit->setDisplayFormat("yyyy-MM-dd");
    taskDueDateEdit->setCalendarPopup(true);

    // Widget do wyboru godziny rozpoczęcia zadania
    taskStartTimeEdit = new QTimeEdit(this);
    taskStartTimeEdit->setDisplayFormat("HH:mm");

    // Widget do wyboru czasu trwania zadania
    taskTimeEdit = new QTimeEdit(this);
    taskTimeEdit->setDisplayFormat("HH:mm");

    // Przycisk zapisz
    saveButton = new QPushButton("Zapisz", this);
    connect(saveButton, &QPushButton::clicked, this, &TaskEditWindow::saveTask);

    // Przycisk usuń
    deleteButton = new QPushButton("Usuń", this);
    connect(deleteButton, &QPushButton::clicked, this, &TaskEditWindow::deleteTask);

    // Przycisk anuluj
    cancelButton = new QPushButton("Anuluj", this);
    connect(cancelButton, &QPushButton::clicked, this, &TaskEditWindow::close);

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
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    setLayout(layout);

    // Załaduj dane zadania do pól tekstowych
    setTaskDetails(taskId);
}

void TaskEditWindow::setTaskDetails(int taskId) {
    QSqlQuery query;
    query.prepare("SELECT person, title, description, due_date, start_time, time FROM tasks WHERE id = :id");
    query.bindValue(":id", taskId);

    if (query.exec() && query.next()) {
        taskPersonEdit->setText(query.value("person").toString());
        taskTitleEdit->setText(query.value("title").toString());
        taskDescriptionEdit->setText(query.value("description").toString());
        taskDueDateEdit->setDate(query.value("due_date").toDate());
        taskStartTimeEdit->setTime(query.value("start_time").toTime());
        taskTimeEdit->setTime(query.value("time").toTime());
    }
}

void TaskEditWindow::saveTask() {
    QString person = taskPersonEdit->text();
    QString title = taskTitleEdit->text();
    QString description = taskDescriptionEdit->text();
    QString dueDate = taskDueDateEdit->date().toString("yyyy-MM-dd");
    QString startTime = taskStartTimeEdit->time().toString("HH:mm");
    QString time = taskTimeEdit->time().toString("HH:mm");

    if (title.isEmpty() || person.isEmpty() || dueDate.isEmpty() || time.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Nie mogą być puste.");
        return;
    }

    if (database.updateTask(taskId, person, title, description, dueDate, startTime, time, 0)) {
        QMessageBox::information(this, "Sukces", "Zadanie zostało zaktualizowane.");
        close();
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się zaktualizować zadania.");
    }
}

void TaskEditWindow::deleteTask() {
    if (database.deleteTask(taskId)) {
        QMessageBox::information(this, "Sukces", "Zadanie zostało usunięte.");
        close();
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć zadania.");
    }
}
