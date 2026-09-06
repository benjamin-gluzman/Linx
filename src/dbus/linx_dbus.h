#pragma once

// D-Bus
#include <gio/gio.h>

#include "../shared.h"


#define LINX_BLUEZ_SYSTEM_NAME "org.bluez"
#define LINX_BLUEZ_SESSION_NAME "org.bluez.obex"


bool linx_connect_to_dbus();
void linx_disconnect_from_dbus();

void *linx_call_dbus_method(
    const gchar *bus_name,
    const gchar *object_path,
    const gchar *interface_name,
    const gchar* method_name,
    GVariant *parameters,
    const GVariantType *reply_type,
    void *(*parse)(GVariant *)
);