#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QPair>


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
                    "start_time TIME, "
                    "time TIME, "
                    "completed INTEGER DEFAULT 0)"))
    {
        qDebug() << "Blad przy tworzeniu tabeli:" << query.lastError().text();
    } else {
        qDebug() << "Tabela tasks zostala utworzona!";
    }
}


bool Database::addTask(const QString& person, const QString &title, const QString &description,  const QString &due_date, const QString &start_time,  const QString &time) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (person, title, description, due_date, start_time, time, completed) VALUES (:person, :title, :description, :due_date, :start_time, :time, 0)");
    query.bindValue(":person", person);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":due_date", due_date);
    query.bindValue(":start_time", start_time);
    query.bindValue(":time", time);

    if (!query.exec()) {
        qDebug() << "Blad przy dodawaniu zadania:" << query.lastError().text();
        return false;
    }

    qDebug() << "Zadanie zostalo dodane!";
    return true;
}

QList<QPair<QString, QString>> Database::getTasksForDate(const QDate &date) {
    QList<QPair<QString, QString>> tasks;

    QSqlQuery query;
    query.prepare("SELECT person, title FROM tasks WHERE due_date = :due_date");
    query.bindValue(":due_date", date.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "Błąd podczas wykonywania zapytania getTasksForDate:" << query.lastError().text();
        return tasks;
    }

    while (query.next()) {
        QString person = query.value("person").toString();
        QString title = query.value("title").toString();
        tasks.append(qMakePair(person, title));
    }

    return tasks;
}

QTime Database::getTaskStartTimeForDate(const QDate &date, const QString &taskTitle) {
    QSqlQuery query;
    query.prepare("SELECT start_time FROM tasks WHERE due_date = :due_date AND title = :title");
    query.bindValue(":due_date", date.toString("yyyy-MM-dd"));
    query.bindValue(":title", taskTitle);

    if (!query.exec()) {
        qDebug() << "Błąd podczas wykonywania zapytania getTaskStartTimeForDate:" << query.lastError();
        return QTime(); // Zwróć nieprawidłowy czas
    }

    if (query.next()) {
        QVariant timeValue = query.value(0);
        if (timeValue.isNull()) {
            qDebug() << "Brak godziny rozpoczęcia dla zadania:" << taskTitle << "z datą:" << date;
            return QTime(); // Zwróć nieprawidłowy czas
        }

        QTime startTime = timeValue.toTime();
        qDebug() << "Godzina rozpoczęcia dla zadania:" << taskTitle << "to:" << startTime;
        return startTime;
    }

    qDebug() << "Nie znaleziono zadania:" << taskTitle << "dla daty:" << date;
    return QTime(); // Zwróć nieprawidłowy czas
}


