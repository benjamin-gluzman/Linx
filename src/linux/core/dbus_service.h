#ifndef DBUS_SERVICE
#define DBUS_SERVICE
#include "shared.h"

// D-Bus
#include <dbus/dbus.h>

#define BLUEZ "org.bluez"

extern DBusConnection *connection;

bool connect_to_dbus(DBusBusType type);
void disconnect_from_dbus();

DBusMessage *call_dbus_method(char *path, char *iface, const char* method);

#endif