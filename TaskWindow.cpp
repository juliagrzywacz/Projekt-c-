#include "TaskWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "database.h"
#include <QDebug>
#include <QTimeEdit>

TaskAddWindow::TaskAddWindow(Database& database, QWidget *parent) : QWidget(parent), database(database) { // Zmiana: Inicjalizacja referencji
    setWindowTitle("Dodaj zadanie");
    setFixedSize(300, 400);
    this->setStyleSheet("background-color: lightgray;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    taskPersonEdit = new QLineEdit(this);
    taskPersonEdit->setPlaceholderText("Wpisz osobę odpowiedzialną");

    taskTitleEdit = new QLineEdit(this);
    taskTitleEdit->setPlaceholderText("Wpisz tytuł zadania");

    taskDescriptionEdit = new QLineEdit(this);
    taskDescriptionEdit->setPlaceholderText("Wpisz opis zadania");

    taskDueDateEdit = new QDateEdit(this);
    if (!taskDueDateEdit) {
        qDebug() << "taskDueDateEdit nie został poprawnie zainicjalizowany!";
    }
    taskDueDateEdit->setDisplayFormat("yyyy-MM-dd");
    taskDueDateEdit->setCalendarPopup(true);

    taskStartTimeEdit = new QTimeEdit(this);
    taskStartTimeEdit->setDisplayFormat("HH:mm");

    taskTimeEdit = new QTimeEdit(this);
    taskTimeEdit->setDisplayFormat("HH:mm");
    //taskTimeEdit->setTimeStep(15);
    //taskTimeEdit->setSingleStep(900);

    saveButton = new QPushButton("Zapisz", this);
    connect(saveButton, &QPushButton::clicked, this, &TaskAddWindow::saveTask);

    cancelButton = new QPushButton("Anuluj", this);
    connect(cancelButton, &QPushButton::clicked, this, &TaskAddWindow::close);

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
    layout->addWidget(new QLabel("Czas trwania zadania:"));
    layout->addWidget(taskTimeEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    setLayout(layout);
}

void TaskAddWindow::setInitialDateTime(const QDate &date, const QTime &time) {
    taskStartTimeEdit->setTime(time);
    if (taskDueDateEdit) {
        taskDueDateEdit->setDate(date);
    } else {
        qDebug() << "taskDueDateEdit nie jest zainicjowane!";
    }
}

void TaskAddWindow::saveTask() {
    Task newTask;
    newTask.person = taskPersonEdit->text();
    newTask.title = taskTitleEdit->text();
    newTask.description = taskDescriptionEdit->text();
    newTask.dueDate = taskDueDateEdit->date();
    newTask.startTime = taskStartTimeEdit->time();
    newTask.time = taskTimeEdit->time();

    if (newTask.title.isEmpty() || newTask.person.isEmpty() || newTask.dueDate.isNull() || newTask.time.isNull()) {
        QMessageBox::warning(this, "Błąd", "Pola tytuł, osoba odpowiedzialna, data i czas trwania nie mogą być puste.");
        return;
    }

    if (!database.addTask(newTask.person, newTask.title, newTask.description,
                          newTask.dueDate.toString("yyyy-MM-dd"),
                          newTask.startTime.toString("HH:mm"),
                          newTask.time.toString("HH:mm"))) {
        QMessageBox::critical(this, "Błąd", "Wystąpił błąd podczas dodawania zadania do bazy danych.");
        return;
    }

    emit taskAdded(); // Zmiana: Emituje sygnał bez parametrów
    QMessageBox::information(this, "Sukces", "Zadanie zostało dodane.");
    close();
}