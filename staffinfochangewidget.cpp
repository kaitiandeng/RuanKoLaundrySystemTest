#include "staffinfochangewidget.h"
#include "ui_staffinfochangewidget.h"

StaffInfoChangeWidget::StaffInfoChangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StaffInfoChangeWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
}

StaffInfoChangeWidget::~StaffInfoChangeWidget()
{
    delete ui;
}

void StaffInfoChangeWidget::DisplayCurrentSelectedData(QString _name, QString _tel, QString _gender, QString _address)
{
    qDebug()<<"Receive ->"<<_name<<_tel<<_gender<<_address;
    NAME = _name;
    TEL=_tel;
    GENDER=_gender;
    ADDRESS=_address;

    ui->Name_lineEdit->setText(_name);
    ui->Tel_lineEdit->setText(_tel);
    ui->Gender_comboBox->setCurrentText(_gender);
    ui->Address_lineEdit->setText(_address);

}

void StaffInfoChangeWidget::on_Confirm_pushButton_clicked()
{
    QString _info = ui->Name_label->text() + ui->Name_lineEdit->text() + "\n"
            + ui->Tel_label->text() + ui->Tel_lineEdit->text() + "\n"
            + ui->Gender_label->text() + ui->Gender_comboBox->currentText() + "\n"
            + ui->Address_label->text() + ui->Address_lineEdit->text();

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认信息"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        emit SendChangedStaffInfo(QString(ui->Name_lineEdit->text()),
                                  QString(ui->Tel_lineEdit->text()),
                                  QString(ui->Gender_comboBox->currentText()),
                                  QString(ui->Address_lineEdit->text()),
                                  NAME,
                                  TEL,
                                  GENDER,
                                  ADDRESS);
        this->close();
    }

}

void StaffInfoChangeWidget::on_Cancel_pushButton_clicked()
{
    this->close();
}
