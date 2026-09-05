#ifndef JOURNEYEVENTVIEWMODEL_H
#define JOURNEYEVENTVIEWMODEL_H

#include <QObject>
#include <QQmlEngine>

class JourneyEventViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit JourneyEventViewModel(QObject *parent = nullptr);

signals:
};

#endif // JOURNEYEVENTVIEWMODEL_H
