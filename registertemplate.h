#ifndef REGISTERTEMPLATE_H
#define REGISTERTEMPLATE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>

class RegisterTemplate : public QObject
{
    Q_OBJECT
public:
    explicit RegisterTemplate(QString *_tableName, QMap<int, QString> *_tableHeader, QMap<QString, QString> *_insertInfo);
    ~RegisterTemplate();
    //Only support SQLite!!!!!
    static bool InsertTable(QString *_tableName, QMap<QString, QString> *_insertInfo);
    static bool CreateTable(QString _tableName, QMap<QString, QString> *_tableHeader);
signals:

public slots:
};

#endif // REGISTERTEMPLATE_H
