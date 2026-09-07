#include "linx_phone.h"

#define DEVICE_IFACE_NAME "org.bluez.Device1"
#define APPLE_MANUFACTURER_ID 0x004C

static void *phone_parse(GVariant *reply);
static bool is_device_phone(GVariant *properties);


LinxPhone *phone;

void linx_get_phone() {
    phone = linx_get_managed_objects(phone_parse);
    if(phone == NULL) printf("OH NO\n");
    else
    printf("Object Path: %s\nAddress: %s\nName: %s\nPaired: %s\nConnected: %s\n", 
        phone->object_path, phone->address, phone->name, phone->paired ? "true" : "false", phone->connected ? "true" : "false");
}

void linx_pair_phone() {

}

void linx_cancel_pairing() {

}


void linx_connect_phone() {
    if(phone->connected) {
        printf("Phone \"%s\" already connected\n", phone->name);
        return;
    }

    linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        phone->object_path,
        "org.bluez.Device1",
        "Connect",
        NULL,
        NULL,
        NULL
    );
}

void linx_disconnect_phone() {
    if(!phone->connected) {
        printf("Phone \"%s\" already disconnected\n", phone->name);
        return;
    }

    linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        phone->object_path,
        "org.bluez.Device1",
        "Disconnect",
        NULL,
        NULL,
        NULL
    );
}

static void *phone_parse(GVariant *reply) {
    LinxPhone *phone = malloc(sizeof(LinxPhone));

    GVariant *objects;
    g_variant_get(reply, "(@a{oa{sa{sv}}})", &objects);

    GVariantIter objects_iter;
    g_variant_iter_init(&objects_iter, objects);
    
    const char *object_path;
    GVariant *interfaces;
    while(g_variant_iter_next(&objects_iter, "{&o@a{sa{sv}}}", &object_path, &interfaces)) {
        GVariantIter interfaces_iter;
        g_variant_iter_init(&interfaces_iter, interfaces);

        const char *iface;
        GVariant *properties;
        while(g_variant_iter_next(&interfaces_iter, "{s@a{sv}}", &iface, &properties)) {
            if(strcmp(iface, DEVICE_IFACE_NAME) != 0) continue;
            
            if(!is_device_phone(properties)) {
                g_variant_unref(properties);
                break;
            }
            
            phone->object_path = strdup(object_path);


            GVariantIter properties_iter;
            g_variant_iter_init(&properties_iter, properties);

            const char *property;
            GVariant *value;
            while(g_variant_iter_next(&properties_iter, "{sv}", &property, &value)) {
                if(strcmp(property, "Address") == 0) {
                    phone->address = strdup(g_variant_get_string(value, NULL));
                }
                else if(strcmp(property, "Name") == 0) {
                    phone->name = strdup(g_variant_get_string(value, NULL));
                }
                else if(strcmp(property, "Paired") == 0) {
                    phone->paired = g_variant_get_boolean(value);
                }
                else if(strcmp(property, "Connected") == 0) {
                    phone->connected = g_variant_get_boolean(value);
                }
            }

            return phone;
        }

        g_variant_unref(properties);
    }
    g_variant_unref(interfaces);

    free(phone);
    return NULL;
}

static bool is_device_phone(GVariant *properties) {
    GVariantIter properties_iter;
    g_variant_iter_init(&properties_iter, properties);

    const char *property;
    GVariant *value;

    while(g_variant_iter_next(&properties_iter, "{sv}", &property, &value)) {
        if(strcmp(property, "ManufacturerData") != 0) continue;

        GVariant *manufacturer_data = g_variant_get_child_value(value, 0);
        
        uint16_t manufacturer_id;
        g_variant_get(manufacturer_data, "{qv}", &manufacturer_id);

        g_variant_unref(manufacturer_data);
        printf("%d\n", manufacturer_id);

        if(manufacturer_id == APPLE_MANUFACTURER_ID) 
            return true;
        else 
            return false;
    }

    return false;
}