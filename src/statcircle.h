#pragma once

#include <QWidget>
#include <QColor>

class StatCircleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatCircleWidget(QWidget *parent = nullptr);

    void setValue(double v); // 0..1
    void setColor(const QColor &c) { m_color = c; update(); }
    void setLabel(const QString &t) { m_label = t; update(); }

protected:
    void paintEvent(QPaintEvent *ev) override;

private:
    double m_value = 0.0;
    QColor m_color = QColor(57, 192, 250);
    QString m_label;
};
