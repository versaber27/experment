#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

class DatabaseManager;

QT_CHARTS_USE_NAMESPACE

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsDialog(DatabaseManager *dbManager, QWidget *parent = nullptr);
    ~StatisticsDialog();

private slots:
    void loadVisitsByMonth();

private:
    DatabaseManager *m_dbManager;
    QChartView *m_chartView;
    QVBoxLayout *m_layout;
};

#endif // STATISTICSDIALOG_H