#ifndef DETERGENTADDWIDGET_H
#define DETERGENTADDWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class DetergentAddWidget;
}

class DetergentAddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetergentAddWidget(QWidget *parent = 0);
    ~DetergentAddWidget();

private slots:
    void on_Confirm_pushButton_clicked();

    void on_Cancel_pushButton_clicked();
signals:
    void SendDetergentAddInfo(QString,QString);

private:
    Ui::DetergentAddWidget *ui;

};

#endif // DETERGENTADDWIDGET_H
