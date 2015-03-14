#ifndef DEAMONINTERFACE_H
#define DEAMONINTERFACE_H

#include "SerialPort.h"
#include "Define.h"

#include <QMutex>
#include <QQueue>
#include <QTimer>
#include <QThread>
#include <QWaitCondition>

class DeamonInterface : public QThread
{
    Q_OBJECT
public:
    static DeamonInterface* getInstance ();
    ~DeamonInterface ();

    bool write (QString value);
    bool enqueue (QString value);

    bool configure ();
    bool stop ();

signals:
    bool modemPlugged (bool status);
    void activation(bool status);

    bool signalStrengthUpdate(int value);
    bool serviceStatusUpdate(int value);
    bool serviceDomainUpdate(int value);
    bool roamingStatusUpdate(int value);
    bool systemModeUpdate(int value);
    bool simStatusUpdate(int value);

    bool prlUpdate(const QString& value);
    bool meidUpdate(const QString& value);
    bool hardwareVersionUpdate(const QString& value);
    bool compileDateUpdate(const QString& value);
    bool esnUpdate(const QString& value);
    bool softwareVersionUpdate(const QString& value);
    bool cdmaEcioUpdate(const QString& value);
    bool hdrEcioUpdate(const QString& value);
    bool hdrcUpdate(const QString& value);
    bool sidUpdate(const QString& value);
    bool nidUpdate(const QString& value);
    bool sectorIdUpdate(const QString& value);

    bool cdmaRssiUpdate(const QString& value);
    bool pnOffsetUpdate(const QString& value);
    bool walshCodeUpdate(const QString& value);
    bool channelNumberUpdate(const QString& value);
    bool bandClassUpdate(const QString& value);
    bool txPowerUpdate(const QString& value);
    bool rxPowerUpdate(const QString& value);
    bool bsPreviousUpdate(const QString& value);
    bool msPreviousUpdate(const QString& value);
    bool previousInUseUpdate(const QString& value);
    bool packetZoneIdUpdate(const QString& value);
    bool lastActiveCallStateUpdate(const QString& value);
    bool hdrRssiUpdate(const QString& value);
    bool perUpdate(const QString& value);
    bool atStateUpdate(const QString& value);
    bool dormantStateUpdate(const QString& value);
    bool activeStatePnOffsetUpdate(const QString& value);
    bool neighborSetPnOffsetUpdate(const QString& value);
    bool neighborSetEcioUpdate(const QString& value);

    bool flashMessageUpdate(const QString& value);
    bool defaultMinUpdate();

public slots:
    bool onSystemUpdateTimer ();

protected:
    virtual void run ();

private:
    explicit DeamonInterface(QObject *parent = 0);
    static DeamonInterface* singleton;

    QString getName (QString number);
    bool removeJunk (QString& data);

    bool checkPort ();
    bool openPort ();

    bool write ();
    bool read ();
    bool parse (const QByteArray& value);


    QString portName;
    SerialPort * port;

    QMutex * mutex;
    QQueue <QString> queue;
    QQueue <QString> waited;

    QTimer * systemInformationTimer;

    volatile bool running;

    bool plugged;

    bool incomingCall;
    bool outgoingCall;

    static const QString end;

};

#endif // DEAMONINTERFACE_H
