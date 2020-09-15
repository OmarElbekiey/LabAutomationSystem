#pragma once

#include <QDialog>
#include "ui_MaintenanceRequestForm.h"

class MaintenanceRequestForm : public QDialog
{
    Q_OBJECT

public:
    MaintenanceRequestForm(QString dbPath, QWidget *parent = Q_NULLPTR);
    ~MaintenanceRequestForm();

    QString m_dbPath;
private:
    Ui::MaintenanceRequestForm ui;
    void accept()override;
public slots:
    void on_comboBox_lab_currentTextChanged(QString txt);

};
