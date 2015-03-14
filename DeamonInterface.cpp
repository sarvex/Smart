#include "At.h"
#include "DeamonInterface.h"
#include "SerialEnumerator.h"

#include <QDebug>
#include <QTime>

DeamonInterface*
DeamonInterface::singleton = NULL;

DeamonInterface*
DeamonInterface::getInstance ()
{

    if (NULL == singleton)
    {

        singleton = new DeamonInterface();
    }

    return(singleton);
}

DeamonInterface::DeamonInterface(QObject *parent) : QThread(parent), running (true), plugged (false) {
    this->mutex = new QMutex();
    this->port = NULL;

    this->write (At::echoOff);
    this->write (At::enableFlashMessage);
    this->write (At::signalStrength);

    this->systemInformationTimer = new QTimer(this);
    connect(systemInformationTimer, SIGNAL(timeout()), SLOT(onSystemUpdateTimer()));
    this->systemInformationTimer->start(SIGNAL_STRENGTH_TIMEOUT);
}

DeamonInterface::~DeamonInterface () {
    this->stop ();

    if (NULL != this->mutex) delete this->mutex;
    if (NULL != this->port) delete this->port;
    if (NULL != this->systemInformationTimer) delete this->systemInformationTimer;
}

bool DeamonInterface::onSystemUpdateTimer () {
    DeamonInterface::getInstance()->write (At::signalStrength);
    DeamonInterface::getInstance()->write (At::systemInformation);
    return true;
}

void
DeamonInterface::run ()
{
    this->running = true;

    while (true == this->running)
    {

        if ( true == this->checkPort ())
        {

            if ( false == this->openPort ())
            {

                continue;
            }


            this->read ();

            this->msleep (THREAD_SLEEP_TIMEOUT);

            this->write ();

            this->msleep (THREAD_SLEEP_TIMEOUT);

        }

        this->msleep (THREAD_SLEEP_TIMEOUT);

    }
}

bool
DeamonInterface::checkPort ()
{

    bool result = false;

    QList<PortInformation> ports = SerialEnumerator::getPorts();

    foreach (PortInformation port, ports)
    {

        if (VENDOR_ID == port.vendorID)
        {

            if (PRODUCT_ID == port.productID)
            {

                if (port.friendName.contains (INTERFACE_PORT))
                {

                    this->portName = port.portName.remove ("\\\\.\\");

                    result = true;
                    break;
                }
            }
        }
    }

    if (false == result)
    {
        this->portName = "";
    }

    if (this->plugged != result)
    {

        this->plugged = result;
        emit modemPlugged (result);
    }

    return (result);
}

bool
DeamonInterface::openPort ()
{

    bool result = false;

    if (true == this->portName.isEmpty ())
    {

        if ( NULL != this->port)
        {

            delete this->port;
            this->port = NULL;
        }

        result = false;

    }
    else
    {
        if ( NULL == this->port)
        {

            this->port = new SerialPort(portName, SerialPort::Polling);

            if ( true == this->port->open (QIODevice::ReadWrite|QIODevice::Unbuffered) )
            {

                qDebug () << "Successfully opened Interface Port" << this->portName;
                this->port->setBaudRate (BAUD115200);
                this->port->setDataBits (DATA_8);
                this->port->setParity (PAR_NONE);
                this->port->setStopBits (STOP_1);
                this->port->setFlowControl (FLOW_HARDWARE);
                this->port->setTimeout (PORT_TIMEOUT);

                result = true;

            }
            else
            {

                result = false;
            }

        }
        else
        {

            result = true;
        }
    }

    return (result);
}

bool
DeamonInterface::write(
        QString value)
{

    this->mutex->lock ();

    this->queue.enqueue (value);

    this->mutex->unlock ();

    return (true);
}

bool
DeamonInterface::enqueue (
        QString value)
{

    this->mutex->lock ();

    this->waited.enqueue (value);

    this->mutex->unlock ();

    return (true);
}

bool
DeamonInterface::write ()
{

    bool result = false;

    this->mutex->lock();

    if ( false == this->queue.isEmpty ())
    {

        QString data = this->queue.dequeue ().append (At::end);

        int written = this->port->write (data.toAscii (), data.length ());

        qDebug() << "Write to AT port :"
                 << QTime::currentTime().toString("hh:mm:ss.zzz")
                 << "[" << written << "]" << data;

        this->port->flush ();

        result = true;
    }

    if ( false == this->waited.isEmpty ())
    {

        QString data = this->waited.dequeue ().append (At::end);

        int written = this->port->write (data.toAscii (), data.length ());

        qDebug() << "Write Wait to AT port :"
                 << QTime::currentTime().toString("hh:mm:ss.zzz")
                 << "[" << written << "]" << data;

        this->port->flush ();

        this->msleep (2000);

        result = true;
    }


    this->mutex->unlock();

    return (result);
}

bool
DeamonInterface::read ()
{

    int size = this->port->bytesAvailable();

    if (0 == size) return (false);

    QByteArray data;
    data.resize (size);

    int read = this->port->read(data.data (), size);
    data.resize (read);

    data.replace ("\0", 1, " ", 1);


    qDebug() << "Read From AT port :"
             << QTime::currentTime().toString("hh:mm:ss.zzz")
             << "[" << read << "]" << data;

    return (this->parse (data));
}

bool
DeamonInterface::parse (
        const QByteArray& value)
{
    bool result = false;

    QString response (value);

    response.remove (At::error);
    response.remove (At::dormant0);
    response.remove (At::dormant1);

    QStringList list = response.split("OK", QString::SkipEmptyParts);

    foreach (QString responseItem, list ) {
        qDebug() << "Parsing Item : " << responseItem;

        if (responseItem.contains(At::systemInformationResponse)){

            responseItem.remove(At::systemInformation).remove(At::systemInformationResponse);
            qDebug() << "System Information" << responseItem.simplified();
            QStringList information = responseItem.simplified().split(COMMA);

            emit serviceStatusUpdate(information.at(0).toInt());
            emit serviceDomainUpdate(information.at(1).toInt());
            emit roamingStatusUpdate(information.at(2).toInt());
            emit systemModeUpdate(information.at(3).toInt());
            emit simStatusUpdate(information.at(4).toInt());

        } else if (responseItem.contains(At::signalStrengthResponse)){

            responseItem.remove(At::signalStrength).remove(At::signalStrengthResponse).simplified();
            qDebug() << "Signal Strength:" << responseItem.simplified();
            int rawSignalStrength = responseItem.simplified().toInt ();
            int signalStrength = 0;

            if(0<rawSignalStrength && 6>=rawSignalStrength){
                signalStrength = 1;
            } else if (7<=rawSignalStrength && 13>rawSignalStrength) {
                signalStrength = 2;
            } else if (13<=rawSignalStrength && 20>rawSignalStrength) {
                signalStrength = 3;
            } else if (20<=rawSignalStrength && 27>rawSignalStrength) {
                signalStrength = 4;
            } else if (27<=rawSignalStrength && 32>=rawSignalStrength) {
                signalStrength = 5;
            }

            qDebug() << "Signal Strength:" << signalStrength;

            emit signalStrengthUpdate(signalStrength);

        } else if (responseItem.contains(At::modemVersionResponse)) {

            responseItem.remove(At::modemVersion).remove(At::modemVersionResponse);
            QStringList list = responseItem.split(LINE, QString::SkipEmptyParts);
            foreach (QString information, list){
                qDebug () << "Parsing Modem Version:" << information;

                if (information.contains(At::modemVersionResponseSoftwareVersion)) {
                    qDebug () << "Software Version:" << information.remove(At::modemVersionResponseSoftwareVersion).simplified();
                    emit softwareVersionUpdate(information.remove(At::modemVersionResponseSoftwareVersion).simplified());

                } else if (information.contains(At::modemVersionResponseCompilationDate)) {
                    qDebug () << "Compile Date:" << information.remove(At::modemVersionResponseCompilationDate).simplified();
                    emit compileDateUpdate(information.remove(At::modemVersionResponseCompilationDate).simplified());
                }
            }

        } else if (responseItem.contains(At::hdrcResponse)) {

            responseItem.remove(At::hdrc).remove(At::hdrcResponse);
            qDebug() << "DRC: " << responseItem.simplified();
            emit hdrcUpdate(responseItem.simplified());

        } else if (responseItem.contains(At::hdrEcioResponse)) {

            responseItem.remove(At::hdrEcio).remove(At::hdrEcioResponse);
            qDebug() << "HDR ECIO:" << responseItem.simplified();
            emit hdrEcioUpdate(responseItem.simplified());

        } else if (responseItem.contains(At::sidResponse)) {

            responseItem.remove(At::sid).remove(At::sidResponse);
            qDebug() << "SID:" << responseItem.simplified();
            emit sidUpdate(responseItem.simplified());

        } else if (responseItem.contains(At::esnResponse)) {

            QStringList list = responseItem.split(LINE);
            foreach (QString information, list){
                if (information.contains(At::esnResponse)) {
                    information.remove(At::esnResponse);
                    qDebug() << "ESN:" << information.simplified();
                    emit esnUpdate(information.simplified());
                }
            }

        } else if (responseItem.contains(At::meidResponse)) {

            responseItem.remove(At::meid).remove(At::meid);
            qDebug() << "MEID:" << responseItem.simplified();
            emit meidUpdate(responseItem.simplified());

        } else if (responseItem.contains(At::hardwareVersionResponse)) {

            responseItem.remove(At::hardwareVersion).remove(At::hardwareVersionResponse);
            qDebug() << "Hardware Version:" << responseItem.simplified();
            emit hardwareVersionUpdate(responseItem.simplified());

        } else if (responseItem.contains(At::nidResponse)) {
            responseItem.remove(At::nid).remove(At::nidResponse);
            qDebug() << "Nid:" << responseItem.simplified();
            emit nidUpdate(responseItem.simplified());

        } else if (responseItem.contains(At::prlResponse)) {

            responseItem.remove(At::prl).remove(At::prlResponse);
            qDebug() << "PRL:" << responseItem.simplified();
            emit prlUpdate(responseItem.simplified());

        } else if (responseItem.contains(At::flashMessageResponse)) {

            responseItem.remove(At::flashMessageResponse);
            qDebug () << "Flash Message:" << responseItem.split(LINE);
            emit flashMessageUpdate(responseItem.split(LINE).at(2).simplified());

        } else if (responseItem.contains(At::messageResponse)) {

            responseItem.remove(At::messageResponse);
            qDebug () << "Message:" << responseItem.split(LINE);
            emit flashMessageUpdate(responseItem.split(LINE).at(2).simplified());

        } else if (responseItem.contains(At::defaultMinResponse)) {

            responseItem.remove(At::defaultMin).remove(At::defaultMinResponse);
            qDebug () << "IMSI:" << responseItem.simplified();
            int min = responseItem.simplified().toInt();
            min /= 10000;
            min %= 1000000;

            if ( 0 == min ) {
                emit defaultMinUpdate();
            }

        } else if (responseItem.contains(At::programmingSuccessful)) {

            emit activation(true);

        } else if (responseItem.contains(At::programmingUnsuccessful)) {

            emit activation(false);

        } else if (responseItem.contains(At::cdmaRssiResponse)) {

            responseItem.remove(At::cdmaRssi).remove (At::cdmaRssiResponse);
            qDebug () << "CDMA RSSI:" << responseItem.simplified ();
            emit cdmaRssiUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::cdmaPnOffsetResponse)) {

            responseItem.remove(At::cdmaPnOffset).remove (At::cdmaPnOffsetResponse);
            qDebug () << "CDMA Pn Offset:" << responseItem.simplified ();
            emit pnOffsetUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::walshCodeResponse)) {

            responseItem.remove(At::walshCode).remove (At::walshCodeResponse);
            qDebug () << "Walsh Code:" << responseItem.simplified ();
            emit walshCodeUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::channelNumberResponse)) {

            responseItem.remove(At::channelNumber).remove (At::channelNumberResponse);
            qDebug () << "Channel Number:" << responseItem.simplified ();
            emit channelNumberUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::bandClassResponse)) {

            responseItem.remove(At::bandClass).remove (At::bandClassResponse);
            qDebug () << "Band Class:" << responseItem.simplified ();
            emit bandClassUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::txPowerResponse)) {

            responseItem.remove(At::txPower).remove (At::txPowerResponse);
            qDebug () << "Tx Power:" << responseItem.simplified ();
            emit txPowerUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::rxPowerResponse)) {

            responseItem.remove(At::rxPowerResponse).remove (At::rxPowerResponse);
            qDebug () << "Rx Power:" << responseItem.simplified ();
            emit rxPowerUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::bsPreviousResponse)) {

            responseItem.remove(At::bsPrevious).remove (At::bsPreviousResponse);
            qDebug () << "BS Previous:" << responseItem.simplified ();
            emit bsPreviousUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::msPreviousResponse)) {

            responseItem.remove(At::msPrevious).remove (At::msPreviousResponse);
            qDebug () << "MS Previous:" << responseItem.simplified ();
            emit msPreviousUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::previousInUseResponse)) {

            responseItem.remove(At::previousInUse).remove (At::previousInUseResponse);
            qDebug () << "Previous In Use:" << responseItem.simplified ();
            emit previousInUseUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::packetZoneIdResponse)) {

            responseItem.remove(At::packetZoneId).remove (At::packetZoneIdResponse);
            qDebug () << "Packet Zone ID:" << responseItem.simplified ();
            emit packetZoneIdUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::lastActiveCallStateResponse)) {

            responseItem.remove(At::lastActiveCallState).remove (At::lastActiveCallStateResponse);
            qDebug () << "Last Active State:" << responseItem.simplified ();
            emit lastActiveCallStateUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::hdrRssiResponse)) {

            responseItem.remove(At::hdrRssi).remove (At::hdrRssiResponse);
            qDebug () << "HDR RSSI:" << responseItem.simplified ();
            emit hdrRssiUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::perResponse)) {

            responseItem.remove(At::per).remove (At::perResponse);
            qDebug () << "PER:" << responseItem.simplified ();
            emit perUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::atStateResponse)) {

            responseItem.remove(At::atState).remove (At::atStateResponse);
            qDebug () << "AT State:" << responseItem.simplified ();
            emit atStateUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::dormantResponse)) {

            responseItem.remove(At::dormantState).remove (At::dormantResponse);
            qDebug () << "Dormant State:" << responseItem.simplified ();
            emit dormantStateUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::activeStatePnOffsetResponse)) {

            responseItem.remove(At::activeStatePnOffset).remove (At::activeStatePnOffsetResponse);
            qDebug () << "Active State Pn Offset:" << responseItem.simplified ();
            emit activeStatePnOffsetUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::neighborSetPnOffsetResponse)) {

            responseItem.remove(At::neighborSetPnOffset).remove (At::neighborSetPnOffsetResponse);
            qDebug () << "Neighbor Set Pn Offset:" << responseItem.simplified ();
            emit neighborSetPnOffsetUpdate(responseItem.simplified ());

        } else if (responseItem.contains(At::neighborSetEcioResponse)) {

            responseItem.remove(At::neighborSetEcio).remove (At::neighborSetEcioResponse);
            qDebug () << "Neighbor Set Ecio:" << responseItem.simplified ();
            emit neighborSetEcioUpdate(responseItem.simplified ());

        }
    }

    return result;
}

bool DeamonInterface::stop (){
    this->running = false;

    return true;
}
