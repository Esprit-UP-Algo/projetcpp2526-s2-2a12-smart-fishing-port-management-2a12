#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "navire.h"
#include "dialognavire.h"
#include "dialogmouvement.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAjouterClicked();
    void onModifierClicked();
    void onSupprimerClicked();
    void onSearchChanged();
    void onSortChanged(int index);
    void onExportPDF();
    void onFetchWeather();
    void onWeatherReply(QNetworkReply *reply);
    void onAddMouvement();
    void onTableSelectionChanged();
    void onSelectAllChanged(int state);
    void updateStats();
    void onCalendarDateClicked(const QDate &date);
    void onSidebarButtonClicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    QList<Navire> navires;
    QList<Mouvement> mouvements;
    int nextId;
    QNetworkAccessManager *networkManager;

    void loadSampleData();
    void refreshTable();
    void refreshMouvementsTable();
    void refreshMouvementsForDate(const QDate &date);
    void highlightCalendarDates();
    void applyFilters();
    void paintDonutRing(QFrame *frame, double ratio, const QColor &arcColor = QColor(0x39,0xc0,0xfa), const QColor &bgArc = QColor(0x23,0x37,0x4e));
};

#endif // MAINWINDOW_H
