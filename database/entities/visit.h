#ifndef VISIT_H
#define VISIT_H

#include <QString>
#include <QDateTime>

class Visit
{
public:
    Visit();

    // Getter方法
    int id() const { return m_id; }
    int patientId() const { return m_patientId; }
    int doctorId() const { return m_doctorId; }
    QDateTime visitDate() const { return m_visitDate; }
    QString diagnosis() const { return m_diagnosis; }
    QString symptoms() const { return m_symptoms; }
    QString treatment() const { return m_treatment; }
    QString notes() const { return m_notes; }

    // Setter方法
    void setId(int id) { m_id = id; }
    void setPatientId(int patientId) { m_patientId = patientId; }
    void setDoctorId(int doctorId) { m_doctorId = doctorId; }
    void setVisitDate(const QDateTime& visitDate) { m_visitDate = visitDate; }
    void setDiagnosis(const QString& diagnosis) { m_diagnosis = diagnosis; }
    void setSymptoms(const QString& symptoms) { m_symptoms = symptoms; }
    void setTreatment(const QString& treatment) { m_treatment = treatment; }
    void setNotes(const QString& notes) { m_notes = notes; }

    // 数据验证
    bool isValid() const;

private:
    int m_id;
    int m_patientId;
    int m_doctorId;
    QDateTime m_visitDate;
    QString m_diagnosis;
    QString m_symptoms;
    QString m_treatment;
    QString m_notes;
};

#endif // VISIT_H