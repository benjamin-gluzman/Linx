#ifndef DBUS_SERVICE
#define DBUS_SERVICE

// D-Bus
#include <dbus/dbus.h>

#define BLUEZ "org.bluez"

extern DBusConnection *linx_connection;

bool linx_connect_to_dbus(DBusBusType type);
void linx_disconnect_from_dbus();

void *linx_call_dbus_method(char *path, char *iface, const char* method);

#endif