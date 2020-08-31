#include "pulseconnector.h"

#include <iostream>
#include <glib.h>
#include <cstdlib>
#include <QJsonObject>

bool check_event(pa_proplist *proplist)
{
    const char *t = pa_proplist_gets(proplist, "module-stream-restore.id");
    return (t && strcmp(t, "sink-input-by-media-role:event") == 0);
}

void sink_update(pa_context *ctx, const pa_sink_info *info, int idx, void *data)
{
    PulseConnector *conn = (PulseConnector *) data;
    if (info != NULL)
    {
        emit conn->updateSink(QString(info->name), info->index);
    }
}

void sink_input_update(pa_context *ctx, const pa_sink_input_info *info, int idx, void *data)
{
    PulseConnector* conn = (PulseConnector *) data;
    if (info != NULL && !check_event(info->proplist))
    {
        emit conn->updateSinkInput(QString(info->name), info->index);
    }
}

void on_event(pa_context *ctx, pa_subscription_event_type_t type, unsigned int index, void *data)
{
    std::cout << "Event received with " << index << " -- ";
    PulseConnector* conn = (PulseConnector*)data;
    switch (type & PA_SUBSCRIPTION_EVENT_FACILITY_MASK)
    {
    case PA_SUBSCRIPTION_EVENT_SINK:
        std::cout << "sink event" << std::endl;
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
        {
            emit conn->removeSink(index);
        } else {
            auto ret = pa_context_get_sink_info_by_index(ctx, index, &sink_update, data);
            pa_operation_unref(ret);
        }
        break;
    case PA_SUBSCRIPTION_EVENT_SINK_INPUT:
        std::cout << "sink input event" << std::endl;
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
        {
            emit conn->removeSinkInput(index);
        } else {
            auto ret = pa_context_get_sink_input_info(ctx, index, &sink_input_update, data);
            pa_operation_unref(ret);
        }
        break;
    case PA_SUBSCRIPTION_EVENT_SOURCE:
        std::cout << "source event" << std::endl;
        break;
    default:
        std::cout << "unmapped event" << std::endl;
        break;
    }
}

void on_connect(pa_context *ctx, void *data)
{
    printf("In callback\n");
    pa_context_state_t state = pa_context_get_state(ctx);
    pa_operation *ret;
    switch (state) {
    case PA_CONTEXT_READY: {
        printf("Connection ready\n");
        pa_context_set_subscribe_callback(ctx, &on_event, data);
        if (!(ret = pa_context_subscribe(ctx, (pa_subscription_mask_t)
                                       (PA_SUBSCRIPTION_MASK_SINK|
                                        PA_SUBSCRIPTION_MASK_SOURCE|
                                        PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                        PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT|
                                        PA_SUBSCRIPTION_MASK_CLIENT|
                                        PA_SUBSCRIPTION_MASK_SERVER|
                                        PA_SUBSCRIPTION_MASK_CARD), NULL, NULL))) {
            std::cerr << "Unable to subscribe to pulse events" << std::endl;
            return;
        }
        ret = pa_context_get_sink_input_info_list(ctx, &sink_input_update, data);
        pa_operation_unref(ret);
        ret = pa_context_get_sink_info_list(ctx, &sink_update, data);
        pa_operation_unref(ret);
        break;
    }
    case PA_CONTEXT_FAILED: {
        printf("Connection failed!");
        break;
    }
    default:
        break;
    }
}

PulseConnector::PulseConnector(QObject *parent) : QObject(parent)
{
    std::cout << "Creating connector" << std::endl;
    return;
}

Q_INVOKABLE void PulseConnector::init()
{
    void *data = this;
    this->loop = pa_glib_mainloop_new(g_main_context_default());
    this->api = pa_glib_mainloop_get_api(this->loop);
    this->ctx = pa_context_new(this->api, "Pulsar");
    pa_context_connect(this->ctx, NULL, PA_CONTEXT_NOFLAGS, NULL);
    pa_context_set_state_callback(this->ctx, &on_connect, data);
}

PulseConnector::~PulseConnector()
{
    pa_context_disconnect(this->ctx);
    pa_glib_mainloop_free(this->loop);
}
