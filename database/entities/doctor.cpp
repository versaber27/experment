#include "doctor.h"
#include <QUuid>

Doctor::Doctor()
    : m_id(0)
    , m_isActive(true)
{
    // 生成医生编号: DOC-XXXX
    QString uuid = QUuid::createUuid().toString().mid(1, 8).toUpper();

    // 修复：使用多参数arg()
    m_doctorId = QString("DOC-%1").arg(uuid);
}

bool Doctor::isValid() const
{
    if (m_name.isEmpty()) return false;
    if (m_department.isEmpty()) return false;
    return true;
}
