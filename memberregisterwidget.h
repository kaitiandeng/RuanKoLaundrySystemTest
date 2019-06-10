#ifndef MEMBERREGISTERWIDGET_H
#define MEMBERREGISTERWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

namespace Ui {
class MemberRegisterWidget;
}

class MemberRegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemberRegisterWidget(QWidget *parent = 0);
    ~MemberRegisterWidget();
private:
    void closeEvent(QCloseEvent *event);
signals:
    void SendMemRegInfo(QString, QString, QString, QString, QString);
private slots:
    //void RegisterMember(QString, QString, QString, QString, QString);
    void on_Confirm_pushButton_clicked();

    void on_Cancel_pushButton_clicked();

private:
    Ui::MemberRegisterWidget *ui;
};

#endif // MEMBERREGISTERWIDGET_H
