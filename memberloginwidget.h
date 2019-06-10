#ifndef MEMBERLOGINWIDGET_H
#define MEMBERLOGINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
namespace Ui {
class MemberLoginWidget;
}

class MemberLoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemberLoginWidget(QWidget *parent = 0);
    ~MemberLoginWidget();
signals:
    void SendLoginInfo(QString, QString);

private slots:
    void on_Confirm_pushButton_clicked();

    void on_Cancle_pushButton_clicked();

private:
    Ui::MemberLoginWidget *ui;
};

#endif // MEMBERLOGINWIDGET_H
