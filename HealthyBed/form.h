#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

signals:
     void signal_edit_set_speed_to_Dialogwin(QString str);

public slots:
    void get_textEdit_content();
private:
    Ui::Form *ui;
};

#endif // FORM_H
