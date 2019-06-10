#ifndef MEMBERPREPAIDWIDGET_H
#define MEMBERPREPAIDWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MemberPrepaidWidget;
}

class MemberPrepaidWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemberPrepaidWidget(QWidget *parent = 0);
    ~MemberPrepaidWidget();
signals:
    void SendMemberPrepaidInfo(QString, QString, QString);

private slots:
    void on_Confirm_pushButton_2_clicked();

    void on_Cancle_pushButton_2_clicked();

private:
    Ui::MemberPrepaidWidget *ui;
};

#endif // MEMBERPREPAIDWIDGET_H
