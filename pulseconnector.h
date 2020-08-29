#ifndef PULSECONNECTOR_H
#define PULSECONNECTOR_H

#include <pulse/pulseaudio.h>
#include <map>

using std::map;

class PulseConnector
{
private:
    pa_context *ctx;
    pa_mainloop_api *api;
    pa_threaded_mainloop *loop;
    map<int, pa_sink_info> sink_map;
public:
    PulseConnector();
    ~PulseConnector();
};

#endif // PULSECONNECTOR_H
