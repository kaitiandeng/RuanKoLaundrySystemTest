#include "staffaddwidget.h"
#include "ui_staffaddwidget.h"

StaffAddWidget::StaffAddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StaffAddWidget)
{
    ui->setupUi(this);
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());
}

StaffAddWidget::~StaffAddWidget()
{
    delete ui;
}

void StaffAddWidget::on_Confirm_pushButton_clicked()
{
    if(ui->Name_lineEdit->text().isEmpty()
            ||ui->Tel_lineEdit->text().isEmpty()
            ||ui->Address_textEdit->document()->isEmpty())
    {
        QMessageBox::information(NULL,tr("提示"),tr("请检查信息是否填写完整"));
        return;
    }
    QString _info = ui->Name_label->text() + ui->Name_lineEdit->text() + "\n"
            + ui->Sex_label->text() + ui->Sex_comboBox->currentText() + "\n"
            + ui->Tel_label->text() + ui->Tel_lineEdit->text() + "\n"
            + ui->Address_label->text() + ui->Address_textEdit->document()->toPlainText();

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认信息"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        emit SendStaffAddInfo(QString(ui->Name_lineEdit->text()),
                              QString(ui->Sex_comboBox->currentText()),
                              QString(ui->Tel_lineEdit->text()),
                              QString(ui->Address_textEdit->document()->toPlainText()));
        this->close();
    }

}

void StaffAddWidget::on_Cancel_pushButton_clicked()
{
    this->close();
}
