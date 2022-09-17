#include "widget.h"
#include "ui_widget.h"
#include "qmqttglobal.h"
#include "qdebug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "dialog.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置pushButton_3贴图
    ui->pushButton_3->setIconSize(QSize(48,48));
    ui->pushButton_3->setIcon(QIcon(":/img/connect.png"));
    ui->pushButton_3->setText(QString::fromLocal8Bit("Connect"));

    //mqtt client 连接设置
    m_client = new QMqttClient(this);
    m_client->setHostname("mqtt.dengsanpeng.com");
    m_client->setPort(1883);
    m_client->setUsername("zhuchen");
    m_client->setPassword("test");
    m_client->setClientId("ZCPcQtClient1");
    m_client->setAutoKeepAlive(1);

    //QT控件信号与槽连接
    connect(ui->pushButton,&QPushButton::clicked,this,&Widget::buttonCreatWin_clicked);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&Widget::button2CreatWin_clicked);
    //mqtt信号与槽连接
    connect(ui->pushButton_3,&QPushButton::clicked,this,&Widget::on_buttonConnect_clicked);
    connect(m_client, &QMqttClient::stateChanged, this, &Widget::updateLogStateChange);

}

Widget::~Widget()
{
    delete ui;
}


//MQTT相关槽
void Widget::on_buttonConnect_clicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        m_client->connectToHost();
        ui->pushButton_3->setText(tr("Disconnect"));
    } else {

        m_client->disconnectFromHost();
        ui->pushButton_3->setText(tr("Connect"));
    }
}

void Widget::updateLogStateChange()
{
    const QString content =QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
    ui->textBrowser->insertPlainText(content);
}

//子窗口 Dialog信号槽处理
void Widget::on_Slider_mqttPublish(int value)
{

   // 构建 JSON 对象
//   QJsonObject json;
//   QJsonValue Speed;
//   Speed.toDouble(value);
//   json.insert("Speed", Speed);

       QJsonObject json;

       QJsonValue Speed;

       Speed=value;

//       Speed=(double)value;
//if(Speed.isDouble()) qDebug()<<"是double";
       json.insert("Speed",Speed.toDouble());

   // 构建 JSON 文档
   QJsonDocument document;
   document.setObject(json);
   QByteArray byteArray = document.toJson(QJsonDocument::Compact);
   QString strJson(byteArray);


QString str1;

str1="QTmqtt";

m_client->publish(str1,strJson.toUtf8(),0);
qDebug()<<"收到子窗口消息"<<value;
//m_client->publish("QTmqtt",value,0,0);
}

void Widget::on_Dialog_PushButton_click(bool val)
{
    if (val==true)
    {
    QJsonObject json;

    json.insert("Positive",1);
    json.insert("Negative",0);

    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    QString str1;
    str1="QTmqtt";
    m_client->publish(str1,strJson.toUtf8(),0);
    }
}


void Widget::on_Dialog_PushButton1_click(bool val)
{

    if (val==true)
    {
    QJsonObject json;

    json.insert("Positive",0);
    json.insert("Negative",1);

    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    QString str1;
    str1="QTmqtt";
    m_client->publish(str1,strJson.toUtf8(),0);
    }

}


//QT相关槽
void Widget::buttonCreatWin_clicked()
{
    dialog= new Dialog();
//    dialog->setModal(false); //模态：此时只能编辑此窗口。
    dialog->show();

    //接收子窗口发来的信号
    //必须在子窗口创建后连接此信号与槽，否则，dialog指针为空 2.必须为(int) (类型)
    connect(dialog,SIGNAL(signal_speed_to_mainwin(int)),this,SLOT(on_Slider_mqttPublish(int)));
    connect(dialog,SIGNAL(singal_motor1_direction0_to_mainwin(bool)),this,SLOT(on_Dialog_PushButton_click(bool)));
    connect(dialog,SIGNAL(singal_motor1_direction1_to_mainwin(bool)),this,SLOT(on_Dialog_PushButton1_click(bool)));
}






void Widget::button2CreatWin_clicked()
{

    dialog2= new Dialog();
    dialog2->setModal(true); //模态：此时只能编辑此窗口。
    dialog2->show();
}



