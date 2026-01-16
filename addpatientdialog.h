#ifndef ADDPATIENTDIALOG_H
#define ADDPATIENTDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>

class AddPatientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPatientDialog(QWidget *parent = nullptr);

    QString patientName() const { return m_nameEdit->text(); }
    QString gender() const { return m_genderCombo->currentText(); }
    int age() const { return m_ageSpin->value(); }
    QString phone() const { return m_phoneEdit->text(); }
    QString address() const { return m_addressEdit->text(); }

private:
    QLineEdit *m_nameEdit;
    QComboBox *m_genderCombo;
    QSpinBox *m_ageSpin;
    QLineEdit *m_phoneEdit;
    QLineEdit *m_addressEdit;
};

#endif // ADDPATIENTDIALOG_H
