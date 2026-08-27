#pragma once

#include "../dbus/linx_dbus.h"


void *linx_get_managed_objects();

void linx_handle_interface_added();
void linx_handle_interface_removed();