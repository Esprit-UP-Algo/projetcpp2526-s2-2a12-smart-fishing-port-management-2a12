/********************************************************************************
** Form generated from reading UI file 'transactiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSACTIONDIALOG_H
#define UI_TRANSACTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TransactionDialog
{
public:
    QVBoxLayout *mainLayout;
    QLabel *dialogTitle;
    QLabel *dialogSubtitle;
    QFormLayout *formLayout;
    QLabel *lblNumFacture;
    QLineEdit *lineNumFacture;
    QLabel *lblPecheur;
    QLineEdit *linePecheur;
    QLabel *lblLot;
    QLineEdit *lineLot;
    QLabel *lblPrixKg;
    QDoubleSpinBox *spinPrixKg;
    QLabel *lblQuantite;
    QDoubleSpinBox *spinQuantite;
    QLabel *lblTotal;
    QLabel *totalValue;
    QLabel *lblDate;
    QDateTimeEdit *dateTransaction;
    QLabel *lblModePaiement;
    QComboBox *comboModePaiement;
    QLabel *lblStatutPaiement;
    QComboBox *comboStatutPaiement;
    QSpacerItem *bottomSpacer;
    QHBoxLayout *buttonLayout;
    QSpacerItem *btnSpacerL;
    QPushButton *btnCancel;
    QPushButton *btnOk;
    QSpacerItem *btnSpacerR;

    void setupUi(QDialog *TransactionDialog)
    {
        if (TransactionDialog->objectName().isEmpty())
            TransactionDialog->setObjectName("TransactionDialog");
        TransactionDialog->resize(520, 640);
        TransactionDialog->setMinimumSize(QSize(460, 580));
        TransactionDialog->setStyleSheet(QString::fromUtf8("\n"
"QDialog#TransactionDialog {\n"
"  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    stop:0 #eaf4fb, stop:0.5 #dceef8, stop:1 #cbdfef);\n"
"}\n"
"QLabel {\n"
"  color: #1a2a3a;\n"
"  font-size: 13px;\n"
"  font-weight: 700;\n"
"  background: transparent;\n"
"}\n"
"QLabel#dialogTitle {\n"
"  color: #0f1f2e;\n"
"  font-size: 22px;\n"
"  font-weight: 800;\n"
"}\n"
"QLabel#dialogSubtitle {\n"
"  color: #5a7a94;\n"
"  font-size: 12px;\n"
"  font-weight: 400;\n"
"}\n"
"QLabel#totalValue {\n"
"  color: #1a8de0;\n"
"  font-size: 16px;\n"
"  font-weight: bold;\n"
"  background-color: #1a2a3a;\n"
"  border: 1px solid #2a3f55;\n"
"  border-radius: 8px;\n"
"  padding: 6px 12px;\n"
"}\n"
"QLineEdit {\n"
"  background-color: #1a2a3a;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #2a3f55;\n"
"  border-radius: 8px;\n"
"  padding: 8px 12px;\n"
"  min-height: 30px;\n"
"  font-size: 13px;\n"
"  font-weight: 400;\n"
"  selection-background-color: #1e8de0;\n"
"}\n"
"QLineEdit:focus {\n"
"  border-color: #39a8d8;\n"
""
                        "}\n"
"QDoubleSpinBox {\n"
"  background-color: #1a2a3a;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #2a3f55;\n"
"  border-radius: 8px;\n"
"  padding: 8px 12px;\n"
"  min-height: 30px;\n"
"  font-size: 13px;\n"
"  font-weight: 400;\n"
"}\n"
"QDoubleSpinBox:focus {\n"
"  border-color: #39a8d8;\n"
"}\n"
"QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {\n"
"  background: #2a3f55;\n"
"  border: none;\n"
"  width: 20px;\n"
"}\n"
"QDoubleSpinBox::up-button:hover, QDoubleSpinBox::down-button:hover {\n"
"  background: #39a8d8;\n"
"}\n"
"QDateTimeEdit {\n"
"  background-color: #1a2a3a;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #2a3f55;\n"
"  border-radius: 8px;\n"
"  padding: 8px 12px;\n"
"  min-height: 30px;\n"
"  font-size: 13px;\n"
"  font-weight: 400;\n"
"}\n"
"QDateTimeEdit:focus {\n"
"  border-color: #39a8d8;\n"
"}\n"
"QDateTimeEdit::drop-down {\n"
"  border: 0px;\n"
"  width: 24px;\n"
"}\n"
"QComboBox {\n"
"  background-color: #1a2a3a;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #2a3f55;\n"
"  bor"
                        "der-radius: 8px;\n"
"  padding: 8px 12px;\n"
"  min-height: 30px;\n"
"  font-size: 13px;\n"
"  font-weight: 400;\n"
"}\n"
"QComboBox:focus {\n"
"  border-color: #39a8d8;\n"
"}\n"
"QComboBox::drop-down {\n"
"  border: 0px;\n"
"  width: 24px;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"  background-color: #1a2a3a;\n"
"  color: #e6eef6;\n"
"  selection-background-color: #1e8de0;\n"
"  border: 1px solid #2a3f55;\n"
"}\n"
"#btnOk {\n"
"  background-color: #2da4d8;\n"
"  color: white;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 11px 32px;\n"
"  font-size: 14px;\n"
"  font-weight: 700;\n"
"  min-width: 120px;\n"
"}\n"
"#btnOk:hover {\n"
"  background-color: #3dbae8;\n"
"}\n"
"#btnCancel {\n"
"  background-color: #2da4d8;\n"
"  color: white;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 11px 32px;\n"
"  font-size: 14px;\n"
"  font-weight: 700;\n"
"  min-width: 120px;\n"
"}\n"
"#btnCancel:hover {\n"
"  background-color: #3dbae8;\n"
"}\n"
"   "));
        mainLayout = new QVBoxLayout(TransactionDialog);
        mainLayout->setSpacing(14);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(24, 20, 24, 20);
        dialogTitle = new QLabel(TransactionDialog);
        dialogTitle->setObjectName("dialogTitle");
        dialogTitle->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        mainLayout->addWidget(dialogTitle);

        dialogSubtitle = new QLabel(TransactionDialog);
        dialogSubtitle->setObjectName("dialogSubtitle");
        dialogSubtitle->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        mainLayout->addWidget(dialogSubtitle);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setLabelAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        formLayout->setHorizontalSpacing(24);
        formLayout->setVerticalSpacing(20);
        lblNumFacture = new QLabel(TransactionDialog);
        lblNumFacture->setObjectName("lblNumFacture");

        formLayout->setWidget(0, QFormLayout::LabelRole, lblNumFacture);

        lineNumFacture = new QLineEdit(TransactionDialog);
        lineNumFacture->setObjectName("lineNumFacture");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineNumFacture);

        lblPecheur = new QLabel(TransactionDialog);
        lblPecheur->setObjectName("lblPecheur");

        formLayout->setWidget(1, QFormLayout::LabelRole, lblPecheur);

        linePecheur = new QLineEdit(TransactionDialog);
        linePecheur->setObjectName("linePecheur");

        formLayout->setWidget(1, QFormLayout::FieldRole, linePecheur);

        lblLot = new QLabel(TransactionDialog);
        lblLot->setObjectName("lblLot");

        formLayout->setWidget(2, QFormLayout::LabelRole, lblLot);

        lineLot = new QLineEdit(TransactionDialog);
        lineLot->setObjectName("lineLot");

        formLayout->setWidget(2, QFormLayout::FieldRole, lineLot);

        lblPrixKg = new QLabel(TransactionDialog);
        lblPrixKg->setObjectName("lblPrixKg");

        formLayout->setWidget(3, QFormLayout::LabelRole, lblPrixKg);

        spinPrixKg = new QDoubleSpinBox(TransactionDialog);
        spinPrixKg->setObjectName("spinPrixKg");
        spinPrixKg->setMinimum(0.000000000000000);
        spinPrixKg->setMaximum(999999999.000000000000000);
        spinPrixKg->setDecimals(2);
        spinPrixKg->setSingleStep(0.500000000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, spinPrixKg);

        lblQuantite = new QLabel(TransactionDialog);
        lblQuantite->setObjectName("lblQuantite");

        formLayout->setWidget(4, QFormLayout::LabelRole, lblQuantite);

        spinQuantite = new QDoubleSpinBox(TransactionDialog);
        spinQuantite->setObjectName("spinQuantite");
        spinQuantite->setMinimum(0.000000000000000);
        spinQuantite->setMaximum(999999999.000000000000000);
        spinQuantite->setDecimals(3);
        spinQuantite->setSingleStep(1.000000000000000);

        formLayout->setWidget(4, QFormLayout::FieldRole, spinQuantite);

        lblTotal = new QLabel(TransactionDialog);
        lblTotal->setObjectName("lblTotal");

        formLayout->setWidget(5, QFormLayout::LabelRole, lblTotal);

        totalValue = new QLabel(TransactionDialog);
        totalValue->setObjectName("totalValue");
        totalValue->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        formLayout->setWidget(5, QFormLayout::FieldRole, totalValue);

        lblDate = new QLabel(TransactionDialog);
        lblDate->setObjectName("lblDate");

        formLayout->setWidget(6, QFormLayout::LabelRole, lblDate);

        dateTransaction = new QDateTimeEdit(TransactionDialog);
        dateTransaction->setObjectName("dateTransaction");
        dateTransaction->setCalendarPopup(true);

        formLayout->setWidget(6, QFormLayout::FieldRole, dateTransaction);

        lblModePaiement = new QLabel(TransactionDialog);
        lblModePaiement->setObjectName("lblModePaiement");

        formLayout->setWidget(7, QFormLayout::LabelRole, lblModePaiement);

        comboModePaiement = new QComboBox(TransactionDialog);
        comboModePaiement->addItem(QString());
        comboModePaiement->addItem(QString());
        comboModePaiement->addItem(QString());
        comboModePaiement->setObjectName("comboModePaiement");

        formLayout->setWidget(7, QFormLayout::FieldRole, comboModePaiement);

        lblStatutPaiement = new QLabel(TransactionDialog);
        lblStatutPaiement->setObjectName("lblStatutPaiement");

        formLayout->setWidget(8, QFormLayout::LabelRole, lblStatutPaiement);

        comboStatutPaiement = new QComboBox(TransactionDialog);
        comboStatutPaiement->addItem(QString());
        comboStatutPaiement->addItem(QString());
        comboStatutPaiement->addItem(QString());
        comboStatutPaiement->setObjectName("comboStatutPaiement");

        formLayout->setWidget(8, QFormLayout::FieldRole, comboStatutPaiement);


        mainLayout->addLayout(formLayout);

        bottomSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(bottomSpacer);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(16);
        buttonLayout->setObjectName("buttonLayout");
        btnSpacerL = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(btnSpacerL);

        btnCancel = new QPushButton(TransactionDialog);
        btnCancel->setObjectName("btnCancel");

        buttonLayout->addWidget(btnCancel);

        btnOk = new QPushButton(TransactionDialog);
        btnOk->setObjectName("btnOk");

        buttonLayout->addWidget(btnOk);

        btnSpacerR = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(btnSpacerR);


        mainLayout->addLayout(buttonLayout);


        retranslateUi(TransactionDialog);

        QMetaObject::connectSlotsByName(TransactionDialog);
    } // setupUi

    void retranslateUi(QDialog *TransactionDialog)
    {
        TransactionDialog->setWindowTitle(QCoreApplication::translate("TransactionDialog", "Transaction", nullptr));
        dialogTitle->setText(QCoreApplication::translate("TransactionDialog", "Nouvelle Transaction", nullptr));
        dialogSubtitle->setText(QCoreApplication::translate("TransactionDialog", "Veuillez remplir les informations de la transaction.", nullptr));
        lblNumFacture->setText(QCoreApplication::translate("TransactionDialog", "Num Facture", nullptr));
        lineNumFacture->setPlaceholderText(QCoreApplication::translate("TransactionDialog", "FCT-100", nullptr));
        lblPecheur->setText(QCoreApplication::translate("TransactionDialog", "P\303\252cheur", nullptr));
        linePecheur->setPlaceholderText(QCoreApplication::translate("TransactionDialog", "Nom du p\303\252cheur", nullptr));
        lblLot->setText(QCoreApplication::translate("TransactionDialog", "Lot", nullptr));
        lineLot->setPlaceholderText(QCoreApplication::translate("TransactionDialog", "LOT-100", nullptr));
        lblPrixKg->setText(QCoreApplication::translate("TransactionDialog", "Prix / kg", nullptr));
        spinPrixKg->setSuffix(QCoreApplication::translate("TransactionDialog", " DT", nullptr));
        lblQuantite->setText(QCoreApplication::translate("TransactionDialog", "Quantit\303\251", nullptr));
        spinQuantite->setSuffix(QCoreApplication::translate("TransactionDialog", " kg", nullptr));
        lblTotal->setText(QCoreApplication::translate("TransactionDialog", "Total", nullptr));
        totalValue->setText(QCoreApplication::translate("TransactionDialog", "0,00 DT", nullptr));
        lblDate->setText(QCoreApplication::translate("TransactionDialog", "Date transaction", nullptr));
        dateTransaction->setDisplayFormat(QCoreApplication::translate("TransactionDialog", "yyyy-MM-dd HH:mm", nullptr));
        lblModePaiement->setText(QCoreApplication::translate("TransactionDialog", "Mode paiement", nullptr));
        comboModePaiement->setItemText(0, QCoreApplication::translate("TransactionDialog", "Esp\303\250ces", nullptr));
        comboModePaiement->setItemText(1, QCoreApplication::translate("TransactionDialog", "Ch\303\250que", nullptr));
        comboModePaiement->setItemText(2, QCoreApplication::translate("TransactionDialog", "Virement", nullptr));

        lblStatutPaiement->setText(QCoreApplication::translate("TransactionDialog", "Statut paiement", nullptr));
        comboStatutPaiement->setItemText(0, QCoreApplication::translate("TransactionDialog", "Pay\303\251", nullptr));
        comboStatutPaiement->setItemText(1, QCoreApplication::translate("TransactionDialog", "En attente", nullptr));
        comboStatutPaiement->setItemText(2, QCoreApplication::translate("TransactionDialog", "Annul\303\251", nullptr));

        btnCancel->setText(QCoreApplication::translate("TransactionDialog", "Annuler", nullptr));
        btnOk->setText(QCoreApplication::translate("TransactionDialog", "Enregistrer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TransactionDialog: public Ui_TransactionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSACTIONDIALOG_H
