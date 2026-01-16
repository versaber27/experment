#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class User
{
public:
    User();

    // Getter方法
    int id() const { return m_id; }
    QString username() const { return m_username; }
    QString passwordHash() const { return m_passwordHash; }
    int roleId() const { return m_roleId; }
    QString realName() const { return m_realName; }
    QString phone() const { return m_phone; }
    QString email() const { return m_email; }
    bool isActive() const { return m_isActive; }
    QDateTime lastLogin() const { return m_lastLogin; }
    QDateTime createdAt() const { return m_createdAt; }

    // Setter方法
    void setId(int id) { m_id = id; }
    void setUsername(const QString& username) { m_username = username; }
    void setPasswordHash(const QString& passwordHash) { m_passwordHash = passwordHash; }
    void setRoleId(int roleId) { m_roleId = roleId; }
    void setRealName(const QString& realName) { m_realName = realName; }
    void setPhone(const QString& phone) { m_phone = phone; }
    void setEmail(const QString& email) { m_email = email; }
    void setIsActive(bool isActive) { m_isActive = isActive; }
    void setLastLogin(const QDateTime& lastLogin) { m_lastLogin = lastLogin; }
    void setCreatedAt(const QDateTime& createdAt) { m_createdAt = createdAt; }

    // 数据验证
    bool isValid() const;

    // 密码相关方法
    static QString hashPassword(const QString& password);
    bool checkPassword(const QString& password) const;

private:
    int m_id;
    QString m_username;
    QString m_passwordHash;
    int m_roleId;
    QString m_realName;
    QString m_phone;
    QString m_email;
    bool m_isActive;
    QDateTime m_lastLogin;
    QDateTime m_createdAt;
};

#endif // USER_H