#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/infra/data/appdata.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("TCC Usp Esalq");
    app.setOrganizationDomain("tcc.usp.esalq");
    app.setApplicationName("Caminhabilidade");

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
