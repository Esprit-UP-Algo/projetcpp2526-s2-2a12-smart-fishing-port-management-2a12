#pragma once

#include <QColor>
#include <QVector>
#include <QWidget>

class MiniMapWidget final : public QWidget
{
    Q_OBJECT

public:
    struct Point
    {
        QString id;
        double x = 0.0;
        double y = 0.0;
        QColor color;
    };

    explicit MiniMapWidget(QWidget *parent = nullptr);

    void setPoints(const QVector<Point> &points);

    void setReferenceEnabled(bool enabled);
    void setReferencePoint(double x, double y);
    bool referenceEnabled() const;
    QPointF referencePoint() const;

    void setPortIllustrationEnabled(bool enabled);

    void setHighlightedId(const QString &id);

signals:
    void referencePointChanged(double x, double y);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QRectF contentRect() const;
    QRectF worldBounds() const;
    QPointF worldToView(const QPointF &world, const QRectF &worldRect, const QRectF &viewRect) const;
    QPointF viewToWorld(const QPointF &view, const QRectF &worldRect, const QRectF &viewRect) const;

    QVector<Point> m_points;
    bool m_refEnabled = true;
    bool m_refValid = false;
    bool m_portIllustrationEnabled = false;
    QPointF m_ref;
    QString m_highlightedId;
};
