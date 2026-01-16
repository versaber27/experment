#include "role.h"

Role::Role()
    : m_id(0)
{
    // 默认创建时间为当前时间
    m_createdAt = QDateTime::currentDateTime();
}

bool Role::isValid() const
{
    // 基本验证：必须有角色名称
    return !m_roleName.isEmpty();
}