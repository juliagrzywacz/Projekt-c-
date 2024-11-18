#include "TaskWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

TaskAddWindow::TaskAddWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Dodaj zadanie");
    setFixedSize(300, 200);

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

    if (title.isEmpty() || person.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Tytuł zadania i osoba nie mogą być puste.");
        return;
    }
    this->database.addTask(person, title, description);
    emit taskAdded(person, title, description);  // Wysyłanie sygnału

    QMessageBox::information(this, "Sukces", "Zadanie zostało dodane.");
    close();
}
