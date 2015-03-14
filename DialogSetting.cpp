#include "DialogSetting.h"
#include "DeamonInterface.h"
#include "At.h"

#include <QPainter>

DialogSetting::DialogSetting(QWidget *parent) :
    QDialog(parent)
{

    this->background = new QImage(":/display.png");

    this->prlLabel = new QLabel(PRL_LABEL, this);
    this->prlLabel->setGeometry (10, 10, 100, 20);

    this->meidLabel = new QLabel(MEID_LABEL, this);
    this->meidLabel->setGeometry (10, 40, 100, 20);

    this->esnLabel = new QLabel(ESN_LABEL, this);
    this->esnLabel->setGeometry (10, 70, 100, 20);

    this->prl = new QLabel(this);
    this->prl->setGeometry (110, 10, 100, 20);

    this->meid = new QLabel(this);
    this->meid->setGeometry (110, 40, 100, 20);

    this->esn = new QLabel(this);
    this->esn->setGeometry (110, 70, 100, 20);

//    this->ok = new ImageButton(OK_STRING, ":/buttonDefault.png", ":/buttonDefault.png", ":/buttonDefault.png", this);
//    this->ok->setGeometry (10, 110, 60, 20);

//    QObject::connect (this->ok, SIGNAL(clicked()), SLOT(onOkClicked()));

//    this->cancel = new ImageButton(CANCEL_STRING, ":/buttonDefault.png", ":/buttonDefault.png", ":/buttonDefault.png", this);
//    this->cancel->setGeometry (110, 110, 60, 20);
//    QObject::connect (this->cancel, SIGNAL(clicked()), SLOT(onCancelClicked()));

    QObject::connect (DeamonInterface::getInstance(), SIGNAL(prlUpdate(const QString&)), SLOT(onPrlUpdate(const QString&)));
    QObject::connect (DeamonInterface::getInstance(), SIGNAL(meidUpdate(const QString&)), SLOT(onEsnUpdate(const QString&)));
    QObject::connect (DeamonInterface::getInstance(), SIGNAL(esnUpdate(const QString&)), SLOT(onPEsnUpdated(const QString&)));
}

void DialogSetting::showEvent (QShowEvent* event){

    DeamonInterface::getInstance ()->write (At::prl);
    DeamonInterface::getInstance ()->write (At::meid);
    DeamonInterface::getInstance ()->write (At::esn);

    QWidget::showEvent(event);
}



bool DialogSetting::onPrlUpdate(const QString& value){
    this->prl->setText(value);

    return (true);
}

bool DialogSetting::onEsnUpdate(const QString& value){
    this->meid->setText(value);

    return (true);
}

bool DialogSetting::onPEsnUpdated(const QString& value){
    this->esn->setText(value);

    return true;
}

bool DialogSetting::onOkClicked() {

    this->close ();
    return (true);
}

bool DialogSetting::onCancelClicked() {

    this->close ();
    return (true);
}

void DialogSetting::paintEvent(QPaintEvent* event) {
    Q_UNUSED (event)

    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), *this->background);
}


