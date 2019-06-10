#ifndef MEMBERINFOCHANGEWIDGET_H
#define MEMBERINFOCHANGEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MemberInfoChangeWidget;
}

class MemberInfoChangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemberInfoChangeWidget(QWidget *parent = 0);
    ~MemberInfoChangeWidget();
private slots:
    void DisplayCurrentSelectedData(QString,QString,QString,QString,QString);

    void on_Confirm_pushButton_clicked();

    void on_Cancle_pushButton_clicked();
signals:
    void SendChangedMemberInfo(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
private:
    Ui::MemberInfoChangeWidget *ui;
    QString NAME;
    QString GENDER;
    QString BIRHDAY;
    QString TEL;
    QString ADDRESS;
};

#endif // MEMBERINFOCHANGEWIDGET_H
