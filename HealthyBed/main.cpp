#include "widget.h"
#include <QApplication>
#include "qmqttglobal.h"
//#include <QtMqtt/QMqttClient>  替换为#include "qmqttglobal.h"
#include "qfile.h"
int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

//       qputenv("QT_IM_MODULE", QByteArray("tgtsml"));
    QApplication a(argc, argv);

    /********* 默认当前目录在resource文件夹下*********/
    //载入qss样式
    QFile qss(":/qss/style.qss");

     if( qss.open(QFile::ReadOnly))

           {

               qDebug("open success");

               QString style = QLatin1String(qss.readAll());

               a.setStyleSheet(style);

               qss.close();



           }

           else

           {

              qDebug("Open failed");

     }

    Widget w;
    w.show();





    return a.exec();
}


