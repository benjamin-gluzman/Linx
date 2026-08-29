#include "linx_dbus.h"

static DBusConnection *system_connection, *session_connection;

static bool error_thrown(DBusError *error);


bool linx_connect_to_dbus() {
    DBusError system_error, session_error;
    dbus_error_init(&system_error);
    dbus_error_init(&session_error);

    system_connection = dbus_bus_get(DBUS_BUS_SYSTEM, &system_error);
    session_connection = dbus_bus_get(DBUS_BUS_SESSION, &session_error);

    if(error_thrown(&system_error) || error_thrown(&session_error)) {
        return false;
    }
    
    if(system_connection == NULL || session_connection == NULL) {
        fprintf(stderr, "Failed to connect to system and/or session D-Bus\n");
        return false;
    }

    printf("Connected to system & session D-Bus!\n");

    dbus_error_free(&system_error);
    dbus_error_free(&session_error);

    return true;
}

void linx_disconnect_from_dbus() {
    dbus_connection_unref(system_connection);
    dbus_connection_unref(session_connection);
}

void *linx_call_dbus_method(
    const char *bus_name,
    const char *object_path,
    const char *iface,
    const char* method,
    void *(*parse)(DBusMessageIter *),
    int first_arg_type,
    ...
) {
    DBusMessage *msg, *reply;
    DBusError error;
    dbus_error_init(&error);

    msg = dbus_message_new_method_call(bus_name, object_path, iface, method);

    if(msg == NULL) {
        fprintf(stderr, "Unable to call %s %s %s %s", bus_name, object_path, iface, method);
        return NULL;
    }

    if(first_arg_type != LINX_NO_ARGS) {
        va_list args;
        va_start(args, first_arg_type);
        dbus_message_append_args_valist(msg, first_arg_type, args);
        va_end(args);
    }

    if(strcmp(bus_name, LINX_BLUEZ_SYSTEM_NAME) == 0)
        reply = dbus_connection_send_with_reply_and_block(system_connection, msg, DBUS_TIMEOUT_USE_DEFAULT, &error);
    else
        reply = dbus_connection_send_with_reply_and_block(session_connection, msg, DBUS_TIMEOUT_USE_DEFAULT, &error);

    if(error_thrown(&error)) {
        return NULL;
    }

    DBusMessageIter iter;
    if(!dbus_message_iter_init(reply, &iter)) {
        printf("Nothing returned\n");
        return NULL;
    }

    void *res = NULL;
    if(parse != LINX_NO_PARSE_FUNC)
        res = parse(&iter);

    dbus_message_unref(msg);
    dbus_message_unref(reply);
    return res;
}

static bool error_thrown(DBusError *error) {
    if(dbus_error_is_set(error)) {
        fprintf(stderr, "D-Bus error: %s\n", error->message);
        dbus_error_free(error);
        return true;
    }
    return false;
}