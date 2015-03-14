#include <QtGui/QApplication>

#include "Define.h"
#include "MainWindow.h"
#include "SingleApplication.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QSplashScreen>
#include <QSystemTrayIcon>
#include <QStringList>
#include <QTextStream>
#include <QTimer>

void myMessageHandler(QtMsgType type, const char *message) {

    QStringList log = QStringList ()  << "" << "Warning" << "Critical" << "Fatal";

    QFile outFile(QDir::tempPath () + SLASH + LOG_FILE_NAME);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream file(&outFile);
    QTextStream stde(stderr);

    file << log.at(type) << QString (message) << endl;
    stde << log.at(type) << QString (message) << endl;
}

int main(int argc, char *argv[])
{
    SingleApplication application(argc, argv, "bb2acd1e-1864-4783-a25f-b26da45856e8-dfbb640a-88bd-449a-9dbf-2f87003ca2c7");

    qInstallMsgHandler(myMessageHandler);

    if (application.isRunning()) {
        application.sendMessage("Message from other instace");
        return (0);
    }

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("System"), QObject::tr("No System Tray on this system."));
        return (1);
    }


    QProcess::startDetached (QApplication::applicationDirPath () + "/drivers/modem.exe", QStringList (), QApplication::applicationDirPath ());

    MainWindow mainWindow;
    mainWindow.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    mainWindow.setWindowIcon(QIcon(APPLICATION_ICON));
    mainWindow.setWindowTitle (APPLICATION_NAME);
    mainWindow.setFixedSize (480, 322);
    mainWindow.setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect (&application, SIGNAL(messageAvailable(QString)), &mainWindow, SLOT(onReceiveMessage(QString)));

    SingleApplication::setQuitOnLastWindowClosed(false);
    QSplashScreen splash (QPixmap (SPLASH_SCREEN));
    splash.show();
    application.processEvents();

    QTimer::singleShot (SPLASH_SCREEN_TIMEOUT,
                        &splash,
                        SLOT(close()));

    QTimer::singleShot (SPLASH_SCREEN_TIMEOUT,
                        &mainWindow,
                        SLOT(show()));


    return application.exec();
}
