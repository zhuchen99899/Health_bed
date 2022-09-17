/********************************************************************************
** Form generated from reading UI file 'dialog2.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG2_H
#define UI_DIALOG2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

QT_BEGIN_NAMESPACE

class Ui_Dialog2
{
public:
    QDialogButtonBox *buttonBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLCDNumber *lcdNumber;
    QSlider *horizontalSlider;
    QPushButton *pushButton_3;

    void setupUi(QDialog *Dialog2)
    {
        if (Dialog2->objectName().isEmpty())
            Dialog2->setObjectName(QStringLiteral("Dialog2"));
        Dialog2->resize(400, 300);
        buttonBox = new QDialogButtonBox(Dialog2);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(290, 20, 81, 241));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
        pushButton = new QPushButton(Dialog2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 60, 93, 28));
        pushButton_2 = new QPushButton(Dialog2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(150, 60, 93, 28));
        lcdNumber = new QLCDNumber(Dialog2);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(100, 140, 64, 23));
        horizontalSlider = new QSlider(Dialog2);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(50, 210, 160, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        pushButton_3 = new QPushButton(Dialog2);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(280, 180, 93, 28));

        retranslateUi(Dialog2);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog2, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog2, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog2);
    } // setupUi

    void retranslateUi(QDialog *Dialog2)
    {
        Dialog2->setWindowTitle(QApplication::translate("Dialog2", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Dialog2", "\346\255\243\350\275\254", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Dialog2", "\345\217\215\350\275\254", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("Dialog2", "\351\200\237\345\272\246\347\262\276\345\207\206\346\216\247\345\210\266", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog2: public Ui_Dialog2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG2_H
