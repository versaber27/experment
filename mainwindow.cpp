#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database/entities/patient.h"
#include <QUuid>

// Qt核心头文件
#include <QToolBar>
#include <QFormLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>
#include <QTime>
#include <QTextStream>
#include <QFile>
#include <QTextEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QSqlRecord>

// Qt SQL头文件
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dbManager(DatabaseManager::instance())
{
    ui->setupUi(this);

    // 动态添加药品搜索框
    QLineEdit *searchMedicineEdit = new QLineEdit(ui->tab_medicine);
    searchMedicineEdit->setObjectName("lineEdit_search_medicine");
    searchMedicineEdit->setPlaceholderText("搜索药品名称...");
    QPushButton *searchMedicineBtn = new QPushButton("搜索", ui->tab_medicine);
    
    // 插入到布局中 (horizontalLayout_5: label, btn_add, spacer)
    // 我们将搜索框插入到 btn_add_medicine 之前
    int index = ui->horizontalLayout_5->indexOf(ui->btn_add_medicine);
    if (index != -1) {
        ui->horizontalLayout_5->insertWidget(index, new QLabel("搜索:", ui->tab_medicine));
        ui->horizontalLayout_5->insertWidget(index + 1, searchMedicineEdit);
        ui->horizontalLayout_5->insertWidget(index + 2, searchMedicineBtn);
    }

    // 连接搜索信号
    connect(searchMedicineBtn, &QPushButton::clicked, this, &MainWindow::onSearchMedicine);
    connect(searchMedicineEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearchMedicine);

    setWindowTitle("社区医疗信息管理系统");
    resize(1200, 800);

    // 初始化数据库
    if (!m_dbManager.initialize()) {
        QMessageBox::critical(this, "数据库错误",
                              QString("数据库初始化失败：%1").arg(m_dbManager.lastError()));
    }

    setupMenuBar();
    setupToolBar();
    setupConnections();
    setupModels();
    loadInitialData();

    statusBar()->showMessage("系统已就绪", 3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenuBar()
{
    // 连接菜单动作
    connect(ui->action_exit, &QAction::triggered, this, &MainWindow::onExitAction);
    connect(ui->action_about, &QAction::triggered, this, &MainWindow::onAboutAction);
    connect(ui->action_export, &QAction::triggered, this, &MainWindow::onExportAction);
    connect(ui->action_backup, &QAction::triggered, this, &MainWindow::onBackupAction);
    connect(ui->action_restore, &QAction::triggered, this, &MainWindow::onRestoreAction);
    connect(ui->action_sync, &QAction::triggered, this, &MainWindow::onSyncAction);
}

void MainWindow::setupToolBar()
{
    // 可以在这里添加工具栏按钮
    QToolBar *mainToolBar = addToolBar("主工具栏");
    mainToolBar->setMovable(false);
    mainToolBar->addAction(ui->action_export);
    mainToolBar->addSeparator();
    mainToolBar->addAction(ui->action_sync);
}

void MainWindow::setupConnections()
{
    // 病人管理标签页
    connect(ui->btn_add_patient, &QPushButton::clicked, this, &MainWindow::onAddPatientClicked);
    connect(ui->btn_edit_patient, &QPushButton::clicked, this, &MainWindow::onEditPatientClicked);
    connect(ui->btn_delete_patient, &QPushButton::clicked, this, &MainWindow::onDeletePatientClicked);
    connect(ui->btn_search, &QPushButton::clicked, this, &MainWindow::onSearchPatient);
    connect(ui->lineEdit_search, &QLineEdit::returnPressed, this, &MainWindow::onSearchPatient);
    connect(ui->btn_export, &QPushButton::clicked, this, &MainWindow::onExportDataClicked);

    // 医生管理标签页
    connect(ui->btn_add_doctor, &QPushButton::clicked, this, &MainWindow::onAddDoctorClicked);
    connect(ui->btn_doctor_stats, &QPushButton::clicked, this, &MainWindow::onDoctorStatsClicked);
    connect(ui->btn_edit_doctor, &QPushButton::clicked, this, &MainWindow::onEditDoctorClicked);
    connect(ui->btn_delete_doctor, &QPushButton::clicked, this, &MainWindow::onDeleteDoctorClicked);

    // 预约管理标签页
    connect(ui->btn_new_appointment, &QPushButton::clicked, this, &MainWindow::onNewAppointmentClicked);
    connect(ui->btn_edit_appointment, &QPushButton::clicked, this, &MainWindow::onEditAppointment);
    connect(ui->btn_delete_appointment, &QPushButton::clicked, this, &MainWindow::onDeleteAppointment);
    connect(ui->comboBox_status, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onAppointmentFilterChanged);
    connect(ui->dateEdit_appointment, &QDateEdit::dateChanged,
            this, &MainWindow::onAppointmentFilterChanged);
    
    // 预约右键菜单
    ui->tableView_appointments->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView_appointments, &QTableView::customContextMenuRequested,
            this, &MainWindow::onAppointmentContextMenu);

    // 药品管理标签页
    connect(ui->btn_add_medicine, &QPushButton::clicked, this, &MainWindow::onAddMedicineClicked);
    connect(ui->btn_edit_medicine, &QPushButton::clicked, this, &MainWindow::onEditMedicine);
    connect(ui->btn_delete_medicine, &QPushButton::clicked, this, &MainWindow::onDeleteMedicine);
    
    // 药品右键菜单
    ui->tableView_medicines->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView_medicines, &QTableView::customContextMenuRequested,
            this, &MainWindow::onMedicineContextMenu);

    // 统计分析标签页
    connect(ui->btn_generate_report, &QPushButton::clicked, this, &MainWindow::onGenerateDailyReport);
    connect(ui->btn_monthly_report, &QPushButton::clicked, this, &MainWindow::onGenerateMonthlyReport);

    // 表格选择变化
    connect(ui->tableView_patients->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onPatientSelectionChanged);
    connect(ui->tableView_doctors->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onDoctorSelectionChanged);
    connect(ui->tableView_appointments->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onAppointmentSelectionChanged);
    connect(ui->tableView_medicines->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onMedicineSelectionChanged);
}

void MainWindow::setupModels()
{
    m_patientModel = new PatientModel(this, m_dbManager.database());
    ui->tableView_patients->setModel(m_patientModel);
    ui->tableView_patients->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_patients->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_patients->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_patients->verticalHeader()->setVisible(false);
    ui->tableView_patients->setAlternatingRowColors(true);

    // 医生模型
    m_doctorModel = new QSqlTableModel(this, m_dbManager.database());
    m_doctorModel->setTable("doctors");
    m_doctorModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 设置列名映射
    m_doctorModel->setHeaderData(1, Qt::Horizontal, tr("医生编号"));
    m_doctorModel->setHeaderData(2, Qt::Horizontal, tr("姓名"));
    m_doctorModel->setHeaderData(3, Qt::Horizontal, tr("科室"));
    m_doctorModel->setHeaderData(4, Qt::Horizontal, tr("职称"));
    m_doctorModel->setHeaderData(5, Qt::Horizontal, tr("电话"));
    m_doctorModel->setHeaderData(6, Qt::Horizontal, tr("邮箱"));
    m_doctorModel->setHeaderData(7, Qt::Horizontal, tr("出诊日"));
    m_doctorModel->setHeaderData(8, Qt::Horizontal, tr("门诊时间"));
    m_doctorModel->setHeaderData(9, Qt::Horizontal, tr("每日限额"));
    m_doctorModel->setHeaderData(10, Qt::Horizontal, tr("状态"));

    // 设置排序
    m_doctorModel->setSort(2, Qt::AscendingOrder); // 按姓名排序
    m_doctorModel->select();

    ui->tableView_doctors->setModel(m_doctorModel);
    ui->tableView_doctors->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_doctors->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_doctors->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_doctors->verticalHeader()->setVisible(false);
    ui->tableView_doctors->setAlternatingRowColors(true);

    // 预约模型
    m_appointmentModel = new QSqlTableModel(this, m_dbManager.database());
    m_appointmentModel->setTable("appointments");
    m_appointmentModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 设置列名映射
    m_appointmentModel->setHeaderData(1, Qt::Horizontal, tr("预约号"));
    m_appointmentModel->setHeaderData(2, Qt::Horizontal, tr("病人ID"));
    m_appointmentModel->setHeaderData(3, Qt::Horizontal, tr("医生ID"));
    m_appointmentModel->setHeaderData(4, Qt::Horizontal, tr("预约日期"));
    m_appointmentModel->setHeaderData(5, Qt::Horizontal, tr("预约时间"));
    m_appointmentModel->setHeaderData(6, Qt::Horizontal, tr("状态"));
    m_appointmentModel->setHeaderData(7, Qt::Horizontal, tr("症状"));
    m_appointmentModel->setHeaderData(8, Qt::Horizontal, tr("备注"));
    m_appointmentModel->setHeaderData(9, Qt::Horizontal, tr("创建时间"));

    // 设置排序
    m_appointmentModel->setSort(9, Qt::DescendingOrder); // 按创建时间倒序
    m_appointmentModel->select();

    ui->tableView_appointments->setModel(m_appointmentModel);
    ui->tableView_appointments->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_appointments->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_appointments->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_appointments->verticalHeader()->setVisible(false);
    ui->tableView_appointments->setAlternatingRowColors(true);

    // 药品模型
    m_medicineModel = new QSqlTableModel(this, m_dbManager.database());
    m_medicineModel->setTable("medicines");
    m_medicineModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 设置列名映射
    m_medicineModel->setHeaderData(1, Qt::Horizontal, tr("药品名称"));
    m_medicineModel->setHeaderData(2, Qt::Horizontal, tr("类别"));
    m_medicineModel->setHeaderData(3, Qt::Horizontal, tr("库存"));
    m_medicineModel->setHeaderData(4, Qt::Horizontal, tr("单价"));
    m_medicineModel->setHeaderData(5, Qt::Horizontal, tr("有效期"));

    // 设置排序
    m_medicineModel->setSort(1, Qt::AscendingOrder); // 按药品名称排序
    m_medicineModel->select();

    ui->tableView_medicines->setModel(m_medicineModel);
    ui->tableView_medicines->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_medicines->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_medicines->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_medicines->verticalHeader()->setVisible(false);
    ui->tableView_medicines->setAlternatingRowColors(true);
}

void MainWindow::loadInitialData()
{
    // 刷新所有模型
    m_patientModel->refresh();
    m_doctorModel->select();
    m_appointmentModel->select();
    m_medicineModel->select();

    // 设置默认日期
    ui->dateEdit_appointment->setDate(QDate::currentDate());

    // 更新统计信息
    updatePatientStats();
    updateStatistics();
    onCheckLowStock();

    onPatientSelectionChanged();
    onDoctorSelectionChanged();
    onAppointmentSelectionChanged();
    onMedicineSelectionChanged();

    statusBar()->showMessage("数据加载完成", 2000);
}

// ==================== 病人管理功能 ====================
void MainWindow::onAddPatientClicked()
{
    AddPatientDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        // 验证输入
        if (dialog.patientName().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "姓名不能为空");
            return;
        }

        // 构造病人实体并生成病历号
        Patient patient;
        patient.setName(dialog.patientName());
        patient.setGender(dialog.gender());
        patient.setAge(dialog.age());
        patient.setPhone(dialog.phone());
        patient.setAddress(dialog.address());

        if (!patient.isValid()) {
            QMessageBox::warning(this, "输入错误", "请输入合法的病人信息");
            return;
        }

        // 执行插入操作（包含必填的 patient_id）
        QSqlQuery query(m_dbManager.database());
        query.prepare("INSERT INTO patients (patient_id, name, gender, age, phone, address) "
                      "VALUES (:patient_id, :name, :gender, :age, :phone, :address)");
        query.bindValue(":patient_id", patient.patientId());
        query.bindValue(":name", patient.name());
        query.bindValue(":gender", patient.gender());
        query.bindValue(":age", patient.age());
        query.bindValue(":phone", patient.phone());
        query.bindValue(":address", patient.address());

        if (query.exec()) {
            m_patientModel->refresh();
            updatePatientStats();
            QMessageBox::information(this, "成功", "病人添加成功！");
        } else {
            QMessageBox::warning(this, "失败",
                                 QString("添加失败：%1").arg(query.lastError().text()));
        }
    }
}

void MainWindow::onEditPatientClicked()
{
    QModelIndexList selection = ui->tableView_patients->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的病人");
        return;
    }

    int row = selection.first().row();
    int patientId = m_patientModel->data(m_patientModel->index(row, 0)).toInt();

    // 获取当前病人的信息
    QSqlQuery query(m_dbManager.database());
    query.prepare("SELECT name, gender, age, phone, address FROM patients WHERE id = ?");
    query.addBindValue(patientId);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "错误", "无法获取病人信息");
        return;
    }

    // 创建编辑对话框
    QDialog dialog(this);
    dialog.setWindowTitle("编辑病人信息");
    dialog.resize(400, 300);

    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit(query.value(0).toString(), &dialog);
    QComboBox *genderCombo = new QComboBox(&dialog);
    genderCombo->addItems({"男", "女", "其他"});
    genderCombo->setCurrentText(query.value(1).toString());

    QSpinBox *ageSpin = new QSpinBox(&dialog);
    ageSpin->setRange(0, 150);
    ageSpin->setValue(query.value(2).toInt());

    QLineEdit *phoneEdit = new QLineEdit(query.value(3).toString(), &dialog);
    QLineEdit *addressEdit = new QLineEdit(query.value(4).toString(), &dialog);

    layout->addRow("姓名:", nameEdit);
    layout->addRow("性别:", genderCombo);
    layout->addRow("年龄:", ageSpin);
    layout->addRow("电话:", phoneEdit);
    layout->addRow("地址:", addressEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // 执行更新操作
        QSqlQuery updateQuery(m_dbManager.database());
        updateQuery.prepare("UPDATE patients SET name = ?, gender = ?, age = ?, "
                            "phone = ?, address = ? "
                            "WHERE id = ?");
        updateQuery.addBindValue(nameEdit->text());
        updateQuery.addBindValue(genderCombo->currentText());
        updateQuery.addBindValue(ageSpin->value());
        updateQuery.addBindValue(phoneEdit->text());
        updateQuery.addBindValue(addressEdit->text());
        updateQuery.addBindValue(patientId);

        if (updateQuery.exec()) {
            m_patientModel->refresh();
            QMessageBox::information(this, "成功", "病人信息更新成功！");
        } else {
            QMessageBox::warning(this, "失败",
                                 QString("更新失败：%1").arg(updateQuery.lastError().text()));
        }
    }
}

void MainWindow::onDeletePatientClicked()
{
    QModelIndexList selection = ui->tableView_patients->selectionModel()->selectedRows();

    if (selection.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的病人");
        return;
    }

    int row = selection.first().row();
    int patientId = m_patientModel->data(m_patientModel->index(row, 0)).toInt();
    QString patientName = m_patientModel->data(m_patientModel->index(row, 2)).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("确定要删除病人 '%1' 吗？").arg(patientName),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query(m_dbManager.database());
        query.prepare("DELETE FROM patients WHERE id = ?");
        query.addBindValue(patientId);

        if (query.exec()) {
            m_patientModel->refresh();
            updatePatientStats();
            QMessageBox::information(this, "成功", "病人删除成功！");
        } else {
            QMessageBox::warning(this, "失败",
                                 QString("删除失败：%1").arg(query.lastError().text()));
        }
    }
}

void MainWindow::onSearchPatient()
{
    QString keyword = ui->lineEdit_search->text();

    if (!keyword.isEmpty()) {
        m_patientModel->search(keyword);
    } else {
        m_patientModel->refresh();
    }
}

void MainWindow::onExportDataClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "导出数据",
                                                    "病人数据.csv",
                                                    "CSV文件 (*.csv)");

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法创建文件");
        return;
    }

    QTextStream out(&file);

    // 写入表头
    for (int col = 0; col < m_patientModel->columnCount(); ++col) {
        out << m_patientModel->headerData(col, Qt::Horizontal).toString();
        if (col < m_patientModel->columnCount() - 1) {
            out << ",";
        }
    }
    out << "\n";

    // 写入数据
    for (int row = 0; row < m_patientModel->rowCount(); ++row) {
        for (int col = 0; col < m_patientModel->columnCount(); ++col) {
            QModelIndex index = m_patientModel->index(row, col);
            out << m_patientModel->data(index).toString();
            if (col < m_patientModel->columnCount() - 1) {
                out << ",";
            }
        }
        out << "\n";
    }

    file.close();
    QMessageBox::information(this, "成功", "数据导出成功！");
}

void MainWindow::updatePatientStats()
{
    QSqlQuery query(m_dbManager.database());
    query.exec("SELECT COUNT(*) FROM patients");

    if (query.next()) {
        int count = query.value(0).toInt();
        ui->label_stats->setText(QString("病人总数: %1").arg(count));
    }
}

// ==================== 医生管理功能 ====================
void MainWindow::onAddDoctorClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("添加医生");
    dialog.resize(500, 400);

    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    QLineEdit *departmentEdit = new QLineEdit(&dialog);
    QLineEdit *titleEdit = new QLineEdit(&dialog);
    QLineEdit *phoneEdit = new QLineEdit(&dialog);
    QLineEdit *emailEdit = new QLineEdit(&dialog);
    QLineEdit *availableDaysEdit = new QLineEdit(&dialog);
    availableDaysEdit->setText("周一至周五");
    QLineEdit *consultationHoursEdit = new QLineEdit(&dialog);
    consultationHoursEdit->setText("08:00-12:00,14:00-17:00");
    QSpinBox *maxPatientsSpin = new QSpinBox(&dialog);
    maxPatientsSpin->setRange(1, 100);
    maxPatientsSpin->setValue(30);

    layout->addRow("姓名:", nameEdit);
    layout->addRow("科室:", departmentEdit);
    layout->addRow("职称:", titleEdit);
    layout->addRow("电话:", phoneEdit);
    layout->addRow("邮箱:", emailEdit);
    layout->addRow("出诊日:", availableDaysEdit);
    layout->addRow("门诊时间:", consultationHoursEdit);
    layout->addRow("每日限额:", maxPatientsSpin);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // 验证输入
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "姓名不能为空");
            return;
        }
        if (departmentEdit->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "科室不能为空");
            return;
        }

        // 生成医生编号
        QString uuid = QUuid::createUuid().toString().mid(1, 8).toUpper();
        QString doctorId = QString("DOC-%1").arg(uuid);

        // 执行插入操作
        QSqlQuery query(m_dbManager.database());
        query.prepare("INSERT INTO doctors (doctor_id, name, department, title, phone, email, "
                      "available_days, consultation_hours, max_patients_per_day) "
                      "VALUES (:doctor_id, :name, :department, :title, :phone, :email, "
                      ":available_days, :consultation_hours, :max_patients)");
        query.bindValue(":doctor_id", doctorId);
        query.bindValue(":name", nameEdit->text());
        query.bindValue(":department", departmentEdit->text());
        query.bindValue(":title", titleEdit->text());
        query.bindValue(":phone", phoneEdit->text());
        query.bindValue(":email", emailEdit->text());
        query.bindValue(":available_days", availableDaysEdit->text());
        query.bindValue(":consultation_hours", consultationHoursEdit->text());
        query.bindValue(":max_patients", maxPatientsSpin->value());

        if (query.exec()) {
            m_doctorModel->select();
            QMessageBox::information(this, "成功", "医生添加成功！");
        } else {
            QMessageBox::warning(this, "失败",
                                 QString("添加失败：%1").arg(query.lastError().text()));
        }
    }
}

void MainWindow::onEditDoctorClicked()
{
    QModelIndexList selection = ui->tableView_doctors->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的医生");
        return;
    }

    int row = selection.first().row();
    int id = m_doctorModel->data(m_doctorModel->index(row, 0)).toInt();

    QSqlQuery query(m_dbManager.database());
    query.prepare("SELECT name, department, title, phone, email, "
                  "available_days, consultation_hours, max_patients_per_day "
                  "FROM doctors WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "错误", "无法获取医生信息");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("编辑医生信息");
    dialog.resize(500, 400);

    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit(query.value(0).toString(), &dialog);
    QLineEdit *departmentEdit = new QLineEdit(query.value(1).toString(), &dialog);
    QLineEdit *titleEdit = new QLineEdit(query.value(2).toString(), &dialog);
    QLineEdit *phoneEdit = new QLineEdit(query.value(3).toString(), &dialog);
    QLineEdit *emailEdit = new QLineEdit(query.value(4).toString(), &dialog);
    QLineEdit *availableDaysEdit = new QLineEdit(query.value(5).toString(), &dialog);
    QLineEdit *consultationHoursEdit = new QLineEdit(query.value(6).toString(), &dialog);
    QSpinBox *maxPatientsSpin = new QSpinBox(&dialog);
    maxPatientsSpin->setRange(1, 100);
    maxPatientsSpin->setValue(query.value(7).toInt());

    layout->addRow("姓名:", nameEdit);
    layout->addRow("科室:", departmentEdit);
    layout->addRow("职称:", titleEdit);
    layout->addRow("电话:", phoneEdit);
    layout->addRow("邮箱:", emailEdit);
    layout->addRow("出诊日:", availableDaysEdit);
    layout->addRow("门诊时间:", consultationHoursEdit);
    layout->addRow("每日限额:", maxPatientsSpin);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "姓名不能为空");
            return;
        }
        if (departmentEdit->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "科室不能为空");
            return;
        }

        QSqlQuery updateQuery(m_dbManager.database());
        updateQuery.prepare("UPDATE doctors SET name = ?, department = ?, title = ?, "
                            "phone = ?, email = ?, available_days = ?, "
                            "consultation_hours = ?, max_patients_per_day = ? "
                            "WHERE id = ?");
        updateQuery.addBindValue(nameEdit->text());
        updateQuery.addBindValue(departmentEdit->text());
        updateQuery.addBindValue(titleEdit->text());
        updateQuery.addBindValue(phoneEdit->text());
        updateQuery.addBindValue(emailEdit->text());
        updateQuery.addBindValue(availableDaysEdit->text());
        updateQuery.addBindValue(consultationHoursEdit->text());
        updateQuery.addBindValue(maxPatientsSpin->value());
        updateQuery.addBindValue(id);

        if (updateQuery.exec()) {
            m_doctorModel->select();
            QMessageBox::information(this, "成功", "医生信息更新成功！");
        } else {
            QMessageBox::warning(this, "失败",
                                 QString("更新失败：%1").arg(updateQuery.lastError().text()));
        }
    }
}

void MainWindow::onDeleteDoctorClicked()
{
    QModelIndexList selection = ui->tableView_doctors->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的医生");
        return;
    }

    int row = selection.first().row();
    int id = m_doctorModel->data(m_doctorModel->index(row, 0)).toInt();
    QString name = m_doctorModel->data(m_doctorModel->index(row, 2)).toString();

    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "确认删除",
                              QString("确定要删除医生 '%1' 吗？").arg(name),
                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query(m_dbManager.database());
        query.prepare("DELETE FROM doctors WHERE id = ?");
        query.addBindValue(id);

        if (query.exec()) {
            m_doctorModel->select();
            QMessageBox::information(this, "成功", "医生删除成功！");
        } else {
            QMessageBox::warning(this, "失败",
                                 QString("删除失败：%1").arg(query.lastError().text()));
        }
    }
}

void MainWindow::onDoctorStatsClicked()
{
    QSqlQuery query(m_dbManager.database());
    query.exec("SELECT department, COUNT(*) as doctor_count, "
               "SUM(max_patients_per_day) as total_capacity "
               "FROM doctors WHERE is_active = 1 "
               "GROUP BY department");

    QString stats = "<h3>科室统计</h3><table border='1' style='border-collapse: collapse;'>";
    stats += "<tr><th>科室</th><th>医生数</th><th>日接诊能力</th></tr>";

    int totalDoctors = 0;
    int totalCapacity = 0;

    while (query.next()) {
        QString department = query.value(0).toString();
        int doctorCount = query.value(1).toInt();
        int capacity = query.value(2).toInt();

        stats += QString("<tr><td>%1</td><td align='center'>%2</td><td align='center'>%3</td></tr>")
                     .arg(department)
                     .arg(doctorCount)
                     .arg(capacity);

        totalDoctors += doctorCount;
        totalCapacity += capacity;
    }

    stats += QString("<tr><td><b>总计</b></td><td align='center'><b>%1</b></td><td align='center'><b>%2</b></td></tr>")
                 .arg(totalDoctors)
                 .arg(totalCapacity);
    stats += "</table>";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("科室统计");
    msgBox.setText(stats);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.exec();
}

// ==================== 预约管理功能 ====================
void MainWindow::onNewAppointmentClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("新建预约");
    dialog.resize(600, 400);
    QFormLayout *layout = new QFormLayout(&dialog);

    QComboBox *patientCombo = new QComboBox(&dialog);
    QSqlQuery patientQuery(m_dbManager.database());
    if (!patientQuery.exec("SELECT id, name, patient_id FROM patients ORDER BY name")) {
        QMessageBox::warning(this, "错误",
                             QString("加载病人列表失败：%1").arg(patientQuery.lastError().text()));
        return;
    }
    while (patientQuery.next()) {
        int id = patientQuery.value(0).toInt();
        QString name = patientQuery.value(1).toString();
        QString patientId = patientQuery.value(2).toString();
        patientCombo->addItem(QString("%1 (%2)").arg(name).arg(patientId), id);
    }

    if (patientCombo->count() == 0) {
        QMessageBox::information(this, "提示", "当前没有可选病人，请先在“病人管理”中添加病人。");
        return;
    }

    QComboBox *doctorCombo = new QComboBox(&dialog);
    QSqlQuery doctorQuery(m_dbManager.database());
    if (!doctorQuery.exec("SELECT id, name, department FROM doctors WHERE is_active = 1 ORDER BY name")) {
        QMessageBox::warning(this, "错误",
                             QString("加载医生列表失败：%1").arg(doctorQuery.lastError().text()));
        return;
    }
    while (doctorQuery.next()) {
        int id = doctorQuery.value(0).toInt();
        QString name = doctorQuery.value(1).toString();
        QString department = doctorQuery.value(2).toString();
        doctorCombo->addItem(QString("%1 - %2").arg(name).arg(department), id);
    }

    if (doctorCombo->count() == 0) {
        QMessageBox::information(this, "提示", "当前没有可选医生，请先在“医生管理”中添加医生。");
        return;
    }

    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate(), &dialog);
    dateEdit->setCalendarPopup(true);
    dateEdit->setMinimumDate(QDate::currentDate());

    QTimeEdit *timeEdit = new QTimeEdit(QTime(9, 0), &dialog);
    timeEdit->setDisplayFormat("hh:mm");

    QTextEdit *symptomsEdit = new QTextEdit(&dialog);
    symptomsEdit->setMaximumHeight(100);

    layout->addRow("选择病人:", patientCombo);
    layout->addRow("选择医生:", doctorCombo);
    layout->addRow("预约日期:", dateEdit);
    layout->addRow("预约时间:", timeEdit);
    layout->addRow("症状描述:", symptomsEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int patientId = patientCombo->currentData().toInt();
        int doctorId = doctorCombo->currentData().toInt();
        QDate appointmentDate = dateEdit->date();
        QTime appointmentTime = timeEdit->time();
        QString symptoms = symptomsEdit->toPlainText();

        // 生成预约号
        QString appointmentNo = QString("APT-%1-%2")
                                    .arg(appointmentDate.toString("yyyyMMdd"))
                                    .arg(QTime::currentTime().toString("hhmmss"));

        // 检查时间冲突
        QSqlQuery conflictQuery(m_dbManager.database());
        conflictQuery.prepare("SELECT COUNT(*) FROM appointments "
                              "WHERE doctor_id = ? AND appointment_date = ? "
                              "AND appointment_time = ? AND status != '已取消'");
        conflictQuery.addBindValue(doctorId);
        conflictQuery.addBindValue(appointmentDate.toString("yyyy-MM-dd"));
        conflictQuery.addBindValue(appointmentTime.toString("hh:mm:ss"));

        if (conflictQuery.exec() && conflictQuery.next() && conflictQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "时间冲突", "该医生在该时间段已有预约，请选择其他时间！");
            return;
        }

        // 执行插入操作
        QSqlQuery query(m_dbManager.database());
        query.prepare("INSERT INTO appointments (appointment_no, patient_id, doctor_id, "
                      "appointment_date, appointment_time, symptoms, status) "
                      "VALUES (:no, :patient_id, :doctor_id, :date, :time, :symptoms, '预约中')");
        query.bindValue(":no", appointmentNo);
        query.bindValue(":patient_id", patientId);
        query.bindValue(":doctor_id", doctorId);
        query.bindValue(":date", appointmentDate.toString("yyyy-MM-dd"));
        query.bindValue(":time", appointmentTime.toString("hh:mm:ss"));
        query.bindValue(":symptoms", symptoms);

        if (query.exec()) {
            m_appointmentModel->select();
            QMessageBox::information(this, "成功",
                                     QString("预约创建成功！\n预约号：%1").arg(appointmentNo));
        } else {
            QMessageBox::warning(this, "失败",
                                 QString("预约创建失败：%1").arg(query.lastError().text()));
        }
    }
}

void MainWindow::onAppointmentFilterChanged()
{
    QString status = ui->comboBox_status->currentText();
    QDate date = ui->dateEdit_appointment->date();

    QString filter;

    if (status != "所有状态") {
        filter = QString("status = '%1'").arg(status);
    }

    if (date.isValid()) {
        if (!filter.isEmpty()) {
            filter += " AND ";
        }
        filter += QString("appointment_date = '%1'").arg(date.toString("yyyy-MM-dd"));
    }

    m_appointmentModel->setFilter(filter);
    m_appointmentModel->select();

    // 更新状态栏显示
    int rowCount = m_appointmentModel->rowCount();
    statusBar()->showMessage(QString("找到 %1 条预约记录").arg(rowCount), 2000);
}

// ==================== 药品管理功能 ====================
void MainWindow::onAddMedicineClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("添加药品");
    dialog.resize(500, 400);

    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    QLineEdit *categoryEdit = new QLineEdit(&dialog);
    categoryEdit->setText("常规药品");
    QSpinBox *stockSpin = new QSpinBox(&dialog);
    stockSpin->setRange(0, 10000);
    stockSpin->setValue(100);

    QDoubleSpinBox *priceSpin = new QDoubleSpinBox(&dialog);
    priceSpin->setRange(0.0, 10000.0);
    priceSpin->setDecimals(2);
    priceSpin->setPrefix("¥ ");
    priceSpin->setValue(25.0);

    QDateEdit *expiryDateEdit = new QDateEdit(QDate::currentDate().addYears(2), &dialog);
    expiryDateEdit->setCalendarPopup(true);

    layout->addRow("药品名称:", nameEdit);
    layout->addRow("药品类别:", categoryEdit);
    layout->addRow("库存数量:", stockSpin);
    layout->addRow("单价:", priceSpin);
    layout->addRow("有效期至:", expiryDateEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // 验证输入
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "药品名称不能为空");
            return;
        }

        QSqlQuery query(m_dbManager.database());
        query.prepare("INSERT INTO medicines (name, category, stock, unit_price, expiry_date) "
                      "VALUES (:name, :category, :stock, :price, :expiry)");
        query.bindValue(":name", nameEdit->text());
        query.bindValue(":category", categoryEdit->text());
        query.bindValue(":stock", stockSpin->value());
        query.bindValue(":price", priceSpin->value());
        query.bindValue(":expiry", expiryDateEdit->date().toString("yyyy-MM-dd"));

        if (query.exec()) {
            m_medicineModel->select();
            onCheckLowStock(); // 重新检查库存
            QMessageBox::information(this, "成功", "药品添加成功！");
        } else {
            QMessageBox::warning(this, "失败",
                                 QString("添加失败：%1").arg(query.lastError().text()));
        }
    }
}

void MainWindow::onCheckLowStock()
{
    QSqlQuery query(m_dbManager.database());
    query.exec("SELECT COUNT(*) FROM medicines WHERE stock < 10");

    if (query.next()) {
        int lowStockCount = query.value(0).toInt();
        if (lowStockCount > 0) {
            ui->label_low_stock->setText(
                QString("库存预警: %1种药品库存不足").arg(lowStockCount));
            ui->label_low_stock->setStyleSheet("color: red; font-weight: bold;");
        } else {
            ui->label_low_stock->setText("库存正常");
            ui->label_low_stock->setStyleSheet("color: green;");
        }
    }
}

void MainWindow::onSearchMedicine()
{
    QLineEdit *edit = findChild<QLineEdit*>("lineEdit_search_medicine");
    if (!edit) return;
    QString text = edit->text().trimmed();
    
    if (text.isEmpty()) {
        m_medicineModel->setFilter("");
    } else {
        QString escaped = text;
        escaped.replace("'", "''");
        m_medicineModel->setFilter(QString("name LIKE '%%1%' OR category LIKE '%%1%'").arg(escaped));
    }
    m_medicineModel->select();
}

void MainWindow::onEditMedicine()
{
    QModelIndexList selection = ui->tableView_medicines->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的药品");
        return;
    }

    int row = selection.first().row();

    QSqlRecord record = m_medicineModel->record(row);
    int id = record.value("id").toInt();
    QString name = record.value("name").toString();
    QString category = record.value("category").toString();
    int stock = record.value("stock").toInt();
    double price = record.value("unit_price").toDouble();
    QDate expiry = record.value("expiry_date").toDate();

    QDialog dialog(this);
    dialog.setWindowTitle("编辑药品");
    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit(name, &dialog);
    QLineEdit *categoryEdit = new QLineEdit(category, &dialog);
    QSpinBox *stockSpin = new QSpinBox(&dialog);
    stockSpin->setRange(0, 10000);
    stockSpin->setValue(stock);
    QDoubleSpinBox *priceSpin = new QDoubleSpinBox(&dialog);
    priceSpin->setRange(0, 10000);
    priceSpin->setValue(price);
    QDateEdit *expiryEdit = new QDateEdit(expiry, &dialog);
    expiryEdit->setCalendarPopup(true);

    layout->addRow("药品名称:", nameEdit);
    layout->addRow("分类:", categoryEdit);
    layout->addRow("库存:", stockSpin);
    layout->addRow("单价:", priceSpin);
    layout->addRow("有效期:", expiryEdit);

    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addRow(box);
    connect(box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query(m_dbManager.database());
        query.prepare("UPDATE medicines SET name=?, category=?, stock=?, unit_price=?, expiry_date=? WHERE id=?");
        query.addBindValue(nameEdit->text());
        query.addBindValue(categoryEdit->text());
        query.addBindValue(stockSpin->value());
        query.addBindValue(priceSpin->value());
        query.addBindValue(expiryEdit->date().toString("yyyy-MM-dd"));
        query.addBindValue(id);

        if (query.exec()) {
            m_medicineModel->select();
            onCheckLowStock();
            QMessageBox::information(this, "成功", "药品信息已更新");
        } else {
            QMessageBox::warning(this, "失败", "更新失败: " + query.lastError().text());
        }
    }
}

void MainWindow::onDeleteMedicine()
{
    QModelIndexList selection = ui->tableView_medicines->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的药品");
        return;
    }

    int row = selection.first().row();
    QString name = m_medicineModel->data(m_medicineModel->index(row, 1)).toString();

    if (QMessageBox::question(this, "确认",
                              QString("确定要删除药品 '%1' 吗？").arg(name)) == QMessageBox::Yes) {
        m_medicineModel->removeRow(row);
        m_medicineModel->submitAll(); // 提交更改
        m_medicineModel->select();    // 刷新
        onCheckLowStock();
    }
}

void MainWindow::onMedicineContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->tableView_medicines->indexAt(pos);
    if (!index.isValid()) return;

    QMenu menu(this);
    menu.addAction("编辑药品", this, &MainWindow::onEditMedicine);
    menu.addAction("删除药品", this, &MainWindow::onDeleteMedicine);
    menu.exec(ui->tableView_medicines->mapToGlobal(pos));
}

void MainWindow::onEditAppointment()
{
    QModelIndexList selection = ui->tableView_appointments->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的预约");
        return;
    }

    int row = selection.first().row();

    QSqlRecord record = m_appointmentModel->record(row);
    int id = record.value("id").toInt();
    QDate date = record.value("appointment_date").toDate();
    QTime time = record.value("appointment_time").toTime();
    QString symptoms = record.value("symptoms").toString();
    QString status = record.value("status").toString();

    QDialog dialog(this);
    dialog.setWindowTitle("编辑预约");
    QFormLayout *layout = new QFormLayout(&dialog);

    QDateEdit *dateEdit = new QDateEdit(date, &dialog);
    dateEdit->setCalendarPopup(true);
    QTimeEdit *timeEdit = new QTimeEdit(time, &dialog);
    QTextEdit *symptomsEdit = new QTextEdit(symptoms, &dialog);
    symptomsEdit->setMaximumHeight(100);
    QComboBox *statusCombo = new QComboBox(&dialog);
    statusCombo->addItems({"预约中", "已确认", "已就诊", "已取消"});
    statusCombo->setCurrentText(status);

    layout->addRow("日期:", dateEdit);
    layout->addRow("时间:", timeEdit);
    layout->addRow("症状:", symptomsEdit);
    layout->addRow("状态:", statusCombo);

    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addRow(box);
    connect(box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query(m_dbManager.database());
        query.prepare("UPDATE appointments SET appointment_date=?, appointment_time=?, symptoms=?, status=? WHERE id=?");
        query.addBindValue(dateEdit->date().toString("yyyy-MM-dd"));
        query.addBindValue(timeEdit->time().toString("hh:mm:ss"));
        query.addBindValue(symptomsEdit->toPlainText());
        query.addBindValue(statusCombo->currentText());
        query.addBindValue(id);

        if (query.exec()) {
            m_appointmentModel->select();
            QMessageBox::information(this, "成功", "预约已更新");
        } else {
            QMessageBox::warning(this, "失败", "更新失败: " + query.lastError().text());
        }
    }
}

void MainWindow::onDeleteAppointment()
{
    QModelIndexList selection = ui->tableView_appointments->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的预约");
        return;
    }

    int row = selection.first().row();

    if (QMessageBox::question(this, "确认", "确定要删除该预约吗？") == QMessageBox::Yes) {
        m_appointmentModel->removeRow(row);
        m_appointmentModel->submitAll();
        m_appointmentModel->select();
    }
}

void MainWindow::onAppointmentContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->tableView_appointments->indexAt(pos);
    if (!index.isValid()) return;

    QMenu menu(this);
    menu.addAction("编辑预约", this, &MainWindow::onEditAppointment);
    menu.addAction("删除预约", this, &MainWindow::onDeleteAppointment);
    menu.exec(ui->tableView_appointments->mapToGlobal(pos));
}

// ==================== 统计分析功能 ====================
void MainWindow::onGenerateDailyReport()
{
    QDate today = QDate::currentDate();

    QSqlQuery query(m_dbManager.database());

    // 查询当日数据
    query.prepare("SELECT COUNT(*) FROM appointments WHERE appointment_date = ?");
    query.addBindValue(today.toString("yyyy-MM-dd"));
    query.exec();
    int appointments = 0;
    if (query.next()) {
        appointments = query.value(0).toInt();
    }

    query.exec("SELECT COUNT(*) FROM patients WHERE DATE(created_at) = DATE('now')");
    int newPatients = 0;
    if (query.next()) {
        newPatients = query.value(0).toInt();
    }

    query.exec("SELECT COUNT(*) FROM visits WHERE DATE(visit_date) = DATE('now')");
    int visits = 0;
    if (query.next()) {
        visits = query.value(0).toInt();
    }

    // 查询科室就诊量
    query.exec("SELECT d.department, COUNT(*) as visit_count "
               "FROM visits v "
               "JOIN doctors d ON v.doctor_id = d.id "
               "WHERE DATE(v.visit_date) = DATE('now') "
               "GROUP BY d.department");

    QString departmentStats;
    while (query.next()) {
        QString department = query.value(0).toString();
        int count = query.value(1).toInt();
        departmentStats += QString("  • %1: %2人次\n").arg(department).arg(count);
    }

    QString report = QString("=== 日报表 (%1) ===\n\n").arg(today.toString("yyyy-MM-dd"));
    report += QString("新增病人: %1人\n").arg(newPatients);
    report += QString("当日预约: %1个\n").arg(appointments);
    report += QString("就诊记录: %1个\n").arg(visits);

    if (!departmentStats.isEmpty()) {
        report += "\n科室就诊量:\n";
        report += departmentStats;
    }

    report += QString("\n生成时间: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss"));

    ui->textEdit_stats->setText(report);
    statusBar()->showMessage("日报生成完成", 2000);
}

void MainWindow::onGenerateMonthlyReport()
{
    QDate currentDate = QDate::currentDate();
    QString month = currentDate.toString("yyyy年MM月");
    int year = currentDate.year();
    int monthNum = currentDate.month();

    QString report = QString("=== 月报表 (%1) ===\n\n").arg(month);

    QSqlQuery query(m_dbManager.database());

    // 月度病人增长
    query.prepare("SELECT COUNT(*) FROM patients "
                  "WHERE strftime('%Y', created_at) = ? AND strftime('%m', created_at) = ?");
    query.addBindValue(QString::number(year));
    query.addBindValue(QString("%1").arg(monthNum, 2, 10, QChar('0')));
    query.exec();

    int monthlyPatients = 0;
    if (query.next()) {
        monthlyPatients = query.value(0).toInt();
    }
    report += QString("月度新增病人: %1人\n").arg(monthlyPatients);

    // 月度预约统计
    query.prepare("SELECT status, COUNT(*) FROM appointments "
                  "WHERE strftime('%Y', appointment_date) = ? AND strftime('%m', appointment_date) = ? "
                  "GROUP BY status");
    query.addBindValue(QString::number(year));
    query.addBindValue(QString("%1").arg(monthNum, 2, 10, QChar('0')));
    query.exec();

    report += "\n月度预约状态:\n";
    while (query.next()) {
        QString status = query.value(0).toString();
        int count = query.value(1).toInt();
        report += QString("  • %1: %2个\n").arg(status).arg(count);
    }

    // 药品使用情况
    query.exec("SELECT name, stock FROM medicines WHERE stock < 20 ORDER BY stock ASC");

    report += "\n药品库存预警:\n";
    while (query.next()) {
        QString name = query.value(0).toString();
        int stock = query.value(1).toInt();
        report += QString("  • %1: 库存%2\n").arg(name).arg(stock);
    }

    report += QString("\n生成时间: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    ui->textEdit_stats->setText(report);
    statusBar()->showMessage("月报生成完成", 2000);
}

void MainWindow::updateStatistics()
{
    QSqlQuery query(m_dbManager.database());

    query.exec("SELECT COUNT(*) FROM patients");
    int totalPatients = 0;
    if (query.next()) totalPatients = query.value(0).toInt();

    query.exec("SELECT COUNT(*) FROM doctors WHERE is_active = 1");
    int activeDoctors = 0;
    if (query.next()) activeDoctors = query.value(0).toInt();

    query.exec("SELECT COUNT(*) FROM appointments WHERE status = '预约中'");
    int pendingAppointments = 0;
    if (query.next()) pendingAppointments = query.value(0).toInt();

    query.exec("SELECT COUNT(*) FROM medicines WHERE stock < 10");
    int lowStockMedicines = 0;
    if (query.next()) lowStockMedicines = query.value(0).toInt();

    QString stats = QString("系统概况:\n\n");
    stats += QString("• 病人总数: %1人\n").arg(totalPatients);
    stats += QString("• 在职医生: %1人\n").arg(activeDoctors);
    stats += QString("• 待处理预约: %1个\n").arg(pendingAppointments);
    stats += QString("• 库存预警药品: %1种\n").arg(lowStockMedicines);
    stats += QString("• 系统状态: 正常\n");
    stats += QString("• 数据库: %1").arg(m_dbManager.databasePath());

    // 更新到统计标签页
    if (ui->tabWidget->currentWidget() == ui->tab_statistics) {
        ui->textEdit_stats->setText(stats);
    }
}

// ==================== 表格选择变化 ====================
void MainWindow::onPatientSelectionChanged()
{
    QModelIndexList selection = ui->tableView_patients->selectionModel()->selectedRows();
    ui->btn_edit_patient->setEnabled(!selection.isEmpty());
    ui->btn_delete_patient->setEnabled(!selection.isEmpty());
}

void MainWindow::onDoctorSelectionChanged()
{
    QModelIndexList selection = ui->tableView_doctors->selectionModel()->selectedRows();
    bool hasSelection = !selection.isEmpty();
    ui->btn_edit_doctor->setEnabled(hasSelection);
    ui->btn_delete_doctor->setEnabled(hasSelection);
}

void MainWindow::onAppointmentSelectionChanged()
{
    QModelIndexList selection = ui->tableView_appointments->selectionModel()->selectedRows();
    bool hasSelection = !selection.isEmpty();
    ui->btn_edit_appointment->setEnabled(hasSelection);
    ui->btn_delete_appointment->setEnabled(hasSelection);
}

void MainWindow::onMedicineSelectionChanged()
{
    QModelIndexList selection = ui->tableView_medicines->selectionModel()->selectedRows();
    bool hasSelection = !selection.isEmpty();
    ui->btn_edit_medicine->setEnabled(hasSelection);
    ui->btn_delete_medicine->setEnabled(hasSelection);
}

// ==================== 菜单栏功能 ====================
void MainWindow::onExitAction()
{
    close();
}

void MainWindow::onAboutAction()
{
    QMessageBox::about(this, "关于社区医疗信息管理系统",
                       "<h3>社区医疗信息管理系统 v1.0</h3>"
                       "<p>Qt课程设计项目</p>"
                       "<p>功能模块:</p>"
                       "<ul>"
                       "<li>病人信息管理</li>"
                       "<li>医生与科室管理</li>"
                       "<li>预约挂号管理</li>"
                       "<li>药品库存管理</li>"
                       "<li>统计分析报表</li>"
                       "</ul>"
                       "<p>技术支持: Qt 6.10.1 + SQLite</p>"
                       "<p>数据库路径: " + m_dbManager.databasePath() + "</p>");
}

void MainWindow::onExportAction()
{
    onExportDataClicked();
}

void MainWindow::onBackupAction()
{
    QString fileName = QFileDialog::getSaveFileName(this, "备份数据库",
                                                    "medical_backup.db",
                                                    "数据库文件 (*.db)");

    if (!fileName.isEmpty()) {
        if (QFile::copy(m_dbManager.databasePath(), fileName)) {
            QMessageBox::information(this, "成功", "数据库备份成功！");
        } else {
            QMessageBox::warning(this, "失败", "备份失败");
        }
    }
}

void MainWindow::onRestoreAction()
{
    QString fileName = QFileDialog::getOpenFileName(this, "恢复数据库",
                                                    "", "数据库文件 (*.db)");

    if (!fileName.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认恢复",
                                      "恢复数据库将覆盖当前数据，确定要继续吗？",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (QFile::copy(fileName, m_dbManager.databasePath())) {
                QMessageBox::information(this, "成功",
                                         "数据库恢复成功！\n请重启应用加载数据。");
            } else {
                QMessageBox::warning(this, "失败", "恢复失败");
            }
        }
    }
}

void MainWindow::onSyncAction()
{
    statusBar()->showMessage("同步功能开发中...", 2000);
    QMessageBox::information(this, "同步数据",
                             "这里可以实现数据同步功能:\n"
                             "1. 从服务器获取最新药品信息\n"
                             "2. 上传就诊记录\n"
                             "3. 同步预约信息");
}
