#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

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
            return QTime(); // Zwróć nieprawidłowy czas
        }

        QTime startTime = timeValue.toTime();
        return startTime;
    }

    qDebug() << "Nie znaleziono zadania:" << taskTitle << "dla daty:" << date;
    return QTime(); // Zwróć nieprawidłowy czas
}

// Metoda do pobierania zadań z bazy danych na dany tydzień
QList<Task> Database::getTasksForWeek(const QDate &startOfWeek) {
    QList<Task> tasks;

    QSqlQuery query;
    // Zapytanie SQL pobierające zadania z tabeli dla danego tygodnia
    query.prepare("SELECT id, person, title, description, due_date, start_time, time, completed "
                  "FROM tasks WHERE due_date >= :startOfWeek AND due_date < :endOfWeek");
    query.bindValue(":startOfWeek", startOfWeek);
    query.bindValue(":endOfWeek", startOfWeek.addDays(7));

    if (query.exec()) {
        while (query.next()) {
            Task task;
            task.id = query.value("id").toInt();                        // Pobieranie id zadania
            task.person = query.value("person").toString();             // Pobieranie osoby odpowiedzialnej
            task.title = query.value("title").toString();               // Pobieranie tytułu
            task.description = query.value("description").toString();   // Pobieranie opisu
            task.dueDate = query.value("due_date").toDate();            // Pobieranie daty wykonania
            task.startTime = query.value("start_time").toTime();        // Pobieranie godziny rozpoczęcia
            task.time = query.value("time").toTime();                   // Pobieranie czasu trwania
            task.completed = query.value("completed").toInt();          // Pobieranie statusu ukończenia
            tasks.append(task);
        }
    } else {
        qDebug() << "Błąd zapytania SQL:" << query.lastError();
    }

    return tasks;
}

bool Database::deleteTask(int taskId) {
    QSqlQuery query;
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", taskId);

    if (!query.exec()) {
        qDebug() << "Błąd podczas usuwania zadania:" << query.lastError().text();
        return false;
    }

    qDebug() << "Zadanie o ID" << taskId << "zostało usunięte!";
    return true;
}

bool Database::updateTask(int taskId, const QString &person, const QString &title, const QString &description, const QString &dueDate, const QString &startTime, const QString &time, int completed) {
    QSqlQuery query;
    query.prepare("UPDATE tasks SET "
                  "person = :person, "
                  "title = :title, "
                  "description = :description, "
                  "due_date = :dueDate, "
                  "start_time = :startTime, "
                  "time = :time, "
                  "completed = :completed "
                  "WHERE id = :taskId");
    query.bindValue(":person", person);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":dueDate", dueDate);
    query.bindValue(":startTime", startTime);
    query.bindValue(":time", time);
    query.bindValue(":completed", completed);
    query.bindValue(":taskId", taskId);

    if (!query.exec()) {
        qDebug() << "Błąd podczas edycji zadania:" << query.lastError().text();
        return false;
    }

    qDebug() << "Zadanie o ID" << taskId << "zostało zaktualizowane!";
    return true;
}

QTime Database::getTaskDuration(int taskId) {
    QSqlQuery query;
    query.prepare("SELECT time FROM tasks WHERE id = :id");
    query.bindValue(":id", taskId);

    // Wykonaj zapytanie
    if (!query.exec()) {
        qDebug() << "Błąd podczas pobierania czasu trwania zadania:" << query.lastError();
        return QTime(); // Zwraca nieprawidłowy czas w przypadku błędu
    }

    // Sprawdź, czy znaleziono czas
    if (query.next()) {
        QVariant durationValue = query.value(0); // Pobierz wartość z kolumny `time`

        // Upewnij się, że wartość nie jest NULL
        if (!durationValue.isNull()) {
            QTime duration = durationValue.toTime(); // Konwertuj na QTime
            if (duration.isValid()) {
                return duration; // Zwraca prawidłowy czas
            } else {
                qDebug() << "Czas w bazie danych jest nieprawidłowy.";
            }
        } else {
            qDebug() << "Pole czasu trwania jest puste dla zadania o ID:" << taskId;
        }
    } else {
        qDebug() << "Nie znaleziono zadania o ID:" << taskId;
    }

    return QTime();
}


