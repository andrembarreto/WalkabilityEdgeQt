#include "journeydispatcherviewmodel.h"
#include "src/application/journey-dispatcher/journeydispatcher.h"
#include "src/application/journey-dispatcher/journeyapi.h"

#include <QJsonObject>
#include <QPromise>
#include <QTimer>
#include <memory>

namespace {

class MockJourneyAPI : public QObject, public JourneyAPI
{
    Q_OBJECT

public:
    explicit MockJourneyAPI(QObject* parent = nullptr)
        : QObject{parent}
    {}

    QFuture<bool> post(const QJsonObject& obj) {
        qDebug() << obj;
        auto promise = std::make_shared<QPromise<bool>>();
        auto future = promise->future();
        promise->start();
        QTimer::singleShot(2000, this, [=](){
            promise->addResult(true);
            promise->finish();
        });
        return future;
    }
};

}

JourneyDispatcherViewModel::JourneyDispatcherViewModel(QObject *parent)
    : QObject{parent}
    , m_isExecuting(false)
{}

void JourneyDispatcherViewModel::execute(const Journey& journey)
{
    auto api = new MockJourneyAPI(this);
    auto dispatcher = new JourneyDispatcher(api, this);
    m_isExecuting = true;
    emit isExecutingChanged();
    dispatcher->execute(journey).then(this, [=](JourneyDispatchResult res){
        dispatcher->deleteLater();
        api->deleteLater();
        if(res.success)
            emit success();
        else
            emit fail(res.details);
        m_isExecuting = false;
        emit isExecutingChanged();
    });
}

#include "journeydispatcherviewmodel.moc"