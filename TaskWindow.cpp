#include "TaskWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "database.h"
#include <QDebug>
#include <QTimeEdit>

TaskAddWindow::TaskAddWindow(Database& database, QWidget *parent)
        : QWidget(parent), database(database) {
    setWindowTitle("Dodaj zadanie");
    setFixedSize(340, 440);

    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor("#D3D3D3")); // Jasnoszare tło
    setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);

    // Style CSS (teraz tło okienek jest lightblue)
    QString inputStyle = "QLineEdit, QDateEdit, QTimeEdit {"
                         "background-color: lightblue;" // Lightblue tło
                         "border: 1px solid lightblue;" // Lightblue border
                         "border-radius: 5px;"
                         "padding: 6px;"
                         "font-size: 14px;"
                         "}"
                         "QLineEdit:focus, QDateEdit:focus, QTimeEdit:focus {"
                         "border: 1px solid #4682B4;"
                         "}";

    QString buttonStyle = "QPushButton {"
                          "background-color: lightblue;" // Lightblue
                          "border: none;"
                          "border-radius: 5px;"
                          "padding: 8px 15px;"
                          "font-size: 14px;"
                          "font-weight: bold;"
                          "color: white;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #87CEEB;"
                          "}"
                          "QPushButton:pressed {"
                          "background-color: #6495ED;"
                          "}";

    auto createLabel = [](const QString& text) {
        QLabel* label = new QLabel(text);
        label->setStyleSheet("font-weight: bold;");
        return label;
    };

    taskPersonEdit = new QLineEdit(this);
    taskPersonEdit->setPlaceholderText("Wpisz osobę odpowiedzialną");
    taskPersonEdit->setStyleSheet(inputStyle);

    taskTitleEdit = new QLineEdit(this);
    taskTitleEdit->setPlaceholderText("Wpisz tytuł zadania");
    taskTitleEdit->setStyleSheet(inputStyle);

    taskDescriptionEdit = new QLineEdit(this);
    taskDescriptionEdit->setPlaceholderText("Wpisz opis zadania");
    taskDescriptionEdit->setStyleSheet(inputStyle);

    taskDueDateEdit = new QDateEdit(this);
    taskDueDateEdit->setDisplayFormat("yyyy-MM-dd");
    taskDueDateEdit->setCalendarPopup(true);
    taskDueDateEdit->setStyleSheet(inputStyle);

    taskStartTimeEdit = new QTimeEdit(this);
    taskStartTimeEdit->setDisplayFormat("HH:mm");
    taskStartTimeEdit->setStyleSheet(inputStyle);

    taskTimeEdit = new QTimeEdit(this);
    taskTimeEdit->setDisplayFormat("HH:mm");
    taskTimeEdit->setStyleSheet(inputStyle);

    layout->addWidget(createLabel("Osoba odpowiedzialna:"));
    layout->addWidget(taskPersonEdit);
    layout->addWidget(createLabel("Tytuł zadania:"));
    layout->addWidget(taskTitleEdit);
    layout->addWidget(createLabel("Opis zadania:"));
    layout->addWidget(taskDescriptionEdit);
    layout->addWidget(createLabel("Termin zadania:"));
    layout->addWidget(taskDueDateEdit);
    layout->addWidget(createLabel("Godzina rozpoczęcia zadania:"));
    layout->addWidget(taskStartTimeEdit);
    layout->addWidget(createLabel("Czas trwania zadania:"));
    layout->addWidget(taskTimeEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(saveButton = new QPushButton("Zapisz", this));
    buttonLayout->addWidget(cancelButton = new QPushButton("Anuluj", this));
    buttonLayout->addStretch(1);

    saveButton->setStyleSheet(buttonStyle);
    connect(saveButton, &QPushButton::clicked, this, &TaskAddWindow::saveTask);

    cancelButton->setStyleSheet(buttonStyle);
    connect(cancelButton, &QPushButton::clicked, this, &TaskAddWindow::close);

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