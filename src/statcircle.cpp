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
    const int penWidth = 10; // Progress bar thickness
    
    // Rectangle for the arc (inset by margin + half pen width to avoid clipping)
    const int offset = margin + penWidth / 2;
    const QRectF circleRect(offset, offset, side - 2 * offset, side - 2 * offset);
    
    const QPointF center(side / 2.0, side / 2.0);

    // Draw background circle (track)
    painter.setPen(QPen(QColor(0x23, 0x37, 0x4e), penWidth)); 
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(circleRect);

    // Draw progress arc
    painter.setPen(QPen(QColor(0x39, 0xc0, 0xfa), penWidth, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(Qt::NoBrush);

    const qreal startAngle = 90.0 * 16;
    const qreal spanAngle = -360.0 * m_value * 16;
    painter.drawArc(circleRect, static_cast<int>(startAngle), static_cast<int>(spanAngle));

    // Draw percentage text (centered in circle)
    painter.setPen(QPen(QColor(0xe6, 0xee, 0xf6)));
    QFont font = painter.font();
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);

    const QString percentText = QString::number(m_value * 100.0, 'f', 1) + "%";
    const QFontMetrics fm(font);
    const int textWidth = fm.horizontalAdvance(percentText);
    const int textHeight = fm.height(); // or ascent
    
    // Draw text centered
    painter.drawText(circleRect, Qt::AlignCenter, percentText);

    // Draw label below circle (if possible, or inside below percentage)
    font.setPointSize(10);
    font.setBold(false);
    painter.setFont(font);
    painter.setPen(QPen(QColor(0x9b, 0xb0, 0xc3)));
    
    const QRectF labelRect(0, center.y() + 15, side, 30);
    painter.drawText(labelRect, Qt::AlignCenter, m_label);
}
