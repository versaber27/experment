#ifndef USERMODEL_H
#define USERMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>

class UserModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit UserModel(QObject* parent = nullptr, const QSqlDatabase& db = QSqlDatabase());

    // 重写headerData方法以提供自定义列名
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 刷新模型数据
    void refresh();

    // 搜索功能
    void search(const QString& keyword);

private:
    // 初始化模型设置
    void setupModel();
};

#endif // USERMODEL_H