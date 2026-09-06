#include "linx_adapter.h"

#define ADAPTER_OBJECT_PATH_PREFIX "hci"
#define ADAPTER_IFACE_PREFIX "org.bluez.Adapter"

static void *adapter_parse(GVariant *reply);

static LinxAdapter *adapter;

void linx_get_adapter() {
    adapter = linx_get_managed_objects(adapter_parse);

    // printf("Object Path: %s\nIFace: %s\n", adapter->object_path, adapter->iface);
}

void linx_start_discovery() {
    // StartDiscovery () -> ()
    linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        adapter->object_path,
        adapter->iface,
        "StartDiscovery",
        NULL,
        NULL,
        NULL
    );
}

void linx_stop_discovery() {
    // StopDiscovery () -> ()
    linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        adapter->object_path,
        adapter->iface,
        "StopDiscovery",
        NULL,
        NULL,
        NULL
    );
}

// GetManagedObjects () -> (a{oa{sa{sv}}})
static void *adapter_parse(GVariant *reply) {
    LinxAdapter *adapter = malloc(sizeof(LinxAdapter));

    GVariant *objects;
    g_variant_get(reply, "(@a{oa{sa{sv}}})", &objects);

    GVariantIter objects_iter;
    g_variant_iter_init(&objects_iter, objects);
    
    char *object_path;
    GVariant *interfaces;
    while(g_variant_iter_next(&objects_iter, "{&o@a{sa{sv}}}", &object_path, &interfaces)) {
        printf("Object: %s\n", object_path);

        GVariantIter interfaces_iter;
        g_variant_iter_init(&interfaces_iter, interfaces);

        char *iface;
        GVariant *properties;
        while(g_variant_iter_next(&interfaces_iter, "{s@a{sv}}", &iface, &properties)) {
            printf("\tIface: %s\n", iface);

            GVariantIter properties_iter;
            g_variant_iter_init(&properties_iter, properties);

            char *property;
            GVariant *value;
            while(g_variant_iter_next(&properties_iter, "{sv}", &property, &value)) {
                printf("\t\tProp: %s\n", property);
                // printf("\t\tVal:  %s\n", g)
            }
        }
    }

    free(adapter);
    return NULL;
}