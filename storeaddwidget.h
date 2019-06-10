#ifndef STOREADDWIDGET_H
#define STOREADDWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class StoreAddWidget;
}

class StoreAddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StoreAddWidget(QWidget *parent = 0);
    ~StoreAddWidget();

private slots:
    void on_Cancel_pushButton_clicked();

    void on_Confirm_pushButton_clicked();
signals:
    void SendStoreAddInfo(QString,QString,QString,QString);
private:
    Ui::StoreAddWidget *ui;
};

#endif // STOREADDWIDGET_H
