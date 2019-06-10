#ifndef SYSTEM_H
#define SYSTEM_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QTableView>
#include <QtCharts/QtCharts>
#include <QDate>
#include <QStandardItem>
#include <QStandardItemModel>

#include "memberregisterwidget.h"
#include "memberloginwidget.h"
#include "memberprepaidwidget.h"
#include "memberinfochangewidget.h"

#include "storeaddwidget.h"
#include "storeinfochangewidget.h"

#include "orderappraisementwidget.h"
#include "staffaddwidget.h"
#include "staffinfochangewidget.h"
#include "detergentaddwidget.h"
#include "detergentinfochangewidget.h"

namespace Ui {
class System;
}

class System : public QWidget
{
    Q_OBJECT

public:
    explicit System(QWidget *parent = 0);
    ~System();
public slots:
    //Member Model
    void On_MemRegBtn_clicked();//MemberRegister_pushButton
    void On_MemLogInBtn_clicked();//MemberLoginIn_pushButton
    void On_MemPpBtn_clicked();//MemberPrepaid_pushButton
    void updateMemberTable(QString,QString,QString,QString,QString);
    void changeMemberTableInfo(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void initMemberTable();
    void loginMember(QString, QString);
    void addMembership(QString, QString, QString);
    //Store Model
    //Store Table
    void initStoreTable();
    void addStoreTable(QString,QString,QString,QString);
    void changeStoreTableInfo(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    //Store Turnover
    void ShowStoreChart();

    //Order Model
    //Order Table
    void initOrderTable();
    void GetAppraisementInfo(QString);
    //Customer Table
    void initCustomerTable();
    //Staff Table
    void initStaffTable();
    void addStaffTable(QString,QString,QString,QString);
    void changeStaffTableInfo(QString,QString,QString,QString,
                              QString,QString,QString,QString);
    //Delergent Table
    void initDetergentTable();
    void addDetergentTable(QString,QString);
    void changeDetergentTable(QString,QString,QString,QString);
    //Shipping Table
    void initShippingTable();



signals:
    void isLogout();
    //Member Model
    void SendCurrentSelectedMemberInfo(QString,QString,QString,QString,QString);
    //Store Model
    void SendCurrentSelectedStoreInfo(QString,QString,QString,QString,QString,QString);
    void StoreInfoChanged();
    //Customer Table
    void OrderTableChanged();
    //Staff Table
    void StaffInfoChanged();
    void SendCurrentSelectedStaffInfo(QString,QString,QString,QString);
    void DetergentInfoChanged();
    void SendCurrentSelectedDetergentInfo(QString,QString);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_ChangeMemberInfo_pushButton_clicked();

    void on_Member_tableView_doubleClicked(const QModelIndex &index);

    void on_StoreAdd_pushButton_clicked();

    void on_StoreDelete_pushButton_clicked();

    void on_StoreInfoChange_pushButton_clicked();


    void on_Store_tableView_doubleClicked(const QModelIndex &index);

    void on_MemberDelete_pushButton_clicked();

    void on_SearchOrder_pushButton_clicked();

    void on_DeleteOrder_pushButton_clicked();

    void on_MakeOrder_pushButton_clicked();

    void on_PieChart_pushButton_clicked();

    void on_LineChart_pushButton_clicked();

    void on_Order_tableView_doubleClicked(const QModelIndex &index);

    void on_FinishOrder_pushButton_clicked();

    void on_Appraisement_pushButton_clicked();


    void on_AddStaff_pushButton_clicked();

    void on_DeleteStaff_pushButton_clicked();

    void on_AlterStaff_pushButton_clicked();

    void on_Staff_tableView_doubleClicked(const QModelIndex &index);

    void on_AddDetergent_pushButton_clicked();

    void on_DeleteDetergent_pushButton_clicked();

    void on_AlterDetergent_pushButton_clicked();

    void on_Detergent_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::System *ui;
    //Member Model
    MemberInfoChangeWidget *memInfoChangeWidget;
    MemberRegisterWidget *memRegWidget;
    MemberLoginWidget *memLgnWidget;
    MemberPrepaidWidget *memPpWidget;
    //Store Model
    StoreAddWidget *storeAddWidget;
    StoreInfoChangeWidget *storeInfoChangeWidget;
    //Order Model
    OrderAppraisementWidget *orderAppraisementWidget;
    //Laundry Model
    StaffAddWidget *staffAddWidget;
    StaffInfoChangeWidget *staffInfoChangeWidget;
    DetergentAddWidget *detergentAddWidget;
    DetergentInfoChangeWidget *detergentInfoChangeWidget;

    void initTableModel(QString _tableName, QTableView *_tableView, QList<int> *_removeColumns, QMap<int, QString> *_headerData);
};

#endif // SYSTEM_H
