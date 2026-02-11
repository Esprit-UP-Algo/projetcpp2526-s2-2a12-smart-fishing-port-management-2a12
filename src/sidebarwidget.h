#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QPixmap>

class SidebarWidget : public QWidget {
    Q_OBJECT
public:
    explicit SidebarWidget(QWidget *parent = nullptr);
    void setSelected(int index);

signals:
    void menuClicked(int index);

private:
    QButtonGroup *buttonGroup;
    QList<QPushButton*> menuButtons;
    QLabel *logoLabel;
    QLabel *titleLabel;
};

#endif // SIDEBARWIDGET_H
