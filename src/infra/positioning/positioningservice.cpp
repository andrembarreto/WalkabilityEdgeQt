#include "src/domain/journey/ipositioningservice.h"
#include "positioningservicefactory.h"

#include <QGeoPositionInfoSource>
#include <QLocationPermission>
#include <QGuiApplication>

namespace {
    class PositioningService final : public IPositioningService
    {
        Q_OBJECT

    public:
        explicit PositioningService(QObject* parent = nullptr)
            : IPositioningService{parent}
        {
            handlePermission();
        }

        void startUpdates(int updateInterval_ms) override {
            if(!m_source)
                return;

            m_source->setUpdateInterval(updateInterval_ms);
            m_source->startUpdates();
        }

        void stopUpdates() override {
            if(!m_source)
                return;

            m_source->stopUpdates();
        }

    private slots:
        void onPositionUpdated(const QGeoPositionInfo& info) {
            Position pos{
                info.coordinate().latitude(),
                info.coordinate().longitude(),
                info.timestamp().toMSecsSinceEpoch()
            };
            emit updated(pos);
        }

        void onErrorOccurred(QGeoPositionInfoSource::Error error) {
            qDebug() << "error retrieving position:" << error;
        }

    private:
        QGeoPositionInfoSource* m_source;

        void handlePermission() {
            QLocationPermission permission;
            permission.setAccuracy(QLocationPermission::Precise);

            switch(qApp->checkPermission(permission))
            {
            case Qt::PermissionStatus::Undetermined:
                qApp->requestPermission(permission, this, &PositioningService::handlePermission);
                break;
            case Qt::PermissionStatus::Denied:
                qFatal() << "Location permission denied";
                break;
            case Qt::PermissionStatus::Granted:
                qInfo() << "Location permission granted";
                setupSource();
                break;
            }
        }

        void setupSource() {
            m_source = QGeoPositionInfoSource::createDefaultSource(this);
            if(m_source)
            {
                connect(
                    m_source, &QGeoPositionInfoSource::positionUpdated,
                    this, &PositioningService::onPositionUpdated
                );
                connect(
                    m_source, &QGeoPositionInfoSource::errorOccurred,
                    this, &PositioningService::onErrorOccurred
                );
            }
        }
    };
}

std::unique_ptr<IPositioningService> createService() {
    return std::make_unique<PositioningService>();
}

#include "positioningservice.moc"