#include "storeaddwidget.h"
#include "ui_storeaddwidget.h"

StoreAddWidget::StoreAddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StoreAddWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
    //Name
    QRegExp NameRegExp("^[\u4E00-\u9FA5A-Za-z]+$");
    ui->StoreLeader_lineEdit->setValidator(new QRegExpValidator(NameRegExp,this));
    //Tel 座机+电话
    QRegExp TelRegExp("^((1[3,5,8][0-9])|(14[5,7])|(17[0,6,7,8])|(19[7]))\\d{8}$");
    ui->StoreTel_lineEdit->setValidator(new QRegExpValidator(TelRegExp,this));
}

StoreAddWidget::~StoreAddWidget()
{
    delete ui;
}

void StoreAddWidget::on_Cancel_pushButton_clicked()
{
    this->close();
}

void StoreAddWidget::on_Confirm_pushButton_clicked()
{
    if(ui->StoreName_lineEdit->text().isEmpty()
            || ui->StoreAddress_lineEdit->text().isEmpty()
            || ui->StoreLeader_lineEdit->text().isEmpty()
            || ui->StoreTel_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, tr("提示"), tr("请检查信息是否完整填写！"));
        return;

    }

    QString _info = ui->StoreName_label->text() + ui->StoreName_lineEdit->text() + "\n"
            + ui->StoreAddress_label->text() + ui->StoreAddress_lineEdit->text() + "\n"
            + ui->StoreLeader_label->text() + ui->StoreLeader_lineEdit->text() + "\n"
            + ui->StoreTel_label->text() + ui->StoreTel_lineEdit->text();

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认信息"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        emit SendStoreAddInfo(QString(ui->StoreName_lineEdit->text()),
                              QString(ui->StoreAddress_lineEdit->text()),
                              QString(ui->StoreLeader_lineEdit->text()),
                              QString(ui->StoreTel_lineEdit->text()));
        this->close();
    }

}
