#pragma once

#include "../../shared.h"
#include <dbus/dbus.h>

typedef uint8_t byte_t;

typedef struct LinxElement LinxElement;
typedef struct LinxDictEntry LinxDictEntry;
typedef struct LinxArray LinxArray;

struct LinxElement{
    int type;
    union {
        byte_t byte;
        bool boolean;

        int16_t int16;
        uint16_t uint16;

        int32_t int32;
        uint32_t uint32;

        int64_t int64;
        uint64_t uint64;

        char *string;
        char *object_path;

        LinxArray *arr; // pointer to single LinxArray
        LinxDictEntry *entry;
    } val;
};

struct LinxDictEntry {
    LinxElement *key;
    LinxElement *value;
};

struct LinxArray{
    LinxElement *array; // array of LinxElement's
    size_t length;
    size_t pos;
};


LinxElement *linx_create_element(int type, void *value);

LinxDictEntry *linx_create_dict_entry(int key_type, void *key_value, int val_type, void *val_value);

LinxArray *linx_create_array(size_t length);

void linx_add_array_element(LinxElement *element, LinxArray *array);