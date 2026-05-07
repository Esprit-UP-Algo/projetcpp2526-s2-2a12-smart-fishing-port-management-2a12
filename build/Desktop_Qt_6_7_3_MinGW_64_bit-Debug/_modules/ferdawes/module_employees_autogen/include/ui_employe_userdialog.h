/********************************************************************************
** Form generated from reading UI file 'employe_userdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYE_USERDIALOG_H
#define UI_EMPLOYE_USERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EmployeUserDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout1;
    QLabel *label1;
    QLineEdit *lineCIN;
    QVBoxLayout *vboxLayout2;
    QLabel *label2;
    QLineEdit *lineNom;
    QHBoxLayout *hboxLayout1;
    QVBoxLayout *vboxLayout3;
    QLabel *label3;
    QLineEdit *linePrenom;
    QVBoxLayout *vboxLayout4;
    QLabel *label4;
    QLineEdit *lineEmail;
    QHBoxLayout *hboxLayout2;
    QVBoxLayout *vboxLayout5;
    QLabel *label5;
    QLineEdit *lineLogin;
    QVBoxLayout *vboxLayout6;
    QLabel *label6;
    QLineEdit *linePassword;
    QHBoxLayout *hboxLayout3;
    QVBoxLayout *vboxLayout7;
    QLabel *label7;
    QComboBox *comboRole;
    QVBoxLayout *vboxLayout8;
    QLabel *label8;
    QComboBox *comboStatut;
    QHBoxLayout *hboxLayout4;
    QVBoxLayout *vboxLayout9;
    QLabel *label9;
    QDoubleSpinBox *spinHeures;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout5;
    QSpacerItem *spacerItem1;
    QPushButton *btnCancel;
    QPushButton *btnOk;

    void setupUi(QDialog *EmployeUserDialog)
    {
        if (EmployeUserDialog->objectName().isEmpty())
            EmployeUserDialog->setObjectName("EmployeUserDialog");
        EmployeUserDialog->resize(600, 520);
        EmployeUserDialog->setStyleSheet(QString::fromUtf8("\n"
"QDialog#EmployeUserDialog {\n"
"  background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"}\n"
"QLabel {\n"
"  color: #9bb0c3;\n"
"  font-size: 13px;\n"
"  font-weight: 700;\n"
"  background: transparent;\n"
"}\n"
"QLineEdit, QComboBox, QDoubleSpinBox, QDateEdit {\n"
"  background-color: #132437;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 8px;\n"
"  padding: 8px 12px;\n"
"  color: #e6eef6;\n"
"  font-size: 13px;\n"
"  min-height: 30px;\n"
"  selection-background-color: #1e8de0;\n"
"}\n"
"QLineEdit:focus, QComboBox:focus, QDoubleSpinBox:focus, QDateEdit:focus {\n"
"  border-color: #39c0fa;\n"
"}\n"
"QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {\n"
"  background: #23374e;\n"
"  border: none;\n"
"  width: 20px;\n"
"}\n"
"QDoubleSpinBox::up-button:hover, QDoubleSpinBox::down-button:hover {\n"
"  background: #39c0fa;\n"
"}\n"
"QComboBox::drop-down {\n"
"  border: 0px;\n"
"  width: 24px;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"  background-color: #132437;\n"
"  color: #e6eef6;\n"
"  selec"
                        "tion-background-color: #1e8de0;\n"
"  border: 1px solid #23374e;\n"
"}\n"
"QDateEdit::drop-down {\n"
"  border: 0px;\n"
"  width: 24px;\n"
"}\n"
"#btnOk {\n"
"  background-color: #39c0fa;\n"
"  color: #0c1a29;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 10px 30px;\n"
"  font-size: 14px;\n"
"  font-weight: 700;\n"
"  min-width: 120px;\n"
"}\n"
"#btnOk:hover {\n"
"  background-color: #56cdfc;\n"
"}\n"
"#btnCancel {\n"
"  background-color: #23374e;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #2b4561;\n"
"  border-radius: 10px;\n"
"  padding: 10px 30px;\n"
"  font-size: 14px;\n"
"  font-weight: 700;\n"
"  min-width: 120px;\n"
"}\n"
"#btnCancel:hover {\n"
"  background-color: #2b4561;\n"
"}\n"
"  "));
        vboxLayout = new QVBoxLayout(EmployeUserDialog);
        vboxLayout->setSpacing(14);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(24, 20, 24, 20);
        label = new QLabel(EmployeUserDialog);
        label->setObjectName("label");
        label->setStyleSheet(QString::fromUtf8("color:#39c0fa;font-size:20px;font-weight:700;"));

        vboxLayout->addWidget(label);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(16);
        hboxLayout->setObjectName("hboxLayout");
        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setObjectName("vboxLayout1");
        label1 = new QLabel(EmployeUserDialog);
        label1->setObjectName("label1");

        vboxLayout1->addWidget(label1);

        lineCIN = new QLineEdit(EmployeUserDialog);
        lineCIN->setObjectName("lineCIN");

        vboxLayout1->addWidget(lineCIN);


        hboxLayout->addLayout(vboxLayout1);

        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setObjectName("vboxLayout2");
        label2 = new QLabel(EmployeUserDialog);
        label2->setObjectName("label2");

        vboxLayout2->addWidget(label2);

        lineNom = new QLineEdit(EmployeUserDialog);
        lineNom->setObjectName("lineNom");

        vboxLayout2->addWidget(lineNom);


        hboxLayout->addLayout(vboxLayout2);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(16);
        hboxLayout1->setObjectName("hboxLayout1");
        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setObjectName("vboxLayout3");
        label3 = new QLabel(EmployeUserDialog);
        label3->setObjectName("label3");

        vboxLayout3->addWidget(label3);

        linePrenom = new QLineEdit(EmployeUserDialog);
        linePrenom->setObjectName("linePrenom");

        vboxLayout3->addWidget(linePrenom);


        hboxLayout1->addLayout(vboxLayout3);

        vboxLayout4 = new QVBoxLayout();
        vboxLayout4->setObjectName("vboxLayout4");
        label4 = new QLabel(EmployeUserDialog);
        label4->setObjectName("label4");

        vboxLayout4->addWidget(label4);

        lineEmail = new QLineEdit(EmployeUserDialog);
        lineEmail->setObjectName("lineEmail");

        vboxLayout4->addWidget(lineEmail);


        hboxLayout1->addLayout(vboxLayout4);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(16);
        hboxLayout2->setObjectName("hboxLayout2");
        vboxLayout5 = new QVBoxLayout();
        vboxLayout5->setObjectName("vboxLayout5");
        label5 = new QLabel(EmployeUserDialog);
        label5->setObjectName("label5");

        vboxLayout5->addWidget(label5);

        lineLogin = new QLineEdit(EmployeUserDialog);
        lineLogin->setObjectName("lineLogin");

        vboxLayout5->addWidget(lineLogin);


        hboxLayout2->addLayout(vboxLayout5);

        vboxLayout6 = new QVBoxLayout();
        vboxLayout6->setObjectName("vboxLayout6");
        label6 = new QLabel(EmployeUserDialog);
        label6->setObjectName("label6");

        vboxLayout6->addWidget(label6);

        linePassword = new QLineEdit(EmployeUserDialog);
        linePassword->setObjectName("linePassword");

        vboxLayout6->addWidget(linePassword);


        hboxLayout2->addLayout(vboxLayout6);


        vboxLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(16);
        hboxLayout3->setObjectName("hboxLayout3");
        vboxLayout7 = new QVBoxLayout();
        vboxLayout7->setObjectName("vboxLayout7");
        label7 = new QLabel(EmployeUserDialog);
        label7->setObjectName("label7");

        vboxLayout7->addWidget(label7);

        comboRole = new QComboBox(EmployeUserDialog);
        comboRole->addItem(QString());
        comboRole->addItem(QString());
        comboRole->addItem(QString());
        comboRole->addItem(QString());
        comboRole->addItem(QString());
        comboRole->addItem(QString());
        comboRole->setObjectName("comboRole");

        vboxLayout7->addWidget(comboRole);


        hboxLayout3->addLayout(vboxLayout7);

        vboxLayout8 = new QVBoxLayout();
        vboxLayout8->setObjectName("vboxLayout8");
        label8 = new QLabel(EmployeUserDialog);
        label8->setObjectName("label8");

        vboxLayout8->addWidget(label8);

        comboStatut = new QComboBox(EmployeUserDialog);
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->setObjectName("comboStatut");

        vboxLayout8->addWidget(comboStatut);


        hboxLayout3->addLayout(vboxLayout8);


        vboxLayout->addLayout(hboxLayout3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(16);
        hboxLayout4->setObjectName("hboxLayout4");
        vboxLayout9 = new QVBoxLayout();
        vboxLayout9->setObjectName("vboxLayout9");
        label9 = new QLabel(EmployeUserDialog);
        label9->setObjectName("label9");

        vboxLayout9->addWidget(label9);

        spinHeures = new QDoubleSpinBox(EmployeUserDialog);
        spinHeures->setObjectName("spinHeures");
        spinHeures->setMaximum(9999.000000000000000);
        spinHeures->setDecimals(1);

        vboxLayout9->addWidget(spinHeures);


        hboxLayout4->addLayout(vboxLayout9);


        vboxLayout->addLayout(hboxLayout4);

        spacerItem = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vboxLayout->addItem(spacerItem);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(14);
        hboxLayout5->setObjectName("hboxLayout5");
        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout5->addItem(spacerItem1);

        btnCancel = new QPushButton(EmployeUserDialog);
        btnCancel->setObjectName("btnCancel");
        btnCancel->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        hboxLayout5->addWidget(btnCancel);

        btnOk = new QPushButton(EmployeUserDialog);
        btnOk->setObjectName("btnOk");
        btnOk->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        hboxLayout5->addWidget(btnOk);


        vboxLayout->addLayout(hboxLayout5);


        retranslateUi(EmployeUserDialog);

        QMetaObject::connectSlotsByName(EmployeUserDialog);
    } // setupUi

    void retranslateUi(QDialog *EmployeUserDialog)
    {
        EmployeUserDialog->setWindowTitle(QCoreApplication::translate("EmployeUserDialog", "Utilisateur", nullptr));
        label->setText(QCoreApplication::translate("EmployeUserDialog", "Informations de l'employe", nullptr));
        label1->setText(QCoreApplication::translate("EmployeUserDialog", "CIN", nullptr));
        lineCIN->setPlaceholderText(QCoreApplication::translate("EmployeUserDialog", "N\302\260 CIN", nullptr));
        label2->setText(QCoreApplication::translate("EmployeUserDialog", "Nom", nullptr));
        lineNom->setPlaceholderText(QCoreApplication::translate("EmployeUserDialog", "Nom de famille", nullptr));
        label3->setText(QCoreApplication::translate("EmployeUserDialog", "Pr\303\251nom", nullptr));
        linePrenom->setPlaceholderText(QCoreApplication::translate("EmployeUserDialog", "Pr\303\251nom", nullptr));
        label4->setText(QCoreApplication::translate("EmployeUserDialog", "Email", nullptr));
        lineEmail->setPlaceholderText(QCoreApplication::translate("EmployeUserDialog", "email@example.com", nullptr));
        label5->setText(QCoreApplication::translate("EmployeUserDialog", "Login", nullptr));
        lineLogin->setPlaceholderText(QCoreApplication::translate("EmployeUserDialog", "Nom d'utilisateur", nullptr));
        label6->setText(QCoreApplication::translate("EmployeUserDialog", "Mot de passe", nullptr));
        linePassword->setPlaceholderText(QCoreApplication::translate("EmployeUserDialog", "Mot de passe", nullptr));
        label7->setText(QCoreApplication::translate("EmployeUserDialog", "R\303\264le", nullptr));
        comboRole->setItemText(0, QCoreApplication::translate("EmployeUserDialog", "RH (Ressources Humaines)", nullptr));
        comboRole->setItemText(1, QCoreApplication::translate("EmployeUserDialog", "Agent d'exploitation", nullptr));
        comboRole->setItemText(2, QCoreApplication::translate("EmployeUserDialog", "Responsable stock", nullptr));
        comboRole->setItemText(3, QCoreApplication::translate("EmployeUserDialog", "Responsable finance", nullptr));
        comboRole->setItemText(4, QCoreApplication::translate("EmployeUserDialog", "ouvrier", nullptr));
        comboRole->setItemText(5, QCoreApplication::translate("EmployeUserDialog", "admin", nullptr));

        label8->setText(QCoreApplication::translate("EmployeUserDialog", "Statut", nullptr));
        comboStatut->setItemText(0, QCoreApplication::translate("EmployeUserDialog", "Actif", nullptr));
        comboStatut->setItemText(1, QCoreApplication::translate("EmployeUserDialog", "Inactif", nullptr));

        label9->setText(QCoreApplication::translate("EmployeUserDialog", "Heures travaill\303\251es", nullptr));
        btnCancel->setText(QCoreApplication::translate("EmployeUserDialog", "Annuler", nullptr));
        btnOk->setText(QCoreApplication::translate("EmployeUserDialog", "Enregistrer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmployeUserDialog: public Ui_EmployeUserDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYE_USERDIALOG_H
