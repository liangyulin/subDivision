/********************************************************************************
** Form generated from reading UI file 'subdivision_new.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBDIVISION_NEW_H
#define UI_SUBDIVISION_NEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_subdivision_newClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *subdivision_newClass)
    {
        if (subdivision_newClass->objectName().isEmpty())
            subdivision_newClass->setObjectName(QStringLiteral("subdivision_newClass"));
        subdivision_newClass->resize(600, 400);
        menuBar = new QMenuBar(subdivision_newClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        subdivision_newClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(subdivision_newClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        subdivision_newClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(subdivision_newClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        subdivision_newClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(subdivision_newClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        subdivision_newClass->setStatusBar(statusBar);

        retranslateUi(subdivision_newClass);

        QMetaObject::connectSlotsByName(subdivision_newClass);
    } // setupUi

    void retranslateUi(QMainWindow *subdivision_newClass)
    {
        subdivision_newClass->setWindowTitle(QApplication::translate("subdivision_newClass", "subdivision_new", 0));
    } // retranslateUi

};

namespace Ui {
    class subdivision_newClass: public Ui_subdivision_newClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBDIVISION_NEW_H
