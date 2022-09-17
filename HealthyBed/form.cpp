#include "form.h"
#include "ui_form.h"
#include "qpushbutton.h"
#include "widget.h"
#include "qdebug.h"
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

//    setInputMethodHints(inputMethodHints() | Qt::InputMethodHint::ImhDigitsOnly);
//setInputMethodHints(Qt::ImhDigitsOnly);


connect(ui->pushButton,&QPushButton::clicked,this,&Form::get_textEdit_content);
}

Form::~Form()
{
    delete ui;
}


void Form::get_textEdit_content()
{

    QString str;
str =ui->textEdit->toPlainText();
qDebug()<<"获取edit"<<str;
emit signal_edit_set_speed_to_Dialogwin(str);
this->close();
}
