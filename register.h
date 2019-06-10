#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();
private:
    void closeEvent(QCloseEvent *event);
private slots:
    void On_CancelBtn_Clicked();
    void On_ConfirmBtn_Clicked();
    //void RegisterUser(QString, QString);

signals:
    void Closed();
    void SendRegInfo(QString, QString);
private:
    Ui::Register *ui;
};

#endif // REGISTER_H
