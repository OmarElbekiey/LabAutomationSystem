#include "CWFID.h"
#include "DBUtils.h"
#include "QMessageBox"
CWFID::CWFID(int ID, QString Lab, QString Machine, QString dbPath, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    fillComboBox(ui.comboBox_lab, { Lab }, false);
    fillComboBox(ui.comboBox_Machine, { Machine }, false);
    m_dbPath = dbPath;
    m_ID = ID;

}

CWFID::~CWFID()
{
}
void CWFID::accept()
{
    if (ui.textEdit->toPlainText().isEmpty())
    {
        QMessageBox::warning(this, "Lab Automation", "Please insert work  description");
    }
    CompleteWork(m_ID, { ui.lineEdit->text(), ui.lineEdit_2->text(), ui.textEdit->toPlainText() ,QString::number(ui.doubleSpinBox_labor->value()) , QString::number(ui.doubleSpinBox_Material->value()) }, m_dbPath);
    QDialog::accept();

}
