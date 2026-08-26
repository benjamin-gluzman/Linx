#ifndef BT_MANAGER_H
#define BT_MANAGER_H

#include "dbus_service.h"

void linx_get_managed_objects();

void linx_start_discovery();
void linx_stop_discovery();

void linx_handle_interface_added();

void linx_pair_phone();
void linx_connect_phone();

#endif