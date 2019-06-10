#include "detergentinfochangewidget.h"
#include "ui_detergentinfochangewidget.h"

DetergentInfoChangeWidget::DetergentInfoChangeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetergentInfoChangeWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
}

DetergentInfoChangeWidget::~DetergentInfoChangeWidget()
{
    delete ui;
}

void DetergentInfoChangeWidget::DisplayCurrentSelectedData(QString _name, QString _num)
{
    qDebug()<<"Receive ->"<<_name<<_num;
    NAME = _name;
    NUM=_num;
    ui->Name_lineEdit->setText(_name);
    ui->Num_lineEdit->setText(_num);
}

void DetergentInfoChangeWidget::on_Confirm_pushButton_clicked()
{
    QString _info = ui->Name_label->text() + ui->Name_lineEdit->text() + "\n"
            + ui->Num_label->text() + ui->Num_lineEdit->text();

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认信息"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        emit SendChangedDetergentInfo(QString(ui->Name_lineEdit->text()),
                                   QString(ui->Num_lineEdit->text()),
                                   NAME,
                                   NUM);
        this->close();
    }

}

void DetergentInfoChangeWidget::on_Cancel_pushButton_clicked()
{
    this->close();
}
