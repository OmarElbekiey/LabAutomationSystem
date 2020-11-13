#include "GUI.h"
#include <QtWidgets/QApplication>
#include "DarkStyle.h"
#include <QSettings>
#include <QInputDialog>
#include <QLineEdit>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    GUI w;
    a.setStyle(new DarkStyle);
    w.show();
    return a.exec();
}
