#ifndef PULSECONNECTOR_H
#define PULSECONNECTOR_H

#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#include <map>
#include <QObject>
#include <QQmlComponent>

using std::map;

/*void on_success(pa_context *ctx, int success, void *data) {

}*/

class PulseConnector : public QObject
{
    Q_OBJECT
    QML_ELEMENT
private:
    pa_context *ctx;
    pa_mainloop_api *api;
    pa_glib_mainloop *loop;
    std::map<unsigned int, QObject *> sinkMap;
    std::map<unsigned int, QObject *> sinkInputMap;
public:
    explicit PulseConnector(QObject *parent = nullptr);
    ~PulseConnector();
    Q_INVOKABLE void init();
    //Q_INVOKABLE void registerSink(unsigned int index, QObject *sink) {  }
signals:
    void updateSink(QString name, unsigned int index);
    void removeSink(unsigned int index);
    void updateSinkInput(QString name, unsigned int index);
    void removeSinkInput(unsigned int index);
};

#endif // PULSECONNECTOR_H
