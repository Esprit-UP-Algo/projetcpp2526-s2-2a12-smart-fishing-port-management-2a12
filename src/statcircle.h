#ifndef STATCIRCLE_H
#define STATCIRCLE_H

#include <QWidget>

class StatCircleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatCircleWidget(QWidget *parent = nullptr);

    void setValue(double value); // 0.0 to 1.0
    void setLabel(const QString &label);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_value;
    QString m_label;
};

#endif // STATCIRCLE_H
