#pragma once

#include "linx_object_manager.h"

typedef struct {
    char *object_path;
    char *address;
    char *name;
    bool paired;
    bool connected;
} LinxPhone;

void linx_get_phone();

void linx_pair_phone();
void linx_cancel_pairing();

void linx_connect_phone();
void linx_disconnect_phone();