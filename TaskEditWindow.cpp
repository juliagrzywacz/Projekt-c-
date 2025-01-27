#include "TaskEditWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

TaskEditWindow::TaskEditWindow(Database& db, int taskId, QWidget *parent)
        : QWidget(parent), database(db), taskId(taskId) {

    setWindowTitle("Edytuj zadanie");
    setFixedSize(300, 400);

    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor("#D3D3D3"));
    setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);

    QString inputStyle = "QLineEdit, QDateEdit, QTimeEdit {"
                         "background-color: lightblue;"
                         "border: 1px solid lightblue;"
                         "border-radius: 5px;"
                         "padding: 2px;"
                         "font-size: 14px;"
                         "}"
                         "QLineEdit:focus, QDateEdit:focus, QTimeEdit:focus {"
                         "border: 1px solid #4682B4;"
                         "}";

    QString buttonStyle = "QPushButton {"
                          "background-color: lightblue;"
                          "border: none;"
                          "border-radius: 5px;"
                          "padding: 4px 15px;"
                          "font-size: 14px;"
                          "font-weight: bold;"
                          "color: DarkSlateGrey;"
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

    completedCheckbox = new QCheckBox("Zadanie ukończone", this);

    layout->addWidget(createLabel("Osoba odpowiedzialna za:"));
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
    layout->addWidget(completedCheckbox);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(saveButton = new QPushButton("Zapisz", this));
    buttonLayout->addWidget(deleteButton = new QPushButton("Usuń", this));
    buttonLayout->addWidget(cancelButton = new QPushButton("Anuluj", this));
    buttonLayout->addStretch(1);

    saveButton->setStyleSheet(buttonStyle);
    connect(saveButton, &QPushButton::clicked, this, &TaskEditWindow::saveTask);

    deleteButton->setStyleSheet(buttonStyle);
    connect(deleteButton, &QPushButton::clicked, this, &TaskEditWindow::deleteTask);

    cancelButton->setStyleSheet(buttonStyle);
    connect(cancelButton, &QPushButton::clicked, this, &TaskEditWindow::close);

    layout->addLayout(buttonLayout);
    setLayout(layout);

    setTaskDetails(taskId);
}

void TaskEditWindow::setTaskDetails(int taskId) {
    QSqlQuery query;
    query.prepare("SELECT person, title, description, due_date, start_time, time, completed FROM tasks WHERE id = :id");
    query.bindValue(":id", taskId);

    if (query.exec() && query.next()) {
        taskPersonEdit->setText(query.value("person").toString());
        taskTitleEdit->setText(query.value("title").toString());
        taskDescriptionEdit->setText(query.value("description").toString());
        taskDueDateEdit->setDate(query.value("due_date").toDate());
        taskStartTimeEdit->setTime(query.value("start_time").toTime());
        taskTimeEdit->setTime(query.value("time").toTime());
        completedCheckbox->setChecked(query.value("completed").toInt() == 1);
    }
}

void TaskEditWindow::saveTask() {
    QString person = taskPersonEdit->text();
    QString title = taskTitleEdit->text();
    QString description = taskDescriptionEdit->text();
    QString dueDate = taskDueDateEdit->date().toString("yyyy-MM-dd");
    QString startTime = taskStartTimeEdit->time().toString("HH:mm");
    QString time = taskTimeEdit->time().toString("HH:mm");
    int completed = completedCheckbox->isChecked() ? 1 : 0;


    if (title.isEmpty() || person.isEmpty() || dueDate.isEmpty() || time.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Nie mogą być puste.");
        return;
    }

    if (database.updateTask(taskId, person, title, description, dueDate, startTime, time, completed)) {
        QMessageBox::information(this, "Sukces", "Zadanie zostało zaktualizowane.");
        emit taskUpdated(taskId);
        close();
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się zaktualizować zadania.");
    }
}

void TaskEditWindow::deleteTask() {
    if (database.deleteTask(taskId)) {
        QMessageBox::information(this, "Sukces", "Zadanie zostało usunięte.");
        emit taskDeleted(taskId);
        close();
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć zadania.");
    }
}
