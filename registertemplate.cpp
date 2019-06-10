#include "registertemplate.h"

RegisterTemplate::RegisterTemplate(QString* _tableName, QMap<int, QString> *_tableHeader, QMap<QString,QString>* _insertInfo) : QObject(parent)
{
    QString selectTable = "SELECT NAME FROM sqlite_master WHERE NAME=" + "'" + *_tableName + "'";
    bool isTableExist = sql_query.exec(selectTable);
    if(isTableExist)
    {
        InsertTable(_tableName,_insertInfo);
    }
    else
    {
        if(CreateTable(*_tableName,_tableHeader))
            InsertTable(_tableName,_insertInfo);
    }
}
RegisterTemplate::~RegisterTemplate()
{

}
static bool RegisterTemplate::CreateTable(QString _tableName, QMap<int, QString> *_tableHeader)
{
    QString selectTable = "SELECT NAME FROM sqlite_master WHERE NAME=" + "'" + _tableName + "'";
    bool isTableExist = sql_query.exec(selectTable);
    if(isTableExist)
    {
        QDebug<<_tableName<<" table has already existed";
        return false;
    }
    else
    {
        QString _headerNames; QString _headerTypes;
        QString _tempDefine = "id INTEGER PRIMARY KEY AUTOINCREMENT,";
        for(QMap<QString,QString>::iterator it = _tableHeader->begin(); it < _tableHeader->end(); it++)
        {
            _tempDefine = *it.key() + " " + *it.value();
            if(*it != _tableHeader->end())
                _tempDefine += ", ";

        }
        QString create_sql ="CREATE TABLE " + _tableName + "(" + _tempDefine +")";
        sql_query.prepare(create_sql); //创建表
        if(!sql_query.exec()) //查看创建表是否成功
        {
            QMessageBox::critical(NULL, tr("错误"), tr("创建失败"));
            qDebug()<<"Error:Member Table Create Failed ->"<<sql_query.lastError();
        }
        else
        {
            qDebug()<<tr("Member Table Created!");
            return true;
        }

    }
}
static bool RegisterTemplate::InsertTable(QString *_tableName, QMap<QString, QString> *_insertInfo)
{
    QSqlQuery sql_query;
    QString selectTable = "SELECT NAME FROM sqlite_master WHERE NAME=" + "'" + _tableName + "'";
    bool isTableExist = sql_query.exec(selectTable);
    QString insert_key; QString insert_value;
    for(QMap<QString,QString>::iterator it = _insertInfo->begin(); it < _insertInfo->end(); it++)
    {
        insert_key += *it.key;
        if(*it != _insertInfo->end())
            insert_key += ",";
        insert_value += "?";
        if(*it != _insertInfo->end())
            insert_value += ",";
    }
    QString insert_sql = "INSERT INTO " + _tableName + " (" + insert_key + ") VALUES(" + insert_value + ")";
    if(isTableExist)
    {
        qDebug() <<_tableName<<" table exists";

        //插入数据
        sql_query.prepare(insert_sql);
        for(QMap<QString,QString>::iterator it = _insertInfo->begin(); it < _insertInfo->end(); it++)
            sql_query.addBindValue(QVariantList::append(*it.value()));

        if(!sql_query.execBatch())
        {
            qDebug()<<"Insert into "<<_tableName<<" table failed! "<<sql_query.lastError();
            QMessageBox::critical(NULL, tr("错误"), tr("创建失败"));
        }
        else
        {
            qDebug()<<"Insert into "<<_tableName<<" table succeed";
            //initMemberTable();
            return true;
        }
    }
    else
        return false;
}
