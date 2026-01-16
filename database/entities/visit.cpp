#include "visit.h"

Visit::Visit()
    : m_id(0)
    , m_patientId(0)
    , m_doctorId(0)
    , m_visitDate(QDateTime::currentDateTime())
{
}

bool Visit::isValid() const
{
    // 基本验证：必须有病人ID、医生ID和就诊日期
    return m_patientId > 0 && m_doctorId > 0 && m_visitDate.isValid();
}