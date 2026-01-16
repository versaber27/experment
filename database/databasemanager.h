#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>  // 修改：使用完整路径
#include <QMutex>

// 前向声明，避免在头文件中包含
class QSqlQuery;
class QSqlError;

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    // 单例访问
    static DatabaseManager& instance();

    // 初始化数据库
    bool initialize();

    // 获取数据库连接
    QSqlDatabase database() const { return m_database; }

    // 获取数据库路径
    QString databasePath() const { return m_databasePath; }

    // 获取最后错误信息
    QString lastError() const { return m_lastError; }

private:
    // 私有构造函数
    explicit DatabaseManager(QObject* parent = nullptr);

    // 禁止拷贝
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // 创建表
    bool createTables();
    bool createPatientsTable();
    bool createDoctorsTable();
    bool createMedicinesTable();
    bool createAppointmentsTable();
    bool createVisitsTable();
    bool createPrescriptionsTable();
    bool createPrescriptionMedicinesTable();

    // 成员变量
    QString m_databasePath;
    QSqlDatabase m_database;
    QMutex m_mutex;
    QString m_lastError;
};

#endif // DATABASEMANAGER_H
