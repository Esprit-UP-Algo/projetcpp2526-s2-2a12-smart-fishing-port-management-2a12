/********************************************************************************
** Form generated from reading UI file 'payslipdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAYSLIPDIALOG_H
#define UI_PAYSLIPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PayslipDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelUser;
    QComboBox *comboUser;
    QLabel *labelMonth;
    QComboBox *comboMonth;
    QLabel *labelHours;
    QSpinBox *spinHours;
    QLabel *labelRate;
    QDoubleSpinBox *spinRate;
    QHBoxLayout *buttonLayout;
    QPushButton *btnExportPDF;
    QPushButton *btnExportExcel;

    void setupUi(QDialog *PayslipDialog)
    {
        if (PayslipDialog->objectName().isEmpty())
            PayslipDialog->setObjectName("PayslipDialog");
        verticalLayout = new QVBoxLayout(PayslipDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelUser = new QLabel(PayslipDialog);
        labelUser->setObjectName("labelUser");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelUser);

        comboUser = new QComboBox(PayslipDialog);
        comboUser->setObjectName("comboUser");

        formLayout->setWidget(0, QFormLayout::FieldRole, comboUser);

        labelMonth = new QLabel(PayslipDialog);
        labelMonth->setObjectName("labelMonth");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelMonth);

        comboMonth = new QComboBox(PayslipDialog);
        comboMonth->setObjectName("comboMonth");

        formLayout->setWidget(1, QFormLayout::FieldRole, comboMonth);

        labelHours = new QLabel(PayslipDialog);
        labelHours->setObjectName("labelHours");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelHours);

        spinHours = new QSpinBox(PayslipDialog);
        spinHours->setObjectName("spinHours");

        formLayout->setWidget(2, QFormLayout::FieldRole, spinHours);

        labelRate = new QLabel(PayslipDialog);
        labelRate->setObjectName("labelRate");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelRate);

        spinRate = new QDoubleSpinBox(PayslipDialog);
        spinRate->setObjectName("spinRate");

        formLayout->setWidget(3, QFormLayout::FieldRole, spinRate);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        btnExportPDF = new QPushButton(PayslipDialog);
        btnExportPDF->setObjectName("btnExportPDF");

        buttonLayout->addWidget(btnExportPDF);

        btnExportExcel = new QPushButton(PayslipDialog);
        btnExportExcel->setObjectName("btnExportExcel");

        buttonLayout->addWidget(btnExportExcel);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(PayslipDialog);

        QMetaObject::connectSlotsByName(PayslipDialog);
    } // setupUi

    void retranslateUi(QDialog *PayslipDialog)
    {
        PayslipDialog->setWindowTitle(QCoreApplication::translate("PayslipDialog", "Fiche de paie", nullptr));
        labelUser->setText(QCoreApplication::translate("PayslipDialog", "Employ\303\251", nullptr));
        labelMonth->setText(QCoreApplication::translate("PayslipDialog", "Mois", nullptr));
        labelHours->setText(QCoreApplication::translate("PayslipDialog", "Heures", nullptr));
        labelRate->setText(QCoreApplication::translate("PayslipDialog", "Taux", nullptr));
        btnExportPDF->setText(QCoreApplication::translate("PayslipDialog", "Exporter PDF", nullptr));
        btnExportExcel->setText(QCoreApplication::translate("PayslipDialog", "Exporter Excel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PayslipDialog: public Ui_PayslipDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAYSLIPDIALOG_H
