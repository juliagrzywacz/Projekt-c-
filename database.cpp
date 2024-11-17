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
                    "name TEXT NOT NULL, "
                    "description TEXT, "
                    "due_date TEXT, "
                    "completed INTEGER DEFAULT 0)"))
    {
        qDebug() << "Blad przy tworzeniu tabeli:" << query.lastError().text();
    } else {
        qDebug() << "Tabela tasks zostala utworzona!";
    }
}

/*bool addTask(const QString& name, const QString& description, const QString& dueDate) {
    QSqlQuery query;

    // Przygotowanie zapytania SQL do dodania nowego zadania
    query.prepare("INSERT INTO tasks (name, description, due_date) VALUES (:name, :description, :due_date)");

    // Ustawianie wartości w zapytaniu
    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":due_date", dueDate);

    // Wykonanie zapytania
    if (!query.exec()) {
        qDebug() << "Błąd przy dodawaniu zadania:" << query.lastError().text();
        return false;
    }

    qDebug() << "Zadanie zostało dodane!";
    return true;
}*/

bool addTask(const QString &name, const QString &description, const QString &dueDate) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (name, description, due_date, completed) VALUES (:name, :description, :dueDate, 0)");
    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":due_date", dueDate);

    if (!query.exec()) {
        qDebug() << "Blad przy dodawaniu zadania:" << query.lastError().text();
        return false;
    }

    qDebug() << "Zadanie zostalo dodane!";
    return true;
}