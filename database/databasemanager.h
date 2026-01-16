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
    bool createRolesTable();
    bool createUsersTable();
    bool createChargesTable();
    bool createChargeRecordsTable();
    
    // 处方相关操作
    bool addPrescription(const QString& prescriptionNo, int visitId, int patientId, int doctorId, double totalAmount);
    bool updatePrescription(int prescriptionId, const QString& prescriptionNo, int visitId, int patientId, int doctorId, double totalAmount, const QString& status);
    bool deletePrescription(int prescriptionId);
    bool addPrescriptionMedicine(int prescriptionId, int medicineId, int quantity, double unitPrice, const QString& dosage, const QString& usage);
    bool updatePrescriptionMedicine(int prescriptionMedicineId, int prescriptionId, int medicineId, int quantity, double unitPrice, const QString& dosage, const QString& usage);
    bool deletePrescriptionMedicine(int prescriptionMedicineId);
    bool updatePrescriptionStatus(int prescriptionId, const QString& status);
    
    // 处方与药品库存联动
    bool checkMedicineStock(int medicineId, int requiredQuantity);
    bool updateMedicineStock(int medicineId, int quantityChange);
    bool decreaseMedicineStockForPrescription(int prescriptionId);
    bool increaseMedicineStockForPrescription(int prescriptionId);
    
    // 角色相关操作
    bool addRole(const QString& roleName, const QString& description);
    bool updateRole(int roleId, const QString& roleName, const QString& description);
    bool deleteRole(int roleId);
    bool getRoleById(int roleId, QString& roleName, QString& description);
    
    // 用户相关操作
    bool addUser(const QString& username, const QString& password, int roleId, const QString& realName, const QString& phone, const QString& email);
    bool updateUser(int userId, const QString& username, int roleId, const QString& realName, const QString& phone, const QString& email, bool isActive);
    bool updateUserPassword(int userId, const QString& password);
    bool deleteUser(int userId);
    bool getUserByUsername(const QString& username, int& userId, QString& passwordHash, int& roleId, QString& realName, QString& phone, QString& email, bool& isActive);
    bool getUserById(int userId, QString& username, int& roleId, QString& realName, QString& phone, QString& email, bool& isActive);
    bool authenticateUser(const QString& username, const QString& password, int& userId, int& roleId, QString& realName);
    bool updateUserLastLogin(int userId);
    
    // 收费项目相关操作
    bool addCharge(const QString& chargeCode, const QString& name, const QString& category, double price, const QString& description);
    bool updateCharge(int chargeId, const QString& chargeCode, const QString& name, const QString& category, double price, const QString& description);
    bool deleteCharge(int chargeId);
    
    // 收费记录相关操作
    bool addChargeRecord(const QString& recordNo, int patientId, int visitId, double totalAmount, const QString& paymentMethod, const QString& paymentStatus, int createdBy);
    bool updateChargeRecord(int chargeRecordId, const QString& paymentMethod, const QString& paymentStatus);
    bool updateChargeRecordPaymentStatus(int chargeRecordId, const QString& paymentStatus);
    bool deleteChargeRecord(int chargeRecordId);

    // 成员变量
    QString m_databasePath;
    QSqlDatabase m_database;
    QMutex m_mutex;
    QString m_lastError;
};

#endif // DATABASEMANAGER_H
