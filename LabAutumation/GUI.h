#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GUI.h"
#include <QSqlTableModel>
class GUI : public QMainWindow
{
    Q_OBJECT

public:
    GUI(QWidget* parent = Q_NULLPTR);
    QString m_dbPath;
    QSqlTableModel* m_pTableModelSchedule;
    QSqlTableModel* m_pTableModelWorkOrders;
    QStringList getMachineInfo(QString Lab, QString Machine);
    void setInfo(const QStringList& info);

private:
    Ui::GUIClass ui;
public slots:
    void on_toolButton_new_pressed();
    void on_comboBox_labs_currentTextChanged(QString txt);
    void on_comboBox_Machines_currentTextChanged(QString txt);
    void on_toolButton_Remove_pressed();
    void on_toolButton_Home_pressed();
    void on_actionEdit_Info_triggered();
    void on_toolButton_MR_pressed();
    void on_toolButton_MW_pressed();
    void on_tableView_doubleClicked(const QModelIndex& index);
    //void on_toolButton_ChooseImage_pressed();
};
