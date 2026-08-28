#pragma once

// D-Bus
#include <dbus/dbus.h>

#include "../shared.h"


#define LINX_BLUEZ_SYSTEM_NAME "org.bluez"
#define LINX_BLUEZ_SESSION_NAME "org.bluez.obex"


bool linx_connect_to_dbus();
void linx_disconnect_from_dbus();

void *linx_call_dbus_method(
    const char *bus_name,
    const char *object_path,
    const char *iface,
    const char* method,
    void *(*parse)(DBusMessageIter *),
    int first_arg_type,
    ...
);