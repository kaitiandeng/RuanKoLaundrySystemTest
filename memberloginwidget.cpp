#include "memberloginwidget.h"
#include "ui_memberloginwidget.h"

MemberLoginWidget::MemberLoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberLoginWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
    //Tel
    QRegExp TelRegExp("^((1[3,5,8][0-9])|(14[5,7])|(17[0,6,7,8])|(19[7]))\\d{8}$");
    ui->Tel_lineEdit->setValidator(new QRegExpValidator(TelRegExp,this));
}

MemberLoginWidget::~MemberLoginWidget()
{
    delete ui;
}
void MemberLoginWidget::on_Confirm_pushButton_clicked()
{
    //bool isEmpty = ui->Name_lineEdit->text().isEmpty() || ui->Tel_lineEdit->text().isEmpty();
    //    (ui->Name_lineEdit->text().isEmpty() || ui->Tel_lineEdit->text().isEmpty())
    //            ?(QMessageBox::warning(NULL, tr("提示"), tr("请检查信息是否完整填写！")))
    //           :(emit SendLoginInfo(ui->Name_lineEdit->text(),ui->Tel_lineEdit->text()));
    if(ui->Name_lineEdit->text().isEmpty() || ui->Tel_lineEdit->text().isEmpty())
        QMessageBox::warning(NULL, tr("提示"), tr("请检查信息是否完整填写！"));
    else
    {
        emit SendLoginInfo(ui->Name_lineEdit->text(),ui->Tel_lineEdit->text());
        this->close();
    }
}

void MemberLoginWidget::on_Cancle_pushButton_clicked()
{
    this->close();
}
