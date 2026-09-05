#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/presentation/journey/journeydimensionviewmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const QList<JourneyDimensionViewModel*> dimensions {
        new JourneyDimensionViewModel("", "Opt 1", {}, JourneyDimensionViewModel::Type::T1, &app),
        new JourneyDimensionViewModel("", "Opt 2", {}, JourneyDimensionViewModel::Type::T2, &app)
    };

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
