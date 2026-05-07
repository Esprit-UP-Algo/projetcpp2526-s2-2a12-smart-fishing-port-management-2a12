/********************************************************************************
** Form generated from reading UI file 'employe_logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYE_LOGINDIALOG_H
#define UI_EMPLOYE_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EmployeLoginDialog
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_1;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_1;
    QFrame *loginFrame;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblTitle;
    QLineEdit *lineUser;
    QLineEdit *linePass;
    QSpacerItem *verticalSpacer_Form;
    QPushButton *btnLogin;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *EmployeLoginDialog)
    {
        if (EmployeLoginDialog->objectName().isEmpty())
            EmployeLoginDialog->setObjectName("EmployeLoginDialog");
        EmployeLoginDialog->resize(450, 550);
        EmployeLoginDialog->setStyleSheet(QString::fromUtf8("\n"
"    QDialog {\n"
"        background-color: #B7CDD1;\n"
"    }\n"
"   "));
        verticalLayout = new QVBoxLayout(EmployeLoginDialog);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_1 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_1);

        loginFrame = new QFrame(EmployeLoginDialog);
        loginFrame->setObjectName("loginFrame");
        loginFrame->setMinimumSize(QSize(380, 260));
        loginFrame->setStyleSheet(QString::fromUtf8("\n"
"            QFrame#loginFrame {\n"
"                background-color: #0F3A44;\n"
"                border-radius: 18px;\n"
"            }\n"
"            QLabel {\n"
"                border: none;\n"
"            }\n"
"        "));
        loginFrame->setFrameShape(QFrame::StyledPanel);
        loginFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(loginFrame);
        verticalLayout_2->setSpacing(15);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(30, 30, 30, 30);
        lblTitle = new QLabel(loginFrame);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setStyleSheet(QString::fromUtf8("\n"
"            color: #6FE3FF;\n"
"            font-size: 22px;\n"
"            font-weight: normal;\n"
"           "));
        lblTitle->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_2->addWidget(lblTitle);

        lineUser = new QLineEdit(loginFrame);
        lineUser->setObjectName("lineUser");
        lineUser->setMinimumSize(QSize(0, 40));
        lineUser->setStyleSheet(QString::fromUtf8("\n"
"            background: transparent;\n"
"            border: 1px solid rgba(255,255,255,0.08);\n"
"            border-radius: 8px;\n"
"            padding: 0 12px;\n"
"            color: #A6DCE6;\n"
"            selection-background-color: #28B6D4;\n"
"           "));

        verticalLayout_2->addWidget(lineUser);

        linePass = new QLineEdit(loginFrame);
        linePass->setObjectName("linePass");
        linePass->setMinimumSize(QSize(0, 40));
        linePass->setStyleSheet(QString::fromUtf8("\n"
"            background: transparent;\n"
"            border: 1px solid rgba(255,255,255,0.08);\n"
"            border-radius: 8px;\n"
"            padding: 0 12px;\n"
"            color: #A6DCE6;\n"
"            selection-background-color: #28B6D4;\n"
"           "));
        linePass->setEchoMode(QLineEdit::Password);

        verticalLayout_2->addWidget(linePass);

        verticalSpacer_Form = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_Form);

        btnLogin = new QPushButton(loginFrame);
        btnLogin->setObjectName("btnLogin");
        btnLogin->setMinimumSize(QSize(0, 45));
        btnLogin->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnLogin->setStyleSheet(QString::fromUtf8("\n"
"            QPushButton {\n"
"                background-color: #28B6D4;\n"
"                border-radius: 10px;\n"
"                color: #0F3A44;\n"
"                font-weight: bold;\n"
"                font-size: 14px;\n"
"            }\n"
"            QPushButton:hover {\n"
"                background-color: #3FD3F3;\n"
"            }\n"
"            QPushButton:pressed {\n"
"                background-color: #1CA2BF;\n"
"            }\n"
"           "));

        verticalLayout_2->addWidget(btnLogin);


        horizontalLayout->addWidget(loginFrame);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(EmployeLoginDialog);

        QMetaObject::connectSlotsByName(EmployeLoginDialog);
    } // setupUi

    void retranslateUi(QDialog *EmployeLoginDialog)
    {
        EmployeLoginDialog->setWindowTitle(QCoreApplication::translate("EmployeLoginDialog", "Connexion", nullptr));
        lblTitle->setText(QCoreApplication::translate("EmployeLoginDialog", "Connexion", nullptr));
        lineUser->setPlaceholderText(QCoreApplication::translate("EmployeLoginDialog", "Login", nullptr));
        linePass->setPlaceholderText(QCoreApplication::translate("EmployeLoginDialog", "Mot de passe", nullptr));
        btnLogin->setText(QCoreApplication::translate("EmployeLoginDialog", "Se connecter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmployeLoginDialog: public Ui_EmployeLoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYE_LOGINDIALOG_H
