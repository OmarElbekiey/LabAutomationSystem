#include "MaintenanceRequestForm.h"
#include "DBUtils.h"

MaintenanceRequestForm::MaintenanceRequestForm(QString dbPath, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_dbPath = dbPath;
    fillComboBox(ui.comboBox_lab, getLabs(m_dbPath), false);
    ui.radioButton->setChecked(true);
}

MaintenanceRequestForm::~MaintenanceRequestForm()
{
}

void MaintenanceRequestForm::on_comboBox_lab_currentTextChanged(QString txt)
{
    if (txt.isEmpty())
    {
        ui.comboBox_Machine->clear();
        return;
    }
    fillComboBox(ui.comboBox_Machine, getMachines(ui.comboBox_lab->currentText(), m_dbPath), false);

}

void MaintenanceRequestForm::accept()
{
    if (!openDB(m_dbPath))
    {
        return;
    }
    QSqlDatabase db;
    //if (!QSqlDatabase::contains("LabAutomation"))
    //{
    //    db = QSqlDatabase::addDatabase("QSQLITE", "LabAutomation");
    //    db.setDatabaseName(m_dbPath + "/LabAutomation.db");
    //}
    //else
    //    db = QSqlDatabase::database("LabAutomation");
    QSqlQuery query(QSqlDatabase::database("LabAutomation"));
    QString err = query.lastError().text();
    query.prepare("INSERT INTO WorkOrders (\"Lab Name\", \"Machine Model\", Cause, \"Request Owner\", \"Request Owner Email\",\"Requested Work Description\", \"Request Time\",\"Request State\")"
        "VALUES (:LN, :MM, :C, :RO, :ROE, :ROD, :RT, :RS)");
    err = query.lastError().text();
    // Bind Patient info Values
    query.bindValue(":LN", ui.comboBox_lab->currentText());
    query.bindValue(":MM", ui.comboBox_Machine->currentText());
    if (ui.radioButton->isChecked())
        query.bindValue(":C", ui.radioButton->text());
    else
        query.bindValue(":C", ui.radioButton_2->text());
    query.bindValue(":RO", ui.lineEdit->text());
    query.bindValue(":ROE", ui.lineEdit_2->text());
    query.bindValue(":ROD", ui.textEdit->toPlainText());
    query.bindValue(":RT", QDateTime::currentDateTime().toString());
    query.bindValue(":RS", "To Do");
    bool success = query.exec();
    QString sss = db.lastError().text();
    QDialog::accept();
    /*     query.bindValue(":G", "F");
     query.bindValue(":A", p.Adress);
     query.bindValue(":J", p.Job);
     query.bindValue(":BD", p.BD);
     query.bindValue(":C", p.Company);
     query.bindValue(":CN", p.Card);
     query.bindValue(":M", p.MobileNumber);
     query.bindValue(":dt", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
     bool success = query.exec();
     err = query.lastError().text();
     bool ok;
     int PID = query.lastInsertId().toInt(&ok);
     if (ok)
         return PID;
     return -1;*/
}
