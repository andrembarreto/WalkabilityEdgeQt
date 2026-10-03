#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/application/index-table/indextable.h"
#include "src/application/utils/waitfor.h"
#include "src/infra/api/index/indexapi.h"
#include "src/presentation/index/indexparametersviewmodels.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("TCC Usp Esalq");
    app.setOrganizationDomain("tcc.usp.esalq");
    app.setApplicationName("Caminhabilidade");

    IndexTable::instance().setApi(new IndexAPI(&app));
    const auto dimensions = toViewModels(waitFor(IndexTable::instance().get()));

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
