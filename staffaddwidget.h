#ifndef STAFFADDWIDGET_H
#define STAFFADDWIDGET_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class StaffAddWidget;
}

class StaffAddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StaffAddWidget(QWidget *parent = 0);
    ~StaffAddWidget();

private slots:
    void on_Confirm_pushButton_clicked();

    void on_Cancel_pushButton_clicked();
signals:
    void SendStaffAddInfo(QString,QString,QString,QString);
private:
    Ui::StaffAddWidget *ui;
};

#endif // STAFFADDWIDGET_H
