#include "linx_object_manager.h"

// GetManagedObjects () -> (a{oa{sa{sv}}})
void *linx_get_managed_objects(void *(*parse)(GVariant *)) {
    return linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        "/",
        "org.freedesktop.DBus.ObjectManager",
        "GetManagedObjects",
        NULL,
        G_VARIANT_TYPE("(a{oa{sa{sv}}})"),
        parse
    );
}

void linx_handle_interface_added() {

}

void linx_handle_interface_removed() {
    
}