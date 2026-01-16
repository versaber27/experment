#include "patient.h"
#include <QUuid>
#include <QDate>

Patient::Patient()
    : m_id(0)
    , m_age(0)
    , m_createdAt(QDateTime::currentDateTime())
{
    // 生成病历号: PAT-YYYYMMDD-XXXX
    QString dateStr = QDate::currentDate().toString("yyyyMMdd");
    QString uuid = QUuid::createUuid().toString().mid(1, 8).toUpper();

    // 修复：使用多参数arg()而不是链式调用
    m_patientId = QString("PAT-%1-%2").arg(dateStr, uuid);
}

bool Patient::isValid() const
{
    if (m_name.isEmpty()) return false;
    if (m_gender.isEmpty()) return false;
    if (m_age < 0 || m_age > 150) return false;
    return true;
}
