#include "user.h"
#include <QCryptographicHash>

User::User()
    : m_id(0)
    , m_roleId(0)
    , m_isActive(true)
{
    // 默认创建时间为当前时间
    m_createdAt = QDateTime::currentDateTime();
}

bool User::isValid() const
{
    // 基本验证：必须有用户名、密码哈希和角色ID
    return !m_username.isEmpty() && 
           !m_passwordHash.isEmpty() && 
           m_roleId > 0 && 
           !m_realName.isEmpty();
}

QString User::hashPassword(const QString& password)
{
    // 使用SHA256哈希算法对密码进行哈希
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}

bool User::checkPassword(const QString& password) const
{
    // 验证密码是否匹配
    return m_passwordHash == hashPassword(password);
}