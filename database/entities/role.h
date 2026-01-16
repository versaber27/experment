#ifndef ROLE_H
#define ROLE_H

#include <QString>
#include <QDateTime>

class Role
{
public:
    Role();

    // Getter方法
    int id() const { return m_id; }
    QString roleName() const { return m_roleName; }
    QString description() const { return m_description; }
    QDateTime createdAt() const { return m_createdAt; }

    // Setter方法
    void setId(int id) { m_id = id; }
    void setRoleName(const QString& roleName) { m_roleName = roleName; }
    void setDescription(const QString& description) { m_description = description; }
    void setCreatedAt(const QDateTime& createdAt) { m_createdAt = createdAt; }

    // 数据验证
    bool isValid() const;

private:
    int m_id;
    QString m_roleName;
    QString m_description;
    QDateTime m_createdAt;
};

#endif // ROLE_H