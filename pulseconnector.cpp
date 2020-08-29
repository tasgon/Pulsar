#include "pulseconnector.h"

#include <iostream>

bool connected = false;

void print_list(pa_context *ctx, const pa_sink_input_info *info, int idx, void *data) {
    //printf("%d connections\n", idx);
    if (info != NULL)
        std::cout << info->index << " " << info->name << std::endl;
}

void on_event(pa_context *ctx, pa_subscription_event_type_t type, unsigned int index, void *data) {
    std::cout << "Event received with " << index << " -- ";
    switch (type & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
    case PA_SUBSCRIPTION_EVENT_SINK:
        std::cout << "sink event" << std::endl;
        if ((type & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE)
        break;
    case PA_SUBSCRIPTION_EVENT_SOURCE:
        std::cout << "source event" << std::endl;
        break;
    default:
        std::cout << "unmapped event" << std::endl;
        break;
    }
}

void on_connect(pa_context *ctx, void *data) {
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
        ret = pa_context_get_sink_input_info_list(ctx, &print_list, data);
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

PulseConnector::PulseConnector()
{
    std::cout << "Creating connector" << std::endl;
    void *data = this;
    this->loop = pa_threaded_mainloop_new();
    this->api = pa_threaded_mainloop_get_api(this->loop);
    this->ctx = pa_context_new(this->api, "Pulsar");
    pa_context_connect(this->ctx, NULL, PA_CONTEXT_NOFLAGS, NULL);
    pa_context_set_state_callback(this->ctx, &on_connect, data);
    pa_threaded_mainloop_start(this->loop);
    return;
}

PulseConnector::~PulseConnector()
{
    pa_context_disconnect(this->ctx);
    pa_threaded_mainloop_stop(this->loop);
    pa_threaded_mainloop_free(this->loop);
}
