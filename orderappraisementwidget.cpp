#include "orderappraisementwidget.h"
#include "ui_orderappraisementwidget.h"

OrderAppraisementWidget::OrderAppraisementWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderAppraisementWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
}

OrderAppraisementWidget::~OrderAppraisementWidget()
{
    delete ui;
}

void OrderAppraisementWidget::on_Confirm_pushButton_clicked()
{
    if(ui->textEdit->document()->isEmpty())
        QMessageBox::information(NULL,tr("提示"),tr("评论不能为空"));
    else
    {
        emit SendAppraisementInfo(ui->textEdit->document()->toPlainText());
        this->close();
    }

}

void OrderAppraisementWidget::on_Cancel_pushButton_clicked()
{
    this->close();
}
