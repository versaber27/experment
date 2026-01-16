#ifndef DOCTOR_H
#define DOCTOR_H

#include <QString>

class Doctor
{
public:
    Doctor();

    // Getter方法
    int id() const { return m_id; }
    QString doctorId() const { return m_doctorId; }
    QString name() const { return m_name; }
    QString department() const { return m_department; }
    QString title() const { return m_title; }
    QString phone() const { return m_phone; }
    bool isActive() const { return m_isActive; }

    // Setter方法
    void setId(int id) { m_id = id; }
    void setDoctorId(const QString& id) { m_doctorId = id; }
    void setName(const QString& name) { m_name = name; }
    void setDepartment(const QString& department) { m_department = department; }
    void setTitle(const QString& title) { m_title = title; }
    void setPhone(const QString& phone) { m_phone = phone; }
    void setIsActive(bool active) { m_isActive = active; }

    bool isValid() const;

private:
    int m_id;
    QString m_doctorId;
    QString m_name;
    QString m_department;
    QString m_title;
    QString m_phone;
    bool m_isActive;
};

#endif // DOCTOR_H
