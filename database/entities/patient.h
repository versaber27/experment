#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QDateTime>

class Patient
{
public:
    Patient();

    // Getter方法
    int id() const { return m_id; }
    QString patientId() const { return m_patientId; }
    QString name() const { return m_name; }
    QString gender() const { return m_gender; }
    int age() const { return m_age; }
    QString phone() const { return m_phone; }
    QString address() const { return m_address; }
    QDateTime createdAt() const { return m_createdAt; }

    // Setter方法
    void setId(int id) { m_id = id; }
    void setPatientId(const QString& id) { m_patientId = id; }
    void setName(const QString& name) { m_name = name; }
    void setGender(const QString& gender) { m_gender = gender; }
    void setAge(int age) { m_age = age; }
    void setPhone(const QString& phone) { m_phone = phone; }
    void setAddress(const QString& address) { m_address = address; }
    void setCreatedAt(const QDateTime& time) { m_createdAt = time; }

    // 数据验证
    bool isValid() const;

private:
    int m_id;
    QString m_patientId;
    QString m_name;
    QString m_gender;
    int m_age;
    QString m_phone;
    QString m_address;
    QDateTime m_createdAt;
};

#endif // PATIENT_H
