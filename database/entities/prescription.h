#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <QString>
#include <QDateTime>

class Prescription
{
public:
    Prescription();

    // Getter方法
    int id() const { return m_id; }
    QString prescriptionNo() const { return m_prescriptionNo; }
    int visitId() const { return m_visitId; }
    int patientId() const { return m_patientId; }
    int doctorId() const { return m_doctorId; }
    QDateTime createDate() const { return m_createDate; }
    double totalAmount() const { return m_totalAmount; }
    QString status() const { return m_status; }

    // Setter方法
    void setId(int id) { m_id = id; }
    void setPrescriptionNo(const QString& prescriptionNo) { m_prescriptionNo = prescriptionNo; }
    void setVisitId(int visitId) { m_visitId = visitId; }
    void setPatientId(int patientId) { m_patientId = patientId; }
    void setDoctorId(int doctorId) { m_doctorId = doctorId; }
    void setCreateDate(const QDateTime& createDate) { m_createDate = createDate; }
    void setTotalAmount(double totalAmount) { m_totalAmount = totalAmount; }
    void setStatus(const QString& status) { m_status = status; }

    // 数据验证
    bool isValid() const;

private:
    int m_id;
    QString m_prescriptionNo;
    int m_visitId;
    int m_patientId;
    int m_doctorId;
    QDateTime m_createDate;
    double m_totalAmount;
    QString m_status;
};

#endif // PRESCRIPTION_H