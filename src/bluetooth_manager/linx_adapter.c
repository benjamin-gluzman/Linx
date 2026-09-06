#include "linx_adapter.h"

#define ADAPTER_OBJECT_PATH_PREFIX "/org/bluez/hci"
#define ADAPTER_IFACE_NAME "org.bluez.Adapter1"


static void power_on_adapter(LinxAdapter *adapter);
static void *adapter_parse(GVariant *reply);

static LinxAdapter *adapter;

void linx_get_adapter() {
    adapter = linx_get_managed_objects(adapter_parse);

    printf("Object Path: %s\nIFace: %s\nPowered: %s\n", adapter->object_path, adapter->iface, adapter->powered ? "true" : "false");
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

static void power_on_adapter(LinxAdapter *adapter) {
    if(adapter->powered) {
        printf("Adapter is already powered\n");
        return;
    }

    GVariant *args = g_variant_new("(ssv)", adapter->iface, "Powered", g_variant_new_boolean(true));

    // Set (ssv) -> ()
    linx_call_dbus_method(
        LINX_BLUEZ_SYSTEM_NAME,
        adapter->object_path,
        "org.freedesktop.DBus.Properties",
        "Set",
        args,
        NULL,
        NULL
    );

    // TODO: FIX BUG: (process:39317): GLib-CRITICAL **: 17:39:47.836: g_atomic_ref_count_dec: assertion 'old_value > 0' failed
    // g_variant_unref(args);
}

// GetManagedObjects () -> (a{oa{sa{sv}}})
static void *adapter_parse(GVariant *reply) {
    LinxAdapter *adapter = malloc(sizeof(LinxAdapter));

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
            if(strcmp(iface, ADAPTER_IFACE_NAME) != 0) continue;

            adapter->object_path = strdup(object_path);
            adapter->iface = strdup(iface);


            GVariantIter properties_iter;
            g_variant_iter_init(&properties_iter, properties);

            const char *property;
            GVariant *value;
            while(g_variant_iter_next(&properties_iter, "{sv}", &property, &value)) {
                if(strcmp(property, "Powered") == 0) {
                    adapter->powered = g_variant_get_boolean(value);

                    return adapter;
                }
            }
        }

        g_variant_unref(properties);
    }
    g_variant_unref(interfaces);

    free(adapter);
    return NULL;
}