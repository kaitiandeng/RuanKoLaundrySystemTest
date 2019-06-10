#include "memberregisterwidget.h"
#include "ui_memberregisterwidget.h"

MemberRegisterWidget::MemberRegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberRegisterWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("会员注册"));
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

    //connect
    //connect(this, SIGNAL(SendMemRegInfo(QString,QString,QString,QString,QString)), this, SLOT(RegisterMember(QString,QString,QString,QString,QString)));

}

MemberRegisterWidget::~MemberRegisterWidget()
{
    delete ui;

}

void MemberRegisterWidget::on_Confirm_pushButton_clicked()
{

    if(ui->Name_lineEdit->text().isEmpty()
            || ui->Tel_lineEdit->text().isEmpty()
            || ui->Address_lineEdit->text().isEmpty()
            || ui->Agenda_comboBox->currentText().isEmpty())
    {
        QMessageBox::warning(NULL, tr("提示"), tr("请检查信息是否完整填写！"));
        return;

    }
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
        emit SendMemRegInfo(QString(ui->Name_lineEdit->text()),
                            QString(ui->Agenda_comboBox->currentText()),
                            QString(_birthday),
                            QString(ui->Tel_lineEdit->text()),
                            QString(ui->Address_lineEdit->text()));
        this->close();
    }
    else
    {

    }

}
void MemberRegisterWidget::closeEvent(QCloseEvent *event)
{
    this->close();
}

void MemberRegisterWidget::on_Cancel_pushButton_clicked()
{
    this->close();
}
