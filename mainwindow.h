#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "database/databasemanager.h"
#include "database/models/patientmodel.h"
#include "database/models/visitmodel.h"
#include "addpatientdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 菜单栏动作
    void onExitAction();
    void onAboutAction();
    void onExportAction();
    void onBackupAction();
    void onRestoreAction();
    void onSyncAction();

    // 病人管理
    void onAddPatientClicked();
    void onEditPatientClicked();
    void onDeletePatientClicked();
    void onSearchPatient();
    void onExportDataClicked();

    // 医生管理
    void onAddDoctorClicked();
    void onDoctorStatsClicked();
    void onEditDoctorClicked();
    void onDeleteDoctorClicked();

    // 预约管理
    void onNewAppointmentClicked();
    void onEditAppointment();      // 新增
    void onDeleteAppointment();    // 新增
    void onAppointmentFilterChanged();
    void onAppointmentContextMenu(const QPoint &pos); // 新增

    // 药品管理
    void onAddMedicineClicked();
    void onEditMedicine();         // 新增
    void onDeleteMedicine();       // 新增
    void onSearchMedicine();       // 新增
    void onCheckLowStock();
    void onMedicineContextMenu(const QPoint &pos);    // 新增

    // 医疗记录管理
    void onAddVisitClicked();
    void onEditVisitClicked();
    void onDeleteVisitClicked();
    void onSearchVisit();

    // 统计分析
    void onGenerateDailyReport();
    void onGenerateMonthlyReport();
    void updateStatistics();

    // 表格选择变化
    void onPatientSelectionChanged();
    void onDoctorSelectionChanged();
    void onAppointmentSelectionChanged();
    void onMedicineSelectionChanged();
    void onVisitSelectionChanged();

private:
    void setupUI();
    void setupConnections();
    void setupMenuBar();
    void setupToolBar();
    void setupModels();
    void loadInitialData();
    void updatePatientStats();

    Ui::MainWindow *ui;

    // Data models
    PatientModel *m_patientModel;
    QSqlTableModel *m_doctorModel;
    QSqlTableModel *m_appointmentModel;
    QSqlTableModel *m_medicineModel;
    VisitModel *m_visitModel;

    // 数据库管理器
    DatabaseManager &m_dbManager;
};

#endif // MAINWINDOW_H
