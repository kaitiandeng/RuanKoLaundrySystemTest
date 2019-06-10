#include "memberinfochangewidget.h"
#include "ui_memberinfochangewidget.h"

MemberInfoChangeWidget::MemberInfoChangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberInfoChangeWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
    //Name
    QRegExp NameRegExp("^[\u4E00-\u9FA5A-Za-z]+$");
    ui->Name_lineEdit->setValidator(new QRegExpValidator(NameRegExp,this));
    //Agenda
    ui->Agenda_comboBox->addItem(tr("男"));
    ui->Agenda_comboBox->addItem(tr("女"));
    ui->Agenda_comboBox->addItem(tr("未知"));
    //Tel
    QRegExp TelRegExp("^((1[3,5,8][0-9])|(14[5,7])|(17[0,6,7,8])|(19[7]))\\d{8}$");
    ui->Tel_lineEdit->setValidator(new QRegExpValidator(TelRegExp,this));
    //Don't need inite systemPage
}

MemberInfoChangeWidget::~MemberInfoChangeWidget()
{
    delete ui;
}
void MemberInfoChangeWidget::DisplayCurrentSelectedData(QString _name, QString _gender, QString _birthday, QString _tel, QString _address)
{
    qDebug()<<"Receive ->"<<_name<<_gender<<_birthday<<_tel<<_address;
    NAME = _name;
    GENDER=_gender;
    BIRHDAY=_birthday;
    TEL=_tel;
    ADDRESS=_address;
    int _year = _birthday.left(4).toInt();
    int _month = _birthday.mid(5,2).toInt();
    int _day = _birthday.right(2).toInt();
    ui->Name_lineEdit->setText(_name);
    ui->Agenda_comboBox->setCurrentText(_gender);
    //ui->Birthday_dateEdit->date().setDate(_year, _month, _day);
    QDate _date(_year,_month,_day);
    ui->Birthday_dateEdit->setDate(_date);
    ui->Tel_lineEdit->setText(_tel);
    ui->Address_lineEdit->setText(_address);
}

void MemberInfoChangeWidget::on_Confirm_pushButton_clicked()
{

    QString _birthday = QString::number(ui->Birthday_dateEdit->date().year())
            + "/" + QString::number(ui->Birthday_dateEdit->date().month())
            + "/" + QString::number(ui->Birthday_dateEdit->date().day());

    QString _info = ui->Name_label->text() + ui->Name_lineEdit->text() + "\n"
            + ui->Agenda_label->text() + ui->Agenda_comboBox->currentText() + "\n"
            + ui->Birthday_label->text() + _birthday + "\n"
            + ui->Tel_label->text() + ui->Tel_lineEdit->text() + "\n"
            + ui->Address_label->text() + ui->Address_lineEdit->text();

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认信息"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        emit SendChangedMemberInfo(QString(ui->Name_lineEdit->text()),
                                   QString(ui->Agenda_comboBox->currentText()),
                                   QString(_birthday),
                                   QString(ui->Tel_lineEdit->text()),
                                   QString(ui->Address_lineEdit->text()),
                                   NAME,
                                   GENDER,
                                   BIRHDAY,
                                   TEL,
                                   ADDRESS);
        this->close();
    }
}

void MemberInfoChangeWidget::on_Cancle_pushButton_clicked()
{
    this->close();
}
