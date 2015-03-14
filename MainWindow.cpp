#include "MainWindow.h"

#include "DeamonInterface.h"
#include "DeamonConnection.h"
#include "DeamonDatabase.h"
#include "DeamonSetting.h"
#include "Define.h"
#include "DialogSetting.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QProcess>
#include <QPainter>

bool MainWindow::onReceiveMessage(const QString& message) {
        qDebug () << "Message Recieved : " << message;
        this->showNormal();

        return (true);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      connected(false),
      plugged (false)
{
    this->createSystemTray();

    this->background = new QImage(":/background.png");

    this->helpButton = new ImageButton("", ":/helpDefault.png", ":/helpHover.png", false, this);
    this->helpButton->setGeometry (10, 12, 24, 25);
    this->helpButton->setEnabled (false);

    this->minimizeButton = new ImageButton("", ":/minimizeDefault.png", ":/minimizeHover.png", false, this);
    this->minimizeButton->setGeometry (408, 12, 25, 25);

    this->closeButton = new ImageButton("", ":/closeDefault.png", ":/closeHover.png", false, this);
    this->closeButton->setGeometry (441, 13, 25, 24);

    this->connectButton = new ImageButton("", ":/connectDefault.png", ":/connectHover.png", ":/connectPress.png", "", this);
    this->connectButton->setGeometry (80, 200, 50, 50);
    this->connectButton->setEnabled (false);

    this->disconnectButton = new ImageButton("", ":/disconnectDefault.png", ":/disconnectHover.png", ":/disconnectPress.png", "", this );
    this->disconnectButton->setGeometry (80, 200, 50, 50);
    this->disconnectButton->setEnabled (false);
    this->disconnectButton->setVisible (false);

    this->connectingButton = new AnimateButton(":/connecting1.png", ":/connecting2.png", ":/connecting3.png", ":/connecting4.png", ":/connecting5.png", ":/connecting6.png", this);
    this->connectingButton->setGeometry (80, 200, 50, 50);
    this->connectingButton->setEnabled (false);
    this->connectingButton->setVisible (false);

    this->disconnectingButton = new AnimateButton(":/disconnecting1.png", ":/disconnecting2.png", ":/disconnecting3.png", ":/disconnecting4.png", ":/disconnecting5.png", ":/disconnecting6.png", this);
    this->disconnectingButton->setGeometry (80, 200, 50, 50);
    this->disconnectingButton->setEnabled (false);
    this->disconnectingButton->setVisible (false);

    this->settingsButton = new ImageButton("", ":/settingsDefault.png", ":/settingsHover.png", ":/settingsPress.png", false, this);
    this->settingsButton->setGeometry (225, 200, 50, 50);
    this->settingsButton->setEnabled (false);

    this->browserButton = new ImageButton("", ":/browserDefault.png", ":/browserHover.png", ":/browserPress.png", false, this);
    this->browserButton->setGeometry (370, 200, 50, 50);
    this->browserButton->setEnabled (false);

    this->signalStrengthImage.append (QPixmap(":/signalStrength0.png"));
    this->signalStrengthImage.append (QPixmap(":/signalStrength1.png"));
    this->signalStrengthImage.append (QPixmap(":/signalStrength2.png"));
    this->signalStrengthImage.append (QPixmap(":/signalStrength3.png"));
    this->signalStrengthImage.append (QPixmap(":/signalStrength4.png"));
    this->signalStrengthImage.append (QPixmap(":/signalStrength5.png"));

    this->signalStrength = new QLabel (this);
    this->signalStrength->setPixmap (this->signalStrengthImage.at(0));
    this->signalStrength->setGeometry (340, 60, 97, 74);

    this->dataRates.append (QPixmap(":/speedMeter1.png"));

    this->dataRate = new QLabel (this);
    this->dataRate->setPixmap (this->dataRates.at(0));
    this->dataRate->setGeometry (35, 60, 133, 69);

    this->graphImage.append (QPixmap(":/graph.png"));

    this->graph = new QLabel (this);
    this->graph->setPixmap (this->graphImage.at(0));
    this->graph->setGeometry (195, 65, 126, 63);

    this->download = new QLabel("Download : ", this);
    this->download->setStyleSheet ("QLabel { color : white; }");
    this->download->setGeometry (320, 150, 60, 18);

    this->downloadImage = new QLabel("0.00 MB", this);
    this->downloadImage->setStyleSheet ("QLabel { color : white; }");
    this->downloadImage->setGeometry (380, 150, 70, 18);

    this->upload = new QLabel("Upload : ", this);
    this->upload->setStyleSheet ("QLabel { color : white; }");
    this->upload->setGeometry (180, 150, 60, 18);

    this->uploadImage = new QLabel("0.00 MB", this);
    this->uploadImage->setStyleSheet ("QLabel { color : white; }");
    this->uploadImage->setGeometry (230, 150, 70, 18);

    this->duration = new QLabel("Duration : ", this);
    this->duration->setStyleSheet ("QLabel { color : white; }");
    this->duration->setGeometry (30, 150, 60, 18);

    //this->durationImage = new ImageButton("00.00",":/dataBg.png", false, this);
    //this->durationImage->setGeometry (90, 150, 70, 18);

    this->durationImage = new TimeLabel (this);
    this->durationImage->setGeometry (70, 152, 70, 18);

    DeamonInterface::getInstance ()->start ();

    QObject::connect (DeamonInterface::getInstance(), SIGNAL(signalStrengthUpdate(int)), SLOT(onSignalStrengthUpdated(int)));
    QObject::connect (DeamonInterface::getInstance (), SIGNAL(modemPlugged (bool)), this, SLOT(onModemPlugged (bool)));
    QObject::connect (this->disconnectButton, SIGNAL(clicked()), this, SLOT(onDisconnectButtonClicked()));
    QObject::connect (this->browserButton, SIGNAL(clicked()),SLOT(onBrowserButtonClicked()));
    QObject::connect (this->settingsButton, SIGNAL(clicked()),SLOT(onSettingsButtonClicked()));
    QObject::connect (this->connectButton, SIGNAL(clicked()),SLOT(onConnectButtonClicked()));
    QObject::connect (this->minimizeButton, SIGNAL(clicked()),SLOT(onMinimizeButtonClicked()));
    QObject::connect (this->closeButton, SIGNAL(clicked()),SLOT(onCloseButtonClicked()));
    QObject::connect (this->helpButton, SIGNAL(clicked()),SLOT(onHelpButtonClicked()));
    QObject::connect (DeamonConnection::getInstance (), SIGNAL(modemConnected (bool)), this, SLOT(onModemConnected(bool)));
    QObject::connect (DeamonConnection::getInstance (), SIGNAL (usageUpdate (double, double)), this, SLOT(onUsageUpdate(double,double)));

}

bool MainWindow::createSystemTray () {

        this->minimizeAction = new QAction(tr("Minimize"), this);
        QObject::connect (this->minimizeAction, SIGNAL(triggered()), SLOT(hide()));

        this->restoreAction = new QAction(tr("Restore"), this);
        QObject::connect (this->restoreAction, SIGNAL(triggered()), SLOT(showNormal()));



        this->quitAction = new QAction(tr("Quit"), this);
        QObject::connect (this->quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

        this->trayIconMenu = new QMenu(this);
        this->trayIconMenu->addAction(this->minimizeAction);
        this->trayIconMenu->addAction(this->restoreAction);
        this->trayIconMenu->addSeparator();
        this->trayIconMenu->addSeparator ();
        this->trayIconMenu->addAction(this->quitAction);

        this->trayIcon = new QSystemTrayIcon(this);
        this->trayIcon->setContextMenu(this->trayIconMenu);
        this->trayIcon->setToolTip(APPLICATION_NAME);

        QObject::connect (this->trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(onSystemTrayActivated(QSystemTrayIcon::ActivationReason)));
        this->trayIcon->setIcon(QIcon(APPLICATION_ICON));
        this->trayIcon->show();

        return (true);
}

bool MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason){

        switch (reason) {
        case QSystemTrayIcon::Trigger: { // do nothing
                }break;
        case QSystemTrayIcon::DoubleClick: {
                        this->showNormal();
                }break;
        case QSystemTrayIcon::MiddleClick: {// fall through
                }break;
        case QSystemTrayIcon::Unknown: {
                }break;
        case QSystemTrayIcon::Context: {
                }break;
        default:{   // do nothing

                };
        }

        return true;
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent* event) {
                Q_UNUSED (event)

                QPainter painter(this);
                painter.drawImage(QPoint(0,0), *this->background);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
        if (this->moving) {
                this->move(event->globalPos() - this->offset);
        }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
                this->moving = true;
                this->offset = event->pos();
        }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
                this->moving = false;
        }
}

void MainWindow::keyPressEvent (QKeyEvent* event) {
        if (Qt::Key_F1 == event->key()) {
                QDesktopServices::openUrl(QUrl::fromLocalFile(qApp->applicationDirPath() + "/help.chm"));
        }
}

bool MainWindow::onHelpButtonClicked() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(qApp->applicationDirPath() + "/help.chm"));

        return true;
}

bool MainWindow::onMinimizeButtonClicked() {
        this->hide();

        return (true);
}

bool MainWindow::onCloseButtonClicked() {

    if (true == this->connected) {

        if ( QMessageBox::Ok ==
             QMessageBox::information (NULL,
                                       "Smartfren",
                                       tr("Do you want to disconnect connection?"),
                                       QMessageBox::Ok|QMessageBox::Cancel,
                                       QMessageBox::Cancel)) {

            DeamonConnection::getInstance ()->disconnectDataCall ();

        } else {

            return false;
        }
    }

    DeamonDatabase::getInstance ()->closeDatabase ();

    DeamonInterface::getInstance ()->stop ();
    DeamonInterface::getInstance ()->wait ();

    qApp->quit ();
    return (true);
}

bool MainWindow::onSettingsButtonClicked() {

    qDebug () << "MainWindow::onSettingsButtonClicked";

    DialogSetting dialogSettings;
    dialogSettings.exec ();

    return (true);
}

bool MainWindow::onSignalStrengthUpdated(int level) {

    qDebug () << "MainWindow::onSignalStrengthUpdated";

    this->signalStrength->setPixmap (this->signalStrengthImage.at (level));
    return (true);
}

bool MainWindow::onBrowserButtonClicked() {
    this->openWebPage (HOME_PAGE);

    return (true);
}


bool
MainWindow::onModemPlugged
(bool status)
{
    this->plugged = status;

    this->connectButton->setEnabled (status);
    this->settingsButton->setEnabled (status);
    this->browserButton->setEnabled (status);

    if (false == status) {
        this->onSignalStrengthUpdated (0);
    }

    return true;
}


bool MainWindow::openWebPage(const QString& url) {
    QDesktopServices::openUrl(QUrl(url));
    return (true);
}

bool
MainWindow::onConnectButtonClicked
()
{
    DeamonConnection::getInstance ()->connectDataCall ();

    this->connectButton->hide ();
    this->connectingButton->show ();

    this->connected = false;

    return (true);
}

bool
MainWindow::onConnectingButtonClicked
()
{
    DeamonConnection::getInstance ()->disconnectDataCall ();

    this->connectingButton->hide ();
    this->connectButton->show ();

    this->connected = false;

    return true;
}

bool MainWindow::onDisconnectButtonClicked(){
    DeamonConnection::getInstance ()->disconnectDataCall ();

    this->disconnectButton->hide ();

    return true;
}

bool MainWindow::onDisconnectingButtonClicked(){

    return true;
}

bool MainWindow::onDisconnected () {
    this->connected = false;

    this->disconnectingButton->hide();
    this->connectingButton->hide ();

    this->connectButton->show();

    this->settingsButton->setEnabled(true);

    return (true);
}

bool MainWindow::onConnected () {
    this->connected = true;

    this->connectingButton->hide();
    this->disconnectButton->show();

    this->settingsButton->setEnabled(false);

    return (true);
}

bool MainWindow::onModemConnected (bool status){
    this->connected = status;

    qDebug () << "MainWindow::onModemConnected";

    if (true == status)
    {
        this->connectingButton->hide ();
        this->disconnectButton->show ();
    }

    else
    {
        this->connectingButton->hide ();
        this->disconnectingButton->hide ();
        this->disconnectButton->hide ();
        this->connectButton->show ();
    }

    return true;
}

bool MainWindow::onUsageUpdate (
        double uploaded,
        double downloaded)
{
    this->uploaded += (uploaded / (1024));
    this->downloaded += (downloaded / (1024));

    this->downloadImage->setText (QString::number (this->downloaded, 'f', 2) + tr(" MB"));
    this->uploadImage->setText (QString::number (this->uploaded, 'f', 2) + tr(" MB"));

    return (true);
}
