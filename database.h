#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QDate>
#include <QList>
#include <QPair>

void initializeDatabase();

class Database {
    public:
    QList<QPair<QString, QString>> getTasksForDate(const QDate &date);
    QTime getTaskStartTimeForDate(const QDate &date, const QString &title);
    static bool addTask(const QString &person, const QString &title, const QString &description, const QString &due_date, const QString &start_time, const QString &time);

};

#endif // DATABASE_H
