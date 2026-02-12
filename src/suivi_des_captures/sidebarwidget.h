#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QPixmap>

class QFrame;
class QPropertyAnimation;

class SidebarWidget : public QWidget {
    Q_OBJECT
public:
    explicit SidebarWidget(QWidget *parent = nullptr);
    void setSelected(int index);
    int selectedIndex() const { return m_selectedIndex; }

signals:
    void menuClicked(int index);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void moveHighlightTo(int index, bool animate);
    QRect highlightTargetRect(const QPushButton *btn) const;

    QButtonGroup *buttonGroup;
    QList<QPushButton*> menuButtons;
    QLabel *logoLabel;
    QLabel *titleLabel;

    QFrame *m_highlight = nullptr;
    QPropertyAnimation *m_highlightAnim = nullptr;
    int m_selectedIndex = -1;
};

#endif // SIDEBARWIDGET_H
