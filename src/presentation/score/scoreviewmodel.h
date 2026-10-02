#ifndef SCOREVIEWMODEL_H
#define SCOREVIEWMODEL_H

#include <qqml.h>
#include <QVariant>

struct dimensionScoreViewModel
{
    Q_GADGET
    QML_ELEMENT

    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(float value MEMBER value CONSTANT)

public:
    QString name;
    float value;
};

struct scoreViewModel
{
    Q_GADGET
    QML_ELEMENT

    Q_PROPERTY(float global MEMBER global CONSTANT)
    Q_PROPERTY(QList<dimensionScoreViewModel> dimensionScores MEMBER dimensionScores CONSTANT)

public:
    float global;
    QList<dimensionScoreViewModel> dimensionScores;
};

#endif // SCOREVIEWMODEL_H
