#include "src/include/database.h"

Database::Database()
{
    DATABASE = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database()
{
    if (DATABASE.isOpen())
    {
        close();
    }
}

void Database::setFile(const QString & DATABASE_NAME)
{
    NAME = DATABASE_NAME;
    DATABASE.setDatabaseName(DATABASE_NAME);
}

bool Database::open()
{
    if (DATABASE.isOpen())
    {
        return true;
    }
    if (DATABASE.open())
    {
        qDebug() << "open database successfully";
        return true;
    } else {
        qDebug() << "failed to open database";
        return false;
    }
}

void Database::setPassword(const QString & PASSWORD)
{
    DATABASE.setPassword(PASSWORD);
}

bool Database::isOpen() const
{
    return DATABASE.isOpen();
}

void Database::close()
{
    DATABASE.close();
}

QSqlQuery Database::query() const
{
    QSqlQuery QUERY(DATABASE);
    return QUERY;
}

void Database::error() const
{
    qDebug() << DATABASE.lastError().text();
}

QString Database::name() const
{
    return NAME;
}
