#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
//SQLite
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include "register.h"
#include "system.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
private slots:
    void On_LoginBtn_Clicked();
    void On_RegisterBtn_Clicked();
    void Reshow();
    void RegisterUser(QString, QString);
signals:
    void LoggedIn();
private:
    Ui::Login *ui;
    //SQLite
    QSqlDatabase DataBase;
    void ConnectToDataBase();
    Register* registerPage;
    System* systemPage;
};

#endif // LOGIN_H
