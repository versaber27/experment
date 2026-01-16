#include "usermodel.h"

UserModel::UserModel(QObject* parent, const QSqlDatabase& db)
    : QSqlTableModel(parent, db)
{
    setupModel();
}

void UserModel::setupModel()
{
    // 设置表名
    setTable("users");
    
    // 设置编辑策略
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    // 设置排序
    setSort(9, Qt::DescendingOrder); // 按创建时间倒序
    
    // 加载数据
    select();
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return tr("ID");
        case 1: return tr("用户名");
        case 2: return tr("密码哈希");
        case 3: return tr("角色ID");
        case 4: return tr("真实姓名");
        case 5: return tr("电话");
        case 6: return tr("邮箱");
        case 7: return tr("是否激活");
        case 8: return tr("最后登录时间");
        case 9: return tr("创建时间");
        default: return QSqlTableModel::headerData(section, orientation, role);
        }
    }
    return QSqlTableModel::headerData(section, orientation, role);
}

void UserModel::refresh()
{
    // 清除过滤条件
    setFilter("");
    // 重新加载数据
    select();
}

void UserModel::search(const QString& keyword)
{
    if (keyword.isEmpty()) {
        refresh();
    } else {
        // 搜索用户名、真实姓名或电话中包含关键字的记录
        QString escaped = keyword;
        escaped.replace("'", "''");
        setFilter(QString("username LIKE '%%1%' OR real_name LIKE '%%1%' OR phone LIKE '%%1%'").arg(escaped));
        select();
    }
}