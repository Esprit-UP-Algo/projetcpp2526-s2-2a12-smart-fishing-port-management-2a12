#include "statcircle.h"

#include <QPainter>
#include <QFontMetrics>

StatCircleWidget::StatCircleWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(120, 120);
}

void StatCircleWidget::setValue(double v)
{
    if (v < 0)
        v = 0;
    if (v > 1)
        v = 1;
    m_value = v;
    update();
}

void StatCircleWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int w = width();
    const int h = height();
    const int side = qMin(w, h);
    const QRectF rect((w - side) / 2 + 6, (h - side) / 2 + 6, side - 12, side - 12);

    // background track
    QPen penTrack(QColor(30, 40, 50), 10, Qt::SolidLine, Qt::FlatCap);
    p.setPen(penTrack);
    p.drawEllipse(rect);

    // progress arc
    QPen penProg(m_color, 10, Qt::SolidLine, Qt::FlatCap);
    p.setPen(penProg);
    const double span = 360.0 * m_value;
    // draw arc from 90 degrees (top) clockwise
    p.drawArc(rect.toRect(), 90 * 16, -span * 16);

    // percentage text
    QString percent = QString::number(int(m_value * 100 + 0.5)) + QStringLiteral(" %");
    QFont f = font();
    f.setBold(true);
    f.setPixelSize(side / 6);
    p.setFont(f);
    p.setPen(QColor(230, 238, 246));
    QFontMetrics fm(f);
    const int tw = fm.horizontalAdvance(percent);
    p.drawText((w - tw) / 2, h / 2 + fm.ascent() / 2, percent);

    // label below
    if (!m_label.isEmpty()) {
        QFont lf = font();
        lf.setPixelSize(side / 12);
        p.setFont(lf);
        p.setPen(QColor(155, 176, 195));
        QFontMetrics lfm(lf);
        const int lw = lfm.horizontalAdvance(m_label);
        p.drawText((w - lw) / 2, h / 2 + fm.ascent() / 2 + lfm.height(), m_label);
    }
}
