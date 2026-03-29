#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPropertyAnimation>
#include <QMenu>
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
    void onGestionQuaisClicked();
    // onSidebarButtonClicked supprimé : boutons autres modules sans connect (clic inactif)

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;

    class QStackedWidget *m_pages = nullptr;
    QWidget *m_naviresPage = nullptr;
    class QuaiWindow *m_quaiPage = nullptr;

    QList<Navire> navires;
    QList<Mouvement> mouvements;
    int nextId;
    QNetworkAccessManager *networkManager;

    bool m_naviresMenuExpanded = false;
    QPropertyAnimation *m_naviresMenuAnim = nullptr;

    QMenu *m_naviresPopupMenu = nullptr;
    QAction *m_naviresActAffichage = nullptr;
    QAction *m_naviresActStatistique = nullptr;
    QAction *m_naviresActAvancees = nullptr;

    void setNaviresMenuExpanded(bool expanded, bool animated);
    void setActiveNaviresOption(QPushButton *active);
    void onToggleNaviresMenu();
    void onNaviresAffichage();
    void onNaviresStatistique();
    void onNaviresAvancees();

    void loadSampleData();
    bool reloadNaviresFromDb(QString *errorMessage = nullptr);
    bool insertNavireToDb(Navire &navire, QString *errorMessage = nullptr);
    bool updateNavireInDb(const Navire &navire, const QString &whereMatricule, QString *errorMessage = nullptr);
    bool deleteNavireFromDb(int id, const QString &matricule, QString *errorMessage = nullptr);
    void refreshTable();
    void refreshMouvementsTable();
    void refreshMouvementsForDate(const QDate &date);
    void highlightCalendarDates();
    void applyFilters();
    void paintDonutRing(QFrame *frame, double ratio, const QColor &arcColor = QColor(0x39,0xc0,0xfa), const QColor &bgArc = QColor(0x23,0x37,0x4e));

    void setNaviresAffichageOnly();
    void setNaviresStatsOnly(bool enabled);
    void setNaviresAdvancedOnly(bool enabled);

    void ensureIntegratedPages();
    void showNaviresPage();
    void showQuaisPage();
};

#endif // MAINWINDOW_H
