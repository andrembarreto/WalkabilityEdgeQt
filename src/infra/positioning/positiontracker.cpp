#include "positiontracker.h"

#include <QGuiApplication>
#include <QLocationPermission>
#include <QGeoPositionInfoSource>
#include <QTimer>

PositionTracker::PositionTracker(QObject *parent)
    : QObject{parent}
{
    QTimer::singleShot(0, this, &PositionTracker::handlePermission);
}

QString PositionTracker::currentPosition() const
{
    return m_currentPositionInfo.coordinate().toString();
}

void PositionTracker::handlePermission()
{
    QLocationPermission permission;
    permission.setAccuracy(QLocationPermission::Precise);

    switch(qApp->checkPermission(permission))
    {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(permission, this, &PositionTracker::handlePermission);
        return;
    case Qt::PermissionStatus::Denied:
        qDebug() << "Permission denied";
        return;
    case Qt::PermissionStatus::Granted:
        qDebug() << "Permission granted";
        startTracking();
    }
}

void PositionTracker::startTracking()
{
    auto source = QGeoPositionInfoSource::createDefaultSource(this);
    if(source)
    {
        QObject::connect(source, &QGeoPositionInfoSource::positionUpdated, this, &PositionTracker::onPositionUpdated);
        QObject::connect(source, &QGeoPositionInfoSource::errorOccurred, this, &PositionTracker::onErrorOccurred);
        source->setUpdateInterval(1000);
        source->startUpdates();
    }
    else
        qDebug() << "Failed to acquire source";
}

void PositionTracker::onPositionUpdated(const QGeoPositionInfo& info)
{
    qDebug() << info;
    m_currentPositionInfo = info;
    emit currentPositionChanged();
}

void PositionTracker::onErrorOccurred(QGeoPositionInfoSource::Error error)
{
    qDebug() << error;
}