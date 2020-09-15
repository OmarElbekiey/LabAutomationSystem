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
#include"DBUtils.h"
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
    m_dbPath = "D:\\Atef\\Codes Snippites";

    if (!CreateSQLiteDatabase(m_dbPath))
    {
        QMessageBox::critical(this, "Fatal Error", "Could not load Data");
        this->close();
    }
    QSqlDatabase db;
    if (!QSqlDatabase::contains("LabAutomation"))
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
        db.setDatabaseName(m_dbPath + "/LabAutomation.db");
    }
    else
        db = QSqlDatabase::database("LabAutomation");
    QDir appDir(m_dbPath);
    appDir.mkpath(m_dbPath);

    //initGUI();

   /* if (!CreateSQLiteDatabase(m_dbPath))
    {
        QMessageBox::critical(this, "Fatal Error", "Could not load Data");
        this->close();
    }*/
  
    m_pTableModelSchedule = new QSqlTableModel(this, QSqlDatabase::database("LabAutomation"));
    m_pTableModelWorkOrders = new QSqlTableModel(this, QSqlDatabase::database("LabAutomation"));
    QString as = m_pTableModelSchedule->lastError().text();

    m_pTableModelSchedule->setTable("Machines");
    m_pTableModelWorkOrders->setTable("WorkOrders");
    m_pTableModelWorkOrders->setFilter("\"Request State\" = 'To Do'");
    m_pTableModelWorkOrders->select();
    m_pTableModelSchedule->select();

    ui.tableView->setModel(m_pTableModelWorkOrders);
    ui.tableView_machines->setModel(m_pTableModelSchedule);
    ui.tableView_machines->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableModelSchedule->insertRow(m_pTableModelSchedule->rowCount(QModelIndex()));

    fillComboBox(ui.comboBox_labs, getLabs(m_dbPath),true);
    setInfo({});
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
void GUI::on_comboBox_labs_currentTextChanged(QString txt)
{
    if (txt.isEmpty())
    {
        ui.comboBox_Machines->clear();

        return;
    }
    fillComboBox(ui.comboBox_Machines, getMachines(ui.comboBox_labs->currentText(),m_dbPath),true);

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

    QModelIndexList selected = select->selectedRows(); // return selected row(s)
    if (!select->selectedRows().isEmpty())
    {
        int r = selected[0].row();
        m_pTableModelSchedule->removeRows(r, 1);
        QString er = m_pTableModelSchedule->lastError().text();
        m_pTableModelSchedule->select();
        er = m_pTableModelSchedule->lastError().text();
    }

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

}
void GUI::on_actionEdit_Info_triggered()
{
    QString pass = QInputDialog::getText(this, "Password", "Enter Password", QLineEdit::Password);
    if (pass != "guc123")
        return;
    ui.stackedWidget->setCurrentIndex(1);
}

void GUI::on_toolButton_MR_pressed()
{
    MaintenanceRequestForm form(m_dbPath, this);
    form.exec();
    //m_pTableModelWorkOrders->select();
}

