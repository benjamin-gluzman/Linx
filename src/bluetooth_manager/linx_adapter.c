#include "linx_adapter.h"

#define ADAPTER_OBJECT_PATH_PREFIX "hci"
#define ADAPTER_IFACE_PREFIX "org.bluez.Adapter"

static void *adapter_parse(DBusMessageIter *iter);

static LinxAdapter *adapter;

void linx_get_adapter() {
    adapter = linx_get_managed_objects(adapter_parse);

    printf("Object Path: %s\nIFace: %s\n", adapter->object_path, adapter->iface);
}

void linx_start_discovery() {
    // StartDiscovery () -> ()
    linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        adapter->object_path,
        adapter->iface,
        "StartDiscovery",
        LINX_NO_PARSE_FUNC,
        LINX_NO_ARGS
    );
}

void linx_stop_discovery() {
    // StopDiscovery () -> ()
    linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        adapter->object_path,
        adapter->iface,
        "StopDiscovery",
        LINX_NO_PARSE_FUNC,
        LINX_NO_ARGS
    );
}

// GetManagedObjects () -> (Dict of {Object Path, Dict of {String, Dict of {String, Variant}}} objects)
static void *adapter_parse(DBusMessageIter *iter) {
    LinxAdapter *adapter = malloc(sizeof(LinxAdapter));

    DBusMessageIter objects;
    dbus_message_iter_recurse(iter, &objects);

    for(; dbus_message_iter_get_arg_type(&objects) != DBUS_TYPE_INVALID; dbus_message_iter_next(&objects)) {
        DBusMessageIter object_entry;
        dbus_message_iter_recurse(&objects, &object_entry);

        char *object_path;
        dbus_message_iter_get_basic(&object_entry, &object_path);

        if(strstr(object_path, ADAPTER_OBJECT_PATH_PREFIX) == NULL) continue;

        dbus_message_iter_next(&object_entry);

        DBusMessageIter interfaces;
        dbus_message_iter_recurse(&object_entry, &interfaces);

        for(; dbus_message_iter_get_arg_type(&interfaces) != DBUS_TYPE_INVALID; dbus_message_iter_next(&interfaces)) {
            DBusMessageIter iface_entry;
            dbus_message_iter_recurse(&interfaces, &iface_entry);

            char *iface;
            dbus_message_iter_get_basic(&iface_entry, &iface);

            if(strstr(iface, ADAPTER_IFACE_PREFIX) == NULL) continue;

            adapter->object_path = strdup(object_path);
            adapter->iface = strdup(iface);
            return adapter;
        }
    }

    free(adapter);
    return NULL;
}