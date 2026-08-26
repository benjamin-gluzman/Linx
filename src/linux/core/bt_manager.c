#include "shared.h"
#include "bt_manager.h"


void linx_get_managed_objects() {
    linx_call_dbus_method("/", "org.freedesktop.DBus.ObjectManager", "GetManagedObjects");
}


void linx_start_discovery() {

}

void linx_stop_discovery() {

}

void linx_handle_interface_added() {

}

void linx_pair_phone() {

}

void linx_connect_phone() {
    
}