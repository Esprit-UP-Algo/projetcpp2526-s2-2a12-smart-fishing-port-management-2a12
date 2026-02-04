#include "statcircle.h"
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>

StatCircleWidget::StatCircleWidget(QWidget *parent)
    : QWidget(parent), m_value(0.0), m_label("Occupation")
{
    setMinimumSize(120, 120);
}

void StatCircleWidget::setValue(double value)
{
    m_value = qBound(0.0, value, 1.0);
    update();
}

void StatCircleWidget::setLabel(const QString &label)
{
    m_label = label;
    update();
}

void StatCircleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int side = qMin(width(), height());
    const int margin = 10;
    const int diameter = side - 2 * margin;
    const int radius = diameter / 2;

    const QRect circleRect(margin, margin, diameter, diameter);
    const QPointF center(width() / 2.0, height() / 2.0);

    // Draw background circle
    painter.setPen(QPen(QColor(0x23, 0x37, 0x4e), 2));
    painter.setBrush(QBrush(QColor(0x10, 0x1c, 0x2c)));
    painter.drawEllipse(circleRect);

    // Draw progress arc
    painter.setPen(QPen(QColor(0x39, 0xc0, 0xfa), 3));
    painter.setBrush(Qt::NoBrush);

    const qreal startAngle = 90.0;
    const qreal spanAngle = -360.0 * m_value;
    painter.drawArc(circleRect, static_cast<int>(startAngle * 16), static_cast<int>(spanAngle * 16));

    // Draw percentage text (centered in circle)
    painter.setPen(QPen(QColor(0xe6, 0xee, 0xf6)));
    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    painter.setFont(font);

    const QString percentText = QString::number(m_value * 100.0, 'f', 1) + "%";
    const QFontMetrics fm(font);
    const int textWidth = fm.horizontalAdvance(percentText);
    const int textHeight = fm.height();

    painter.drawText(static_cast<int>(center.x() - textWidth / 2.0),
                     static_cast<int>(center.y() - textHeight / 2.0 + fm.ascent()),
                     percentText);

    // Draw label below circle
    font.setPointSize(10);
    font.setBold(false);
    painter.setFont(font);
    painter.setPen(QPen(QColor(0x9b, 0xb0, 0xc3)));

    const QFontMetrics fmLabel(font);
    const int labelWidth = fmLabel.horizontalAdvance(m_label);
    const int labelY = static_cast<int>(center.y() + radius + 15);
    
    painter.drawText(static_cast<int>(center.x() - labelWidth / 2.0),
                     labelY,
                     m_label);
}
