/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_exit;
    QAction *action_about;
    QAction *action_export;
    QAction *action_backup;
    QAction *action_restore;
    QAction *action_sync;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_patient;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_search;
    QLabel *label_search;
    QLineEdit *lineEdit_search;
    QPushButton *btn_search;
    QPushButton *btn_add_patient;
    QPushButton *btn_edit_patient;
    QPushButton *btn_delete_patient;
    QSpacerItem *horizontalSpacer;
    QTableView *tableView_patients;
    QHBoxLayout *horizontalLayout_stats;
    QLabel *label_stats;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_export;
    QWidget *tab_doctor;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_add_doctor;
    QPushButton *btn_doctor_stats;
    QPushButton *btn_edit_doctor;
    QPushButton *btn_delete_doctor;
    QSpacerItem *horizontalSpacer_3;
    QTableView *tableView_doctors;
    QWidget *tab_appointment;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btn_new_appointment;
    QPushButton *btn_edit_appointment;
    QPushButton *btn_delete_appointment;
    QComboBox *comboBox_status;
    QDateEdit *dateEdit_appointment;
    QSpacerItem *horizontalSpacer_4;
    QTableView *tableView_appointments;
    QWidget *tab_medicine;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_low_stock;
    QPushButton *btn_add_medicine;
    QPushButton *btn_edit_medicine;
    QPushButton *btn_delete_medicine;
    QSpacerItem *horizontalSpacer_5;
    QTableView *tableView_medicines;
    QWidget *tab_statistics;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_6;
    QTextEdit *textEdit_stats;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btn_generate_report;
    QPushButton *btn_monthly_report;
    QSpacerItem *horizontalSpacer_7;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        action_exit = new QAction(MainWindow);
        action_exit->setObjectName("action_exit");
        action_about = new QAction(MainWindow);
        action_about->setObjectName("action_about");
        action_export = new QAction(MainWindow);
        action_export->setObjectName("action_export");
        action_backup = new QAction(MainWindow);
        action_backup->setObjectName("action_backup");
        action_restore = new QAction(MainWindow);
        action_restore->setObjectName("action_restore");
        action_sync = new QAction(MainWindow);
        action_sync->setObjectName("action_sync");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab_patient = new QWidget();
        tab_patient->setObjectName("tab_patient");
        verticalLayout_2 = new QVBoxLayout(tab_patient);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_search = new QHBoxLayout();
        horizontalLayout_search->setObjectName("horizontalLayout_search");
        label_search = new QLabel(tab_patient);
        label_search->setObjectName("label_search");

        horizontalLayout_search->addWidget(label_search);

        lineEdit_search = new QLineEdit(tab_patient);
        lineEdit_search->setObjectName("lineEdit_search");

        horizontalLayout_search->addWidget(lineEdit_search);

        btn_search = new QPushButton(tab_patient);
        btn_search->setObjectName("btn_search");

        horizontalLayout_search->addWidget(btn_search);

        btn_add_patient = new QPushButton(tab_patient);
        btn_add_patient->setObjectName("btn_add_patient");

        horizontalLayout_search->addWidget(btn_add_patient);

        btn_edit_patient = new QPushButton(tab_patient);
        btn_edit_patient->setObjectName("btn_edit_patient");

        horizontalLayout_search->addWidget(btn_edit_patient);

        btn_delete_patient = new QPushButton(tab_patient);
        btn_delete_patient->setObjectName("btn_delete_patient");

        horizontalLayout_search->addWidget(btn_delete_patient);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_search->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_search);

        tableView_patients = new QTableView(tab_patient);
        tableView_patients->setObjectName("tableView_patients");
        tableView_patients->setAlternatingRowColors(true);
        tableView_patients->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView_patients->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_2->addWidget(tableView_patients);

        horizontalLayout_stats = new QHBoxLayout();
        horizontalLayout_stats->setObjectName("horizontalLayout_stats");
        label_stats = new QLabel(tab_patient);
        label_stats->setObjectName("label_stats");

        horizontalLayout_stats->addWidget(label_stats);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_stats->addItem(horizontalSpacer_2);

        btn_export = new QPushButton(tab_patient);
        btn_export->setObjectName("btn_export");

        horizontalLayout_stats->addWidget(btn_export);


        verticalLayout_2->addLayout(horizontalLayout_stats);

        tabWidget->addTab(tab_patient, QString());
        tab_doctor = new QWidget();
        tab_doctor->setObjectName("tab_doctor");
        verticalLayout_3 = new QVBoxLayout(tab_doctor);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        btn_add_doctor = new QPushButton(tab_doctor);
        btn_add_doctor->setObjectName("btn_add_doctor");

        horizontalLayout_3->addWidget(btn_add_doctor);

        btn_doctor_stats = new QPushButton(tab_doctor);
        btn_doctor_stats->setObjectName("btn_doctor_stats");

        horizontalLayout_3->addWidget(btn_doctor_stats);

        btn_edit_doctor = new QPushButton(tab_doctor);
        btn_edit_doctor->setObjectName("btn_edit_doctor");

        horizontalLayout_3->addWidget(btn_edit_doctor);

        btn_delete_doctor = new QPushButton(tab_doctor);
        btn_delete_doctor->setObjectName("btn_delete_doctor");

        horizontalLayout_3->addWidget(btn_delete_doctor);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_3);

        tableView_doctors = new QTableView(tab_doctor);
        tableView_doctors->setObjectName("tableView_doctors");

        verticalLayout_3->addWidget(tableView_doctors);

        tabWidget->addTab(tab_doctor, QString());
        tab_appointment = new QWidget();
        tab_appointment->setObjectName("tab_appointment");
        verticalLayout_4 = new QVBoxLayout(tab_appointment);
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        btn_new_appointment = new QPushButton(tab_appointment);
        btn_new_appointment->setObjectName("btn_new_appointment");

        horizontalLayout_4->addWidget(btn_new_appointment);

        btn_edit_appointment = new QPushButton(tab_appointment);
        btn_edit_appointment->setObjectName("btn_edit_appointment");

        horizontalLayout_4->addWidget(btn_edit_appointment);

        btn_delete_appointment = new QPushButton(tab_appointment);
        btn_delete_appointment->setObjectName("btn_delete_appointment");

        horizontalLayout_4->addWidget(btn_delete_appointment);

        comboBox_status = new QComboBox(tab_appointment);
        comboBox_status->addItem(QString());
        comboBox_status->addItem(QString());
        comboBox_status->addItem(QString());
        comboBox_status->addItem(QString());
        comboBox_status->addItem(QString());
        comboBox_status->setObjectName("comboBox_status");

        horizontalLayout_4->addWidget(comboBox_status);

        dateEdit_appointment = new QDateEdit(tab_appointment);
        dateEdit_appointment->setObjectName("dateEdit_appointment");
        dateEdit_appointment->setCalendarPopup(true);

        horizontalLayout_4->addWidget(dateEdit_appointment);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout_4->addLayout(horizontalLayout_4);

        tableView_appointments = new QTableView(tab_appointment);
        tableView_appointments->setObjectName("tableView_appointments");

        verticalLayout_4->addWidget(tableView_appointments);

        tabWidget->addTab(tab_appointment, QString());
        tab_medicine = new QWidget();
        tab_medicine->setObjectName("tab_medicine");
        verticalLayout_5 = new QVBoxLayout(tab_medicine);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_low_stock = new QLabel(tab_medicine);
        label_low_stock->setObjectName("label_low_stock");

        horizontalLayout_5->addWidget(label_low_stock);

        btn_add_medicine = new QPushButton(tab_medicine);
        btn_add_medicine->setObjectName("btn_add_medicine");

        horizontalLayout_5->addWidget(btn_add_medicine);

        btn_edit_medicine = new QPushButton(tab_medicine);
        btn_edit_medicine->setObjectName("btn_edit_medicine");

        horizontalLayout_5->addWidget(btn_edit_medicine);

        btn_delete_medicine = new QPushButton(tab_medicine);
        btn_delete_medicine->setObjectName("btn_delete_medicine");

        horizontalLayout_5->addWidget(btn_delete_medicine);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout_5->addLayout(horizontalLayout_5);

        tableView_medicines = new QTableView(tab_medicine);
        tableView_medicines->setObjectName("tableView_medicines");

        verticalLayout_5->addWidget(tableView_medicines);

        tabWidget->addTab(tab_medicine, QString());
        tab_statistics = new QWidget();
        tab_statistics->setObjectName("tab_statistics");
        verticalLayout_6 = new QVBoxLayout(tab_statistics);
        verticalLayout_6->setObjectName("verticalLayout_6");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_2 = new QLabel(tab_statistics);
        label_2->setObjectName("label_2");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label_2->setFont(font);

        horizontalLayout_6->addWidget(label_2);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout_6->addLayout(horizontalLayout_6);

        textEdit_stats = new QTextEdit(tab_statistics);
        textEdit_stats->setObjectName("textEdit_stats");
        textEdit_stats->setReadOnly(true);

        verticalLayout_6->addWidget(textEdit_stats);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        btn_generate_report = new QPushButton(tab_statistics);
        btn_generate_report->setObjectName("btn_generate_report");

        horizontalLayout_7->addWidget(btn_generate_report);

        btn_monthly_report = new QPushButton(tab_statistics);
        btn_monthly_report->setObjectName("btn_monthly_report");

        horizontalLayout_7->addWidget(btn_monthly_report);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);


        verticalLayout_6->addLayout(horizontalLayout_7);

        tabWidget->addTab(tab_statistics, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action_exit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272(&X)", nullptr));
#if QT_CONFIG(shortcut)
        action_exit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        action_about->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216(&A)", nullptr));
        action_export->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\346\225\260\346\215\256(&E)", nullptr));
#if QT_CONFIG(shortcut)
        action_export->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        action_backup->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\345\244\207\344\273\275(&B)", nullptr));
        action_restore->setText(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\346\201\242\345\244\215(&R)", nullptr));
        action_sync->setText(QCoreApplication::translate("MainWindow", "\345\220\214\346\255\245\346\225\260\346\215\256(&S)", nullptr));
        label_search->setText(QCoreApplication::translate("MainWindow", "\346\220\234\347\264\242:", nullptr));
        lineEdit_search->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\345\247\223\345\220\215\343\200\201\347\224\265\350\257\235\346\210\226\347\227\205\345\216\206\345\217\267", nullptr));
        btn_search->setText(QCoreApplication::translate("MainWindow", "\346\220\234\347\264\242", nullptr));
        btn_add_patient->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\347\227\205\344\272\272", nullptr));
        btn_edit_patient->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221", nullptr));
        btn_delete_patient->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
        label_stats->setText(QCoreApplication::translate("MainWindow", "\347\227\205\344\272\272\346\200\273\346\225\260: 0", nullptr));
        btn_export->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\346\225\260\346\215\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_patient), QCoreApplication::translate("MainWindow", "\347\227\205\344\272\272\347\256\241\347\220\206", nullptr));
        btn_add_doctor->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\345\214\273\347\224\237", nullptr));
        btn_doctor_stats->setText(QCoreApplication::translate("MainWindow", "\347\247\221\345\256\244\347\273\237\350\256\241", nullptr));
        btn_edit_doctor->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221", nullptr));
        btn_delete_doctor->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_doctor), QCoreApplication::translate("MainWindow", "\345\214\273\347\224\237\347\256\241\347\220\206", nullptr));
        btn_new_appointment->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272\351\242\204\347\272\246", nullptr));
        btn_edit_appointment->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221", nullptr));
        btn_delete_appointment->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
        comboBox_status->setItemText(0, QCoreApplication::translate("MainWindow", "\346\211\200\346\234\211\347\212\266\346\200\201", nullptr));
        comboBox_status->setItemText(1, QCoreApplication::translate("MainWindow", "\351\242\204\347\272\246\344\270\255", nullptr));
        comboBox_status->setItemText(2, QCoreApplication::translate("MainWindow", "\345\267\262\347\241\256\350\256\244", nullptr));
        comboBox_status->setItemText(3, QCoreApplication::translate("MainWindow", "\345\267\262\345\260\261\350\257\212", nullptr));
        comboBox_status->setItemText(4, QCoreApplication::translate("MainWindow", "\345\267\262\345\217\226\346\266\210", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_appointment), QCoreApplication::translate("MainWindow", "\351\242\204\347\272\246\347\256\241\347\220\206", nullptr));
        label_low_stock->setText(QCoreApplication::translate("MainWindow", "\345\272\223\345\255\230\351\242\204\350\255\246: 0\347\247\215\350\215\257\345\223\201\345\272\223\345\255\230\344\270\215\350\266\263", nullptr));
        btn_add_medicine->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\350\215\257\345\223\201", nullptr));
        btn_edit_medicine->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221", nullptr));
        btn_delete_medicine->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_medicine), QCoreApplication::translate("MainWindow", "\350\215\257\345\223\201\345\272\223\345\255\230", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\347\273\237\350\256\241\346\212\245\350\241\250", nullptr));
        btn_generate_report->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\346\227\245\346\212\245", nullptr));
        btn_monthly_report->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\346\234\210\346\212\245", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_statistics), QCoreApplication::translate("MainWindow", "\347\273\237\350\256\241\345\210\206\346\236\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
