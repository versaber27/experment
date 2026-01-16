#include "databasemanager.h"

// 添加必要的Qt SQL头文件
#include <QtSql/QSqlQuery>      // 添加这行
#include <QtSql/QSqlError>      // 添加这行
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
    , m_lastError("")
{
    // 设置数据库文件路径
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (appDataDir.isEmpty()) {
        appDataDir = QDir::homePath() + "/.CommunityMedicalSystem";
    }

    QDir dir(appDataDir);
    if (!dir.exists()) {
        dir.mkpath(appDataDir);
    }

    m_databasePath = appDataDir + "/medical_data.db";
    qDebug() << "Database path:" << m_databasePath;
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initialize()
{
    QMutexLocker locker(&m_mutex);

    // 如果数据库已经打开，先关闭
    if (m_database.isOpen()) {
        m_database.close();
    }

    // 设置数据库连接
    m_database = QSqlDatabase::addDatabase("QSQLITE", "medical_connection");
    m_database.setDatabaseName(m_databasePath);

    // 尝试打开数据库
    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        qCritical() << "Failed to open database:" << m_lastError;
        return false;
    }

    qDebug() << "Database opened successfully";

    // 创建表
    if (!createTables()) {
        m_lastError = "Failed to create tables";
        return false;
    }

    qDebug() << "All tables created successfully";
    return true;
}

bool DatabaseManager::createTables()
{
    // 启用外键约束
    QSqlQuery query(m_database);
    if (!query.exec("PRAGMA foreign_keys = ON;")) {
        qWarning() << "Failed to enable foreign keys:" << query.lastError().text();
    }

    // 不再删除表，避免数据丢失。
    // 使用CREATE TABLE IF NOT EXISTS确保表结构自动更新（仅添加新字段）
    // 对于字段修改等复杂Schema变更，建议使用专门的迁移工具

    // 创建所有表
    return createPatientsTable() &&
           createDoctorsTable() &&
           createMedicinesTable() &&
           createAppointmentsTable() &&
           createVisitsTable() &&
           createPrescriptionsTable() &&
           createPrescriptionMedicinesTable();
}

bool DatabaseManager::createPatientsTable()
{
    QString queryStr =
        "CREATE TABLE IF NOT EXISTS patients ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "patient_id TEXT UNIQUE NOT NULL,"
        "name TEXT NOT NULL,"
        "gender TEXT,"
        "age INTEGER,"
        "phone TEXT,"
        "address TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create patients table:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::createDoctorsTable()
{
    QString queryStr =
        "CREATE TABLE IF NOT EXISTS doctors ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "doctor_id TEXT UNIQUE NOT NULL,"
        "name TEXT NOT NULL,"
        "department TEXT NOT NULL,"
        "title TEXT,"
        "phone TEXT,"
        "email TEXT,"
        "available_days TEXT,"
        "consultation_hours TEXT,"
        "max_patients_per_day INTEGER DEFAULT 30,"
        "is_active INTEGER DEFAULT 1"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create doctors table:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::createMedicinesTable()
{
    QString queryStr =
        "CREATE TABLE IF NOT EXISTS medicines ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "category TEXT,"
        "stock INTEGER DEFAULT 0,"
        "unit_price REAL,"
        "expiry_date DATE"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create medicines table:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::createAppointmentsTable()
{
    QString queryStr =
        "CREATE TABLE IF NOT EXISTS appointments ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "appointment_no TEXT UNIQUE,"
        "patient_id INTEGER NOT NULL,"
        "doctor_id INTEGER NOT NULL,"
        "appointment_date DATE NOT NULL,"
        "appointment_time TIME,"
        "status TEXT DEFAULT 'pending',"
        "symptoms TEXT,"
        "note TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (patient_id) REFERENCES patients(id),"
        "FOREIGN KEY (doctor_id) REFERENCES doctors(id)"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create appointments table:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::createVisitsTable()
{
    QString queryStr =
        "CREATE TABLE IF NOT EXISTS visits ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "patient_id INTEGER NOT NULL,"
        "doctor_id INTEGER NOT NULL,"
        "visit_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "symptoms TEXT,"
        "diagnosis TEXT,"
        "treatment TEXT,"
        "notes TEXT,"
        "appointment_id INTEGER,"
        "FOREIGN KEY (patient_id) REFERENCES patients(id),"
        "FOREIGN KEY (doctor_id) REFERENCES doctors(id),"
        "FOREIGN KEY (appointment_id) REFERENCES appointments(id)"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create visits table:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::createPrescriptionsTable()
{
    QString queryStr =
        "CREATE TABLE IF NOT EXISTS prescriptions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "prescription_no TEXT UNIQUE NOT NULL,"
        "visit_id INTEGER NOT NULL,"
        "patient_id INTEGER NOT NULL,"
        "doctor_id INTEGER NOT NULL,"
        "create_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "total_amount REAL DEFAULT 0,"
        "status TEXT DEFAULT '有效',"
        "FOREIGN KEY (visit_id) REFERENCES visits(id),"
        "FOREIGN KEY (patient_id) REFERENCES patients(id),"
        "FOREIGN KEY (doctor_id) REFERENCES doctors(id)"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create prescriptions table:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::createPrescriptionMedicinesTable()
{
    QString queryStr = 
        "CREATE TABLE IF NOT EXISTS prescription_medicines ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "prescription_id INTEGER NOT NULL,"
        "medicine_id INTEGER NOT NULL,"
        "quantity INTEGER NOT NULL,"
        "unit_price REAL NOT NULL,"
        "total_price REAL NOT NULL,"
        "dosage TEXT,"
        "usage TEXT,"
        "FOREIGN KEY (prescription_id) REFERENCES prescriptions(id),"
        "FOREIGN KEY (medicine_id) REFERENCES medicines(id)"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create prescription_medicines table:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::addPrescription(const QString& prescriptionNo, int visitId, int patientId, int doctorId, double totalAmount)
{
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO prescriptions (prescription_no, visit_id, patient_id, doctor_id, total_amount) VALUES (:prescriptionNo, :visitId, :patientId, :doctorId, :totalAmount)");
    query.bindValue(":prescriptionNo", prescriptionNo);
    query.bindValue(":visitId", visitId);
    query.bindValue(":patientId", patientId);
    query.bindValue(":doctorId", doctorId);
    query.bindValue(":totalAmount", totalAmount);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to add prescription:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::updatePrescription(int prescriptionId, const QString& prescriptionNo, int visitId, int patientId, int doctorId, double totalAmount, const QString& status)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE prescriptions SET prescription_no = :prescriptionNo, visit_id = :visitId, patient_id = :patientId, doctor_id = :doctorId, total_amount = :totalAmount, status = :status WHERE id = :prescriptionId");
    query.bindValue(":prescriptionId", prescriptionId);
    query.bindValue(":prescriptionNo", prescriptionNo);
    query.bindValue(":visitId", visitId);
    query.bindValue(":patientId", patientId);
    query.bindValue(":doctorId", doctorId);
    query.bindValue(":totalAmount", totalAmount);
    query.bindValue(":status", status);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to update prescription:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::deletePrescription(int prescriptionId)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM prescriptions WHERE id = :prescriptionId");
    query.bindValue(":prescriptionId", prescriptionId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to delete prescription:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::addPrescriptionMedicine(int prescriptionId, int medicineId, int quantity, double unitPrice, const QString& dosage, const QString& usage)
{
    double totalPrice = quantity * unitPrice;
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO prescription_medicines (prescription_id, medicine_id, quantity, unit_price, total_price, dosage, usage) VALUES (:prescriptionId, :medicineId, :quantity, :unitPrice, :totalPrice, :dosage, :usage)");
    query.bindValue(":prescriptionId", prescriptionId);
    query.bindValue(":medicineId", medicineId);
    query.bindValue(":quantity", quantity);
    query.bindValue(":unitPrice", unitPrice);
    query.bindValue(":totalPrice", totalPrice);
    query.bindValue(":dosage", dosage);
    query.bindValue(":usage", usage);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to add prescription medicine:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::updatePrescriptionMedicine(int prescriptionMedicineId, int prescriptionId, int medicineId, int quantity, double unitPrice, const QString& dosage, const QString& usage)
{
    double totalPrice = quantity * unitPrice;
    QSqlQuery query(m_database);
    query.prepare("UPDATE prescription_medicines SET prescription_id = :prescriptionId, medicine_id = :medicineId, quantity = :quantity, unit_price = :unitPrice, total_price = :totalPrice, dosage = :dosage, usage = :usage WHERE id = :prescriptionMedicineId");
    query.bindValue(":prescriptionMedicineId", prescriptionMedicineId);
    query.bindValue(":prescriptionId", prescriptionId);
    query.bindValue(":medicineId", medicineId);
    query.bindValue(":quantity", quantity);
    query.bindValue(":unitPrice", unitPrice);
    query.bindValue(":totalPrice", totalPrice);
    query.bindValue(":dosage", dosage);
    query.bindValue(":usage", usage);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to update prescription medicine:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::deletePrescriptionMedicine(int prescriptionMedicineId)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM prescription_medicines WHERE id = :prescriptionMedicineId");
    query.bindValue(":prescriptionMedicineId", prescriptionMedicineId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to delete prescription medicine:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::updatePrescriptionStatus(int prescriptionId, const QString& status)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE prescriptions SET status = :status WHERE id = :prescriptionId");
    query.bindValue(":prescriptionId", prescriptionId);
    query.bindValue(":status", status);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to update prescription status:" << m_lastError;
        return false;
    }
    return true;
}
