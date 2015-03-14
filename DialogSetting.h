#ifndef DIALOGSETTING_H
#define DIALOGSETTING_H

#include <QDialog>

#include "ImageButton.h"

#include <QImage>
#include <QLabel>
#include <QSettings>

class DialogSetting : public QDialog
{
    Q_OBJECT
public:
    explicit DialogSetting(QWidget *parent = 0);

signals:

public slots:
    bool onOkClicked();
    bool onCancelClicked();

    bool onPrlUpdate(const QString& value);
    bool onEsnUpdate(const QString& value);
    bool onPEsnUpdated(const QString& value);

protected:
    void paintEvent (QPaintEvent* event);
    void showEvent (QShowEvent* event);

private:

    QImage * background;


    QLabel * prlLabel;
    QLabel * meidLabel;
    QLabel * esnLabel;
    QLabel * softwareVersionLabel;
    QLabel * hardwareVersionLabel;


    QLabel * prl;
    QLabel * meid;
    QLabel * esn;
    QLabel * softwareVersion;
    QLabel * hardwareVersion;

    ImageButton * ok;
    ImageButton * cancel;

    QSettings * settings;

};

#endif // DIALOGSETTING_H
