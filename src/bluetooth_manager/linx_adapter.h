#pragma once

#include "linx_object_manager.h"

typedef struct {
    char *object_path;
    char *iface;
} LinxAdapter;

void linx_get_adapter();

void linx_start_discovery();
void linx_stop_discovery();