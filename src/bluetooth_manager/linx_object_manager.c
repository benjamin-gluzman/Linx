#include "linx_object_manager.h"

// GetManagedObjects () ↦ (Dict of {Object Path, Dict of {String, Dict of {String, Variant}}} objects)
void *linx_get_managed_objects(void *(*parse)(DBusMessageIter *)) {
    return linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        "/",
        "org.freedesktop.DBus.ObjectManager",
        "GetManagedObjects",
        parse,
        LINX_NO_ARGS
    );
}

void linx_handle_interface_added() {

}

void linx_handle_interface_removed() {
    
}