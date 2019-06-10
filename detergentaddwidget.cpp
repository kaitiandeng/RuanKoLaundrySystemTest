#include "detergentaddwidget.h"
#include "ui_detergentaddwidget.h"

DetergentAddWidget::DetergentAddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetergentAddWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
}

DetergentAddWidget::~DetergentAddWidget()
{
    delete ui;
}

void DetergentAddWidget::on_Confirm_pushButton_clicked()
{
    if(ui->Name_lineEdit->text().isEmpty()
            ||ui->Num_lineEdit->text().isEmpty())
    {
        QMessageBox::information(NULL,tr("提示"),tr("请检查信息是否填写完整"));
        return;
    }
    QString _info = ui->Name_label->text() + ui->Name_lineEdit->text() + "\n"
            + ui->Num_label->text() + ui->Num_lineEdit->text();

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认信息"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        emit SendDetergentAddInfo(QString(ui->Name_lineEdit->text()),
                                  QString(ui->Num_lineEdit->text()));
        this->close();
    }

}

void DetergentAddWidget::on_Cancel_pushButton_clicked()
{
    this->close();
}
