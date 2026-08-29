#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "src/infra/positioning/positiontracker.h"

void sendHttpRequest()
{
    auto *manager = new QNetworkAccessManager(qApp);

    QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "HTTP SUCESSO! Resposta do Google:";

            QByteArray response = reply->readAll();

            qDebug() << response.left(100) << "...";
        } else {
            qDebug() << "HTTP ERRO:" << reply->errorString();
        }
        reply->deleteLater();
    });
    QNetworkRequest request(QUrl("http://www.google.com"));
    manager->get(request);

    qDebug() << "Requisição HTTP enviada, aguardando resposta...";
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    PositionTracker positionTracker;
    // sendHttpRequest();

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("WalkabilityEdgeQt", "Main");

    return app.exec();
}
