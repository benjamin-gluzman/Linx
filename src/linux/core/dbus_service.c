#include "shared.h"
#include "dbus_service.h"

DBusConnection *connection;

static bool error_thrown(DBusError *error);

/*
    Returns true on success, false on failure
*/
bool connect_to_dbus(DBusBusType type) {
    DBusError error;
    dbus_error_init(&error);

    connection = dbus_bus_get(type, &error);

    if(error_thrown(&error)) {
        return false;
    }
    
    if(connection == NULL) {
        fprintf(stderr, "Failed to connect to system D-Bus\n");
        return false;
    }

    printf("Connected to system D-Bus!\n");

    dbus_error_free(&error);

    return true;
}

void disconnect_from_dbus() {
    dbus_connection_unref(connection);
}

DBusMessage *call_dbus_method(char *path, char *iface, const char* method) {
    DBusMessage *msg, *reply;
    DBusError error;
    dbus_error_init(&error);

    msg = dbus_message_new_method_call(BLUEZ, path, iface, method);

    if(msg == NULL) {
        fprintf(stderr, "Unable to call %s %s %s", path, iface, method);
        return NULL;
    }

    reply = dbus_connection_send_with_reply_and_block(connection, msg, DBUS_TIMEOUT_USE_DEFAULT, &error);

    if(error_thrown(&error)) {
        return NULL;
    }

    dbus_message_unref(msg);
    
    return reply;
}


static bool error_thrown(DBusError *error) {
    if(dbus_error_is_set(error)) {
        fprintf(stderr, "D-Bus error: %s\n", error->message);
        dbus_error_free(error);
        return true;
    }

    return false;
}