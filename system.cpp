#include "system.h"
#include "ui_system.h"

System::System(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::System)
{
    ui->setupUi(this);

    memRegWidget = new MemberRegisterWidget;
    memLgnWidget = new MemberLoginWidget;
    memPpWidget = new MemberPrepaidWidget;
    memInfoChangeWidget = new MemberInfoChangeWidget;

    storeAddWidget = new StoreAddWidget;
    storeInfoChangeWidget = new StoreInfoChangeWidget;

    orderAppraisementWidget = new OrderAppraisementWidget;

    staffAddWidget = new StaffAddWidget;
    staffInfoChangeWidget = new StaffInfoChangeWidget;

    detergentAddWidget = new DetergentAddWidget;
    detergentInfoChangeWidget = new DetergentInfoChangeWidget;

    setWindowTitle(tr("软酷洗衣店管理系统"));
    //关闭缩小+全屏窗口
    setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    //关闭缩小窗口
    //setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);
    setFixedHeight(frameGeometry().height());
    setFixedWidth(frameGeometry().width());

    //会员表属性设置
    {
        ui->Member_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//只能选中行
        ui->Member_tableView->setSelectionMode ( QAbstractItemView::SingleSelection);//只能单选
        ui->Member_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可修改
    }
    //店铺表属性设置
    {
        ui->Store_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//只能选中行
        ui->Store_tableView->setSelectionMode ( QAbstractItemView::SingleSelection);//只能单选
        ui->Store_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可修改

        //ui->Store_tableView->setMaximumHeight(ui->Store_tab->height()-30);
    }
    //订单表属性设置
    {
        ui->Order_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//只能选中行
        ui->Order_tableView->setSelectionMode ( QAbstractItemView::SingleSelection);//只能单选
        ui->Order_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可修改
    }
    //Connect
    {
        //Member Model
        {
            connect(ui->MemberRegister_pushButton, SIGNAL(clicked(bool)), this, SLOT(On_MemRegBtn_clicked()));
            connect(ui->MemberLoginIn_pushButton, SIGNAL(clicked(bool)), this, SLOT(On_MemLogInBtn_clicked()));
            connect(ui->MemberPrepaid_pushButton, SIGNAL(clicked(bool)), this, SLOT(On_MemPpBtn_clicked()));
            //connect(memRegWidget, SIGNAL(SendMemRegInfo(QString,QString,QString,QString,QString)), this, SLOT(initMemberTable()));
            connect(memRegWidget, SIGNAL(SendMemRegInfo(QString,QString,QString,QString,QString)), this, SLOT(updateMemberTable(QString,QString,QString,QString,QString)));
            connect(memLgnWidget, SIGNAL(SendLoginInfo(QString,QString)), this, SLOT(loginMember(QString,QString)));
            connect(memPpWidget, SIGNAL(SendMemberPrepaidInfo(QString,QString,QString)), this, SLOT(addMembership(QString,QString,QString)));
            //用户信息修改
            connect(this, SIGNAL(SendCurrentSelectedMemberInfo(QString,QString,QString,QString,QString)), memInfoChangeWidget, SLOT(DisplayCurrentSelectedData(QString,QString,QString,QString,QString)));
            connect(memInfoChangeWidget, SIGNAL(SendChangedMemberInfo(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(changeMemberTableInfo(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));
        }
        //Store Model
        {
            connect(storeAddWidget, SIGNAL(SendStoreAddInfo(QString,QString,QString,QString)), this, SLOT(addStoreTable(QString,QString,QString,QString)));
            connect(this, SIGNAL(SendCurrentSelectedStoreInfo(QString,QString,QString,QString,QString,QString)), storeInfoChangeWidget, SLOT(DisplayCurrentSelectedStoreInfo(QString,QString,QString,QString,QString,QString)));
            connect(storeInfoChangeWidget, SIGNAL(SendChangedStoreInfo(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(changeStoreTableInfo(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));
            connect(this, SIGNAL(StoreInfoChanged()), this, SLOT(ShowStoreChart()));
        }
        //Order Model
        {
            connect(orderAppraisementWidget, SIGNAL(SendAppraisementInfo(QString)), this, SLOT(GetAppraisementInfo(QString)));

        }
        //Landury Model
        {
            connect(this,SIGNAL(OrderTableChanged()),this,SLOT(initCustomerTable()));
            connect(staffAddWidget,SIGNAL(SendStaffAddInfo(QString,QString,QString,QString)),this,SLOT(addStaffTable(QString,QString,QString,QString)));
            connect(this,SIGNAL(StaffInfoChanged()),this,SLOT(initStaffTable()));

            connect(this,SIGNAL(SendCurrentSelectedStaffInfo(QString,QString,QString,QString)),staffInfoChangeWidget,SLOT(DisplayCurrentSelectedData(QString,QString,QString,QString)));
            connect(staffInfoChangeWidget,SIGNAL(SendChangedStaffInfo(QString,QString,QString,QString,QString,QString,QString,QString)),this,SLOT(changeStaffTableInfo(QString,QString,QString,QString,QString,QString,QString,QString)));

            connect(detergentAddWidget,SIGNAL(SendDetergentAddInfo(QString,QString)),this,SLOT(addDetergentTable(QString,QString)));
            connect(this,SIGNAL(DetergentInfoChanged()),this,SLOT(initDetergentTable()));

            connect(this,SIGNAL(SendCurrentSelectedDetergentInfo(QString,QString)),detergentInfoChangeWidget,SLOT(DisplayCurrentSelectedData(QString,QString)));
            connect(detergentInfoChangeWidget,SIGNAL(SendChangedDetergentInfo(QString,QString,QString,QString)),this,SLOT(changeDetergentTable(QString,QString,QString,QString)));
        }
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("DataBase.db");
    db.setUserName("cqupt2016214290");
    db.setPassword("cqupt123456");
    if(db.open())
    {
        //init
        initMemberTable();//会员表
        initStoreTable();//店铺表
        ShowStoreChart();//店铺图

        initOrderTable();//订单表
        initCustomerTable();//客户表
        initStaffTable();//员工表
        initDetergentTable();//洗衣品表
        initShippingTable();//配送表
    }

}
void System::updateMemberTable(QString _name, QString _gender, QString _birthday, QString _tel, QString _address)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        /*
        方法一：找到已经存在的所有表，手动判断是否需要建表
        SELECT name FROM SQLITE_MASTER WHERE type='table'ORDER BY name"
        返回的结果集将包含所有的存在的表。
        方法二：建表时sqlite自动判断
        create table if not exists nodetype(id integer PRIMARY KEY autoincrement,type int)
         */
        QSqlQuery sql_query;
        bool isMemberTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='member'"));
        QString insert_sql = "INSERT INTO member (name , gender , birthday , tel , address , isLogined , membership) VALUES(?,?,?,?,?,?,?)";
        if(isMemberTableExist)
        {
            qDebug() << "member table exists";

            //插入数据
            sql_query.prepare(insert_sql);

            QVariantList GroupNames; GroupNames.append(_name);
            QVariantList GroupGenders; GroupGenders.append(_gender);
            QVariantList GroupBirthdays; GroupBirthdays.append(_birthday);
            QVariantList GroupTels; GroupTels.append(_tel);
            QVariantList GroupAddresses; GroupAddresses.append(_address);
            QVariantList GroupIsLogined; GroupIsLogined.append(tr("否"));
            QVariantList GroupMemberships; GroupMemberships.append(0);

            sql_query.addBindValue(GroupNames);
            sql_query.addBindValue(GroupGenders);
            sql_query.addBindValue(GroupBirthdays);
            sql_query.addBindValue(GroupTels);
            sql_query.addBindValue(GroupAddresses);
            sql_query.addBindValue(GroupIsLogined);
            sql_query.addBindValue(GroupMemberships);

            if(!sql_query.execBatch())
            {
                qDebug()<<"Insert into member table failed! "<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("创建新会员失败"));
            }
            else
            {
                qDebug()<<"Insert into member table succeed";
                initMemberTable();
            }
        }
        else
        {
            qDebug() << "member table does not exist";
            QString create_sql = "CREATE TABLE admin(id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR, gender CHAR, birthday DATE, tel INTEGER, address CHAR, isLogined TEXT, membership INTEGER)";

            sql_query.prepare(create_sql); //创建表
            if(!sql_query.exec()) //查看创建表是否成功
            {
                QMessageBox::critical(NULL, tr("错误"), tr("创建新会员失败"));
                qDebug()<<"Error:Member Table Create Failed ->"<<sql_query.lastError();
            }
            else
            {
                qDebug()<<tr("Member Table Created!");
                //插入数据

                sql_query.prepare(insert_sql);

                QVariantList GroupNames; GroupNames.append(_name);
                QVariantList GroupGenders; GroupGenders.append(_gender);
                QVariantList GroupBirthdays; GroupBirthdays.append(_birthday);
                QVariantList GroupTels; GroupTels.append(_tel);
                QVariantList GroupAddresses; GroupAddresses.append(_address);
                QVariantList GroupIsLogined; GroupIsLogined.append(tr("否"));
                QVariantList GroupMemberships; GroupMemberships.append(0);

                sql_query.addBindValue(GroupNames);
                sql_query.addBindValue(GroupGenders);
                sql_query.addBindValue(GroupBirthdays);
                sql_query.addBindValue(GroupTels);
                sql_query.addBindValue(GroupAddresses);
                sql_query.addBindValue(GroupIsLogined);
                sql_query.addBindValue(GroupMemberships);

                if(!sql_query.execBatch())
                {
                    qDebug()<<"Insert into member table failed! "<<sql_query.lastError();
                    QMessageBox::critical(NULL, tr("错误"), tr("创建新会员失败"));
                }
                else
                {
                    qDebug()<<"Insert into member table succeed";
                    initMemberTable();
                }
            }
        }
    }

}

void System::changeMemberTableInfo(QString _name, QString _gender, QString _birthday, QString _tel, QString _address,
                                   QString name, QString gender, QString birthday, QString tel, QString address)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isMemberTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='member'"));
        if(isMemberTableExist)
        {
            qDebug() << "member table exists";
            //UPDATE member SET name='kpp',gender='男',birthday='1997/11/21',tel='138565464',address='CQUPT-21111' WHERE id='6';
            //id有毒
            //QString sql_update = "UPDATE member SET name='"+_name+"',gender='"+_gender+"',birthday='"+_birthday+"',tel='"+_tel+"',address='"+_address+"' WHERE id='"+_id+"';";

            //UPDATE member SET name='匡俊嘉2',gender='男',birthday='1997/11/21',tel='138565464',address='CQUPT-21111' WHERE name='匡俊嘉' AND gender='男' AND birthday='1998/11/21' AND tel='13000000000' AND address='CQUPT';
            QString sql_update = "UPDATE member SET name='"+_name+"',gender='"+_gender+"',birthday='"+_birthday+"',tel='"+_tel+"',address='"+_address+"' WHERE name='"+name+"' AND gender='"+gender+"' AND birthday='"+birthday+"' AND tel='"+tel+"' AND address='"+address+"';";
            qDebug()<<sql_update;
            //修改数据
            sql_query.prepare(sql_update);
            if(!sql_query.exec())
            {
                qDebug()<<"Update members' info failed! "<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("修改会员信息失败！-2"));
            }
            else
            {
                qDebug()<<"Update members' info succeed";
                initMemberTable();
            }

        }
        else
            QMessageBox::critical(NULL, tr("错误"), tr("修改会员信息失败！-3"));
    }
    else
        QMessageBox::critical(NULL, tr("错误"), tr("修改会员信息失败！-4"));
}

void System::initMemberTable()
{
    //init
    QList<int>* _removeColumns = new QList<int>();
    QMap<int, QString>* _headerData = new QMap<int, QString>();
    _removeColumns->append(0);
    _headerData->insert(0,tr("姓名"));_headerData->insert(1,tr("性别"));
    _headerData->insert(2,tr("生日"));_headerData->insert(3,tr("电话"));
    _headerData->insert(4,tr("地址"));_headerData->insert(5,tr("是否登陆"));
    _headerData->insert(6,tr("剩余时长/月"));
    initTableModel("member",ui->Member_tableView,_removeColumns,_headerData);
}

void System::initTableModel(QString _tableName, QTableView* _tableView, QList<int>* _removeColumns = NULL, QMap<int, QString>* _headerData = NULL)
{

    QSqlTableModel* model = new QSqlTableModel(_tableView);
    model->setTable(_tableName);
    model->select();
    if(_removeColumns != NULL)
    {
        for(int i=0;i<_removeColumns->length();i++)
        {
            model->removeColumn(_removeColumns->value(i));
        }
    }
    if(_headerData != NULL)
    {
        for(QMap<int,QString>::iterator it = _headerData->begin(); it != _headerData->end();it++)
        {
            model->setHeaderData(it.key(),Qt::Horizontal,it.value());
        }
    }
    /*
    resizeColumnsToContents(); //根据内容自动调整列宽
    resizeRowsToContents(); //根据内容自动调整行高

    这两句必须在setModel（）之前调用，否则无效。
    */
    //    _tableView->resizeColumnsToContents();//自适应
    //    _tableView->resizeRowsToContents();//自适应


    _tableView->setModel(model);
    _tableView->resizeColumnsToContents();//自适应
    _tableView->resizeRowsToContents();//自适应
}

void System::loginMember(QString _name, QString _tel)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isMemberTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='member'"));
        if(isMemberTableExist)
        {
            //UPDATE member set isLogined='是' WHERE name='lpf' AND tel='13436465654'
            QString sql_update = "UPDATE member SET isLogined='"+tr("是")+"' WHERE name="+"'"+_name+"'"+" AND tel="+"'"+_tel+"'"+";";
            qDebug()<<sql_update;

            QString select_all_sql = "select * from member";
            bool isNameExists = false;
            bool isTelExists = false;
            sql_query.prepare(select_all_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("会员登录失败!-1"));
            }
            else
            {
                QList<QString> _nameList;
                QList<QString> _telList;
                while(sql_query.next())
                {
                    _nameList.append(sql_query.value(1).toString());
                    _telList.append(sql_query.value(4).toString());
                }
                for(QList<QString>::iterator it = _nameList.begin(); it < _nameList.end();it++)
                {
                    if(*it == _name)
                        isNameExists = true;
                }
                for(QList<QString>::iterator it = _telList.begin(); it < _telList.end();it++)
                {
                    if(*it == _tel)
                        isTelExists = true;
                }
                if(isNameExists || isTelExists)
                {
                    sql_query.prepare(sql_update);
                    if(!sql_query.exec())
                    {
                        qDebug()<<sql_query.lastError();
                        QMessageBox::critical(NULL, tr("错误"), tr("会员登录失败!-2"));
                    }
                    initMemberTable();
                }
                else
                    QMessageBox::critical(NULL, tr("错误"), tr("会员登录失败!-3"));
            }
        }
        else
            QMessageBox::critical(NULL, tr("错误"), tr("会员登录失败!-4"));
    }
}

void System::addMembership(QString _name, QString _tel, QString _month)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isMemberTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='member'"));
        if(isMemberTableExist)
        {
            //UPDATE member set isLogined='是' WHERE name='lpf' AND tel='13436465654'
            QString sql_update = "UPDATE member SET membership='"+_month+"' WHERE name="+"'"+_name+"'"+" AND tel="+"'"+_tel+"'"+";";
            qDebug()<<sql_update;

            QString select_all_sql = "select * from member";
            bool isNameExists = false;
            bool isTelExists = false;
            sql_query.prepare(select_all_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("会员续费失败!-1"));
            }
            else
            {
                QList<QString> _nameList;
                QList<QString> _telList;
                while(sql_query.next())
                {
                    _nameList.append(sql_query.value(1).toString());
                    _telList.append(sql_query.value(4).toString());
                }
                for(QList<QString>::iterator it = _nameList.begin(); it < _nameList.end();it++)
                {
                    if(*it == _name)
                        isNameExists = true;
                }
                for(QList<QString>::iterator it = _telList.begin(); it < _telList.end();it++)
                {
                    if(*it == _tel)
                        isTelExists = true;
                }
                if(isNameExists || isTelExists)
                {
                    sql_query.prepare(sql_update);
                    if(!sql_query.exec())
                    {
                        qDebug()<<sql_query.lastError();
                        QMessageBox::critical(NULL, tr("错误"), tr("会员续费失败!-2"));
                    }
                    initMemberTable();
                }
                else
                    QMessageBox::critical(NULL, tr("错误"), tr("会员续费失败!-3"));
            }
        }
        else
            QMessageBox::critical(NULL, tr("错误"), tr("会员续费失败!-4"));
    }
}

System::~System()
{
    delete ui;
}

void System::On_MemLogInBtn_clicked()
{
    memLgnWidget->show();
}

void System::On_MemPpBtn_clicked()
{
    memPpWidget->show();
}

void System::On_MemRegBtn_clicked()
{
    memRegWidget->show();
}

void System::on_pushButton_clicked()
{
    emit isLogout();
    this->close();
}

void System::on_pushButton_2_clicked()
{
    this->close();
}

void System::on_ChangeMemberInfo_pushButton_clicked()
{
    qDebug()<<ui->Member_tableView->currentIndex().row()<<" "<<ui->Member_tableView->currentIndex().column();

    int _currentRow = ui->Member_tableView->currentIndex().row();
    int _currentColum = ui->Member_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;
    QAbstractItemModel *currentModel = ui->Member_tableView->model();
    QModelIndex currentIndex = currentModel->index(_currentRow,_currentColum);
    QVariant currentData = currentModel->data(currentIndex);
    qDebug()<<ui->Member_tableView->model()->data(currentIndex,Qt::DisplayRole).toString();
    qDebug()<<currentData.toString();


    QModelIndex nameIndex = ui->Member_tableView->model()->index(_currentRow,0);
    QString _name = ui->Member_tableView->model()->data(nameIndex).toString();

    QModelIndex genderIndex = ui->Member_tableView->model()->index(_currentRow,1);
    QString _gender = ui->Member_tableView->model()->data(genderIndex).toString();

    QModelIndex birthdayIndex = ui->Member_tableView->model()->index(_currentRow,2);
    QString _birthday = ui->Member_tableView->model()->data(birthdayIndex).toString();

    QModelIndex telIndex = ui->Member_tableView->model()->index(_currentRow,3);
    QString _tel = ui->Member_tableView->model()->data(telIndex).toString();

    QModelIndex _addressIndex = ui->Member_tableView->model()->index(_currentRow,4);
    QString _address = ui->Member_tableView->model()->data(_addressIndex).toString();

    qDebug()<<_name<<"  "<<_gender<<"   "<<_birthday<<" "<<_tel<<"  "<<_address;

    emit SendCurrentSelectedMemberInfo(_name, _gender, _birthday, _tel, _address);
    memInfoChangeWidget->show();
}

void System::on_Member_tableView_doubleClicked(const QModelIndex &index)
{
    on_ChangeMemberInfo_pushButton_clicked();
}

void System::on_StoreAdd_pushButton_clicked()
{
    storeAddWidget->show();
}

void System::on_MemberDelete_pushButton_clicked()
{
    qDebug()<<ui->Member_tableView->currentIndex().row()<<" "<<ui->Member_tableView->currentIndex().column();

    int _currentRow = ui->Member_tableView->currentIndex().row();
    int _currentColum = ui->Member_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QAbstractItemModel *currentModel = ui->Member_tableView->model();
    QModelIndex currentIndex = currentModel->index(_currentRow,_currentColum);
    QVariant currentData = currentModel->data(currentIndex);
    qDebug()<<ui->Member_tableView->model()->data(currentIndex,Qt::DisplayRole).toString();
    qDebug()<<currentData.toString();

    QModelIndex nameIndex = ui->Member_tableView->model()->index(_currentRow,0);
    QString _name = ui->Member_tableView->model()->data(nameIndex).toString();

    QModelIndex genderIndex = ui->Member_tableView->model()->index(_currentRow,1);
    QString _gender = ui->Member_tableView->model()->data(genderIndex).toString();

    QModelIndex birthdayIndex = ui->Member_tableView->model()->index(_currentRow,2);
    QString _birthday = ui->Member_tableView->model()->data(birthdayIndex).toString();

    QModelIndex telIndex = ui->Member_tableView->model()->index(_currentRow,3);
    QString _tel = ui->Member_tableView->model()->data(telIndex).toString();

    QModelIndex addressIndex = ui->Member_tableView->model()->index(_currentRow,4);
    QString _address = ui->Member_tableView->model()->data(addressIndex).toString();

    QModelIndex isLoginedIndex = ui->Member_tableView->model()->index(_currentRow,5);
    QString _isLogined = ui->Member_tableView->model()->data(isLoginedIndex).toString();

    QModelIndex membershipIndex = ui->Member_tableView->model()->index(_currentRow,6);
    QString _membership = ui->Member_tableView->model()->data(membershipIndex).toString();

    qDebug()<<_currentRow+1<<"  "<<_name<<"  "<<_gender<<"   "<<_birthday<<" "<<_tel<<"  "<<_address<<" "<<_isLogined<<"    "<<_membership;

    QString _info = tr("姓名：") + _name + "\n"
            + tr("性别：") + _gender + "\n"
            + tr("生日：") + _birthday + "\n"
            + tr("电话：") + _tel + "\n"
            + tr("地址：") + _address + "\n"
            + tr("登陆状态：") + _isLogined + "\n"
            + tr("剩余时长/月") + _membership + "\n";

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认删除"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
        DataBase.setDatabaseName("DataBase.db");
        DataBase.setUserName("cqupt2016214290");
        DataBase.setPassword("cqupt123456");
        if(DataBase.open())
        {
            QSqlQuery sql_query;
            bool isMemberTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='member'"));
            QString delete_sql = "DELETE FROM member WHERE name='"+_name+"' AND gender='"+_gender+"' AND birthday='"+_birthday+"' AND tel='"+_tel+"' AND address='"+_address+"' AND isLogined='"+_isLogined+"' AND membership='"+_membership+"';";
            if(isMemberTableExist)
            {
                qDebug() << "member table exists";
                sql_query.prepare(delete_sql);
                if(!sql_query.exec())
                    QMessageBox::critical(NULL, tr("错误"), tr("删除会员失败!-1"));
                else
                {
                    QMessageBox::information(NULL, tr("提示"), tr("删除会员成功！"));
                    initMemberTable();
                }
            }
            else
                QMessageBox::critical(NULL, tr("错误"), tr("删除会员失败"));
        }
    }

}

void System::on_StoreDelete_pushButton_clicked()
{
    qDebug()<<ui->Store_tableView->currentIndex().row()<<" "<<ui->Store_tableView->currentIndex().column();

    int _currentRow = ui->Store_tableView->currentIndex().row();
    int _currentColum = ui->Store_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QAbstractItemModel *currentModel = ui->Store_tableView->model();
    QModelIndex currentIndex = currentModel->index(_currentRow,_currentColum);
    QVariant currentData = currentModel->data(currentIndex);
    qDebug()<<ui->Store_tableView->model()->data(currentIndex,Qt::DisplayRole).toString();
    qDebug()<<currentData.toString();

    QModelIndex nameIndex = ui->Store_tableView->model()->index(_currentRow,0);
    QString _name = ui->Store_tableView->model()->data(nameIndex).toString();

    QModelIndex addressIndex = ui->Store_tableView->model()->index(_currentRow,1);
    QString _address = ui->Store_tableView->model()->data(addressIndex).toString();

    QModelIndex leaderIndex = ui->Store_tableView->model()->index(_currentRow,2);
    QString _leader = ui->Store_tableView->model()->data(leaderIndex).toString();

    QModelIndex telIndex = ui->Store_tableView->model()->index(_currentRow,3);
    QString _tel = ui->Store_tableView->model()->data(telIndex).toString();

    QModelIndex operationIndex = ui->Store_tableView->model()->index(_currentRow,4);
    QString _operation = ui->Store_tableView->model()->data(operationIndex).toString();

    QModelIndex turnoverIndex = ui->Store_tableView->model()->index(_currentRow,5);
    QString _turnover = ui->Store_tableView->model()->data(turnoverIndex).toString();

    qDebug()<<_currentRow+1<<"  "<<_name<<"  "<<_address<<"   "<<_leader<<" "<<_tel<<"  "<<_operation<<" "<<_turnover;

    QString _info = tr("店名：") + _name + "\n"
            + tr("地址：") + _address + "\n"
            + tr("负责人：") + _leader + "\n"
            + tr("电话：") + _tel + "\n"
            + tr("状态：") + _operation + "\n"
            + tr("营业额：") + _turnover + "\n";

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认删除"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
        DataBase.setDatabaseName("DataBase.db");
        DataBase.setUserName("cqupt2016214290");
        DataBase.setPassword("cqupt123456");
        if(DataBase.open())
        {
            QSqlQuery sql_query;
            bool isStoreTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='store'"));
            QString delete_sql = "DELETE FROM store WHERE name='"+_name+"' AND address='"+_address+"' AND leader='"+_leader+"' AND tel='"+_tel+"' AND operation='"+_operation+"' AND turnover='"+_turnover+"';";
            if(isStoreTableExist)
            {
                qDebug() << "store table exists";
                sql_query.prepare(delete_sql);
                if(!sql_query.exec())
                    QMessageBox::critical(NULL, tr("错误"), tr("删除店铺失败!-1"));
                else
                {
                    QMessageBox::information(NULL, tr("提示"), tr("删除成功！"));
                    initStoreTable();
                    emit StoreInfoChanged();
                }
            }
            else
                QMessageBox::critical(NULL, tr("错误"), tr("删除店铺失败"));
        }
    }
}

void System::addStoreTable(QString _name, QString _address, QString _leader, QString _tel)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isStoreTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='store'"));
        QString insert_sql = "INSERT INTO store (name , address , leader , tel , operation , turnover ) VALUES(?,?,?,?,?,?)";
        if(isStoreTableExist)
        {
            qDebug() << "store table exists";

            //插入数据
            sql_query.prepare(insert_sql);

            QVariantList GroupNames; GroupNames.append(_name);
            QVariantList GroupAddresses; GroupAddresses.append(_address);
            QVariantList GroupLeaders; GroupLeaders.append(_leader);
            QVariantList GroupTels; GroupTels.append(_tel);
            QVariantList GroupOperations; GroupOperations.append(tr("未营业"));
            QVariantList GroupTurnovers; GroupTurnovers.append(0);

            sql_query.addBindValue(GroupNames);
            sql_query.addBindValue(GroupAddresses);
            sql_query.addBindValue(GroupLeaders);
            sql_query.addBindValue(GroupTels);
            sql_query.addBindValue(GroupOperations);
            sql_query.addBindValue(GroupTurnovers);

            if(!sql_query.execBatch())
            {
                qDebug()<<"Insert into store table failed! "<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("添加店铺失败"));
            }
            else
            {
                qDebug()<<"Insert into store table succeed";
                initStoreTable();
                emit StoreInfoChanged();
            }
        }
        else
        {
            qDebug() << "store table does not exist";
            QString create_sql = "CREATE TABLE store(id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR, address TEXT, leader CHAR, tel INTEGER, operation CHAR, turnover INTEGER)";

            sql_query.prepare(create_sql); //创建表
            if(!sql_query.exec()) //查看创建表是否成功
            {
                QMessageBox::critical(NULL, tr("错误"), tr("添加店铺失败"));
                qDebug()<<"Error:Store Table Create Failed ->"<<sql_query.lastError();
            }
            else
            {
                qDebug()<<tr("Store Table Created!");
                //插入数据
                sql_query.prepare(insert_sql);

                QVariantList GroupNames; GroupNames.append(_name);
                QVariantList GroupAddresses; GroupAddresses.append(_address);
                QVariantList GroupLeaders; GroupLeaders.append(_leader);
                QVariantList GroupTels; GroupTels.append(_tel);
                QVariantList GroupOperations; GroupOperations.append(tr("未营业"));
                QVariantList GroupTurnovers; GroupTurnovers.append(0);

                sql_query.addBindValue(GroupNames);
                sql_query.addBindValue(GroupAddresses);
                sql_query.addBindValue(GroupLeaders);
                sql_query.addBindValue(GroupTels);
                sql_query.addBindValue(GroupOperations);
                sql_query.addBindValue(GroupTurnovers);

                if(!sql_query.execBatch())
                {
                    qDebug()<<"Insert into store table failed! "<<sql_query.lastError();
                    QMessageBox::critical(NULL, tr("错误"), tr("添加店铺失败"));
                }
                else
                {
                    qDebug()<<"Insert into store table succeed";
                    initStoreTable();
                    emit StoreInfoChanged();
                }
            }
        }
    }
}

void System::initStoreTable()
{
    //init
    QList<int>* _removeColumns = new QList<int>();
    QMap<int, QString>* _headerData = new QMap<int, QString>();
    _removeColumns->append(0);
    _headerData->insert(0,tr("店铺名"));_headerData->insert(1,tr("地址"));
    _headerData->insert(2,tr("负责人"));_headerData->insert(3,tr("电话"));
    _headerData->insert(4,tr("状态"));_headerData->insert(5,tr("收入"));
    initTableModel("store",ui->Store_tableView,_removeColumns,_headerData);

}

void System::on_StoreInfoChange_pushButton_clicked()
{
    qDebug()<<ui->Store_tableView->currentIndex().row()<<" "<<ui->Store_tableView->currentIndex().column();

    int _currentRow = ui->Store_tableView->currentIndex().row();
    int _currentColum = ui->Store_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QAbstractItemModel *currentModel = ui->Store_tableView->model();
    QModelIndex currentIndex = currentModel->index(_currentRow,_currentColum);
    QVariant currentData = currentModel->data(currentIndex);
    qDebug()<<ui->Store_tableView->model()->data(currentIndex,Qt::DisplayRole).toString();
    qDebug()<<currentData.toString();

    QModelIndex nameIndex = ui->Store_tableView->model()->index(_currentRow,0);
    QString _name = ui->Store_tableView->model()->data(nameIndex).toString();

    QModelIndex addressIndex = ui->Store_tableView->model()->index(_currentRow,1);
    QString _address = ui->Store_tableView->model()->data(addressIndex).toString();

    QModelIndex leaderIndex = ui->Store_tableView->model()->index(_currentRow,2);
    QString _leader = ui->Store_tableView->model()->data(leaderIndex).toString();

    QModelIndex telIndex = ui->Store_tableView->model()->index(_currentRow,3);
    QString _tel = ui->Store_tableView->model()->data(telIndex).toString();

    QModelIndex operationIndex = ui->Store_tableView->model()->index(_currentRow,4);
    QString _operation = ui->Store_tableView->model()->data(operationIndex).toString();

    QModelIndex turnoverIndex = ui->Store_tableView->model()->index(_currentRow,5);
    QString _turnover = ui->Store_tableView->model()->data(turnoverIndex).toString();

    qDebug()<<_name<<"  "<<_address<<"   "<<_leader<<" "<<_tel<<"  "<<_operation<<" "<<_turnover;

    emit SendCurrentSelectedStoreInfo(_name, _address, _leader, _tel, _operation, _turnover);
    storeInfoChangeWidget->show();

}

void System::on_Store_tableView_doubleClicked(const QModelIndex &index)
{
    on_StoreInfoChange_pushButton_clicked();
}

void System::changeStoreTableInfo(QString _name, QString _address, QString _leader, QString _tel, QString _operation, QString _turnover,
                                  QString name, QString address, QString leader, QString tel, QString operation, QString turnover)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isStoreTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='store'"));
        if(isStoreTableExist)
        {
            qDebug() << "store table exists";
            //UPDATE member SET name='匡俊嘉2',gender='男',birthday='1997/11/21',tel='138565464',address='CQUPT-21111' WHERE name='匡俊嘉' AND gender='男' AND birthday='1998/11/21' AND tel='13000000000' AND address='CQUPT';
            QString sql_update = "UPDATE store SET name='"+_name+"',address='"+_address+"',leader='"+_leader+"',tel='"+_tel+"',operation='"+_operation+"',turnover='"+_turnover+"' WHERE name='"+name+"' AND address='"+address+"' AND leader='"+leader+"' AND tel='"+tel+"' AND operation='"+operation+"' AND turnover='"+turnover+"';";
            qDebug()<<sql_update;
            //修改数据
            sql_query.prepare(sql_update);
            if(!sql_query.exec())
            {
                qDebug()<<"Update store's info failed! "<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("修改店铺信息失败！-2"));
            }
            else
            {
                qDebug()<<"Update store's info succeed";
                initStoreTable();
                emit StoreInfoChanged();
            }

        }
        else
            QMessageBox::critical(NULL, tr("错误"), tr("修改店铺信息失败！-3"));
    }
    else
        QMessageBox::critical(NULL, tr("错误"), tr("修改店铺信息失败！-4"));
}

void System::ShowStoreChart()
{
    //QChart负责管理数据
    //QChartView负责显示

    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isStoreTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='store'"));
        if(isStoreTableExist)
        {
            QString select_all_sql = "SELECT name,turnover FROM store";
            sql_query.prepare(select_all_sql);
            if(!sql_query.exec())
                qDebug()<<"SELECT name,turnover FROM member Failed ->"<<sql_query.lastError();
            else
            {
                QPieSeries *pieSeries = new QPieSeries();
                QLineSeries *lineSeries = new QLineSeries();
                //QBarSeries *barSeries = new QBarSeries();
                int _count=0;
                QMap<QString,QString> _list;
                //QString _tempName;
                while(sql_query.next())
                {
                    qDebug()<<sql_query.value(0).toString()<<"   "<<sql_query.value(1).toString();

                    pieSeries->append(sql_query.value(0).toString(), sql_query.value(1).toInt());
                    lineSeries->append(_count, sql_query.value(1).toInt());
                    _count++;
                    //barSeries->append(new QBarSet(sql_query.value(0).toString())<<sql_query.value(1));
                    _list.key(sql_query.value(0).toString());
                    _list.value(sql_query.value(1).toString());
                }
                //                QBarSet *barSet;
                //                for(QMap<QString,QString>::iterator it=_list.begin();it!=_list.end();it++)
                //                {
                //                    qDebug()<<it.key()<<it.value();
                //                    barSet = new QBarSet(it.key());
                //                    //barSet->append(it.value().toInt());
                //                    *barSet<<it.value().toInt();
                //                    barSeries->append(barSet);
                //                }


                //                    QPieSlice *slice_red = series->slices().at(0);
                //                    QPieSlice *slice_green = series->slices().at(1);
                //                    QPieSlice *slice_blue = series->slices().at(2);
                //                    slice_red->setColor(QColor(255,0,0,255));
                //                    slice_green->setColor(QColor(0,255,0,255));
                //                    slice_blue->setColor(QColor(0,0,255,255));

                QChart *pieChart = new QChart();
                pieSeries->setLabelsVisible();
                pieChart->addSeries(pieSeries);
                pieChart->setTitle(tr("饼状图"));
                pieChart->legend()->setAlignment(Qt::AlignBottom);  // 设置图例靠右显示
                //chart->legend()->hide();
                ui->StorePieChart_graphicsView->setChart(pieChart);
                ui->StorePieChart_graphicsView->setRenderHint(QPainter::Antialiasing);

                QChart *lineChart = new QChart();
                lineChart->legend()->hide();

                //lineChart->legend()->setAlignment(Qt::AlignBottom);
                //lineSeries->
                //lineChart->legend()->hide();
                lineChart->addSeries(lineSeries);
                lineChart->createDefaultAxes();
                lineChart->setTitle(tr("折线图"));
                ui->StoreLineChart_graphicsView->setChart(lineChart);
                ui->StoreLineChart_graphicsView->setRenderHint(QPainter::Antialiasing);

                //                QChart *barChart = new QChart();
                //                barChart->addSeries(barSeries);
                //                barChart->setTitle(tr("柱状图"));
                //                barChart->setAnimationOptions(QChart::SeriesAnimations);
                //                barChart->legend()->setVisible(true);
                //                barChart->legend()->setAlignment(Qt::AlignBottom);
                //                ui->StoreBarChart_graphicsView->setChart(barChart);
                //                ui->StoreBarChart_graphicsView->setRenderHint(QPainter::Antialiasing);
                /*
      QStringList categories;
      categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
      QBarCategoryAxis *axis = new QBarCategoryAxis();
      axis->append(categories);
      chart->createDefaultAxes();
      chart->setAxisX(axis, series);
      */
            }
        }
    }

    //    QBarSeries* barSeries = new QBarSeries();
    //    barSeries->append();
}

void System::initOrderTable()
{
    //init
    QList<int>* _removeColumns = new QList<int>();
    QMap<int, QString>* _headerData = new QMap<int, QString>();
    _removeColumns->append(0);
    _headerData->insert(0,tr("客户名"));_headerData->insert(1,tr("电话"));
    _headerData->insert(2,tr("地址"));_headerData->insert(3,tr("衣物类型"));
    _headerData->insert(4,tr("服务类型"));_headerData->insert(5,tr("数量"));
    _headerData->insert(6,tr("备注"));_headerData->insert(7,tr("预约服务"));
    _headerData->insert(8,tr("状态"));_headerData->insert(9,tr("评价"));
    _headerData->insert(10,tr("费用"));_headerData->insert(11,tr("下单时间"));
    initTableModel("orders",ui->Order_tableView,_removeColumns,_headerData);

}

void System::on_SearchOrder_pushButton_clicked()
{

}

void System::on_DeleteOrder_pushButton_clicked()
{
    qDebug()<<"Current Table -> "<<ui->Order_tableView->currentIndex().row()<<" "<<ui->Order_tableView->currentIndex().column();

    int _currentRow = ui->Order_tableView->currentIndex().row();
    int _currentColum = ui->Order_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QAbstractItemModel *currentModel = ui->Order_tableView->model();
    QModelIndex currentIndex = currentModel->index(_currentRow,_currentColum);
    QVariant currentData = currentModel->data(currentIndex);
    //qDebug()<<ui->Order_tableView->model()->data(currentIndex,Qt::DisplayRole).toString();
    //qDebug()<<currentData.toString();

    QModelIndex nameIndex = ui->Order_tableView->model()->index(_currentRow,0);
    QString _name = ui->Order_tableView->model()->data(nameIndex).toString();

    QModelIndex telIndex = ui->Order_tableView->model()->index(_currentRow,1);
    QString _tel = ui->Order_tableView->model()->data(telIndex).toString();

    QModelIndex addressIndex = ui->Order_tableView->model()->index(_currentRow,2);
    QString _address = ui->Order_tableView->model()->data(addressIndex).toString();

    QModelIndex clothesTypeIndex = ui->Order_tableView->model()->index(_currentRow,3);
    QString _clothesType = ui->Order_tableView->model()->data(clothesTypeIndex).toString();

    QModelIndex serviceTypeIndex = ui->Order_tableView->model()->index(_currentRow,4);
    QString _serviceType = ui->Order_tableView->model()->data(serviceTypeIndex).toString();

    QModelIndex clothesNumIndex = ui->Order_tableView->model()->index(_currentRow,5);
    QString _clothesNum = ui->Order_tableView->model()->data(clothesNumIndex).toString();

    QModelIndex remarkIndex = ui->Order_tableView->model()->index(_currentRow,6);
    QString _remark = ui->Order_tableView->model()->data(remarkIndex).toString();

    QModelIndex reservationServiceIndex = ui->Order_tableView->model()->index(_currentRow,7);
    QString _reservationService = ui->Order_tableView->model()->data(reservationServiceIndex).toString();

    QModelIndex conditionIndex = ui->Order_tableView->model()->index(_currentRow,8);
    QString _condition = ui->Order_tableView->model()->data(conditionIndex).toString();

    QModelIndex appraisementIndex = ui->Order_tableView->model()->index(_currentRow,9);
    QString _appraisement = ui->Order_tableView->model()->data(appraisementIndex).toString();

    QModelIndex chargeIndex = ui->Order_tableView->model()->index(_currentRow,10);
    QString _charge = ui->Order_tableView->model()->data(chargeIndex).toString();

    QModelIndex timeIndex = ui->Order_tableView->model()->index(_currentRow,11);
    QString _time = ui->Order_tableView->model()->data(timeIndex).toString();

    qDebug()<<_currentRow+1<<"  "<<_name<<"  "<<_tel<<"   "<<_address<<" "<<_clothesType<<"  "<<_serviceType<<" "<<_clothesNum<<"    "<<_remark<<"  "<<_reservationService<<"   "<<_condition<<"    "<<_appraisement<<" "<<_charge<<"   "<<_time;

    QString _info = tr("客户名：") + _name + "\n"
            + tr("联系电话：") + _tel + "\n"
            + tr("地址：") + _address + "\n"
            + tr("衣物类型：") + _clothesType + "\n"
            + tr("服务类型：") + _serviceType + "\n"
            + tr("数量：") + _clothesNum + "\n"
            + tr("备注：") + _remark + "\n"
            + tr("预约服务：") + _reservationService + "\n"
            + tr("状态：") + _condition + "\n"
            + tr("评价：") + _appraisement + "\n"
            + tr("费用：") + _charge + "\n"
            + tr("下单时间：") + _time + "\n";

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认删除"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
        DataBase.setDatabaseName("DataBase.db");
        DataBase.setUserName("cqupt2016214290");
        DataBase.setPassword("cqupt123456");
        if(DataBase.open())
        {
            QSqlQuery sql_query;
            bool isOrderTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='orders'"));
            QString delete_sql = "DELETE FROM orders WHERE customer='"+_name
                    +"' AND address='"+_address
                    +"' AND clothes_type='"+_clothesType
                    +"' AND service_type='"+_serviceType
                    +"' AND clothes_num='"+_clothesNum
                    +"' AND remark='"+_remark
                    +"' AND reservation_service='"+_reservationService
                    +"' AND condition='"+_condition
                    +"' AND appraisement='"+_appraisement
                    +"' AND charge='"+_charge
                    +"' AND time='"+_time
                    +"';";
            if(isOrderTableExist)
            {
                qDebug() << "orders table exists";
                sql_query.prepare(delete_sql);
                if(!sql_query.exec())
                {
                    QMessageBox::critical(NULL, tr("错误"), tr("删除订单失败!-1"));
                    qDebug()<<sql_query.lastError();
                }
                else
                {
                    QMessageBox::information(NULL, tr("提示"), tr("删除订单成功！"));
                    initOrderTable();
                    emit OrderTableChanged();
                }
            }
            else
                QMessageBox::critical(NULL, tr("错误"), tr("删除订单失败"));
        }
    }

}

void System::on_MakeOrder_pushButton_clicked()
{
    if(ui->ClothesNum_spinBox->text() == "0"
            || ui->Customer_lineEdit->text().isEmpty()
            || ui->Tel_lineEdit->text().isEmpty()
            || ui->Address_label->text().isEmpty())
    {
        QMessageBox::information(NULL, tr("提示"), tr("请检查信息是否完善！"));
        return;
    }
    QString _customer = ui->Customer_lineEdit->text();
    QString _tel = ui->Tel_lineEdit->text();
    QString _address = ui->Address_lineEdit->text();
    QString _clothes_type = ui->ClothesType_comboBox->currentText();
    QString _service_type = ui->ServiceType_comboBox->currentText();
    QString _clothes_num = ui->ClothesNum_spinBox->text();
    //remark 备注
    QString _remark;
    if(ui->Remark_textEdit->document()->toPlainText().isEmpty())
        _remark = tr("无");
    else
        _remark = ui->Remark_textEdit->document()->toPlainText();
    //time 下单时间
    QDate _currentDate = QDate::currentDate();
    QString _time = QString::number(_currentDate.year(),10)+"/"+QString::number(_currentDate.month(),10)+"/"+QString::number(_currentDate.day(),10);
    //charge 费用
    QString _charge;int __charge=0;
    if(ui->ClothesType_comboBox->currentIndex() == 0 || ui->ClothesType_comboBox->currentIndex() == 3)
        __charge+=15;
    else
        __charge+=10;

    if(ui->ServiceType_comboBox->currentIndex()==0)
        __charge+=15;
    else if(ui->ServiceType_comboBox->currentIndex()==1)
        __charge+=5;
    else
        __charge+=10;

    __charge *= _clothes_num.toInt();

    _charge=QString::number(__charge,10);

    QString _info = ui->Customer_label->text() + _customer + "\n"
            + ui->Tel_label->text() + _tel + "\n"
            + ui->Address_label->text() + _address + "\n"
            + ui->ClothesType_label->text() + _clothes_type + "\n"
            + ui->ServiceType_label->text() + _service_type + "\n"
            + ui->ClothesNum_label->text() + _clothes_num + "\n"
            + tr("费用：") + _charge + "\n"
            + tr("下单时间：") + _time + "\n";

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认下单"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
        DataBase.setDatabaseName("DataBase.db");
        DataBase.setUserName("cqupt2016214290");
        DataBase.setPassword("cqupt123456");
        if(DataBase.open())
        {
            QSqlQuery sql_query;
            bool isOrderTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='orders'"));
            QString insert_sql = "INSERT INTO orders (customer, tel, address, clothes_type, service_type, clothes_num, remark, reservation_service, condition, appraisement, charge, time) VALUES(?,?,?,?,?,?,?,?,?,?,?,?)";
            if(isOrderTableExist)
            {
                //插入数据
                sql_query.prepare(insert_sql);

                QVariantList GroupNames; GroupNames.append(_customer);
                QVariantList GroupTels; GroupTels.append(_tel);
                QVariantList GroupAddresses; GroupAddresses.append(_address);
                QVariantList GroupClothesTypes; GroupClothesTypes.append(_clothes_type);
                QVariantList GroupServiceTypes; GroupServiceTypes.append(_service_type);
                QVariantList GroupClothesNums;GroupClothesNums.append(_clothes_num);
                QVariantList GroupRemarks;GroupRemarks.append(_remark);
                QVariantList GroupReservation;GroupReservation.append(tr("无"));
                QVariantList GroupConditions;GroupConditions.append(tr("进行中"));
                QVariantList GroupAppraisements;GroupAppraisements.append(_remark);
                QVariantList GroupCharges;GroupCharges.append(_charge);
                QVariantList GroupTimes;GroupTimes.append(_time);

                sql_query.addBindValue(GroupNames);
                sql_query.addBindValue(GroupTels);
                sql_query.addBindValue(GroupAddresses);
                sql_query.addBindValue(GroupClothesTypes);
                sql_query.addBindValue(GroupServiceTypes);
                sql_query.addBindValue(GroupClothesNums);
                sql_query.addBindValue(GroupRemarks);
                sql_query.addBindValue(GroupReservation);
                sql_query.addBindValue(GroupConditions);
                sql_query.addBindValue(GroupAppraisements);
                sql_query.addBindValue(GroupCharges);
                sql_query.addBindValue(GroupTimes);

                if(!sql_query.execBatch())
                {
                    qDebug()<<"Insert into orders table failed! "<<sql_query.lastError();
                    QMessageBox::critical(NULL, tr("错误"), tr("下单失败！"));
                }
                else
                {
                    qDebug()<<"Insert into orders table succeed";
                    QMessageBox::information(NULL, tr("提示"), tr("下单成功！"));
                    initOrderTable();
                    emit OrderTableChanged();
                    //清除显示框内容
                    ui->ClothesNum_spinBox->setValue(0);
                    ui->Customer_lineEdit->clear();
                    ui->Tel_lineEdit->clear();
                    ui->Address_lineEdit->clear();
                    ui->Remark_textEdit->clear();
                }
            }
        }

    }
}

void System::on_PieChart_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void System::on_LineChart_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void System::on_Order_tableView_doubleClicked(const QModelIndex &index)
{

}

void System::on_FinishOrder_pushButton_clicked()
{
    int _currentRow = ui->Order_tableView->currentIndex().row();
    int _currentColum = ui->Order_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QModelIndex nameIndex = ui->Order_tableView->model()->index(_currentRow,0);
    QString _name = ui->Order_tableView->model()->data(nameIndex).toString();

    QModelIndex telIndex = ui->Order_tableView->model()->index(_currentRow,1);
    QString _tel = ui->Order_tableView->model()->data(telIndex).toString();

    QModelIndex addressIndex = ui->Order_tableView->model()->index(_currentRow,2);
    QString _address = ui->Order_tableView->model()->data(addressIndex).toString();

    QModelIndex clothesTypeIndex = ui->Order_tableView->model()->index(_currentRow,3);
    QString _clothesType = ui->Order_tableView->model()->data(clothesTypeIndex).toString();

    QModelIndex serviceTypeIndex = ui->Order_tableView->model()->index(_currentRow,4);
    QString _serviceType = ui->Order_tableView->model()->data(serviceTypeIndex).toString();

    QModelIndex clothesNumIndex = ui->Order_tableView->model()->index(_currentRow,5);
    QString _clothesNum = ui->Order_tableView->model()->data(clothesNumIndex).toString();

    QModelIndex remarkIndex = ui->Order_tableView->model()->index(_currentRow,6);
    QString _remark = ui->Order_tableView->model()->data(remarkIndex).toString();

    QModelIndex reservationServiceIndex = ui->Order_tableView->model()->index(_currentRow,7);
    QString _reservationService = ui->Order_tableView->model()->data(reservationServiceIndex).toString();

    QModelIndex conditionIndex = ui->Order_tableView->model()->index(_currentRow,8);
    QString _condition = ui->Order_tableView->model()->data(conditionIndex).toString();

    QModelIndex appraisementIndex = ui->Order_tableView->model()->index(_currentRow,9);
    QString _appraisement = ui->Order_tableView->model()->data(appraisementIndex).toString();

    QModelIndex chargeIndex = ui->Order_tableView->model()->index(_currentRow,10);
    QString _charge = ui->Order_tableView->model()->data(chargeIndex).toString();

    QModelIndex timeIndex = ui->Order_tableView->model()->index(_currentRow,11);
    QString _time = ui->Order_tableView->model()->data(timeIndex).toString();

    qDebug()<<_currentRow+1<<"  "<<_name<<"  "<<_tel<<"   "<<_address<<" "<<_clothesType<<"  "<<_serviceType<<" "<<_clothesNum<<"    "<<_remark<<"  "<<_reservationService<<"   "<<_condition<<"    "<<_appraisement<<" "<<_charge<<"   "<<_time;

    if(_condition == tr("已完成"))
    {
        QMessageBox::information(NULL, tr("提示"), tr("此订单已结束！"));
        return;
    }
    QString _info = tr("客户名：") + _name + "\n"
            + tr("联系电话：") + _tel + "\n"
            + tr("地址：") + _address + "\n"
            + tr("衣物类型：") + _clothesType + "\n"
            + tr("服务类型：") + _serviceType + "\n"
            + tr("数量：") + _clothesNum + "\n"
            + tr("备注：") + _remark + "\n"
            + tr("预约服务：") + _reservationService + "\n"
            + tr("状态：") + _condition + "\n"
            + tr("评价：") + _appraisement + "\n"
            + tr("费用：") + _charge + "\n"
            + tr("下单时间：") + _time + "\n";

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认结单"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
        DataBase.setDatabaseName("DataBase.db");
        DataBase.setUserName("cqupt2016214290");
        DataBase.setPassword("cqupt123456");
        if(DataBase.open())
        {
            QSqlQuery sql_query;
            bool isOrderTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='orders'"));
            QString update_sql = "UPDATE orders SET condition='"+tr("已完成")
                    +"' WHERE customer='"+_name
                    +"' AND address='"+_address
                    +"' AND clothes_type='"+_clothesType
                    +"' AND service_type='"+_serviceType
                    +"' AND clothes_num='"+_clothesNum
                    +"' AND remark='"+_remark
                    +"' AND reservation_service='"+_reservationService
                    //+"' AND condition='"+_condition
                    +"' AND appraisement='"+_appraisement
                    +"' AND charge='"+_charge
                    +"' AND time='"+_time
                    +"';";
            if(isOrderTableExist)
            {
                qDebug() << "orders table exists";
                sql_query.prepare(update_sql);
                if(!sql_query.exec())
                {
                    QMessageBox::critical(NULL, tr("错误"), tr("结单失败!-1"));
                    qDebug()<<sql_query.lastError();
                }
                else
                {
                    QMessageBox::information(NULL, tr("提示"), tr("结单成功！"));
                    initOrderTable();
                }
            }
            else
                QMessageBox::critical(NULL, tr("错误"), tr("结单失败"));
        }
    }

}

void System::on_Appraisement_pushButton_clicked()
{
    int _currentRow = ui->Order_tableView->currentIndex().row();
    int _currentColum = ui->Order_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QModelIndex appraisementIndex = ui->Order_tableView->model()->index(_currentRow,9);
    QString _appraisement = ui->Order_tableView->model()->data(appraisementIndex).toString();

    if(_appraisement != tr("无"))
    {
        QMessageBox::information(NULL, tr("提示"), tr("此订单已评价！"));
        return;
    }
    orderAppraisementWidget->show();

}

void System::GetAppraisementInfo(QString _info)
{
    int _currentRow = ui->Order_tableView->currentIndex().row();
    int _currentColum = ui->Order_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QModelIndex nameIndex = ui->Order_tableView->model()->index(_currentRow,0);
    QString _name = ui->Order_tableView->model()->data(nameIndex).toString();

    QModelIndex telIndex = ui->Order_tableView->model()->index(_currentRow,1);
    QString _tel = ui->Order_tableView->model()->data(telIndex).toString();

    QModelIndex addressIndex = ui->Order_tableView->model()->index(_currentRow,2);
    QString _address = ui->Order_tableView->model()->data(addressIndex).toString();

    QModelIndex clothesTypeIndex = ui->Order_tableView->model()->index(_currentRow,3);
    QString _clothesType = ui->Order_tableView->model()->data(clothesTypeIndex).toString();

    QModelIndex serviceTypeIndex = ui->Order_tableView->model()->index(_currentRow,4);
    QString _serviceType = ui->Order_tableView->model()->data(serviceTypeIndex).toString();

    QModelIndex clothesNumIndex = ui->Order_tableView->model()->index(_currentRow,5);
    QString _clothesNum = ui->Order_tableView->model()->data(clothesNumIndex).toString();

    QModelIndex remarkIndex = ui->Order_tableView->model()->index(_currentRow,6);
    QString _remark = ui->Order_tableView->model()->data(remarkIndex).toString();

    QModelIndex reservationServiceIndex = ui->Order_tableView->model()->index(_currentRow,7);
    QString _reservationService = ui->Order_tableView->model()->data(reservationServiceIndex).toString();

    QModelIndex conditionIndex = ui->Order_tableView->model()->index(_currentRow,8);
    QString _condition = ui->Order_tableView->model()->data(conditionIndex).toString();

    //    QModelIndex appraisementIndex = ui->Order_tableView->model()->index(_currentRow,9);
    //    QString _appraisement = ui->Order_tableView->model()->data(appraisementIndex).toString();

    QModelIndex chargeIndex = ui->Order_tableView->model()->index(_currentRow,10);
    QString _charge = ui->Order_tableView->model()->data(chargeIndex).toString();

    QModelIndex timeIndex = ui->Order_tableView->model()->index(_currentRow,11);
    QString _time = ui->Order_tableView->model()->data(timeIndex).toString();

    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isOrderTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='orders'"));
        QString update_sql = "UPDATE orders SET appraisement='"+_info
                +"' WHERE customer='"+_name
                +"' AND address='"+_address
                +"' AND clothes_type='"+_clothesType
                +"' AND service_type='"+_serviceType
                +"' AND clothes_num='"+_clothesNum
                +"' AND remark='"+_remark
                +"' AND reservation_service='"+_reservationService
                +"' AND condition='"+_condition
                //+"' AND appraisement='"+_appraisement
                +"' AND charge='"+_charge
                +"' AND time='"+_time
                +"';";
        if(isOrderTableExist)
        {
            qDebug() << "orders table exists";
            sql_query.prepare(update_sql);
            if(!sql_query.exec())
            {
                QMessageBox::critical(NULL, tr("错误"), tr("评价失败!-1"));
                qDebug()<<sql_query.lastError();
            }
            else
            {
                QMessageBox::information(NULL, tr("提示"), tr("评价成功！"));
                initOrderTable();
            }
        }
        else
            QMessageBox::critical(NULL, tr("错误"), tr("评价失败"));
    }
}

void System::initCustomerTable()
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isOrderTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='orders'"));
        if(isOrderTableExist)
        {
            QString select_customer= "SELECT distinct customer,tel,address FROM orders;";
            sql_query.prepare(select_customer);
            if(!sql_query.exec())
                qDebug()<<sql_query.lastError();
            else
            {
                QStandardItemModel* model = new QStandardItemModel();
                model->setHorizontalHeaderItem(0, new QStandardItem(tr("客户名")));
                model->setHorizontalHeaderItem(1, new QStandardItem(tr("电话")));
                model->setHorizontalHeaderItem(2, new QStandardItem(tr("地址")));
                model->setHorizontalHeaderItem(3, new QStandardItem(tr("累计消费")));

                //model->setHorizontalHeaderItem(4, new QStandardItem(tr("是否是会员"));
                //model->setHorizontalHeaderItem(5, new QStandardItem(tr("最近下单时间"));;
                int _colum=0;
                QList<QString> _nameList;
                while(sql_query.next())
                {
                    _nameList.append(sql_query.value(0).toString());
                    model->setItem(_colum, 0, new QStandardItem(sql_query.value(0).toString()));
                    model->setItem(_colum, 1, new QStandardItem(sql_query.value(1).toString()));
                    model->setItem(_colum, 2, new QStandardItem(sql_query.value(2).toString()));
                    _colum++;
                }
                //SELECT sum(charge) FROM orders WHERE customer='匡俊嘉';
                QSqlQuery getAllCost;
                _colum=0;
                for(QList<QString>::iterator it=_nameList.begin();it!=_nameList.end();it++)
                {
                    QString sql_customer_AllCost = "SELECT sum(charge) FROM orders WHERE customer='"
                            +*it
                            +"';";
                    qDebug()<<*it;
                    qDebug()<<sql_customer_AllCost;
                    getAllCost.prepare(sql_customer_AllCost);
                    if(!getAllCost.exec())
                        qDebug()<<getAllCost.lastError();
                    else
                    {
                        while (getAllCost.next())
                            model->setItem(_colum, 3, new QStandardItem(getAllCost.value(0).toString()));

                        if(_colum<_nameList.count())
                            _colum++;
                    }

                }



                ui->Customer_tableView->setModel(model);
            }
        }
        else
            qDebug()<<sql_query.lastError();
    }

}

void System::initStaffTable()
{
    //init
    QList<int>* _removeColumns = new QList<int>();
    QMap<int, QString>* _headerData = new QMap<int, QString>();
    _removeColumns->append(0);
    _headerData->insert(0,tr("姓名"));_headerData->insert(1,tr("电话"));
    _headerData->insert(2,tr("性别"));_headerData->insert(3,tr("地址"));
    initTableModel("staff",ui->Staff_tableView,_removeColumns,_headerData);
}


void System::on_AddStaff_pushButton_clicked()
{
    staffAddWidget->show();
}

void System::addStaffTable(QString _name, QString _gender, QString _tel, QString _address)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isStaffTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='staff'"));
        QString insert_sql = "INSERT INTO staff (name, gender, tel, address) VALUES(?,?,?,?)";
        if(isStaffTableExist)
        {
            qDebug() << "staff table exists";

            //插入数据
            sql_query.prepare(insert_sql);

            QVariantList GroupNames; GroupNames.append(_name);
            QVariantList GroupGenders; GroupGenders.append(_gender);
            QVariantList GroupTels; GroupTels.append(_tel);
            QVariantList GroupAddresses; GroupAddresses.append(_address);

            sql_query.addBindValue(GroupNames);
            sql_query.addBindValue(GroupTels);
            sql_query.addBindValue(GroupTels);
            sql_query.addBindValue(GroupAddresses);

            if(!sql_query.execBatch())
            {
                qDebug()<<"Insert into staff table failed! "<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("添加员工失败"));
            }
            else
            {
                qDebug()<<"Insert into staff table succeed";
                //initStaffTable();
                QMessageBox::information(NULL,tr("提示"),tr("添加员工成功"));
                emit StaffInfoChanged();
            }
        }

    }
}

void System::on_DeleteStaff_pushButton_clicked()
{
    qDebug()<<"Current Table -> "<<ui->Staff_tableView->currentIndex().row()<<" "<<ui->Staff_tableView->currentIndex().column();

    int _currentRow = ui->Staff_tableView->currentIndex().row();
    int _currentColum = ui->Staff_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QModelIndex nameIndex = ui->Staff_tableView->model()->index(_currentRow,0);
    QString _name = ui->Staff_tableView->model()->data(nameIndex).toString();

    QModelIndex telIndex = ui->Staff_tableView->model()->index(_currentRow,1);
    QString _tel = ui->Staff_tableView->model()->data(telIndex).toString();

    QModelIndex genderIndex = ui->Staff_tableView->model()->index(_currentRow,2);
    QString _gender = ui->Staff_tableView->model()->data(genderIndex).toString();

    QModelIndex addressIndex = ui->Staff_tableView->model()->index(_currentRow,3);
    QString _address = ui->Staff_tableView->model()->data(addressIndex).toString();

    qDebug()<<_currentRow+1<<"  "<<_name<<"  "<<_tel<<"   "<<_gender<<" "<<_address;

    QString _info = tr("姓名：") + _name + "\n"
            + tr("电话：") + _tel + "\n"
            + tr("性别：") + _gender + "\n"
            + tr("地址：") + _address;

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认删除"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
        DataBase.setDatabaseName("DataBase.db");
        DataBase.setUserName("cqupt2016214290");
        DataBase.setPassword("cqupt123456");
        if(DataBase.open())
        {
            QSqlQuery sql_query;
            bool isOrderTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='staff'"));
            QString delete_sql = "DELETE FROM staff WHERE name='"+_name
                    +"' AND tel='"+_tel
                    +"' AND gender='"+_gender
                    +"' AND address='"+_address
                    +"';";
            if(isOrderTableExist)
            {
                qDebug() << "staff table exists";
                sql_query.prepare(delete_sql);
                if(!sql_query.exec())
                {
                    QMessageBox::critical(NULL, tr("错误"), tr("删除员工失败!-1"));
                    qDebug()<<sql_query.lastError();
                }
                else
                {
                    QMessageBox::information(NULL, tr("提示"), tr("删除员工成功！"));
                    emit StaffInfoChanged();
                }
            }
            else
                QMessageBox::critical(NULL, tr("错误"), tr("删除员工失败"));
        }
    }
}

void System::on_AlterStaff_pushButton_clicked()
{
    qDebug()<<"Current Table -> "<<ui->Staff_tableView->currentIndex().row()<<" "<<ui->Staff_tableView->currentIndex().column();

    int _currentRow = ui->Staff_tableView->currentIndex().row();
    int _currentColum = ui->Staff_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QModelIndex nameIndex = ui->Staff_tableView->model()->index(_currentRow,0);
    QString _name = ui->Staff_tableView->model()->data(nameIndex).toString();

    QModelIndex telIndex = ui->Staff_tableView->model()->index(_currentRow,1);
    QString _tel = ui->Staff_tableView->model()->data(telIndex).toString();

    QModelIndex genderIndex = ui->Staff_tableView->model()->index(_currentRow,2);
    QString _gender = ui->Staff_tableView->model()->data(genderIndex).toString();

    QModelIndex addressIndex = ui->Staff_tableView->model()->index(_currentRow,3);
    QString _address = ui->Staff_tableView->model()->data(addressIndex).toString();

    qDebug()<<_currentRow+1<<"  "<<_name<<"  "<<_tel<<"   "<<_gender<<" "<<_address;

    emit SendCurrentSelectedStaffInfo(_name, _tel, _gender, _address);
    staffInfoChangeWidget->show();

}

void System::on_Staff_tableView_doubleClicked(const QModelIndex &index)
{
    on_AlterStaff_pushButton_clicked();
}

void System::changeStaffTableInfo(QString _name, QString _tel, QString _gender, QString _address,
                                  QString name, QString tel, QString gender, QString address)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isStaffTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='staff'"));
        if(isStaffTableExist)
        {
            qDebug() << "staff table exists";
            QString sql_update = "UPDATE staff SET name='"+_name+"',tel='"+_tel+"',gender='"+_gender+"',address='"+_address+"' WHERE name='"+name+"' AND tel='"+tel+"' AND gender='"+gender+"' AND address='"+address+"';";
            qDebug()<<sql_update;
            //修改数据
            sql_query.prepare(sql_update);
            if(!sql_query.exec())
            {
                qDebug()<<"Update staff's info failed! "<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("修改员工信息失败！-2"));
            }
            else
            {
                qDebug()<<"Update staff's info succeed";
                emit StaffInfoChanged();
            }

        }
        else
            QMessageBox::critical(NULL, tr("错误"), tr("修改员工信息失败！-3"));
    }
    else
        QMessageBox::critical(NULL, tr("错误"), tr("修改员工信息失败！-4"));
}

void System::initDetergentTable()
{
    //init
    QList<int>* _removeColumns = new QList<int>();
    QMap<int, QString>* _headerData = new QMap<int, QString>();
    _removeColumns->append(0);
    _headerData->insert(0,tr("名称"));_headerData->insert(1,tr("库存"));
    initTableModel("detergent",ui->Detergent_tableView,_removeColumns,_headerData);
}

void System::initShippingTable()
{
    //init
    QList<int>* _removeColumns = new QList<int>();
    QMap<int, QString>* _headerData = new QMap<int, QString>();
    _removeColumns->append(0);_removeColumns->append(6);
    _removeColumns->append(8);_removeColumns->append(8);
    _headerData->insert(0,tr("客户名"));_headerData->insert(1,tr("电话"));
    _headerData->insert(2,tr("地址"));_headerData->insert(3,tr("衣物类型"));
    _headerData->insert(4,tr("服务类型"));_headerData->insert(5,tr("数量"));
    _headerData->insert(6,tr("预约服务"));_headerData->insert(7,tr("状态"));
    _headerData->insert(8,tr("下单时间"));
    initTableModel("orders",ui->Shipping_tableView,_removeColumns,_headerData);
}

void System::on_AddDetergent_pushButton_clicked()
{
    detergentAddWidget->show();
}

void System::on_DeleteDetergent_pushButton_clicked()
{
    qDebug()<<"Current Table -> "<<ui->Detergent_tableView->currentIndex().row()<<" "<<ui->Detergent_tableView->currentIndex().column();

    int _currentRow = ui->Detergent_tableView->currentIndex().row();
    int _currentColum = ui->Detergent_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QModelIndex nameIndex = ui->Detergent_tableView->model()->index(_currentRow,0);
    QString _name = ui->Detergent_tableView->model()->data(nameIndex).toString();

    QModelIndex numIndex = ui->Detergent_tableView->model()->index(_currentRow,1);
    QString _num = ui->Detergent_tableView->model()->data(numIndex).toString();

    qDebug()<<_currentRow+1<<"  "<<_name<<"  "<<_num;

    QString _info = tr("名称：") + _name + "\n"
            + tr("库存：") + _num;

    QMessageBox::StandardButton msgBox = QMessageBox::information(NULL, tr("确认删除"), _info, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(msgBox == QMessageBox::Yes)
    {
        QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
        DataBase.setDatabaseName("DataBase.db");
        DataBase.setUserName("cqupt2016214290");
        DataBase.setPassword("cqupt123456");
        if(DataBase.open())
        {
            QSqlQuery sql_query;
            bool isOrderTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='detergent'"));
            QString delete_sql = "DELETE FROM detergent WHERE name='"+_name
                    +"' AND stock='"+_num
                    +"';";
            if(isOrderTableExist)
            {
                qDebug() << "detergent table exists";
                sql_query.prepare(delete_sql);
                if(!sql_query.exec())
                {
                    QMessageBox::critical(NULL, tr("错误"), tr("删除洗衣品失败!-1"));
                    qDebug()<<sql_query.lastError();
                }
                else
                {
                    QMessageBox::information(NULL, tr("提示"), tr("删除洗衣品成功！"));
                    emit DetergentInfoChanged();
                }
            }
            else
                QMessageBox::critical(NULL, tr("错误"), tr("删除洗衣品失败"));
        }
    }
}

void System::on_AlterDetergent_pushButton_clicked()
{
    qDebug()<<"Current Table -> "<<ui->Detergent_tableView->currentIndex().row()<<" "<<ui->Detergent_tableView->currentIndex().column();

    int _currentRow = ui->Detergent_tableView->currentIndex().row();
    int _currentColum = ui->Detergent_tableView->currentIndex().column();

    if( _currentRow == -1 || _currentColum == -1)
        return;

    QModelIndex nameIndex = ui->Detergent_tableView->model()->index(_currentRow,0);
    QString _name = ui->Detergent_tableView->model()->data(nameIndex).toString();

    QModelIndex numIndex = ui->Detergent_tableView->model()->index(_currentRow,1);
    QString _num = ui->Detergent_tableView->model()->data(numIndex).toString();

    qDebug()<<_currentRow+1<<"  "<<_name<<"  "<<_num;

    SendCurrentSelectedDetergentInfo(_name,_num);
    detergentInfoChangeWidget->show();
}

void System::on_Detergent_tableView_doubleClicked(const QModelIndex &index)
{
    on_AlterDetergent_pushButton_clicked();
}
void System::addDetergentTable(QString _name, QString _num)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isDetergentTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='detergent'"));
        QString insert_sql = "INSERT INTO detergent (name, stock) VALUES(?,?)";
        if(isDetergentTableExist)
        {
            qDebug() << "detergent table exists";

            //插入数据
            sql_query.prepare(insert_sql);

            QVariantList GroupNames; GroupNames.append(_name);
            QVariantList GroupStocks; GroupStocks.append(_num);

            sql_query.addBindValue(GroupNames);
            sql_query.addBindValue(GroupStocks);

            if(!sql_query.execBatch())
            {
                qDebug()<<"Insert into detergent table failed! "<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("添加洗衣品失败"));
            }
            else
            {
                qDebug()<<"Insert into detergent table succeed";
                QMessageBox::information(NULL,tr("提示"),tr("添加洗衣品成功"));
                emit DetergentInfoChanged();
            }
        }

    }
}

void System::changeDetergentTable(QString _name, QString _num, QString name, QString num)
{
    QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");
    DataBase.setDatabaseName("DataBase.db");
    DataBase.setUserName("cqupt2016214290");
    DataBase.setPassword("cqupt123456");
    if(DataBase.open())
    {
        QSqlQuery sql_query;
        bool isDetergentTableExist = sql_query.exec(QString("SELECT NAME FROM sqlite_master WHERE NAME='detergent'"));
        if(isDetergentTableExist)
        {
            qDebug() << "detergent table exists";
            QString sql_update = "UPDATE detergent SET name='"+_name+"',stock='"+_num+"' WHERE name='"+name+"' AND stock='"+num+"';";
            qDebug()<<sql_update;
            //修改数据
            sql_query.prepare(sql_update);
            if(!sql_query.exec())
            {
                qDebug()<<"Update detergent's info failed! "<<sql_query.lastError();
                QMessageBox::critical(NULL, tr("错误"), tr("修改洗衣品信息失败！-2"));
            }
            else
            {
                qDebug()<<"Update detergent's info succeed";
                emit DetergentInfoChanged();
            }

        }
        else
            QMessageBox::critical(NULL, tr("错误"), tr("修改洗衣品信息失败！-3"));
    }
    else
        QMessageBox::critical(NULL, tr("错误"), tr("修改洗衣品信息失败！-4"));
}
