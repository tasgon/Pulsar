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
public:
    explicit PulseConnector(QObject *parent = nullptr);
    ~PulseConnector();
    Q_INVOKABLE void init();
    map<int, pa_sink_input_info> sink_map;
signals:
    void updateSinkInput(QString name, unsigned int index, int x, int y);
//public slots:
//    void moveInput(unsigned int index, unsigned int sink_idx) { pa_context_move_sink_input_by_index(this->ctx, index, sink_idx, &on_success, NULL); }
};

#endif // PULSECONNECTOR_H
