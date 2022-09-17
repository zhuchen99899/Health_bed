#include "dialog.h"
#include "ui_dialog.h"
#include "qdebug.h"
#include "qpushbutton.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->horizontalSlider->setRange(0, 250);

    //获取值：
//    int val = ui->horizontalSlider->value();



//连接LCD显示槽
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->lcdNumber,SLOT(display(int)));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));

//连接发送主窗口信号槽
    connect(ui->horizontalSlider,SIGNAL(sliderReleased()),this,SLOT(on_horizontalSlider_release()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(QPushButton_clicked()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(QPushButton1_clicked()));
//连接创建新窗口槽
    connect(ui->pushButton_3,&QPushButton::clicked,this,&Dialog::button_create_widget);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_horizontalSlider_valueChanged(int value)
{

   qDebug()<<"滑动中"<<value<<endl;
}


void Dialog::QPushButton_clicked()
{

emit singal_motor1_direction0_to_mainwin(true);


}

void Dialog::QPushButton1_clicked()
{

emit singal_motor1_direction1_to_mainwin(true);

}


void Dialog::on_horizontalSlider_release()
{
    int val = ui->horizontalSlider->value();
    qDebug()<<val;
    emit signal_speed_to_mainwin(val);

}

void Dialog::button_create_widget()
{
//注：此时不能传递父窗口指针 this，如果传递父窗口指针，则会直接在父窗口上创建子窗口控件
    form= new Form();
    form->setWindowModality(Qt::ApplicationModal);//模态：此时只能编辑此窗口。
    form->show();
   connect(form,SIGNAL(signal_edit_set_speed_to_Dialogwin(QString)),this,SLOT(Edit_set_speed_toMainwin(QString)));
   connect(form,SIGNAL(signal_edit_set_speed_to_Dialogwin(QString)),this,SLOT(Edit_set_speed_toMyhorizontalSlider(QString)));
   connect(form,SIGNAL(signal_edit_set_speed_to_Dialogwin(QString)),this,SLOT(Edit_set_speed_toMyLcdNumber(QString)));
}



//从子窗口Form中获取设置速度值给Mainwin

void Dialog::Edit_set_speed_toMainwin(QString str)
{

qDebug()<<"Dialog窗口打印，来自FORM窗口"<<str;
int val=str.toInt();
emit signal_speed_to_mainwin(val);

}

void Dialog::Edit_set_speed_toMyhorizontalSlider(QString str)
{
int val= str.toInt();
ui->horizontalSlider->setValue(val);

}
void Dialog::Edit_set_speed_toMyLcdNumber(QString str)
{


ui->lcdNumber->display(str);

}


