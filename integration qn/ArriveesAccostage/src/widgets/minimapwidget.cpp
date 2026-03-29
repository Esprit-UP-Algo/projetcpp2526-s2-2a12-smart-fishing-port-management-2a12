#include "minimapwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>

MiniMapWidget::MiniMapWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(160);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void MiniMapWidget::setPoints(const QVector<Point> &points)
{
    m_points = points;
    update();
}

void MiniMapWidget::setReferenceEnabled(bool enabled)
{
    if (m_refEnabled == enabled)
        return;
    m_refEnabled = enabled;
    update();
}

void MiniMapWidget::setReferencePoint(double x, double y)
{
    m_refValid = true;
    m_ref = QPointF(x, y);
    update();
}

bool MiniMapWidget::referenceEnabled() const
{
    return m_refEnabled;
}

QPointF MiniMapWidget::referencePoint() const
{
    return m_ref;
}

void MiniMapWidget::setPortIllustrationEnabled(bool enabled)
{
    if (m_portIllustrationEnabled == enabled)
        return;
    m_portIllustrationEnabled = enabled;
    update();
}

void MiniMapWidget::setHighlightedId(const QString &id)
{
    if (m_highlightedId == id)
        return;
    m_highlightedId = id;
    update();
}

QRectF MiniMapWidget::contentRect() const
{
    return rect().adjusted(10, 10, -10, -10);
}

QRectF MiniMapWidget::worldBounds() const
{
    if (m_points.isEmpty())
        return QRectF(-1, -1, 2, 2);

    double minX = m_points.first().x;
    double maxX = m_points.first().x;
    double minY = m_points.first().y;
    double maxY = m_points.first().y;

    for (const auto &p : m_points)
    {
        minX = qMin(minX, p.x);
        maxX = qMax(maxX, p.x);
        minY = qMin(minY, p.y);
        maxY = qMax(maxY, p.y);
    }

    if (m_refValid)
    {
        minX = qMin(minX, m_ref.x());
        maxX = qMax(maxX, m_ref.x());
        minY = qMin(minY, m_ref.y());
        maxY = qMax(maxY, m_ref.y());
    }

    const double w = qMax(1.0, maxX - minX);
    const double h = qMax(1.0, maxY - minY);
    const double padX = w * 0.12;
    const double padY = h * 0.12;

    return QRectF(minX - padX, minY - padY, w + 2 * padX, h + 2 * padY);
}

QPointF MiniMapWidget::worldToView(const QPointF &world, const QRectF &worldRect, const QRectF &viewRect) const
{
    const double nx = (world.x() - worldRect.left()) / worldRect.width();
    const double ny = (world.y() - worldRect.top()) / worldRect.height();

    const double x = viewRect.left() + nx * viewRect.width();
    const double y = viewRect.top() + ny * viewRect.height();
    return QPointF(x, y);
}

QPointF MiniMapWidget::viewToWorld(const QPointF &view, const QRectF &worldRect, const QRectF &viewRect) const
{
    const double nx = (view.x() - viewRect.left()) / viewRect.width();
    const double ny = (view.y() - viewRect.top()) / viewRect.height();

    const double x = worldRect.left() + nx * worldRect.width();
    const double y = worldRect.top() + ny * worldRect.height();
    return QPointF(x, y);
}

void MiniMapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRectF view = contentRect();

    // Background
    {
        QLinearGradient g(view.topLeft(), view.bottomRight());
        g.setColorAt(0, QColor(18, 40, 60));
        g.setColorAt(1, QColor(10, 24, 40));
        p.setPen(Qt::NoPen);
        p.setBrush(g);
        p.drawRoundedRect(view, 14, 14);

        p.setPen(QPen(QColor(255, 255, 255, 18), 1));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(view, 14, 14);
    }

    // Never allow drawing to extend past the rounded field.
    {
        QPainterPath clip;
        clip.addRoundedRect(view, 14, 14);
        p.setClipPath(clip);
    }

    const QRectF world = worldBounds();

    // Example: simple isometric ("3D") fishing port illustration.
    // Drawn under the occupancy points as a visual reference.
    if (m_portIllustrationEnabled)
    {
        const QRectF r = view.adjusted(30, 30, -30, -30);
        const QPointF c = r.center();
        const qreal side = qMax<qreal>(0.0, qMin(r.width(), r.height()));
        const qreal s = qMax<qreal>(6.0, side * 0.048);

        const auto iso = [&](double x, double y, double z) {
            const double sx = (x - y) * s;
            const double sy = (x + y) * s * 0.50 - z * s * 0.90;
            return QPointF(c.x() + sx, c.y() + sy);
        };

        const QColor line(255, 255, 255, 26);
        const QColor waterA(18, 40, 60, 70);
        const QColor waterB(10, 24, 40, 70);
        const QColor land(255, 255, 255, 18);
        const QColor dockTop(255, 255, 255, 26);
        const QColor dockSide(0, 0, 0, 28);

        // Water plane
        QPolygonF water;
        water << iso(-8, -8, 0) << iso(8, -8, 0) << iso(8, 8, 0) << iso(-8, 8, 0);
        {
            QLinearGradient g(water.boundingRect().topLeft(), water.boundingRect().bottomRight());
            g.setColorAt(0, waterA);
            g.setColorAt(1, waterB);
            p.setPen(QPen(line, 1));
            p.setBrush(g);
            p.drawPolygon(water);
        }

        // Land plane
        QPolygonF landPoly;
        landPoly << iso(-6, -2, 0) << iso(6, -2, 0) << iso(6, 7, 0) << iso(-6, 7, 0);
        p.setPen(QPen(line, 1));
        p.setBrush(land);
        p.drawPolygon(landPoly);

        const auto drawPrism = [&](double x, double y, double w, double h, double z) {
            const QPointF a0 = iso(x, y, 0);
            const QPointF b0 = iso(x + w, y, 0);
            const QPointF c0 = iso(x + w, y + h, 0);
            const QPointF d0 = iso(x, y + h, 0);

            const QPointF a1 = iso(x, y, z);
            const QPointF b1 = iso(x + w, y, z);
            const QPointF c1 = iso(x + w, y + h, z);
            const QPointF d1 = iso(x, y + h, z);

            QPolygonF face1;
            face1 << a0 << b0 << b1 << a1;
            QPolygonF face2;
            face2 << b0 << c0 << c1 << b1;
            QPolygonF top;
            top << a1 << b1 << c1 << d1;

            p.setPen(QPen(line, 1));
            p.setBrush(dockSide);
            p.drawPolygon(face1);
            p.drawPolygon(face2);

            p.setBrush(dockTop);
            p.drawPolygon(top);
        };

        // Docks
        drawPrism(-5.5, -1.3, 11.0, 0.8, 1.0);
        drawPrism(-4.8, 0.3, 3.2, 0.7, 0.9);
        drawPrism(-0.6, 0.3, 3.2, 0.7, 0.9);
        drawPrism(3.6, 0.3, 1.4, 0.7, 0.9);

        // Boats
        const QColor boat("#39c0fa");
        p.setPen(QPen(QColor(0, 0, 0, 50), 1));
        p.setBrush(QColor(boat.red(), boat.green(), boat.blue(), 70));

        QPolygonF b1;
        b1 << iso(-4.2, -3.4, 0.15) << iso(-3.3, -3.6, 0.15) << iso(-3.0, -2.9, 0.15) << iso(-3.9, -2.7, 0.15);
        p.drawPolygon(b1);

        QPolygonF b2;
        b2 << iso(2.8, -4.0, 0.15) << iso(3.8, -4.2, 0.15) << iso(4.1, -3.4, 0.15) << iso(3.1, -3.2, 0.15);
        p.drawPolygon(b2);
    }

    // Points
    for (const auto &pt : m_points)
    {
        const QPointF v = worldToView(QPointF(pt.x, pt.y), world, view);

        const bool highlighted = (!m_highlightedId.isEmpty() && pt.id.compare(m_highlightedId, Qt::CaseInsensitive) == 0);
        const qreal radius = highlighted ? 9.0 : 6.0;

        QColor c = pt.color.isValid() ? pt.color : QColor("#39c0fa");
        if (highlighted)
            c = QColor("#ffffff");

        p.setPen(QPen(QColor(0, 0, 0, 70), 2));
        p.setBrush(c);
        p.drawEllipse(v, radius, radius);

        if (highlighted)
        {
            p.setPen(QPen(QColor("#39c0fa"), 2));
            p.setBrush(Qt::NoBrush);
            p.drawEllipse(v, radius + 5.0, radius + 5.0);
        }
    }

    // Reference marker
    if (m_refEnabled && m_refValid)
    {
        const QPointF v = worldToView(m_ref, world, view);
        p.setPen(QPen(QColor("#ffcc66"), 2));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(v, 7.0, 7.0);
        p.drawLine(QPointF(v.x() - 10, v.y()), QPointF(v.x() + 10, v.y()));
        p.drawLine(QPointF(v.x(), v.y() - 10), QPointF(v.x(), v.y() + 10));
    }
}

void MiniMapWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_refEnabled)
        return;

    const QRectF view = contentRect();
    if (!view.contains(event->position()))
        return;

    const QRectF world = worldBounds();
    const QPointF worldPt = viewToWorld(event->position(), world, view);

    setReferencePoint(worldPt.x(), worldPt.y());
    emit referencePointChanged(worldPt.x(), worldPt.y());
}
