#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


void initializeDatabase() {
    // Tworzenie połączenia z bazą danych SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.sqlite"); // Lokalny plik bazy danych

    if (!db.open()) {
        qDebug() << "Nie udało się otworzyć bazy danych:" << db.lastError().text();
        return;
    }

    // Tworzenie tabeli w bazie danych
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS tasks ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "person TEXT NOT NULL, "
                    "title TEXT NOT NULL, "
                    "description TEXT, "
                    "due_date DATE, "
                    "time TIME, "
                    "completed INTEGER DEFAULT 0)"))
    {
        qDebug() << "Blad przy tworzeniu tabeli:" << query.lastError().text();
    } else {
        qDebug() << "Tabela tasks zostala utworzona!";
    }
}


bool Database::addTask(const QString& person, const QString &title, const QString &description,  const QString &due_date,  const QString &time) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (person, title, description, completed, due_date, time) VALUES (:person, :title, :description, 0, :due_date, :time)");
    query.bindValue(":person", person);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":due_date", due_date);
    query.bindValue(":time", time);

    if (!query.exec()) {
        qDebug() << "Blad przy dodawaniu zadania:" << query.lastError().text();
        return false;
    }

    qDebug() << "Zadanie zostalo dodane!";
    return true;
}

/*
bool addTask(const QString& person, const QString &title, const QString &description, const QString &dueDate) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (person, title, description, due_date, completed) VALUES (:person, :title, :description, :dueDate, 0)");
    query.bindValue(":person", person);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":due_date", dueDate);

    if (!query.exec()) {
        qDebug() << "Blad przy dodawaniu zadania:" << query.lastError().text();
        return false;
    }

    qDebug() << "Zadanie zostalo dodane!";
    return true;
}*/
