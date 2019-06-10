#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
    //账户 密码 格式设置
    {
        //ID
        QRegExp IDRegExp("^[a-zA-Z0-9_-]{4,16}$");
        ui->Id_lineEdit->setValidator(new QRegExpValidator(IDRegExp,this));
        //Password
        QRegExp PasswordRegExp("^[a-zA-Z0-9]{6,18}$");
        ui->Password_lineEdit->setValidator(new QRegExpValidator(PasswordRegExp,this));

    }
    //Connect
    {
        connect(ui->Cancel_pushButton, SIGNAL(clicked(bool)), this, SLOT(On_CancelBtn_Clicked()));
        connect(ui->Confirm_pushButton, SIGNAL(clicked(bool)), this, SLOT(On_ConfirmBtn_Clicked()));
        //connect(this, SIGNAL(SendRegInfo(QString,QString)), this, SLOT(RegisterUser(QString,QString)));
    }
}

Register::~Register()
{
    delete ui;
}

void Register::closeEvent(QCloseEvent *event)
{
    emit Closed();
}

void Register::On_CancelBtn_Clicked()
{
    emit close();
}
void Register::On_ConfirmBtn_Clicked()
{
    //if(ui->Id_lineEdit->text() != NULL || ui->Password_lineEdit->text() != NULL)
    if(ui->Id_lineEdit->text().isEmpty() || ui->Password_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, tr("提示"), tr("请检查信息是否完整填写！"));
        return;
    }
    else
    {
        QString _info = tr("账户：") + ui->Id_lineEdit->text() + "\n"
                + tr("密码：") + ui->Password_lineEdit->text() + "\n";
        QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认信息"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(msgBox == QMessageBox::Yes)
        {
            emit SendRegInfo(ui->Id_lineEdit->text(), ui->Password_lineEdit->text());
            //Regist User
            emit Closed();
            this->close();
        }
    }
}
