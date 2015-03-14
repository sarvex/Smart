#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "TimeLabel.h"

#include <QDateTime>
#include <QLabel>
#include <QPainter>
#include <QRect>
#include <QDebug>
#include <QTimerEvent>

TimeLabel::TimeLabel (
        QWidget *parent)
    : QLabel(parent)
{
    this->setText ("00:00:00");
    this->setStyleSheet ("font: 12px;");

    this->start.start ();

    this->timer = new QTimer;

    QObject::connect (this->timer,
                      SIGNAL(timeout()),
                      this,
                      SLOT(onTimer()));

    QObject::connect (DeamonConnection::getInstance (),
                      SIGNAL(modemConnected (bool)),
                      this,
                      SLOT(onModemConnected (bool)));

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(modemPlugged (bool)),
                      this,
                      SLOT(onModemPlugged (bool)));
}

void
TimeLabel::paintEvent (
        QPaintEvent * event)
{
    Q_UNUSED (event);

    QPainter painter(this);
    painter.setRenderHint (QPainter::Antialiasing);
    painter.setPen (Qt::white);
    painter.drawText (QRect(0,0, this->width (), this->height ()),
                      Qt::AlignRight, this->text ());
}

bool
TimeLabel::onTimer ()
{

    this->duration = QTime (0,0).addMSecs (this->start.elapsed ());

    this->setText (this->duration.toString (TIME_FORMAT));

    return (true);
}

bool
TimeLabel::onModemConnected (bool status)
{

    if (true == status)
    {

        this->start.restart ();
        this->timer->start (1000);

    }

    else
    {

        this->timer->stop ();
        this->setText ("00:00:00");
    }
    return (true);
}

bool
TimeLabel::onModemPlugged (
        bool status)
{
    Q_UNUSED (status);

    return (true);
}
