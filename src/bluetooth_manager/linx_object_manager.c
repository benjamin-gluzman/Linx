#include "linx_object_manager.h"


void *linx_get_managed_objects() {
    linx_call_dbus_method(LINX_BLUEZ_SYSTEM_NAME, "/", "org.freedesktop.DBus.ObjectManager", "GetManagedObjects");
}

void linx_handle_interface_added() {

}

void linx_handle_interface_removed() {
    
}