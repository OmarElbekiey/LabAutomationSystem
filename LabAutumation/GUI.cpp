#include "GUI.h"
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QDir>
#include <QMessageBox>
#include <QObject>
#include <QCompleter>
#include <QCoreApplication>
#include <QImage>
#include <QFileDialog>
#include <QItemSelectionModel>
#include <QInputDialog>
#include <QAbstractItemModel>
#include "MaintenanceRequestForm.h"
#include "DBUtils.h"
#include "CWF.h"
#include "CWFID.h"
#include <QSettings>
static void UpdateComboBoxWithCompleter(QComboBox* b, QStringList l, QWidget* parent)
{
    const QString Current_Text = b->currentText();
    b->clear();
    b->insertItems(0, l);
    if (!Current_Text.isEmpty() && l.size() > 0)
        b->setCurrentText(Current_Text);
    if (!(l.contains(Current_Text)) && l.size() > 0)
        b->setCurrentText(l.at(0));
    QCompleter* c = new QCompleter(l, parent);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    b->setCompleter(c);
}
static void renderQImage(QGraphicsView* pViewer, QImage* pImage)
{
    if (!pViewer || !pImage)
    {
        return;
    }
    QGraphicsScene* scene = pViewer->scene();
    if (!scene)
    {
        scene = new QGraphicsScene(pViewer);
        scene->setSceneRect(pImage->rect());
        pViewer->setScene(scene);
    }
    //scene->clear(); // This will clear any previous objects.
    scene->addPixmap(QPixmap::fromImage(*pImage, Qt::MonoOnly | Qt::OrderedDither | Qt::OrderedAlphaDither | Qt::NoFormatConversion));

    pViewer->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    pViewer->show();
}

GUI::GUI(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDir(m_dbPath);
    appDir.mkpath(m_dbPath);

    if (!CreateSQLiteDatabase(m_dbPath))
    {
        QMessageBox::critical(this, "Fatal Error", "Could not load Data");
        this->close();
    }
    QSqlDatabase db;
    if (!QSqlDatabase::contains("LabAutomation"))
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "LabAutomation");
        db.setDatabaseName(m_dbPath + "/LabAutomation.db");
    }
    else
        db = QSqlDatabase::database("LabAutomation");
    QPixmap logo(":/GUI/Resources/Logo.jpg");
    ui.Logo->setPixmap(logo.scaled(276, 200, Qt::KeepAspectRatio));
    ui.Logo->setScaledContents(true);
    //initGUI();

   /* if (!CreateSQLiteDatabase(m_dbPath))
    {
        QMessageBox::critical(this, "Fatal Error", "Could not load Data");
        this->close();
    }*/

    InitializeTables();
}
void GUI::InitializeTables()
{
    m_pTableModelSchedule = new QSqlTableModel(this, QSqlDatabase::database("LabAutomation"));
    QString as = m_pTableModelSchedule->lastError().text();
    m_pTableModelSchedule->setTable("Machines");
    m_pTableModelSchedule->select();
    ui.tableView_machines->setModel(m_pTableModelSchedule);
    // ui.tableView_machines->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_pTableModelWorkOrders = new QSqlTableModel(this, QSqlDatabase::database("LabAutomation"));
    m_pTableModelWorkOrders->setTable("WorkOrders");
    m_pTableModelWorkOrders->setFilter("\"Request State\" = 'To Do'");
    m_pTableModelWorkOrders->select();
    ui.tableView->setModel(m_pTableModelWorkOrders);
    //  ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    fillComboBox(ui.comboBox_labs, getLabs(m_dbPath), true);
    setInfo({});

    m_pTableModelWorkOrders->removeColumn(9);
    m_pTableModelWorkOrders->removeColumn(9);
    m_pTableModelWorkOrders->removeColumn(9);
    m_pTableModelWorkOrders->removeColumn(9);
    m_pTableModelWorkOrders->removeColumn(9);
    m_pTableModelWorkOrders->removeColumn(9);


    m_pCompleteTableModelWorkOrders = new QSqlTableModel(this, QSqlDatabase::database("LabAutomation"));
    m_pCompleteTableModelWorkOrders->setTable("WorkOrders");
    m_pCompleteTableModelWorkOrders->select();
    ui.tableView_machines_WorkOrders->setModel(m_pCompleteTableModelWorkOrders);
    ui.tableView_machines_WorkOrders->setVisible(false);

}

void GUI::on_toolButton_new_pressed()
{
    ui.tableView_machines->setDisabled(true);
    ui.tableView_machines->setDisabled(false);
    if (!m_pTableModelSchedule->submit())
    {
        QMessageBox::critical(this, "Error", "Please, Insert a Unique Lab Name and Machine model.");
        return;
    }

    m_pTableModelSchedule->insertRow(m_pTableModelSchedule->rowCount(QModelIndex()));
    QString as = m_pTableModelSchedule->lastError().text();

}

void GUI::on_toolButton_Submit_pressed()
{
    ui.tableView_machines->setDisabled(true);
    ui.tableView_machines->setDisabled(false);
    if (!m_pTableModelSchedule->submit())
    {
        QString as = m_pTableModelSchedule->lastError().text();
        QMessageBox::critical(this, "Error", "Please, Insert a Unique Lab Name and Machine model.");
        return;
    }
}

void GUI::on_comboBox_labs_currentTextChanged(QString txt)
{
    if (txt.isEmpty())
    {
        ui.comboBox_Machines->clear();

        return;
    }
    fillComboBox(ui.comboBox_Machines, getMachines(ui.comboBox_labs->currentText(), m_dbPath), true);

}
void GUI::on_comboBox_Machines_currentTextChanged(QString txt)
{
    if (txt.isEmpty())
    {
        setInfo({});
        return;
    }
    setInfo(getMachineInfo(ui.comboBox_labs->currentText(), ui.comboBox_Machines->currentText()));
}
void GUI::on_toolButton_Remove_pressed()
{
    ui.tableView_machines->setDisabled(true);
    ui.tableView_machines->setDisabled(false);
    QItemSelectionModel* select = ui.tableView_machines->selectionModel();

    QModelIndexList selected = select->selectedIndexes(); // return selected row(s)
    if (!selected.isEmpty())
    {
        int r = selected[0].row();
        m_pTableModelSchedule->removeRows(r, 1);
        QString er = m_pTableModelSchedule->lastError().text();
        m_pTableModelSchedule->select();
        er = m_pTableModelSchedule->lastError().text();
    }
    m_pTableModelSchedule->removeRow(m_pTableModelSchedule->rowCount(QModelIndex()));

}



QStringList GUI::getMachineInfo(QString Lab, QString Machine)
{
    QStringList MachineInfo;

    QSqlDatabase db;
    if (!QSqlDatabase::contains("LA"))
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "LabAutomation");
        db.setDatabaseName(m_dbPath + "/LabAutomation.db");
    }
    else
        db = QSqlDatabase::database("LabAutomation");

    QSqlQuery query(QSqlDatabase::database("LabAutomation"));

    QString err = query.lastError().text();
    query.prepare("SELECT * FROM Machines where \"Lab Name\" = :LN ANd \"Machine Model\" = :MM;");
    query.bindValue(":LN", Lab);
    query.bindValue(":MM", Machine);
    query.exec();
    err = query.lastError().text();

    while (query.next())
    {
        MachineInfo.push_back(query.value(0).toString());
        MachineInfo.push_back(query.value(1).toString());
        MachineInfo.push_back(query.value(2).toString());
        MachineInfo.push_back(query.value(3).toString());
        MachineInfo.push_back(query.value(4).toString());
        MachineInfo.push_back(query.value(5).toString());
        MachineInfo.push_back(query.value(6).toString());
        MachineInfo.push_back(query.value(7).toString());
        MachineInfo.push_back(query.value(8).toString());
        MachineInfo.push_back(query.value(9).toString());
        MachineInfo.push_back(query.value(10).toString());
    }
    return MachineInfo;
}

void GUI::setInfo(const QStringList& info)
{
    if (info.isEmpty())
    {
        ui.label_tchData->setText("");
        ui.label_asstPurpose->setText("");
        ui.label_timeInterval->setText("");
        ui.label_EstimatLife->setText("");
        ui.label_RoomID->setText("");
        ui.label_StaffInCharge->setText("");
        ui.label_MT->setText("");

        if (ui.graphicsView_Image->scene())
            ui.graphicsView_Image->scene()->clear();
        if (ui.graphicsView_QR->scene())
            ui.graphicsView_QR->scene()->clear();
        return;
    }
    ui.label_tchData->setText(info[2]);
    ui.label_asstPurpose->setText(info[3]);
    ui.label_timeInterval->setText(info[4]);
    ui.label_EstimatLife->setText(info[5]);
    ui.label_RoomID->setText(info[6]);
    ui.label_StaffInCharge->setText(info[7]);
    if (info[8].isEmpty() && ui.graphicsView_Image->scene())
        ui.graphicsView_Image->scene()->clear();
    else
        renderQImage(ui.graphicsView_Image, &QImage(info[8]));

    if (info[9].isEmpty() && ui.graphicsView_QR->scene())
        ui.graphicsView_QR->scene()->clear();
    else
        renderQImage(ui.graphicsView_QR, &QImage(info[9]));
    ui.label_MT->setText(info[10]);
}

//void GUI::on_toolButton_ChooseImage_pressed()
//{
//    QString inputFilename = QFileDialog::getOpenFileName(this, tr("select file"));
//    QItemSelectionModel* select = ui.tableView_machines->selectionModel();
//    if (inputFilename.isEmpty()|| select->selectedRows().isEmpty())
//        return;
//    QModelIndex row = select->selectedRows()[0];
//    QModelIndex id = m_pTableModelSchedule->index(row.row(), 8, QModelIndex());
//    ui.tableView_machines->edit(id);
//    m_pTableModelSchedule->setData(id, QVariant(inputFilename));  
//    m_pTableModelSchedule->select();
//}
void GUI::on_toolButton_Home_pressed()
{
    ui.stackedWidget->setCurrentIndex(0);
    fillComboBox(ui.comboBox_labs, getLabs(m_dbPath), true);
    setInfo({});

}

void GUI::on_actionChange_Password_triggered()
{
    QString pass = QInputDialog::getText(this, "Password", "Enter Password", QLineEdit::PasswordEchoOnEdit);
    QSettings mySettings("Settings.ini", QSettings::IniFormat);
    QString currPass = mySettings.value("Password").toString();
    if (pass != currPass)
    {
        QMessageBox::critical(this, "Error", "Invalid Password");
        return;
    }
    bool ok;
    QString text = QInputDialog::getText(NULL, QObject::tr("Please Set The Admin Password"),
        QObject::tr("Password:"), QLineEdit::EchoMode::PasswordEchoOnEdit,
        "", &ok);
    if (ok)
    {
        mySettings.setValue("Password", text);
    }
}

void GUI::on_actionEdit_Info_triggered()
{
    QString pass = QInputDialog::getText(this, "Password", "Enter Password", QLineEdit::Password);
    QSettings mySettings("Settings.ini", QSettings::IniFormat);
    QString settingPass = mySettings.value("Password").toString();
    if (pass != settingPass)
    {
        QMessageBox::critical(this, "Error", "Invalid Password");
        return;
    }

    ui.stackedWidget->setCurrentIndex(1);
}

void GUI::on_toolButton_MR_pressed()
{
    MaintenanceRequestForm form(m_dbPath, this);
    if (form.exec())
    {
        m_pTableModelWorkOrders->setFilter("\"Request State\" = 'To Do'");
        ui.tableView->setModel(m_pTableModelWorkOrders);
        m_pTableModelWorkOrders->select();
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
    }
}

void GUI::on_toolButton_MW_pressed()
{
    CWF form(m_dbPath, this);
    if (form.exec())
    {
        QMessageBox::information(this, "Success", "Work Saved Successfully.");
    }

}

void GUI::on_tableView_doubleClicked(const QModelIndex& index)
{
    int ID = ui.tableView->model()->data(ui.tableView->model()->index(index.row(), 0)).toInt();
    if (ID <= 0)
    {
        QMessageBox::warning(this, "Lab Automation", "Invalid Request");
        return;
    }
    CWFID form(ID, ui.tableView->model()->data(ui.tableView->model()->index(index.row(), 1)).toString(), ui.tableView->model()->data(ui.tableView->model()->index(index.row(), 2)).toString(), m_dbPath, this);
    if (form.exec())
    {
        m_pTableModelWorkOrders->setFilter("\"Request State\" = 'To Do'");
        ui.tableView->setModel(m_pTableModelWorkOrders);
        m_pTableModelWorkOrders->select();
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
        m_pTableModelWorkOrders->removeColumn(9);
    }
}

void GUI::on_radioButton_Machines_toggled(bool b)
{
    ui.tableView_machines->setVisible(b);
    ui.tableView_machines_WorkOrders->setHidden(b);
}

void GUI::on_radioButton_WorkOrders_toggled(bool b)
{
    ui.tableView_machines_WorkOrders->setVisible(b);
    ui.tableView_machines->setHidden(b);
}

void GUI::on_actionLoad_Data_triggered()
{
    QSettings mySettings("Settings.ini", QSettings::IniFormat);
    QString lastPath = mySettings.value("Last Loaded Data Path").toString();
    QString inputFilename = QFileDialog::getOpenFileName(this, tr("select file"), lastPath, "*.db");
    if (inputFilename == NULL)
        return;
    mySettings.setValue("Last Loaded Data Path", inputFilename);
    QFile file(inputFilename);
    if (!file.exists())
        return;
    int reply = QMessageBox::question(this, "Change Data", " Old data will be deleted, Are you sure you want to change the data?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
        return;
    if (QSqlDatabase::contains("LabAutomation"))
    {
        QSqlDatabase::database("LabAutomation").close();
        QSqlDatabase::removeDatabase("LabAutomation");
    }
    QFile oldfile(m_dbPath + "/LabAutomation.db");
    oldfile.remove();
    file.copy(m_dbPath + "/LabAutomation.db");
    QSqlDatabase db;
    if (!QSqlDatabase::contains("LabAutomation"))
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "LabAutomation");
        db.setDatabaseName(m_dbPath + "/LabAutomation.db");
    }
    else
        db = QSqlDatabase::database("LabAutomation");
    InitializeTables();
}