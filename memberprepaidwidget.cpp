#include "memberprepaidwidget.h"
#include "ui_memberprepaidwidget.h"

MemberPrepaidWidget::MemberPrepaidWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberPrepaidWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
}

MemberPrepaidWidget::~MemberPrepaidWidget()
{
    delete ui;
}

void MemberPrepaidWidget::on_Confirm_pushButton_2_clicked()
{
    if(ui->Name_lineEdit_2->text().isEmpty() || ui->Tel_lineEdit_2->text().isEmpty() || ui->Month_spinBox->text().isEmpty())
            QMessageBox::warning(NULL, tr("提示"), tr("请检查信息是否完整填写！"));
    else
    {
        SendMemberPrepaidInfo(ui->Name_lineEdit_2->text(), ui->Tel_lineEdit_2->text(), ui->Month_spinBox->text());
        this->close();
    }
}

void MemberPrepaidWidget::on_Cancle_pushButton_2_clicked()
{
    this->close();
}
