#ifndef STAFFINFOCHANGEWIDGET_H
#define STAFFINFOCHANGEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class StaffInfoChangeWidget;
}

class StaffInfoChangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StaffInfoChangeWidget(QWidget *parent = 0);
    ~StaffInfoChangeWidget();
private slots:
    void DisplayCurrentSelectedData(QString,QString,QString,QString);
    void on_Confirm_pushButton_clicked();

    void on_Cancel_pushButton_clicked();

signals:
    void SendChangedStaffInfo(QString,QString,QString,QString,
                              QString,QString,QString,QString);
private:
    Ui::StaffInfoChangeWidget *ui;
    QString NAME;
    QString GENDER;
    QString TEL;
    QString ADDRESS;
};

#endif // STAFFINFOCHANGEWIDGET_H
