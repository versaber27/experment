#include "prescription.h"

Prescription::Prescription()
    : m_id(0)
    , m_visitId(0)
    , m_patientId(0)
    , m_doctorId(0)
    , m_totalAmount(0.0)
    , m_status("有效")
{
    // 默认创建日期为当前时间
    m_createDate = QDateTime::currentDateTime();
}

bool Prescription::isValid() const
{
    // 基本验证：必须有处方号、就诊ID、病人ID和医生ID
    return !m_prescriptionNo.isEmpty() && 
           m_visitId > 0 && 
           m_patientId > 0 && 
           m_doctorId > 0;
}