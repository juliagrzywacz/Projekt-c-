#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

void initializeDatabase();

class Database {
    public:
        bool addTask(const QString &person, const QString &title, const QString &description, const QString &due_date, const QString &start_time, const QString &time);

};

#endif // DATABASE_H
