#pragma once

#include <QDialog>
#include "ui_CWFID.h"

class CWFID : public QDialog
{
    Q_OBJECT

public:
    CWFID(int ID, QString Lab, QString Machine, QString dbPath, QWidget* parent = Q_NULLPTR);
    ~CWFID();
    QString m_dbPath;
    int m_ID;
private:
    Ui::CWFID ui;
    void accept()override;
};
