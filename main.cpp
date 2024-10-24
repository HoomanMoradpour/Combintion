#include "mainwindow.h"
#include <QChar>
#include <QApplication>
#include <QThread>
//#include "DistanceSensor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

   // DistanceSensor sensor;
   // if (!sensor.initialize()) {
   //     return -1; // Initialization failed
   // }

   // while (true) {
   //     double distance = sensor.getDistance();
   //     qDebug() << "Measured Distance =" << distance << "cm";
    //    QThread::sleep(1); // Sleep for 1 second
   // }

   // sensor.cleanup();
   // return a.exec();



    MainWindow w;
    w.show();
    return a.exec();
}
