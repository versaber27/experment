#include "medicine.h"

Medicine::Medicine()
    : m_id(0)
    , m_stock(0)
    , m_warningThreshold(10)
    , m_unitPrice(0.0)
{}

bool Medicine::isValid() const
{
    // 基本验证：必须有药品名称
    return !m_name.isEmpty();
}

bool Medicine::isBelowWarningThreshold() const
{
    // 检查当前库存是否低于预警阈值
    return m_stock <= m_warningThreshold;
}