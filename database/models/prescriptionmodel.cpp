#include "prescriptionmodel.h"

PrescriptionModel::PrescriptionModel(QObject* parent, const QSqlDatabase& db)
    : QSqlTableModel(parent, db)
{
    setupModel();
}

void PrescriptionModel::setupModel()
{
    // 设置表名
    setTable("prescriptions");
    
    // 设置编辑策略
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    // 设置排序
    setSort(5, Qt::DescendingOrder); // 按创建日期倒序
    
    // 加载数据
    select();
}

QVariant PrescriptionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return tr("ID");
        case 1: return tr("处方号");
        case 2: return tr("就诊ID");
        case 3: return tr("病人ID");
        case 4: return tr("医生ID");
        case 5: return tr("创建日期");
        case 6: return tr("总金额");
        case 7: return tr("状态");
        default: return QSqlTableModel::headerData(section, orientation, role);
        }
    }
    return QSqlTableModel::headerData(section, orientation, role);
}

void PrescriptionModel::refresh()
{
    // 清除过滤条件
    setFilter("");
    // 重新加载数据
    select();
}

void PrescriptionModel::search(const QString& keyword)
{
    if (keyword.isEmpty()) {
        refresh();
    } else {
        // 搜索处方号、病人ID或医生ID中包含关键字的记录
        QString escaped = keyword;
        escaped.replace("'", "''");
        setFilter(QString("prescription_no LIKE '%%1%' OR patient_id LIKE '%%1%' OR doctor_id LIKE '%%1%'").arg(escaped));
        select();
    }
}