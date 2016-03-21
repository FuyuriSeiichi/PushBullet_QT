/********************************************************************************
** Form generated from reading UI file 'setupwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPWINDOW_H
#define UI_SETUPWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *lineeditAccount;
    QLabel *labelAccount;
    QLineEdit *lineeditPassword;
    QLabel *label_2;
    QLineEdit *lineeditAccessToken;
    QPushButton *pushButton;

    void setupUi(QMainWindow *SetupWindow)
    {
        if (SetupWindow->objectName().isEmpty())
            SetupWindow->setObjectName(QString::fromUtf8("SetupWindow"));
        SetupWindow->resize(391, 392);
        SetupWindow->setUnifiedTitleAndToolBarOnMac(true);
        centralWidget = new QWidget(SetupWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setAutoFillBackground(true);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 391, 391));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        lineeditAccount = new QLineEdit(verticalLayoutWidget);
        lineeditAccount->setObjectName(QString::fromUtf8("lineeditAccount"));

        verticalLayout->addWidget(lineeditAccount);

        labelAccount = new QLabel(verticalLayoutWidget);
        labelAccount->setObjectName(QString::fromUtf8("labelAccount"));
        sizePolicy.setHeightForWidth(labelAccount->sizePolicy().hasHeightForWidth());
        labelAccount->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(labelAccount);

        lineeditPassword = new QLineEdit(verticalLayoutWidget);
        lineeditPassword->setObjectName(QString::fromUtf8("lineeditPassword"));

        verticalLayout->addWidget(lineeditPassword);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(label_2);

        lineeditAccessToken = new QLineEdit(verticalLayoutWidget);
        lineeditAccessToken->setObjectName(QString::fromUtf8("lineeditAccessToken"));

        verticalLayout->addWidget(lineeditAccessToken);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(pushButton);

        SetupWindow->setCentralWidget(centralWidget);

        retranslateUi(SetupWindow);

        QMetaObject::connectSlotsByName(SetupWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SetupWindow)
    {
        SetupWindow->setWindowTitle(QApplication::translate("SetupWindow", "Login:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SetupWindow", "Account", 0, QApplication::UnicodeUTF8));
        labelAccount->setText(QApplication::translate("SetupWindow", "Password", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SetupWindow", "Access_Token:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("SetupWindow", "Login", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SetupWindow: public Ui_SetupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPWINDOW_H
