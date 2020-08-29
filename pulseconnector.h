#ifndef PULSECONNECTOR_H
#define PULSECONNECTOR_H

#include <pulse/pulseaudio.h>

class PulseConnector
{
private:
    pa_context *ctx;
    pa_mainloop_api *api;
    pa_threaded_mainloop *loop;
public:
    PulseConnector();
    ~PulseConnector();
};

#endif // PULSECONNECTOR_H
