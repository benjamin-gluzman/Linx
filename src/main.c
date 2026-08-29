#include "main.h"

int main() {
    linx_connect_to_dbus();

    linx_get_adapter();
    linx_start_discovery();
    sleep(10);
    linx_stop_discovery();
    
    linx_disconnect_from_dbus();

    return EXIT_SUCCESS;
}