#pragma once

// D-Bus
#include <dbus/dbus.h>

#include "./types/linx_types.h"

#define LINX_BLUEZ_SYSTEM_NAME "org.bluez"
#define LINX_BLUEZ_SESSION_NAME "org.bluez.obex"


bool linx_connect_to_dbus();
void linx_disconnect_from_dbus();

void *linx_call_dbus_method(char *bus_name, char *object_path, char *iface, const char* method);