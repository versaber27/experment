#include "addpatientdialog.h"

AddPatientDialog::AddPatientDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("添加病人");
    resize(400, 300);

    QFormLayout *layout = new QFormLayout(this);

    m_nameEdit = new QLineEdit(this);
    m_genderCombo = new QComboBox(this);
    m_genderCombo->addItems({"男", "女", "其他"});
    m_ageSpin = new QSpinBox(this);
    m_ageSpin->setRange(0, 150);
    m_phoneEdit = new QLineEdit(this);
    m_addressEdit = new QLineEdit(this);

    layout->addRow("姓名:", m_nameEdit);
    layout->addRow("性别:", m_genderCombo);
    layout->addRow("年龄:", m_ageSpin);
    layout->addRow("电话:", m_phoneEdit);
    layout->addRow("地址:", m_addressEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // 设置样式
    setStyleSheet(
        "QDialog { background-color: #f5f5f5; }"
        "QLabel { font-weight: bold; }"
        "QLineEdit, QComboBox, QSpinBox { "
        "  padding: 5px; "
        "  border: 1px solid #ccc; "
        "  border-radius: 3px; "
        "}"
        );
}
