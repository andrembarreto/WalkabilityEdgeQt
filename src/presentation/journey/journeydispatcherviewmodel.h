#ifndef JOURNEYDISPATCHERVIEWMODEL_H
#define JOURNEYDISPATCHERVIEWMODEL_H

#include <QObject>
#include <QQmlEngine>

#include "src/domain/journey/journey.h"

class JourneyDispatcherViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("JourneyDispatcherViewModel só pode ser instanciada por C++")
    Q_PROPERTY(bool isExecuting READ isExecuting NOTIFY isExecutingChanged)

public:
    explicit JourneyDispatcherViewModel(QObject *parent = nullptr);

    void execute(const Journey& journey);
    bool isExecuting() const { return m_isExecuting; }

signals:
    void isExecutingChanged();
    void success();
    void fail(const QString& reason);

private:
    bool m_isExecuting;
};

#endif // JOURNEYDISPATCHERVIEWMODEL_H
