#include "mainwindow.h"
#include <QApplication>
#include <QQueue>
//#include <qt_library.h>
//#include <qt_library_global.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    Qt_library lib;
//    lib.hello();



    return a.exec();
}
