#ifndef POSITIONTRACKER_H
#define POSITIONTRACKER_H

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

class PositionTracker : public QObject
{
    Q_OBJECT
public:
    explicit PositionTracker(QObject *parent = nullptr);

private slots:
    void onPositionUpdated(const QGeoPositionInfo&);
    void onErrorOccurred(QGeoPositionInfoSource::Error);

private:
    void startTracking();
    void handlePermission();
};

#endif // POSITIONTRACKER_H
