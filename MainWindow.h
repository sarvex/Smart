#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "AnimateButton.h"
#include "ImageButton.h"
#include "TimeLabel.h"

#include <QBuffer>
#include <QByteArray>
#include <QHttp>
#include <QLabel>
#include <QPixmap>
#include <QSystemTrayIcon>
#include <QTranslator>
#include <QUrl>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    bool onReceiveMessage (const QString& message);

    bool onHelpButtonClicked ();
    bool onMinimizeButtonClicked ();
    bool onCloseButtonClicked ();

    bool onSettingsButtonClicked ();
    bool onBrowserButtonClicked ();

    bool onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);

    bool onSignalStrengthUpdated(int level);

    bool onModemPlugged (bool status);
    bool onModemConnected (bool status);

    bool openWebPage (const QString& url);

    bool onConnectButtonClicked ();
    bool onConnectingButtonClicked ();
    bool onDisconnectButtonClicked ();
    bool onDisconnectingButtonClicked ();

    bool onUsageUpdate (double uploaded, double downloaded);


    bool onConnected ();
    bool onDisconnected ();

signals:

protected:
        void paintEvent(QPaintEvent* event);

        void mouseMoveEvent(QMouseEvent* event);
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);

        void keyPressEvent (QKeyEvent* event);


private:
        ImageButton * helpButton;
        ImageButton * minimizeButton;
        ImageButton * closeButton;

        ImageButton * connectButton;
        ImageButton * disconnectButton;

        AnimateButton * connectingButton;
        AnimateButton * disconnectingButton;

        ImageButton * settingsButton;
        ImageButton * browserButton;
        ImageButton * header;

        QLabel * downloadImage;
        QLabel * uploadImage;

        TimeLabel * durationImage;


        QImage * background;

        bool moving;
        bool connected;
        bool plugged;
        bool createSystemTray();

        QAction * minimizeAction;
        QAction * restoreAction;
        QAction * safelyRemoveAction;
        QAction * quitAction;

        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;

        QLabel * dataRate;
        QLabel * signalStrength;
        QLabel * graph;
        QLabel * download;
        QLabel * upload;
        QLabel * duration;
        QLabel * logoText;
        QLabel * activateText;

        QList <QPixmap> signalStrengthImage;
        QList <QPixmap> dataRates;
        QList <QPixmap> graphImage;


        QPoint offset;

        double uploaded;
        double downloaded;

};

#endif // MAINWINDOW_H
