#include "pulseconnector.h"

#include <iostream>

bool connected = false;

void print_list(pa_context *ctx, const pa_sink_input_info *info, unsigned int idx, void *data) {
    //printf("%d connections\n", idx);
    if (info != NULL)
        std::cout << info->name << std::endl;
}

void on_event(pa_context *c, pa_subscription_event_type_t type, int index, void *data) {

}

void on_connect(pa_context *ctx, void *data) {
    printf("In callback\n");
    pa_context_state_t state = pa_context_get_state(ctx);
    pa_operation *ret;
    switch (state) {
    case PA_CONTEXT_READY: {
        printf("Connection ready\n");
        pa_context_set_subscribe_callback(ctx, &on_event, NULL);
        if (!(ret = pa_context_subscribe(ctx, (pa_subscription_mask_t)
                                       (PA_SUBSCRIPTION_MASK_SINK|
                                        PA_SUBSCRIPTION_MASK_SOURCE|
                                        PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                        PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT|
                                        PA_SUBSCRIPTION_MASK_CLIENT|
                                        PA_SUBSCRIPTION_MASK_SERVER|
                                        PA_SUBSCRIPTION_MASK_CARD), nullptr, nullptr))) {

            return;
        }
        ret = pa_context_get_sink_input_info_list(ctx, &print_list, NULL);
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
    this->loop = pa_threaded_mainloop_new();
    this->api = pa_threaded_mainloop_get_api(this->loop);
    this->ctx = pa_context_new(this->api, "Pulsar");
    pa_context_connect(this->ctx, NULL, PA_CONTEXT_NOFLAGS, NULL);
    pa_context_set_state_callback(this->ctx, &on_connect, NULL);
    pa_threaded_mainloop_start(this->loop);
    return;
}

PulseConnector::~PulseConnector()
{
    pa_context_disconnect(this->ctx);
    pa_threaded_mainloop_stop(this->loop);
    pa_threaded_mainloop_free(this->loop);
}
