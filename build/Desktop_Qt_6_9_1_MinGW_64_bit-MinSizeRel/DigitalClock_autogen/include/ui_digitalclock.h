/********************************************************************************
** Form generated from reading UI file 'digitalclock.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIGITALCLOCK_H
#define UI_DIGITALCLOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DigitalClock
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DigitalClock)
    {
        if (DigitalClock->objectName().isEmpty())
            DigitalClock->setObjectName("DigitalClock");
        DigitalClock->resize(800, 600);
        centralwidget = new QWidget(DigitalClock);
        centralwidget->setObjectName("centralwidget");
        DigitalClock->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DigitalClock);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        DigitalClock->setMenuBar(menubar);
        statusbar = new QStatusBar(DigitalClock);
        statusbar->setObjectName("statusbar");
        DigitalClock->setStatusBar(statusbar);

        retranslateUi(DigitalClock);

        QMetaObject::connectSlotsByName(DigitalClock);
    } // setupUi

    void retranslateUi(QMainWindow *DigitalClock)
    {
        DigitalClock->setWindowTitle(QCoreApplication::translate("DigitalClock", "DigitalClock", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DigitalClock: public Ui_DigitalClock {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGITALCLOCK_H
