#include "linx.h"

int main(int argc, char **argv) {
    DBusError error;
    dbus_error_init(&error);

    DBusConnection *connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

    if(dbus_error_is_set(&error)) {
        fprintf(stderr, "D-Bus error: %s\n", error.message);
        dbus_error_free(&error);
        return EXIT_FAILURE;
    }

    if(connection == NULL) {
        fprintf(stderr, "Failed to connect to system D-Bus\n");
        return EXIT_FAILURE;
    }

    printf("Connected to system D-Bus!\n");

    dbus_connection_unref(connection);

    return EXIT_SUCCESS;
}

