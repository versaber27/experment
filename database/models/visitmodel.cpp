#include "visitmodel.h"

VisitModel::VisitModel(QObject* parent, const QSqlDatabase& db)
    : QSqlTableModel(parent, db)
{
    setupModel();
}

void VisitModel::setupModel()
{
    // 设置表名
    setTable("visits");
    
    // 设置编辑策略
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    // 设置排序
    setSort(3, Qt::DescendingOrder); // 按就诊日期倒序
    
    // 加载数据
    select();
}

QVariant VisitModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return tr("ID");
        case 1: return tr("病人ID");
        case 2: return tr("医生ID");
        case 3: return tr("就诊日期");
        case 4: return tr("症状");
        case 5: return tr("诊断");
        case 6: return tr("治疗方案");
        case 7: return tr("备注");
        case 8: return tr("预约ID");
        default: return QSqlTableModel::headerData(section, orientation, role);
        }
    }
    return QSqlTableModel::headerData(section, orientation, role);
}

void VisitModel::refresh()
{
    // 清除过滤条件
    setFilter("");
    // 重新加载数据
    select();
}

void VisitModel::search(const QString& keyword)
{
    if (keyword.isEmpty()) {
        refresh();
    } else {
        // 搜索症状、诊断或治疗方案中包含关键字的记录
        QString escaped = keyword;
        escaped.replace("'", "''");
        setFilter(QString("symptoms LIKE '%%1%' OR diagnosis LIKE '%%1%' OR treatment LIKE '%%1%'").arg(escaped));
        select();
    }
}