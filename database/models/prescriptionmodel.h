#ifndef PRESCRIPTIONMODEL_H
#define PRESCRIPTIONMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>

class PrescriptionModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit PrescriptionModel(QObject* parent = nullptr, const QSqlDatabase& db = QSqlDatabase());

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

#endif // PRESCRIPTIONMODEL_H