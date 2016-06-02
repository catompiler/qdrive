#include "mainwindow.h"
#include <QApplication>
#include <stdlib.h>
#include <sys/time.h>
#include "settings.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Settings::get().read();

    //srand(time(NULL));
    testParams();

    MainWindow w;
    w.show();

    int res = a.exec();

    Settings::get().write();

    return res;
}
