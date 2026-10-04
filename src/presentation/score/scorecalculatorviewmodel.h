#ifndef SCORECALCULATORVIEWMODEL_H
#define SCORECALCULATORVIEWMODEL_H

#include <QObject>
#include <qqml.h>

#include "scoreviewmodel.h"

class ScoreCalculatorViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(scoreViewModel score READ score NOTIFY scoreChanged)

public:
    enum class Status
    {
        Idle,
        Calculating,
        Ready,
        Failed
    };
    Q_ENUM(Status)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

    explicit ScoreCalculatorViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void calculate(int journeyID = -1);
    Status status() const { return m_status; }
    scoreViewModel score() const { return m_score; }

signals:
    void scoreChanged();
    void statusChanged();

private:
    Status m_status;
    scoreViewModel m_score;

    void calculateFor(const QString& resourceId);
};

#endif // SCORECALCULATORVIEWMODEL_H
