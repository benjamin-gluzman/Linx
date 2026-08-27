#include "main.h"

int main() {
    linx_connect_to_dbus();

    // linx_call_dbus_method(LINX_BLUEZ_SYSTEM_NAME, "/org/bluez/hci0", "org.bluez.Adapter1", "StartDiscovery");

    // linx_call_dbus_method(LINX_BLUEZ_SYSTEM_NAME, "/org/bluez/hci0/dev_E8_7F_95_89_AA_0C", "org.bluez.Device1", "Connect");
    linx_get_managed_objects();
    
    linx_disconnect_from_dbus();

    return EXIT_SUCCESS;
}