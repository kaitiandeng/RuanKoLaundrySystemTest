#ifndef DETERGENTINFOCHANGEWIDGET_H
#define DETERGENTINFOCHANGEWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class DetergentInfoChangeWidget;
}

class DetergentInfoChangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetergentInfoChangeWidget(QWidget *parent = 0);
    ~DetergentInfoChangeWidget();
private slots:
    void DisplayCurrentSelectedData(QString,QString);
    void on_Confirm_pushButton_clicked();

    void on_Cancel_pushButton_clicked();

signals:
    void SendChangedDetergentInfo(QString,QString,QString,QString);
private:
    Ui::DetergentInfoChangeWidget *ui;
    QString NAME;
    QString NUM;
};

#endif // DETERGENTINFOCHANGEWIDGET_H
