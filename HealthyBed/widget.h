#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<dialog.h>
#include <qmqttclient.h>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


//公共槽
public slots:
void on_Slider_mqttPublish(int value);
void on_Dialog_PushButton_click(bool val);
void on_Dialog_PushButton1_click(bool val);

//私有槽
private slots:
//MQTT相关槽函数
void on_buttonConnect_clicked();
void updateLogStateChange();

//QT相关槽函数
void buttonCreatWin_clicked();
void button2CreatWin_clicked();


//void brokerDisconnected();

//void on_buttonPublish_clicked();

//void on_buttonSubscribe_clicked();



private:
    Ui::Widget *ui;

    //创建MQTT客户端对象
        QMqttClient *m_client;
//创建子对话框对象
    QDialog *dialog;
    QDialog *dialog2;

};

#endif // WIDGET_H


