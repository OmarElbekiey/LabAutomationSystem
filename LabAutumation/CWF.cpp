#include "CWF.h"
#include "DBUtils.h"
CWF::CWF(QString dbPath, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_dbPath = dbPath;
    fillComboBox(ui.comboBox_lab, getLabs(m_dbPath), false);
    ui.radioButton->setChecked(true);
}

CWF::~CWF()
{
}
void CWF::accept()
{
    QStringList items;
    QString cause = ui.radioButton->isChecked() ? ui.radioButton->text() : ui.radioButton_2->text();
    items << ui.comboBox_lab->currentText() << ui.comboBox_Machine->currentText() << cause;
    items << ui.lineEdit->text() << ui.lineEdit_2->text() << ui.textEdit->toPlainText() << ui.lineEdit->text() << ui.lineEdit_2->text() << ui.textEdit->toPlainText();
    items<<QString::number(ui.doubleSpinBox_labor->value())<< QString::number(ui.doubleSpinBox_Material->value());
    createCompleteWork(items, m_dbPath);
    QDialog::accept();

}
void CWF::on_comboBox_lab_currentTextChanged(QString txt)
{
    if (txt.isEmpty())
    {
        ui.comboBox_Machine->clear();
        return;
    }
    fillComboBox(ui.comboBox_Machine, getMachines(ui.comboBox_lab->currentText(), m_dbPath), false);

}