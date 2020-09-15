#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>

using namespace std;

static bool openDB(QString dpPath)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dpPath + "/LabAutomation.db");

    // connect to database
    if (!db.open())
    {
        QString ss = db.lastError().text();
        return false;
    }
    return true;

}

static bool CreateSQLiteDatabase(QString dbPath)
{
    // load database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath + "/LabAutomation.db");

    // connect to database
    if (!db.open())
    {
        QString ss = db.lastError().text();
        return false;
    }
    else
    {
        QSqlQuery query;
        bool ok = query.exec(
            "CREATE TABLE WorkOrders("
            "[Lab Name]                   VARCHAR NOT NULL, "
            "[Machine Model]              VARCHAR NOT NULL, "
            "Cause                        VARCHAR,          "
            "[Request Owner]            VARCHAR NOT NULL, "
            "[Request Owner Email]        VARCHAR,          "
            "[Requested Work Description] VARCHAR NOT NULL, "
            "[Request Time]               VARCHAR,          "
            "[Request State]              VARCHAR,          "
            "[Request Compeleter]         VARCHAR,          "
            "[Compeletion Time]           VARCHAR,          "
            "[Request Compeleter Email]   VARCHAR,          "
            "[Completed Work Description] VARCHAR,          "
            "[Cost Of Labor]              VARCHAR,          "
            "[Cost Of Material]           VARCHAR           "
            ");"
        );
        ok = query.exec(
            " CREATE TABLE Machines("
            "[Lab Name]                  NOT NULL,"
            "[Machine Model]     STRING  NOT NULL,"
            "[Technical Data]    STRING,          "
            "[Asset Purpose]     STRING,          "
            "[Time Interval]     STRING,          "
            "[Estimate Life]     STRING,          "
            "[Room ID]           STRING,          "
            "[StaffIn Charge]    STRING,          "
            "[Image Path]        STRING,          "
            "[QR Path]           STRING,          "
            "[Manitenance Tasks] VARCHAR,         "
            "PRIMARY KEY(                         "
            "    [Lab Name],                      "
            "    [Machine Model]                  "
            " ),                                  "
            "  UNIQUE("
            "  [Lab Name],"
            " [Machine Model]"
            "   )"
            "  )"
            "WITHOUT ROWID;");
        db.close();
        return true;
    }
}

static QStringList getLabs(QString dbPath)
{
    QStringList Labs;
    QSqlDatabase db;
    if (!QSqlDatabase::contains("LA"))
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "LabAutomation");
        db.setDatabaseName(dbPath + "/LabAutomation.db");
    }
    else
        db = QSqlDatabase::database("LabAutomation");

    QSqlQuery query(QSqlDatabase::database("LabAutomation"));

    QString err = query.lastError().text();
    query.prepare("SELECT DISTINCT \"Lab Name\" FROM Machines; ");
    query.exec();
    err = query.lastError().text();


    while (query.next())
        Labs.push_back(query.value(0).toString());
    return Labs;
}

static QStringList getMachines(QString Lab, QString dbPath)
{
    QStringList Machines;
    QSqlDatabase db;
    if (!QSqlDatabase::contains("LA"))
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "LA");
        db.setDatabaseName(dbPath + "/LabAutomation.db");
    }
    else
        db = QSqlDatabase::database("LabAutomation");

    QSqlQuery query(QSqlDatabase::database("LabAutomation"));

    QString err = query.lastError().text();
    query.prepare("SELECT DISTINCT Machines.\"Machine Model\" FROM Machines where \"Lab Name\" = :LN; ");
    query.bindValue(":LN", Lab);
    query.exec();
    err = query.lastError().text();

    while (query.next())
        Machines.push_back(query.value(0).toString());
    return Machines;
}

static void fillComboBox(QComboBox* pBox, QStringList elments, bool bAddEmptyElement)
{
    pBox->clear();
    if (bAddEmptyElement)
        pBox->addItem("");
    pBox->addItems(elments);
}



//static PatientInfo  getNewPatient(QString dbPath, QString Name)
//{
//    if (!openDB(dbPath))
//    {
//        return {};
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    QString err = query.lastError().text();
//
//    query.prepare("SELECT * FROM BasicData where Name = :patientName  ");
//    query.bindValue(":patientName", Name);
//    bool success = query.exec();
//    query.next();
//    PatientInfo patient;
//    patient.Name = query.value(0).toString();
//    patient.Gender = query.value(1).toString().toInt();
//    patient.Adress = query.value(2).toString();
//    patient.Job = query.value(3).toString();
//    patient.ID = query.value(4).toString();
//    patient.BD = query.value(5).toString();
//    patient.Company = query.value(6).toString();
//    patient.Card = query.value(7).toString();
//    patient.MobileNumber = query.value(8).toString();
//    return patient;
//}
//static PatientInfo getNewPatient(QString dbPath, int ID)
//{
//    if (!openDB(dbPath))
//    {
//        return {};
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    QString err = query.lastError().text();
//
//    query.prepare("SELECT * FROM BasicData where ID = :patientID  ");
//    query.bindValue(":patientID", ID);
//    bool success = query.exec();
//    query.next();
//    PatientInfo patient;
//    patient.ID = query.value(0).toString();
//    patient.Name = query.value(1).toString();
//    patient.Gender = query.value(2).toString();
//    patient.Adress = query.value(3).toString();
//    patient.Job = query.value(4).toString();
//    patient.BD = query.value(5).toString();
//    patient.Company = query.value(6).toString();
//    patient.Card = query.value(7).toString();
//    patient.MobileNumber = query.value(8).toString();
//    return patient;
//}
//
//static PatientInfo getPatientByMob(QString dbPath, QString phone)
//{
//    if (!openDB(dbPath))
//    {
//        return {};
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    QString err = query.lastError().text();
//
//    query.prepare("SELECT * FROM BasicData where Mobile = :patientMobile  ");
//    query.bindValue(":patientMobile", phone);
//    bool success = query.exec();
//    query.next();
//    PatientInfo patient;
//    patient.Name = query.value(0).toString();
//    patient.Gender = query.value(1).toString().toInt();
//    patient.Adress = query.value(2).toString();
//    patient.Job = query.value(3).toString();
//    patient.ID = query.value(4).toString();
//    patient.BD = query.value(5).toString();
//    patient.Company = query.value(6).toString();
//    patient.Card = query.value(7).toString();
//    patient.MobileNumber = query.value(8).toString();
//    return patient;
//}
//
//static vector<PatientInfo> getPatients(QString dbPath, QDate d)
//{
//    if (!openDB(dbPath))
//    {
//        return {};
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//    vector<PatientInfo> patients;
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    QString err = query.lastError().text();
//    QString date = d.toString();
//    query.prepare("SELECT BasicData.ID, BasicData.Name  FROM Schedule INNER JOIN BasicData on Schedule.FK_patientID = BasicData.ID  where Schedule.Date = :selectedDate");
//    query.bindValue(":selectedDate", date);
//    bool success = query.exec();
//    while (query.next())
//    {
//        PatientInfo patient;
//        patient.ID = query.value(0).toString();
//        patient.Name = query.value(1).toString();
//        patients.push_back(patient);
//    }
//    return patients;
//}
//
//static int addPatient(QString dbPath, PatientInfo p)
//{
//    if (!openDB(dbPath))
//    {
//        return -1;
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    QString err = query.lastError().text();
//    query.prepare("INSERT INTO BasicData (Name, Gender, Address, Job, BirthDate, Company, CardNumber, Mobile, dateTime)"
//        "VALUES (:N, :G, :A, :J, :BD, :C, :CN, :M, :dt)");
//    err = query.lastError().text();
//    // Bind Patient info Values
//    query.bindValue(":N", p.Name);
//    if (p.Gender == "0" || p.Gender == "M")
//        query.bindValue(":G", "M");
//    else
//        query.bindValue(":G", "F");
//    query.bindValue(":A", p.Adress);
//    query.bindValue(":J", p.Job);
//    query.bindValue(":BD", p.BD);
//    query.bindValue(":C", p.Company);
//    query.bindValue(":CN", p.Card);
//    query.bindValue(":M", p.MobileNumber);
//    query.bindValue(":dt", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
//    bool success = query.exec();
//    err = query.lastError().text();
//    bool ok;
//    int PID = query.lastInsertId().toInt(&ok);
//    if (ok)
//        return PID;
//    return -1;
//}
//
//static void removePatient(QString dbPath, PatientInfo p)
//{
//
//}


//----------
//static bool addSessionDate(QString dbPath, int PID, QString Name, QDate date)
//{
//    if (!openDB(dbPath))
//    {
//        return {};
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//    QSqlQuery query(QSqlDatabase::database("cs"));
//
//    QString err = query.lastError().text();
//    query.prepare("INSERT INTO Schedule(FK_patientID, Name, Date)"
//        "VALUES (:ID, :N, :D)");
//    err = query.lastError().text();
//    // Bind Patient info Values
//    query.bindValue(":ID", PID);
//    query.bindValue(":N", Name);
//    query.bindValue(":D", date.toString());
//    bool success = query.exec();
//    err = query.lastError().text();
//    return success;
//}
//static bool addSession(QString dbPath, SessionInfo SI)
//{
//    if (!openDB(dbPath))
//    {
//        return {};
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    QString err = query.lastError().text();
//    query.prepare("INSERT INTO Visits ( "
//        "FK_patientID, Date, Time, Complaint,PresentHistory, PastHistory, General,Consciousness, Motor, Sensory, Cranialnerves, Speech, Reflexes, ExaminationOthers, XRay, MRI, CT, Neuro, DXA, InvestigationOthers, Impression, Dose1, Dose2, Dose3, Dose4, Dose5, Dose6, Med1,  Med2,  Med3,  Med4,  Med5,  Med6, Med7) "
//        "VALUES (:ID,        :date, :time, :com,     :presentH,      :pastH,      :g,     :cons,         :mo,    :sen,    :cr,           :speech, :ref,    :exOthers,        :xr,   :mr, :ctt, :nu, :dxa, :InvOthers,            :Imp,    :d1,    :d2,   :d3,   :d4,   :d5,   :d6,   :m1,   :m2,   :m3,   :m4,    :m5,  :m6,  :m7)");
//    err = query.lastError().text();
//    // Bind Patient info Values
//    query.bindValue(":ID", SI.patientID);
//    query.bindValue(":date", SI.sessionDate.toString());
//    query.bindValue(":time", SI.sessionTime.toString());
//    query.bindValue(":com", SI.Complaint);
//    query.bindValue(":presentH", SI.PresentH);
//    query.bindValue(":pastH", SI.PastH);
//    query.bindValue(":g", SI.General);
//    query.bindValue(":cons", SI.Concious);
//    query.bindValue(":mo", SI.Motor);
//    query.bindValue(":sen", SI.Sensory);
//    query.bindValue(":cr", SI.CranialNerv);
//    query.bindValue(":speech", SI.Speech);
//    query.bindValue(":ref", SI.Reflex);
//    query.bindValue(":exOthers", SI.examinationOthers);
//    query.bindValue(":xr", SI.XRay);
//    query.bindValue(":mr", SI.MRI);
//    query.bindValue(":ctt", SI.CT);
//    query.bindValue(":nu", SI.NeuroPhys);
//    query.bindValue(":dxa", SI.DXA);
//    query.bindValue(":InvOthers", SI.InvestOthers);
//    query.bindValue(":Imp", SI.Impression);
//    query.bindValue(":d1", SI.Dose1);
//    query.bindValue(":d2", SI.Dose2);
//    query.bindValue(":d3", SI.Dose3);
//    query.bindValue(":d4", SI.Dose4);
//    query.bindValue(":d5", SI.Dose5);
//    query.bindValue(":d6", SI.Dose6);
//    query.bindValue(":m1", SI.Med1);
//    query.bindValue(":m2", SI.Med2);
//    query.bindValue(":m3", SI.Med3);
//    query.bindValue(":m4", SI.Med4);
//    query.bindValue(":m5", SI.Med5);
//    query.bindValue(":m6", SI.Med6);
//    query.bindValue(":m7", SI.Med7);
//    bool success = query.exec();
//    err = query.lastError().text();
//    return success;
//
//}
//static QStringList getSessions(QString dbPath, int PID)
//{
//    if (!openDB(dbPath))
//        return {};
//
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    query.prepare("SELECT Visits.Date FROM Visits where FK_patientID = :patientID  ");
//    query.bindValue(":patientID", PID);
//    QStringList dates;
//
//    bool success = query.exec();
//    QString err = query.lastError().text();
//    while (query.next())
//        dates.push_back(query.value(0).toString());
//
//    return dates;
//}
//
//static SessionInfo getSessionInfoFromDB(QString dbPath, int PID, QString Date)
//{
//    if (!openDB(dbPath))
//    {
//        return {};
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    QString err = query.lastError().text();
//    query.prepare("SELECT * FROM Visits where FK_patientID = :patientID AND Date = :requestedDate  ");
//    query.bindValue(":patientID", PID);
//    query.bindValue(":requestedDate", Date);
//    err = query.lastError().text();
//    bool success = query.exec();
//    SessionInfo SI;
//    // Bind Patient info Values
//    while (query.next())
//    {
//        SI.patientID = query.value(0).toString();
//        SI.sessionDate = QDate::fromString(query.value(1).toString());
//        SI.sessionTime = QTime::fromString(query.value(2).toString());
//        SI.Complaint = query.value(3).toString();
//        SI.PresentH = query.value(4).toString();
//        SI.PastH = query.value(5).toString();
//        SI.General = query.value(6).toString();
//        SI.Concious = query.value(7).toString();
//        SI.Motor = query.value(8).toString();
//        SI.Sensory = query.value(9).toString();
//        SI.CranialNerv = query.value(10).toString();
//        SI.Speech = query.value(11).toString();
//        SI.Reflex = query.value(12).toString();
//        SI.examinationOthers = query.value(13).toString();
//        SI.XRay = query.value(14).toString();
//        SI.MRI = query.value(15).toString();
//        SI.CT = query.value(16).toString();
//        SI.NeuroPhys = query.value(17).toString();
//        SI.DXA = query.value(18).toString();
//        SI.InvestOthers = query.value(19).toString();
//        SI.Impression = query.value(20).toString();
//        SI.Dose1 = query.value(21).toString();
//        SI.Dose2 = query.value(22).toString();
//        SI.Dose3 = query.value(23).toString();
//        SI.Dose4 = query.value(24).toString();
//        SI.Dose5 = query.value(25).toString();
//        SI.Dose6 = query.value(26).toString();
//        SI.Med1 = query.value(27).toString();
//        SI.Med2 = query.value(28).toString();
//        SI.Med3 = query.value(29).toString();
//        SI.Med4 = query.value(30).toString();
//        SI.Med5 = query.value(31).toString();
//        SI.Med6 = query.value(32).toString();
//        SI.Med7 = query.value(33).toString();
//    }
//    err = query.lastError().text();
//    return SI;
//}
//
//static vector<SessionInfo> getAllSessionInfoFromID(QString dbPath, int PID, QString Date)
//{
//    if (!openDB(dbPath))
//    {
//        return {};
//    }
//    QSqlDatabase db;
//    if (!QSqlDatabase::contains("cs"))
//    {
//        db = QSqlDatabase::addDatabase("QSQLITE", "cs");
//        db.setDatabaseName(dbPath + "/cs.db");
//    }
//    else
//        db = QSqlDatabase::database("cs");
//    QSqlQuery query(QSqlDatabase::database("cs"));
//    QString err = query.lastError().text();
//    query.prepare("SELECT * FROM Visits where FK_patientID = :patientID  ");
//    query.bindValue(":patientID", PID);
//    err = query.lastError().text();
//    bool success = query.exec();
//    vector<SessionInfo> AllSessions;
//    // Bind Patient info Values
//    while (query.next())
//    {
//        SessionInfo SI;
//        SI.patientID = query.value(0).toString();
//        SI.sessionDate = QDate::fromString(query.value(1).toString());
//        SI.sessionTime = QTime::fromString(query.value(2).toString());
//        SI.Complaint = query.value(3).toString();
//        SI.PresentH = query.value(4).toString();
//        SI.PastH = query.value(5).toString();
//        SI.General = query.value(6).toString();
//        SI.Concious = query.value(7).toString();
//        SI.Motor = query.value(8).toString();
//        SI.Sensory = query.value(9).toString();
//        SI.CranialNerv = query.value(10).toString();
//        SI.Speech = query.value(11).toString();
//        SI.Reflex = query.value(12).toString();
//        SI.XRay = query.value(13).toString();
//        SI.examinationOthers = query.value(14).toString();
//        SI.XRay = query.value(15).toString();
//        SI.MRI = query.value(16).toString();
//        SI.CT = query.value(17).toString();
//        SI.NeuroPhys = query.value(18).toString();
//        SI.DXA = query.value(19).toString();
//        SI.InvestOthers = query.value(20).toString();
//        SI.Impression = query.value(21).toString();
//        SI.Dose1 = query.value(22).toString();
//        SI.Dose2 = query.value(23).toString();
//        SI.Dose3 = query.value(24).toString();
//        SI.Dose4 = query.value(25).toString();
//        SI.Dose5 = query.value(26).toString();
//        SI.Dose6 = query.value(27).toString();
//        SI.Med1 = query.value(28).toString();
//        SI.Med2 = query.value(29).toString();
//        SI.Med3 = query.value(30).toString();
//        SI.Med4 = query.value(31).toString();
//        SI.Med5 = query.value(32).toString();
//        SI.Med6 = query.value(33).toString();
//        SI.Med7 = query.value(34).toString();
//        AllSessions.push_back(SI);
//    }
//    err = query.lastError().text();
//    return AllSessions;
//}