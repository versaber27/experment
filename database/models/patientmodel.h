#ifndef PATIENTMODEL_H
#define PATIENTMODEL_H

#include <QSqlTableModel>
#include <QObject>
#include <QSqlDatabase>

class PatientModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit PatientModel(QObject *parent = nullptr, const QSqlDatabase &db = QSqlDatabase());

    // 重写基类方法
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 刷新数据
    void refresh();

    // 搜索病人
    void search(const QString& keyword);

private:
    void setupHeaders();
};

#endif // PATIENTMODEL_H
