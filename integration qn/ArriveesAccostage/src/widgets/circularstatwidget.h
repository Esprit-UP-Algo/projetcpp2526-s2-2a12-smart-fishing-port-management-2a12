#pragma once

#include <QColor>
#include <QWidget>

class CircularStatWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit CircularStatWidget(QWidget *parent = nullptr);

    void setTitle(const QString &title);
    void setValueText(const QString &valueText);
    void setSubText(const QString &subText);

    // progress in [0..1]. Use a negative value to hide the ring progress.
    void setProgress(double progress);

    void setAccentColor(const QColor &accent);

    // Dual segment mode (for combined stats like "Etat").
    // Values are normalized internally (only their ratio matters).
    void setDualSegments(const QString &labelA,
                         double valueA,
                         const QColor &colorA,
                         const QString &labelB,
                         double valueB,
                         const QColor &colorB);
    void setDualLegendFormat(int decimals, const QString &suffix);
    void clearDualSegments();

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_title;
    QString m_valueText;
    QString m_subText;

    double m_progress = -1.0;

    QColor m_accent = QColor("#39c0fa");

    bool m_dualEnabled = false;
    QString m_labelA;
    QString m_labelB;
    double m_valueA = 0.0;
    double m_valueB = 0.0;
    QColor m_colorA = QColor("#39c0fa");
    QColor m_colorB = QColor("#2ecc71");
    int m_dualDecimals = 0;
    QString m_dualSuffix;
};
