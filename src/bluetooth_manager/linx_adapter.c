#include "linx_adapter.h"

#define ADAPTER_OBJECT_PATH_PREFIX "hci"
#define ADAPTER_IFACE_PREFIX "org.bluez.Adapter"

static void *adapter_parse(DBusMessageIter *iter);

void linx_get_adapter() {
    LinxAdapter *adapter = linx_get_managed_objects(adapter_parse);

    printf("Object Path: %s\nIFace: %s\n", adapter->object_path, adapter->iface);
}

void linx_start_discovery() {
    
}

void linx_stop_discovery() {

}

// GetManagedObjects () ↦ (Dict of {Object Path, Dict of {String, Dict of {String, Variant}}} objects)
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


// LinxAdapter *adapter = malloc(sizeof(LinxAdapter));

// DBUS_FOREACH(object_entry, linx_dbus_iter_recurse(iter)) {
//     char *object_path = linx_dbus_iter_get_basic(linx_dbus_iter_get_dict_key(object_entry));

//     if(!strstr(object_path, ADAPTER_OBJECT_PATH_PREFIX)) continue;

//     DBUS_FOREACH(iface_iter, linx_dbus_iter_recurse(linx_dbus_iter_get_dict_value(object_entry))) {
//         char *iface = linx_dbus_iter_get_basic(linx_dbus_iter_get_dict_key(iface_iter));

//         if(!strstr(iface, ADAPTER_IFACE_PREFIX)) continue;

//         adapter->object_path = object_path;
//         adapter->iface = iface;
        
//         return adapter;
//     }
// }