#ifndef POSITIONTRACKER_H
#define POSITIONTRACKER_H

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

class PositionTracker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentPosition READ currentPosition NOTIFY currentPositionChanged)
public:
    explicit PositionTracker(QObject *parent = nullptr);

    QString currentPosition() const;

signals:
    void currentPositionChanged();

private slots:
    void onPositionUpdated(const QGeoPositionInfo&);
    void onErrorOccurred(QGeoPositionInfoSource::Error);

private:
    QGeoPositionInfo m_currentPositionInfo;

    void startTracking();
    void handlePermission();
};

#endif // POSITIONTRACKER_H
