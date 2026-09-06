#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/infra/data/appdata.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const auto dimensions = appdata::loadData();

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.setInitialProperties({
        { "dimensions", QVariant::fromValue(dimensions) }
    });
    engine.loadFromModule("WalkabilityEdgeQt", "Main");

    return app.exec();
}
