#ifndef STOREINFOCHANGEWIDGET_H
#define STOREINFOCHANGEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class StoreInfoChangeWidget;
}

class StoreInfoChangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StoreInfoChangeWidget(QWidget *parent = 0);
    ~StoreInfoChangeWidget();

private slots:
    void on_Confirm_pushButton_clicked();

    void on_Cancel_pushButton_clicked();

    void DisplayCurrentSelectedStoreInfo(QString,QString,QString,QString,QString,QString);
signals:
    void SendChangedStoreInfo(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
private:
    Ui::StoreInfoChangeWidget *ui;
    QString NAME;
    QString ADDRESS;
    QString LEADER;
    QString TEL;
    QString OPERATION;
    QString TURNOVER;
};

#endif // STOREINFOCHANGEWIDGET_H
