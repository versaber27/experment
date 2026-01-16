#include "statisticsdialog.h"
#include "database/databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPainter>

StatisticsDialog::StatisticsDialog(DatabaseManager *dbManager, QWidget *parent)
    : QDialog(parent), m_dbManager(dbManager)
{
    setWindowTitle("系统统计");
    setMinimumSize(800, 600);

    m_layout = new QVBoxLayout(this);
    m_chartView = new QChartView(this);
    m_layout->addWidget(m_chartView);

    loadVisitsByMonth();
}

StatisticsDialog::~StatisticsDialog()
{
}

void StatisticsDialog::loadVisitsByMonth()
{
    QSqlQuery query(m_dbManager->database());
    query.prepare("SELECT strftime('%Y-%m', visit_date) as month, COUNT(*) as count FROM visits GROUP BY month ORDER BY month");

    if (!query.exec()) {
        qWarning() << "Failed to load visits by month:" << query.lastError().text();
        return;
    }

    QBarSet *set = new QBarSet("就诊量");
    QStringList categories;

    while (query.next()) {
        QString month = query.value("month").toString();
        int count = query.value("count").toInt();
        *set << count;
        categories << month;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("月度就诊量统计");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("月份");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("就诊数量");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setChart(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
}
