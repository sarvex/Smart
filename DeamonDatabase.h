#ifndef DEAMONDATABASE_H
#define DEAMONDATABASE_H

#include "Data.h"
#include <QObject>
#include <QSqlDatabase>

class DeamonDatabase : public QObject
{
    Q_OBJECT
public:
    static DeamonDatabase* getInstance ();

    bool initilize();
    bool closeDatabase();

    int addData (const Data& data);
    Data checkData ();

signals:

public slots:

protected:

private:
    explicit DeamonDatabase(QObject *parent = 0);
    static DeamonDatabase* singleton;

    QSqlDatabase sqlDatabase;
};

#endif // DEAMONDATABASE_H
