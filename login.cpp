#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ConnectToDataBase();
    registerPage = new Register;
    systemPage = new System;
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
        //注册按键
        connect(ui->Register_pushButton, SIGNAL(clicked(bool)), this, SLOT(On_RegisterBtn_Clicked()));
        //注册页面关闭
        connect(registerPage, SIGNAL(Closed()), this, SLOT(Reshow()));
        //注册反馈信息
        connect(registerPage, SIGNAL(SendRegInfo(QString,QString)), this, SLOT(RegisterUser(QString,QString)));
        //登陆按键
        connect(ui->Login_pushButton, SIGNAL(clicked(bool)), this, SLOT(On_LoginBtn_Clicked()));
        //注销时
        connect(systemPage, SIGNAL(isLogout()), this, SLOT(Reshow()));
    }

}

Login::~Login()
{
    delete ui;
}


void Login::ConnectToDataBase()
{
    //SQLite
    DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        qDebug()<<"Connect to DataBase";
    }
    else
    {
        qDebug()<<"Fail to connect to DataBase";
        QMessageBox::critical(NULL, tr("错误"), tr("连接数据库失败！"));
        this->close();
    }
}

void Login::On_LoginBtn_Clicked()
{
    DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    QString _account = ui->Id_lineEdit->text();
    QString _password = ui->Password_lineEdit->text();
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        QString select_sql = "SELECT account, password FROM admin";
        if(!sql_query.exec(select_sql))
        {
            qDebug()<<sql_query.lastError();
            QMessageBox::information(NULL,tr("提示"),tr("密码错误"));
        }
        else
        {
            QList<QString> _accountList;
            QList<QString> _passwordList;
            bool isCorrect = false;
            int _total = 0;
            while(sql_query.next())
            {
                _accountList.append(sql_query.value(0).toString());
                _passwordList.append(sql_query.value(1).toString());
                _total++;
            }
            for(int i=0;i<_total;i++)
            {
                if(_accountList.value(i) == _account && _passwordList.value(i) == _password)
                    isCorrect = true;
            }
            if(isCorrect)
            {
                QMessageBox::information(NULL,tr("提示"),tr("登陆成功"));
                this->hide();
                systemPage->show();
            }
            else
                QMessageBox::information(NULL,tr("提示"),tr("密码错误"));

        }

    }
}

void Login::On_RegisterBtn_Clicked()
{
    this->hide();
    registerPage->show();
}

void Login::Reshow()
{
    ui->Password_lineEdit->clear();
    this->show();
}

void Login::RegisterUser(QString _account, QString _password)
{
    DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        //QString create_sql = "create table admin (id char(30), password char(30))";
        QString create_sql = "CREATE TABLE admin(id INTEGER PRIMARY KEY AUTOINCREMENT, account CHAR, password CHAR)";
        //create table if not exists nodetype(id integer PRIMARY KEY autoincrement,type int)
        QString insert_sql = "INSERT INTO admin (account, password) VALUES(?,?)";
        bool isAdminTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='admin'"));
        if(isAdminTableExist)
        {
            qDebug()<<"Admin exists";
            //查询所有用户名
            QString select_all_sql = "select * from admin";
            bool isAccountExists = false;
            sql_query.prepare(select_all_sql);
            if(!sql_query.exec())
                qDebug()<<sql_query.lastError();
            else
            {
                QList<QString> _accountList;
                while(sql_query.next())
                {
                    _accountList.append(sql_query.value(1).toString());
                }
                for(QList<QString>::iterator it = _accountList.begin(); it < _accountList.end();it++)
                {
                    if(*it == _account)
                        isAccountExists = true;
                }
                if(isAccountExists)
                    QMessageBox::critical(NULL, tr("错误"), tr("用户名已存在"));
                else
                {
                    //插入数据
                    sql_query.prepare(insert_sql);

                    QVariantList GroupAccounts;
                    GroupAccounts.append(_account);
                    QVariantList GroupPasswords;
                    GroupPasswords.append(_password);

                    sql_query.addBindValue(GroupAccounts);
                    sql_query.addBindValue(GroupPasswords);

                    if(!sql_query.execBatch())
                    {
                        qDebug()<<"Insert into admin table failed! "<<sql_query.lastError();
                        QMessageBox::critical(NULL, tr("错误"), tr("创建新用户失败"));
                    }
                    else
                    {
                        qDebug()<<"Insert into admin table succeed";
                    }
                }
            }

        }
        else
        {
            qDebug()<<"Admin does not exist";

            sql_query.prepare(create_sql); //创建表
            if(!sql_query.exec()) //查看创建表是否成功
            {
                QMessageBox::critical(NULL, tr("错误"), tr("创建新用户失败"));
                qDebug()<<"Error:Admin Table Create Failed ->"<<sql_query.lastError();
            }
            else
            {
                qDebug()<<tr("Admin Table Created!");
                //插入数据

                sql_query.prepare(insert_sql);

                QVariantList GroupAccounts;
                GroupAccounts.append(_account);
                QVariantList GroupPasswords;
                GroupPasswords.append(_password);

                sql_query.addBindValue(GroupAccounts);
                sql_query.addBindValue(GroupPasswords);

                if(!sql_query.execBatch())
                {
                    qDebug()<<"Insert into admin table failed! "<<sql_query.lastError();
                    QMessageBox::critical(NULL, tr("错误"), tr("创建新用户失败"));
                }
                else
                {
                    qDebug()<<"Insert into admin table succeed";
                }
            }

            //查询所有记录
            QString select_all_sql = "select * from admin";
            sql_query.prepare(select_all_sql);
            if(!sql_query.exec())
                qDebug()<<sql_query.lastError();
            else
            {
                while(sql_query.next())
                {
                    QString id = sql_query.value(0).toString();
                    QString account = sql_query.value(1).toString();
                    QString password = sql_query.value(2).toString();
                    qDebug()<<QString("ID:%1 Account:%2 Password:%3").arg(id).arg(account).arg(password);
                }
            }
        }
    }
}
