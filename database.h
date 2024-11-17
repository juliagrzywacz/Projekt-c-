#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

void initializeDatabase();

bool addTask(const QString& name, const QString& description, const QString& dueDate);

#endif // DATABASE_H
