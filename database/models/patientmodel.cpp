#include "patientmodel.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>

PatientModel::PatientModel(QObject *parent, const QSqlDatabase &db)
    : QSqlTableModel(parent, db.isValid() ? db : QSqlDatabase::database("medical_connection"))
{
    setTable("patients");
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    setupHeaders();
    refresh();
}

void PatientModel::setupHeaders()
{
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("病历号"));
    setHeaderData(2, Qt::Horizontal, tr("姓名"));
    setHeaderData(3, Qt::Horizontal, tr("性别"));
    setHeaderData(4, Qt::Horizontal, tr("年龄"));
    setHeaderData(5, Qt::Horizontal, tr("电话"));
    setHeaderData(6, Qt::Horizontal, tr("地址"));
    setHeaderData(7, Qt::Horizontal, tr("创建时间"));
}

QVariant PatientModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return QSqlTableModel::data(index, role);
    }

    return QVariant();
}

QVariant PatientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return tr("ID");
        case 1: return tr("病历号");
        case 2: return tr("姓名");
        case 3: return tr("性别");
        case 4: return tr("年龄");
        case 5: return tr("电话");
        case 6: return tr("地址");
        case 7: return tr("创建时间");
        default: return QVariant();
        }
    }

    return QSqlTableModel::headerData(section, orientation, role);
}

void PatientModel::refresh()
{
    setFilter("");
    setSort(2, Qt::AscendingOrder); // 按姓名排序
    select();
}

void PatientModel::search(const QString& keyword)
{
    QString trimmed = keyword.trimmed();

    if (trimmed.isEmpty()) {
        refresh();
        return;
    }

    QString escaped = trimmed;
    escaped.replace("'", "''");

    QString filter = QString(
                         "(name LIKE '%%1%' OR "
                         "phone LIKE '%%1%' OR "
                         "patient_id LIKE '%%1%')")
                         .arg(escaped);

    setFilter(filter);
    setSort(2, Qt::AscendingOrder);
    select();
}
