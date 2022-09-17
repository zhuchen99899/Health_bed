#ifndef DIALOG_H
#define DIALOG_H


#include <QDialog>
#include <widget.h>
#include <qmqttclient.h>
#include "form.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    //发送给主窗口的信号
    void signal_speed_to_mainwin(int value);
    void singal_motor1_direction0_to_mainwin(bool value);
    void singal_motor1_direction1_to_mainwin(bool value);

public slots:
    //horizontalSlider控件数值变动槽
    void on_horizontalSlider_valueChanged(int value);
    //horizontalSlider值释放
    void on_horizontalSlider_release();

    //正传按钮
    void QPushButton_clicked();
    //反转按钮
    void QPushButton1_clicked();

    //按键创建新的widget
    void button_create_widget();
    //Form窗口Edit消息槽
    void Edit_set_speed_toMainwin(QString str);
    void Edit_set_speed_toMyhorizontalSlider(QString str);
    void Edit_set_speed_toMyLcdNumber(QString str);
private:
    Ui::Dialog *ui;
    Form *form;
};

#endif // DIALOG_H
