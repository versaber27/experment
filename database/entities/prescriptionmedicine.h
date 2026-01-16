#ifndef PRESCRIPTIONMEDICINE_H
#define PRESCRIPTIONMEDICINE_H

#include <QString>

class PrescriptionMedicine
{
public:
    PrescriptionMedicine();

    // Getter方法
    int id() const { return m_id; }
    int prescriptionId() const { return m_prescriptionId; }
    int medicineId() const { return m_medicineId; }
    int quantity() const { return m_quantity; }
    double unitPrice() const { return m_unitPrice; }
    double totalPrice() const { return m_totalPrice; }
    QString dosage() const { return m_dosage; }
    QString usage() const { return m_usage; }
    QString medicineName() const { return m_medicineName; }
    QString medicineCategory() const { return m_medicineCategory; }

    // Setter方法
    void setId(int id) { m_id = id; }
    void setPrescriptionId(int prescriptionId) { m_prescriptionId = prescriptionId; }
    void setMedicineId(int medicineId) { m_medicineId = medicineId; }
    void setQuantity(int quantity) { m_quantity = quantity; }
    void setUnitPrice(double unitPrice) { m_unitPrice = unitPrice; }
    void setTotalPrice(double totalPrice) { m_totalPrice = totalPrice; }
    void setDosage(const QString& dosage) { m_dosage = dosage; }
    void setUsage(const QString& usage) { m_usage = usage; }
    void setMedicineName(const QString& medicineName) { m_medicineName = medicineName; }
    void setMedicineCategory(const QString& medicineCategory) { m_medicineCategory = medicineCategory; }

    // 数据验证
    bool isValid() const;

private:
    int m_id;
    int m_prescriptionId;
    int m_medicineId;
    int m_quantity;
    double m_unitPrice;
    double m_totalPrice;
    QString m_dosage;
    QString m_usage;
    
    // 非数据库字段，用于显示
    QString m_medicineName;
    QString m_medicineCategory;
};

#endif // PRESCRIPTIONMEDICINE_H