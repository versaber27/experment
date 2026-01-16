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
    return createRolesTable() &&
           createUsersTable() &&
           createPatientsTable() &&
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

bool DatabaseManager::createRolesTable()
{
    QString queryStr = 
        "CREATE TABLE IF NOT EXISTS roles ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "role_name TEXT UNIQUE NOT NULL,"
        "description TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create roles table:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::createUsersTable()
{
    QString queryStr = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "role_id INTEGER NOT NULL,"
        "real_name TEXT NOT NULL,"
        "phone TEXT,"
        "email TEXT,"
        "is_active INTEGER DEFAULT 1,"
        "last_login DATETIME,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (role_id) REFERENCES roles(id)"
        ");";

    QSqlQuery query(m_database);
    if (!query.exec(queryStr)) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to create users table:" << m_lastError;
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

bool DatabaseManager::checkMedicineStock(int medicineId, int requiredQuantity)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT stock FROM medicines WHERE id = :medicineId");
    query.bindValue(":medicineId", medicineId);

    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to check medicine stock:" << m_lastError;
        return false;
    }

    int currentStock = query.value("stock").toInt();
    return currentStock >= requiredQuantity;
}

bool DatabaseManager::updateMedicineStock(int medicineId, int quantityChange)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE medicines SET stock = stock + :quantityChange WHERE id = :medicineId");
    query.bindValue(":medicineId", medicineId);
    query.bindValue(":quantityChange", quantityChange);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to update medicine stock:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::decreaseMedicineStockForPrescription(int prescriptionId)
{
    // 开始事务
    m_database.transaction();

    // 获取处方药品列表
    QSqlQuery query(m_database);
    query.prepare("SELECT medicine_id, quantity FROM prescription_medicines WHERE prescription_id = :prescriptionId");
    query.bindValue(":prescriptionId", prescriptionId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to get prescription medicines:" << m_lastError;
        m_database.rollback();
        return false;
    }

    // 逐个减少药品库存
    while (query.next()) {
        int medicineId = query.value("medicine_id").toInt();
        int quantity = query.value("quantity").toInt();

        // 检查库存是否足够
        if (!checkMedicineStock(medicineId, quantity)) {
            m_database.rollback();
            return false;
        }

        // 减少库存
        if (!updateMedicineStock(medicineId, -quantity)) {
            m_database.rollback();
            return false;
        }
    }

    // 提交事务
    if (!m_database.commit()) {
        m_lastError = m_database.lastError().text();
        qWarning() << "Failed to commit transaction:" << m_lastError;
        m_database.rollback();
        return false;
    }

    return true;
}

bool DatabaseManager::increaseMedicineStockForPrescription(int prescriptionId)
{
    // 开始事务
    m_database.transaction();

    // 获取处方药品列表
    QSqlQuery query(m_database);
    query.prepare("SELECT medicine_id, quantity FROM prescription_medicines WHERE prescription_id = :prescriptionId");
    query.bindValue(":prescriptionId", prescriptionId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to get prescription medicines:" << m_lastError;
        m_database.rollback();
        return false;
    }

    // 逐个增加药品库存
    while (query.next()) {
        int medicineId = query.value("medicine_id").toInt();
        int quantity = query.value("quantity").toInt();

        // 增加库存
        if (!updateMedicineStock(medicineId, quantity)) {
            m_database.rollback();
            return false;
        }
    }

    // 提交事务
    if (!m_database.commit()) {
        m_lastError = m_database.lastError().text();
        qWarning() << "Failed to commit transaction:" << m_lastError;
        m_database.rollback();
        return false;
    }

    return true;
}

bool DatabaseManager::addRole(const QString& roleName, const QString& description)
{
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO roles (role_name, description) VALUES (:roleName, :description)");
    query.bindValue(":roleName", roleName);
    query.bindValue(":description", description);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to add role:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::updateRole(int roleId, const QString& roleName, const QString& description)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE roles SET role_name = :roleName, description = :description WHERE id = :roleId");
    query.bindValue(":roleId", roleId);
    query.bindValue(":roleName", roleName);
    query.bindValue(":description", description);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to update role:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::deleteRole(int roleId)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM roles WHERE id = :roleId");
    query.bindValue(":roleId", roleId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to delete role:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::getRoleById(int roleId, QString& roleName, QString& description)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT role_name, description FROM roles WHERE id = :roleId");
    query.bindValue(":roleId", roleId);

    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to get role by id:" << m_lastError;
        return false;
    }

    roleName = query.value("role_name").toString();
    description = query.value("description").toString();
    return true;
}

bool DatabaseManager::addUser(const QString& username, const QString& password, int roleId, const QString& realName, const QString& phone, const QString& email)
{
    // 对密码进行哈希处理
    QString passwordHash = User::hashPassword(password);

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO users (username, password_hash, role_id, real_name, phone, email) VALUES (:username, :passwordHash, :roleId, :realName, :phone, :email)");
    query.bindValue(":username", username);
    query.bindValue(":passwordHash", passwordHash);
    query.bindValue(":roleId", roleId);
    query.bindValue(":realName", realName);
    query.bindValue(":phone", phone);
    query.bindValue(":email", email);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to add user:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::updateUser(int userId, const QString& username, int roleId, const QString& realName, const QString& phone, const QString& email, bool isActive)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE users SET username = :username, role_id = :roleId, real_name = :realName, phone = :phone, email = :email, is_active = :isActive WHERE id = :userId");
    query.bindValue(":userId", userId);
    query.bindValue(":username", username);
    query.bindValue(":roleId", roleId);
    query.bindValue(":realName", realName);
    query.bindValue(":phone", phone);
    query.bindValue(":email", email);
    query.bindValue(":isActive", isActive ? 1 : 0);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to update user:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::updateUserPassword(int userId, const QString& password)
{
    // 对密码进行哈希处理
    QString passwordHash = User::hashPassword(password);

    QSqlQuery query(m_database);
    query.prepare("UPDATE users SET password_hash = :passwordHash WHERE id = :userId");
    query.bindValue(":userId", userId);
    query.bindValue(":passwordHash", passwordHash);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to update user password:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::deleteUser(int userId)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM users WHERE id = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to delete user:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::getUserByUsername(const QString& username, int& userId, QString& passwordHash, int& roleId, QString& realName, QString& phone, QString& email, bool& isActive)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT id, password_hash, role_id, real_name, phone, email, is_active FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to get user by username:" << m_lastError;
        return false;
    }

    userId = query.value("id").toInt();
    passwordHash = query.value("password_hash").toString();
    roleId = query.value("role_id").toInt();
    realName = query.value("real_name").toString();
    phone = query.value("phone").toString();
    email = query.value("email").toString();
    isActive = query.value("is_active").toInt() == 1;
    return true;
}

bool DatabaseManager::getUserById(int userId, QString& username, int& roleId, QString& realName, QString& phone, QString& email, bool& isActive)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT username, role_id, real_name, phone, email, is_active FROM users WHERE id = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec() || !query.next()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to get user by id:" << m_lastError;
        return false;
    }

    username = query.value("username").toString();
    roleId = query.value("role_id").toInt();
    realName = query.value("real_name").toString();
    phone = query.value("phone").toString();
    email = query.value("email").toString();
    isActive = query.value("is_active").toInt() == 1;
    return true;
}

bool DatabaseManager::authenticateUser(const QString& username, const QString& password, int& userId, int& roleId, QString& realName)
{
    // 获取用户信息
    QString passwordHash;
    QString phone;
    QString email;
    bool isActive;

    if (!getUserByUsername(username, userId, passwordHash, roleId, realName, phone, email, isActive)) {
        return false;
    }

    // 检查用户是否激活
    if (!isActive) {
        m_lastError = "User account is not active";
        qWarning() << m_lastError;
        return false;
    }

    // 验证密码
    User user;
    user.setPasswordHash(passwordHash);
    if (!user.checkPassword(password)) {
        m_lastError = "Invalid password";
        qWarning() << m_lastError;
        return false;
    }

    // 更新最后登录时间
    if (!updateUserLastLogin(userId)) {
        qWarning() << "Failed to update last login time:" << m_lastError;
        // 不影响认证结果
    }

    return true;
}

bool DatabaseManager::updateUserLastLogin(int userId)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE users SET last_login = CURRENT_TIMESTAMP WHERE id = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qWarning() << "Failed to update user last login:" << m_lastError;
        return false;
    }
    return true;
}
