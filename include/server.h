#pragma once

#include <WebServer.h>
#include "midi2dmx.h"

void server_init(Midi2DMX& midi2dmx, MappingConfig& config, uint16_t port = 80);
void server_tick();
