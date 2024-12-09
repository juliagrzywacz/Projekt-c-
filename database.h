#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QDate>
#include <QList>
#include <QTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>


void initializeDatabase();

// Struktura Task, zawierająca kolumny z bazy danych
struct Task {
    int id;                // identyfikator zadania
    QString person;        // osoba odpowiedzialna
    QString title;         // tytuł zadania
    QString description;   // opis zadania
    QDate dueDate;         // termin wykonania zadania
    QTime startTime;       // czas rozpoczęcia zadania
    QTime time;            // czas trwania zadania
    bool completed;        // status ukończenia zadania (0 - nieukończone, 1 - ukończone)
};

class Database {
public:
    QTime getTaskStartTimeForDate(const QDate &date, const QString &title);
    static bool addTask(const QString &person, const QString &title, const QString &description, const QString &due_date, const QString &start_time, const QString &time);

    QList<Task> getTasksForWeek(const QDate &startOfWeek);

};

#endif // DATABASE_H
