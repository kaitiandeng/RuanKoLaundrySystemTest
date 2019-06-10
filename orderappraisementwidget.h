#ifndef ORDERAPPRAISEMENTWIDGET_H
#define ORDERAPPRAISEMENTWIDGET_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class OrderAppraisementWidget;
}

class OrderAppraisementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderAppraisementWidget(QWidget *parent = 0);
    ~OrderAppraisementWidget();
signals:
    void SendAppraisementInfo(QString);

private slots:
    void on_Confirm_pushButton_clicked();

    void on_Cancel_pushButton_clicked();

private:
    Ui::OrderAppraisementWidget *ui;
};

#endif // ORDERAPPRAISEMENTWIDGET_H
