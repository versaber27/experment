#include "prescriptionmedicine.h"

PrescriptionMedicine::PrescriptionMedicine()
    : m_id(0)
    , m_prescriptionId(0)
    , m_medicineId(0)
    , m_quantity(0)
    , m_unitPrice(0.0)
    , m_totalPrice(0.0)
{
}

bool PrescriptionMedicine::isValid() const
{
    // 基本验证：必须有处方ID、药品ID、数量和单价
    return m_prescriptionId > 0 && 
           m_medicineId > 0 && 
           m_quantity > 0 && 
           m_unitPrice > 0.0;
}