#pragma once

#include <QDateTime>
#include <QString>

struct Quai
{
    QString idQuai;
    QString matricule;
    QDateTime dateHeureArrivee;
    QDateTime dateHeureDepart;
    QString etatQuai;
    double prixLocation = 0.0;

    // Computed: minutes beyond a baseline docking duration.
    // This gives a meaningful 'retard' without requiring extra input fields.
    int retardMinutes(int plannedDockingMinutes = 8 * 60) const
    {
        const qint64 durationMinutes = dateHeureArrivee.secsTo(dateHeureDepart) / 60;
        const qint64 over = durationMinutes - plannedDockingMinutes;
        return over > 0 ? static_cast<int>(over) : 0;
    }
};
