#ifndef PULSECONNECTOR_H
#define PULSECONNECTOR_H

#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#include <map>

using std::map;

class PulseConnector
{
private:
    pa_context *ctx;
    pa_mainloop_api *api;
    pa_glib_mainloop *loop;
public:
    PulseConnector();
    ~PulseConnector();
    map<int, pa_sink_input_info> sink_map;
};

#endif // PULSECONNECTOR_H
