/********************************************************************************
** Form generated from reading UI file 'userdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERDIALOG_H
#define UI_USERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UserDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelCIN;
    QLineEdit *lineCIN;
    QLabel *labelNom;
    QLineEdit *lineNom;
    QLabel *labelPrenom;
    QLineEdit *linePrenom;
    QLabel *labelEmail;
    QLineEdit *lineEmail;
    QLabel *labelLogin;
    QLineEdit *lineLogin;
    QLabel *labelPassword;
    QLineEdit *linePassword;
    QLabel *labelRole;
    QComboBox *comboRole;
    QLabel *labelStatut;
    QComboBox *comboStatut;
    QLabel *labelHeures;
    QSpinBox *spinHeures;
    QLabel *labelDate;
    QDateEdit *dateCreation;
    QHBoxLayout *buttonLayout;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QDialog *UserDialog)
    {
        if (UserDialog->objectName().isEmpty())
            UserDialog->setObjectName("UserDialog");
        verticalLayout = new QVBoxLayout(UserDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        labelCIN = new QLabel(UserDialog);
        labelCIN->setObjectName("labelCIN");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelCIN);

        lineCIN = new QLineEdit(UserDialog);
        lineCIN->setObjectName("lineCIN");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineCIN);

        labelNom = new QLabel(UserDialog);
        labelNom->setObjectName("labelNom");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelNom);

        lineNom = new QLineEdit(UserDialog);
        lineNom->setObjectName("lineNom");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineNom);

        labelPrenom = new QLabel(UserDialog);
        labelPrenom->setObjectName("labelPrenom");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelPrenom);

        linePrenom = new QLineEdit(UserDialog);
        linePrenom->setObjectName("linePrenom");

        formLayout->setWidget(2, QFormLayout::FieldRole, linePrenom);

        labelEmail = new QLabel(UserDialog);
        labelEmail->setObjectName("labelEmail");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelEmail);

        lineEmail = new QLineEdit(UserDialog);
        lineEmail->setObjectName("lineEmail");

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEmail);

        labelLogin = new QLabel(UserDialog);
        labelLogin->setObjectName("labelLogin");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelLogin);

        lineLogin = new QLineEdit(UserDialog);
        lineLogin->setObjectName("lineLogin");

        formLayout->setWidget(4, QFormLayout::FieldRole, lineLogin);

        labelPassword = new QLabel(UserDialog);
        labelPassword->setObjectName("labelPassword");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelPassword);

        linePassword = new QLineEdit(UserDialog);
        linePassword->setObjectName("linePassword");
        linePassword->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(5, QFormLayout::FieldRole, linePassword);

        labelRole = new QLabel(UserDialog);
        labelRole->setObjectName("labelRole");

        formLayout->setWidget(6, QFormLayout::LabelRole, labelRole);

        comboRole = new QComboBox(UserDialog);
        comboRole->setObjectName("comboRole");

        formLayout->setWidget(6, QFormLayout::FieldRole, comboRole);

        labelStatut = new QLabel(UserDialog);
        labelStatut->setObjectName("labelStatut");

        formLayout->setWidget(7, QFormLayout::LabelRole, labelStatut);

        comboStatut = new QComboBox(UserDialog);
        comboStatut->setObjectName("comboStatut");

        formLayout->setWidget(7, QFormLayout::FieldRole, comboStatut);

        labelHeures = new QLabel(UserDialog);
        labelHeures->setObjectName("labelHeures");

        formLayout->setWidget(8, QFormLayout::LabelRole, labelHeures);

        spinHeures = new QSpinBox(UserDialog);
        spinHeures->setObjectName("spinHeures");

        formLayout->setWidget(8, QFormLayout::FieldRole, spinHeures);

        labelDate = new QLabel(UserDialog);
        labelDate->setObjectName("labelDate");

        formLayout->setWidget(9, QFormLayout::LabelRole, labelDate);

        dateCreation = new QDateEdit(UserDialog);
        dateCreation->setObjectName("dateCreation");

        formLayout->setWidget(9, QFormLayout::FieldRole, dateCreation);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        btnOk = new QPushButton(UserDialog);
        btnOk->setObjectName("btnOk");

        buttonLayout->addWidget(btnOk);

        btnCancel = new QPushButton(UserDialog);
        btnCancel->setObjectName("btnCancel");

        buttonLayout->addWidget(btnCancel);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(UserDialog);

        QMetaObject::connectSlotsByName(UserDialog);
    } // setupUi

    void retranslateUi(QDialog *UserDialog)
    {
        UserDialog->setWindowTitle(QCoreApplication::translate("UserDialog", "Utilisateur", nullptr));
        labelCIN->setText(QCoreApplication::translate("UserDialog", "CIN", nullptr));
        labelNom->setText(QCoreApplication::translate("UserDialog", "Nom", nullptr));
        labelPrenom->setText(QCoreApplication::translate("UserDialog", "Pr\303\251nom", nullptr));
        labelEmail->setText(QCoreApplication::translate("UserDialog", "Email", nullptr));
        labelLogin->setText(QCoreApplication::translate("UserDialog", "Login", nullptr));
        labelPassword->setText(QCoreApplication::translate("UserDialog", "Mot de passe", nullptr));
        labelRole->setText(QCoreApplication::translate("UserDialog", "R\303\264le", nullptr));
        labelStatut->setText(QCoreApplication::translate("UserDialog", "Statut", nullptr));
        labelHeures->setText(QCoreApplication::translate("UserDialog", "Heures", nullptr));
        labelDate->setText(QCoreApplication::translate("UserDialog", "Date de cr\303\251ation", nullptr));
        btnOk->setText(QCoreApplication::translate("UserDialog", "OK", nullptr));
        btnCancel->setText(QCoreApplication::translate("UserDialog", "Annuler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserDialog: public Ui_UserDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERDIALOG_H
