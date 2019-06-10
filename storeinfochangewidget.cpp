#include "storeinfochangewidget.h"
#include "ui_storeinfochangewidget.h"

StoreInfoChangeWidget::StoreInfoChangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StoreInfoChangeWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
    //Turnover
    QRegExp TurnoverRegExp("[0-9]+$");
    ui->Turnover_lineEdit->setValidator(new QRegExpValidator(TurnoverRegExp,this));
    //Tel
    QRegExp TelRegExp("^((1[3,5,8][0-9])|(14[5,7])|(17[0,6,7,8])|(19[7]))\\d{8}$");
    ui->Tel_lineEdit->setValidator(new QRegExpValidator(TelRegExp,this));

}

StoreInfoChangeWidget::~StoreInfoChangeWidget()
{
    delete ui;
}

void StoreInfoChangeWidget::DisplayCurrentSelectedStoreInfo(QString _name, QString _address, QString _leader, QString _tel, QString _operation, QString _turnover)
{
    qDebug()<<"Receive ->"<<_name<<_address<<_leader<<_tel<<_operation<<_turnover;
    NAME=_name;
    ADDRESS=_address;
    LEADER=_leader;
    TEL=_tel;
    OPERATION=_operation;
    TURNOVER=_turnover;
    ui->StoreName_lineEdit->setText(_name);
    ui->Address_lineEdit->setText(_address);
    ui->Leader_lineEdit->setText(_leader);
    ui->Tel_lineEdit->setText(_tel);
    ui->Operation_lineEdit->setText(_operation);
    ui->Turnover_lineEdit->setText(_turnover);
}

void StoreInfoChangeWidget::on_Confirm_pushButton_clicked()
{

    QString _info = ui->StoreName_label->text() + ui->StoreName_lineEdit->text() + "\n"
            + ui->Address_label->text() + ui->Address_lineEdit->text() + "\n"
            + ui->Leader_label->text() + ui->Leader_lineEdit->text() + "\n"
            + ui->Tel_label->text() + ui->Tel_lineEdit->text() + "\n"
            + ui->Operation_label->text() + ui->Operation_lineEdit->text() + "\n"
            + ui->Turnover_label->text() + ui->Turnover_lineEdit->text();

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认信息"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        emit SendChangedStoreInfo(QString(ui->StoreName_lineEdit->text()),
                                  QString(ui->Address_lineEdit->text()),
                                  QString(ui->Leader_lineEdit->text()),
                                  QString(ui->Tel_lineEdit->text()),
                                  QString(ui->Operation_lineEdit->text()),
                                  QString(ui->Turnover_lineEdit->text()),
                                  NAME,
                                  ADDRESS,
                                  LEADER,
                                  TEL,
                                  OPERATION,
                                  TURNOVER);
        this->close();
    }

}

void StoreInfoChangeWidget::on_Cancel_pushButton_clicked()
{
    this->close();

}
