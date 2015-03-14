#ifndef DEAMON_CONNECTION
#define DEAMON_CONNECTION

#include <QObject>
#include <QTime>
#include <QTimer>

class DeamonConnection
        : public QObject
{
    Q_OBJECT

public:
    static DeamonConnection* getInstance ();


    bool connectDataCall ();
    bool disconnectDataCall ();

    bool createConnectionEntry ();
    bool removeConnectionEntry ();

    bool error (int errorNumber, QString errorMessage);
    bool connected (bool status);

signals:
    bool modemConnected (bool status);

    bool rasError (int errorNumber, QString errorMessage);

    bool usageUpdate (double upload, double download);
    bool speedUpdate (double upload, double download);
    bool durationUpdate (QTime duration);

public slots:
    bool onResetUsage ();
    bool onTimer ();

    bool onModemConnected (bool status);

protected:

private:
    explicit DeamonConnection (QObject *parent = 0);
    static DeamonConnection* singleton;

    QString getModemName ();

    double bytesTransmitted;
    double bytesRecieved;

    double transmitOffset;
    double recieveOffset;

    double duration;
    double durationOffset;

    QTimer * timer;
};

#endif // DEAMON_CONNECTION
