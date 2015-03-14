#include "DeamonDatabase.h"
#include "Define.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

DeamonDatabase*
DeamonDatabase::singleton = NULL;

DeamonDatabase*
DeamonDatabase::getInstance ()
{

    if (NULL == singleton)
    {

        singleton = new DeamonDatabase();
    }

    return(singleton);
}

DeamonDatabase::DeamonDatabase (
        QObject * parent)
    : QObject(parent)
{
    sqlDatabase = QSqlDatabase::addDatabase(DATABASE_TYPE, DATABASE_CONNECTION);

    sqlDatabase.setHostName(DATABASE_HOST);

    sqlDatabase.setDatabaseName(DATABASE_NAME);
    sqlDatabase.setUserName(DATABASE_USER);
    sqlDatabase.setPassword(DATABASE_PASSWORD);


    if (false == sqlDatabase.open())
    {

        qDebug() <<" Unable to open sqlDatabase " << sqlDatabase.lastError();
    }

    QSqlQuery sqlQuery (sqlDatabase);

    if (false == (sqlQuery.exec (CREATE_TABLE_DATA)))
    {

        qDebug () << "Data table not created" << sqlQuery.lastError ();
    }
}

bool
DeamonDatabase::closeDatabase()
{

    sqlDatabase.commit();
    sqlDatabase.close();

    sqlDatabase = QSqlDatabase ();
    sqlDatabase.removeDatabase (DATABASE_CONNECTION);

    return(true);
}

int
DeamonDatabase::addData(
        const Data& data)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (INSERT_DATA);
    sqlQuery.bindValue (DOWNLOAD, data.getDownload ());
    sqlQuery.bindValue (UPLOAD, data.getUpload ());
    sqlQuery.bindValue (START, data.getStart ());

    if (false == sqlQuery.exec ())
    {

        qDebug () << "SQL Unable to add Data" << sqlQuery.lastError ();
        return (-1);
    }

    sqlQuery.clear ();
    if (false == sqlQuery.exec (GET_INDEX))
    {

        qDebug () << "SQL Unable to add Data" << sqlQuery.lastError ();
        return (-1);

    }
    else
    {

        sqlQuery.first ();
        return (sqlQuery.value (0).toInt ());
    }
}

Data
DeamonDatabase::checkData ()
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    bool result = sqlQuery.exec (CHECK_DATA);
    Data data;

    if (false == result)
    {

        qDebug () << "SQL Unable to check data" << sqlQuery.lastError ();
    }
    else
    {

        while ( true == sqlQuery.next ())
        {

            data.addUpload (sqlQuery.value (0).toDouble ());
            data.addDownload (sqlQuery.value (1).toDouble ());
        }
    }

    return (data);
}
