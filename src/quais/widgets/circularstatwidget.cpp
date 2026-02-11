#include "circularstatwidget.h"

#include <QPainter>
#include <QPaintEvent>

CircularStatWidget::CircularStatWidget(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(220, 220);
}

void CircularStatWidget::setTitle(const QString &title)
{
    if (m_title == title)
        return;
    m_title = title;
    update();
}

void CircularStatWidget::setValueText(const QString &valueText)
{
    if (m_valueText == valueText)
        return;
    m_valueText = valueText;
    update();
}

void CircularStatWidget::setSubText(const QString &subText)
{
    if (m_subText == subText)
        return;
    m_subText = subText;
    update();
}

void CircularStatWidget::setProgress(double progress)
{
    if (progress < 0.0)
        progress = -1.0;
    if (progress > 1.0)
        progress = 1.0;
    if (qFuzzyCompare(m_progress, progress))
        return;
    m_progress = progress;
    update();
}

void CircularStatWidget::setAccentColor(const QColor &accent)
{
    if (m_accent == accent)
        return;
    m_accent = accent;
    update();
}

void CircularStatWidget::setDualSegments(const QString &labelA,
                                        double valueA,
                                        const QColor &colorA,
                                        const QString &labelB,
                                        double valueB,
                                        const QColor &colorB)
{
    m_dualEnabled = true;
    m_labelA = labelA;
    m_labelB = labelB;
    m_valueA = qMax(0.0, valueA);
    m_valueB = qMax(0.0, valueB);
    m_colorA = colorA;
    m_colorB = colorB;
    update();
}

void CircularStatWidget::setDualLegendFormat(int decimals, const QString &suffix)
{
    m_dualDecimals = qMax(0, decimals);
    m_dualSuffix = suffix;
    update();
}

void CircularStatWidget::clearDualSegments()
{
    if (!m_dualEnabled)
        return;
    m_dualEnabled = false;
    update();
}

QSize CircularStatWidget::sizeHint() const
{
    return QSize(240, 240);
}

void CircularStatWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRectF r = rect().adjusted(10, 10, -10, -10);
    const qreal side = qMax<qreal>(0.0, qMin(r.width(), r.height()));
    const QRectF square(r.center().x() - side / 2.0, r.center().y() - side / 2.0, side, side);

    // Ring geometry
    const qreal thickness = qMax<qreal>(10.0, square.width() * 0.12);
    QRectF ringRect = square;
    ringRect.adjust(thickness / 2.0, thickness / 2.0, -thickness / 2.0, -thickness / 2.0);

    const QColor track = QColor(255, 255, 255, 22);
    const QColor accent = m_accent;

    // Track
    QPen trackPen(track, thickness, Qt::SolidLine, Qt::RoundCap);
    p.setPen(trackPen);
    p.drawArc(ringRect, 0, 360 * 16);

    const int startAngle = 90 * 16;

    // Progress (single or dual)
    if (m_dualEnabled)
    {
        const double total = m_valueA + m_valueB;
        if (total > 0.0)
        {
            const double a = m_valueA / total;
            const double b = m_valueB / total;

            int cursor = startAngle;
            {
                QPen penA(m_colorA, thickness, Qt::SolidLine, Qt::RoundCap);
                p.setPen(penA);
                const int spanA = static_cast<int>(-a * 360.0 * 16.0);
                p.drawArc(ringRect, cursor, spanA);
                cursor += spanA;
            }
            {
                QPen penB(m_colorB, thickness, Qt::SolidLine, Qt::RoundCap);
                p.setPen(penB);
                const int spanB = static_cast<int>(-b * 360.0 * 16.0);
                p.drawArc(ringRect, cursor, spanB);
            }
        }
    }
    else if (m_progress >= 0.0)
    {
        QPen progressPen(accent, thickness, Qt::SolidLine, Qt::RoundCap);
        p.setPen(progressPen);
        const int spanAngle = static_cast<int>(-m_progress * 360.0 * 16.0);
        p.drawArc(ringRect, startAngle, spanAngle);
    }

    // Text
    const QColor textMain = QColor("#e6eef6");
    const QColor textMuted = QColor("#9bb0c3");

    const QRectF innerRect = square.adjusted(thickness * 1.10, thickness * 1.10, -thickness * 1.10, -thickness * 1.10);

    // Value (single-mode only). In dual-mode we render two large lines instead.
    QFont valueFont = font();
    valueFont.setBold(true);
    valueFont.setPixelSize(qRound(qMax<qreal>(14.0, square.height() * 0.12)));

    // Title
    QFont titleFont = font();
    titleFont.setBold(true);
    titleFont.setPixelSize(qRound(qMax<qreal>(10.0, square.height() * 0.060)));

    // Centered text layout inside the ring.
    const qreal gap = qMax<qreal>(4.0, square.height() * 0.015);
    if (m_dualEnabled)
    {
        QFont dualFont = valueFont;
        int px = dualFont.pixelSize();
        px = qMax(10, qRound(px * 0.70));
        dualFont.setPixelSize(px);

        const QString vA = QLocale().toString(m_valueA, 'f', m_dualDecimals) + m_dualSuffix;
        const QString vB = QLocale().toString(m_valueB, 'f', m_dualDecimals) + m_dualSuffix;
        const QString lineA = QStringLiteral("%1 %2").arg(m_labelA, vA);
        const QString lineB = QStringLiteral("%1 %2").arg(m_labelB, vB);

        p.setFont(titleFont);
        const QFontMetrics titleFm(titleFont);
        p.setFont(dualFont);
        const QFontMetrics dualFm(dualFont);

        const qreal titleH = titleFm.height();
        const qreal lineH = dualFm.height();
        const qreal blockH = titleH + gap + (2.0 * lineH) + gap;
        const qreal startY = innerRect.center().y() - (blockH / 2.0);

        const QRectF titleRect(innerRect.left(), startY, innerRect.width(), titleH);
        const QRectF line1(innerRect.left(), startY + titleH + gap, innerRect.width(), lineH);
        const QRectF line2(innerRect.left(), startY + titleH + gap + lineH + gap, innerRect.width(), lineH);

        p.setFont(titleFont);
        p.setPen(QColor("#39c0fa"));
        p.drawText(titleRect, Qt::AlignHCenter | Qt::AlignVCenter, m_title);

        p.setFont(dualFont);
        p.setPen(m_colorA);
        p.drawText(line1, Qt::AlignHCenter | Qt::AlignVCenter, lineA);
        p.setPen(textMain);
        p.drawText(line2, Qt::AlignHCenter | Qt::AlignVCenter, lineB);
    }
    else
    {
        QFont subFont = font();
        subFont.setPixelSize(qRound(qMax<qreal>(9.0, square.height() * 0.045)));

        p.setFont(titleFont);
        const QFontMetrics titleFm(titleFont);
        p.setFont(valueFont);
        const QFontMetrics valueFm(valueFont);
        p.setFont(subFont);
        const QFontMetrics subFm(subFont);

        const bool hasSub = !m_subText.isEmpty();
        const qreal titleH = titleFm.height();
        const qreal valueH = valueFm.height();
        const qreal subH = hasSub ? subFm.height() : 0.0;

        qreal blockH = titleH + gap + valueH;
        if (hasSub)
            blockH += gap + subH;

        const qreal startY = innerRect.center().y() - (blockH / 2.0);

        const QRectF titleRect(innerRect.left(), startY, innerRect.width(), titleH);
        const QRectF valueRect(innerRect.left(), startY + titleH + gap, innerRect.width(), valueH);
        const QRectF subRect(innerRect.left(), startY + titleH + gap + valueH + gap, innerRect.width(), subH);

        p.setFont(titleFont);
        p.setPen(accent);
        p.drawText(titleRect, Qt::AlignHCenter | Qt::AlignVCenter, m_title);

        p.setFont(valueFont);
        p.setPen(textMain);
        p.drawText(valueRect, Qt::AlignHCenter | Qt::AlignVCenter, m_valueText);

        if (hasSub)
        {
            p.setFont(subFont);
            p.setPen(textMuted);
            p.drawText(subRect, Qt::AlignHCenter | Qt::AlignVCenter, m_subText);
        }
    }
}
