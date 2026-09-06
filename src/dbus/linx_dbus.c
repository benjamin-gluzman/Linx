#include "linx_dbus.h"

#define DEFAULT_TIMEOUT -1

static GDBusConnection *system_connection, *session_connection;

static bool error_is_set(GError *error);


bool linx_connect_to_dbus() {
    GError *system_error = NULL, *session_error = NULL;
    
    system_connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &system_error);
    session_connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &session_error);

    if(error_is_set(system_error) || error_is_set(session_error)) {
        return false;
    }

    printf("Connected to system & session D-Bus!\n");

    return true;
}

void linx_disconnect_from_dbus() {
    g_object_unref(system_connection);
    g_object_unref(session_connection);
}


/*
 * args is borrowed; caller retains ownership.
 * reply is owned by this function.
 * parse() must not unref reply.
 */
void *linx_call_dbus_method(
    const gchar *bus_name,
    const gchar *object_path,
    const gchar *interface_name,
    const gchar* method_name,
    GVariant *args,
    const GVariantType *reply_type,
    void *(*parse)(GVariant *)
) {
    GVariant *reply = NULL;
    GError *error = NULL;

    reply = g_dbus_connection_call_sync(
        strcmp(bus_name, LINX_BLUEZ_SYSTEM_NAME) == 0 ? system_connection : session_connection,
        bus_name,
        object_path,
        interface_name,
        method_name,
        args,
        reply_type,
        G_DBUS_CALL_FLAGS_NONE,
        DEFAULT_TIMEOUT,
        NULL,
        &error
    );

    if(error_is_set(error)) {
        return NULL;
    }

    void *res = NULL;
    if(parse != NULL) res = parse(reply);

    g_variant_unref(reply);
    return res;
}

static bool error_is_set(GError *error) {
    if(error != NULL) {
        fprintf(stderr, "Error: %s\n", error->message);
        g_error_free(error);
        return true;
    }
    return false;
}