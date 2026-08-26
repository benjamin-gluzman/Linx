#include "shared.h"
#include "linx.h"

int main() {
    linx_connect_to_dbus(DBUS_BUS_SYSTEM);

    // call_dbus_method("/org/bluez/hci0/dev_E8_7F_95_89_AA_0C", "org.bluez.Device1", "Connect");
    linx_get_managed_objects();
    
    
    linx_disconnect_from_dbus();

    return EXIT_SUCCESS;
}