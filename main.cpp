#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QuickQanava>
#include "moduleregistry.h"
#include "pulseconnector.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //ModuleRegistry reg;
    auto connector = new PulseConnector();
    //connector->init();

    QQmlApplicationEngine engine;
    QuickQanava::initialize(&engine);
    engine.rootContext()->setContextProperty(QStringLiteral("pulse"), connector);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
