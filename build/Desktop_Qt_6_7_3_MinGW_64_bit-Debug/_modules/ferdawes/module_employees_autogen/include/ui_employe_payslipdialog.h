/********************************************************************************
** Form generated from reading UI file 'employe_payslipdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYE_PAYSLIPDIALOG_H
#define UI_EMPLOYE_PAYSLIPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EmployePayslipDialog
{
public:
    QVBoxLayout *vboxLayout;
    QFrame *payslipCard;
    QVBoxLayout *vboxLayout1;
    QLabel *lblPayTitle;
    QLabel *lblPaySub;
    QLabel *lblPayInfo;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout;
    QPushButton *btnExportPDF;
    QPushButton *btnExportExcelDlg;
    QPushButton *btnCloseDlg;

    void setupUi(QDialog *EmployePayslipDialog)
    {
        if (EmployePayslipDialog->objectName().isEmpty())
            EmployePayslipDialog->setObjectName("EmployePayslipDialog");
        EmployePayslipDialog->resize(500, 400);
        EmployePayslipDialog->setMinimumSize(QSize(460, 360));
        EmployePayslipDialog->setStyleSheet(QString::fromUtf8("\n"
"QDialog{background:#F0F2F5;font-family:\"Segoe UI\",\"Roboto\",Arial;}\n"
"#payslipCard{background:#FFFFFF;border:1px solid #E0E0E0;border-radius:16px;}\n"
"QLabel{color:#2C3E50;font-size:13px;font-weight:600;}\n"
"#lblPayTitle{font-size:20px;font-weight:800;color:#2C3E50;}\n"
"#lblPaySub{color:#7F8C8D;font-size:12px;font-weight:400;}\n"
"#lblPayInfo{color:#7F8C8D;font-size:13px;font-weight:400;padding:8px;background:#F8F9FA;border:1px solid #E0E0E0;border-radius:8px;}\n"
"#btnExportPDF{background:#E67E22;color:#FFF;border:none;border-radius:10px;font-size:14px;font-weight:700;min-height:42px;padding:10px 24px;}\n"
"#btnExportPDF:hover{background:#F39C12;}\n"
"#btnExportExcelDlg{background:#1E8449;color:#FFF;border:none;border-radius:10px;font-size:14px;font-weight:700;min-height:42px;padding:10px 24px;}\n"
"#btnExportExcelDlg:hover{background:#27AE60;}\n"
"#btnCloseDlg{background:transparent;color:#7F8C8D;border:1px solid #BDC3C7;border-radius:10px;font-size:13px;font-weight:600;min-height:38px;padding:8"
                        "px 20px;}\n"
"#btnCloseDlg:hover{background:#F8F9FA;}\n"
"  "));
        vboxLayout = new QVBoxLayout(EmployePayslipDialog);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(20, 20, 20, 20);
        payslipCard = new QFrame(EmployePayslipDialog);
        payslipCard->setObjectName("payslipCard");
        payslipCard->setFrameShape(QFrame::StyledPanel);
        vboxLayout1 = new QVBoxLayout(payslipCard);
        vboxLayout1->setSpacing(14);
        vboxLayout1->setObjectName("vboxLayout1");
        vboxLayout1->setContentsMargins(24, 24, 24, 24);
        lblPayTitle = new QLabel(payslipCard);
        lblPayTitle->setObjectName("lblPayTitle");

        vboxLayout1->addWidget(lblPayTitle);

        lblPaySub = new QLabel(payslipCard);
        lblPaySub->setObjectName("lblPaySub");

        vboxLayout1->addWidget(lblPaySub);

        lblPayInfo = new QLabel(payslipCard);
        lblPayInfo->setObjectName("lblPayInfo");

        vboxLayout1->addWidget(lblPayInfo);

        spacerItem = new QSpacerItem(0, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        vboxLayout1->addItem(spacerItem);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(10);
        hboxLayout->setObjectName("hboxLayout");
        btnExportPDF = new QPushButton(payslipCard);
        btnExportPDF->setObjectName("btnExportPDF");
        btnExportPDF->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        hboxLayout->addWidget(btnExportPDF);

        btnExportExcelDlg = new QPushButton(payslipCard);
        btnExportExcelDlg->setObjectName("btnExportExcelDlg");
        btnExportExcelDlg->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        hboxLayout->addWidget(btnExportExcelDlg);


        vboxLayout1->addLayout(hboxLayout);

        btnCloseDlg = new QPushButton(payslipCard);
        btnCloseDlg->setObjectName("btnCloseDlg");
        btnCloseDlg->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        vboxLayout1->addWidget(btnCloseDlg);


        vboxLayout->addWidget(payslipCard);


        retranslateUi(EmployePayslipDialog);

        QMetaObject::connectSlotsByName(EmployePayslipDialog);
    } // setupUi

    void retranslateUi(QDialog *EmployePayslipDialog)
    {
        EmployePayslipDialog->setWindowTitle(QCoreApplication::translate("EmployePayslipDialog", "Fiche de Paie", nullptr));
        lblPayTitle->setText(QCoreApplication::translate("EmployePayslipDialog", "\360\237\222\260 Fiche de Paie", nullptr));
        lblPaySub->setText(QCoreApplication::translate("EmployePayslipDialog", "G\303\251n\303\251rez et exportez la fiche de paie de l'employ\303\251 s\303\251lectionn\303\251", nullptr));
        lblPayInfo->setText(QCoreApplication::translate("EmployePayslipDialog", "Employ\303\251 : \342\200\224\n"
"Mois : \342\200\224\n"
"Heures : \342\200\224", nullptr));
        btnExportPDF->setText(QCoreApplication::translate("EmployePayslipDialog", "\360\237\223\204 Exporter PDF", nullptr));
        btnExportExcelDlg->setText(QCoreApplication::translate("EmployePayslipDialog", "\360\237\223\212 Exporter Excel", nullptr));
        btnCloseDlg->setText(QCoreApplication::translate("EmployePayslipDialog", "Fermer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmployePayslipDialog: public Ui_EmployePayslipDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYE_PAYSLIPDIALOG_H
