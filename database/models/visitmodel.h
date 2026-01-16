#ifndef VISITMODEL_H
#define VISITMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>

class VisitModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit VisitModel(QObject* parent = nullptr, const QSqlDatabase& db = QSqlDatabase());

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

#endif // VISITMODEL_H