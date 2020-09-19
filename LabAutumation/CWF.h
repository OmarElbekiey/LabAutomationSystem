#pragma once

#include <QDialog>
#include "ui_CWF.h"

class CWF : public QDialog
{
    Q_OBJECT

public:
    CWF(QString dbPath, QWidget* parent = Q_NULLPTR);
    ~CWF();
    QString m_dbPath;

private:
    Ui::CWF ui;
    void accept()override;
public slots:
    void on_comboBox_lab_currentTextChanged(QString txt);
};
