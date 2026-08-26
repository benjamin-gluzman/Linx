#include "shared.h"
#include "dbus_service.h"

DBusConnection *connection;

static void linx_parse_reply(DBusMessageIter *iter, int tabs);

static bool linx_error_thrown(DBusError *error);

/*
    Returns true on success, false on failure
*/
bool linx_connect_to_dbus(DBusBusType type) {
    DBusError error;
    dbus_error_init(&error);

    connection = dbus_bus_get(type, &error);

    if(linx_error_thrown(&error)) {
        return false;
    }
    
    if(connection == NULL) {
        fprintf(stderr, "Failed to connect to system D-Bus\n");
        return false;
    }

    printf("Connected to system D-Bus!\n");

    dbus_error_free(&error);

    return true;
}

void linx_disconnect_from_dbus() {
    dbus_connection_unref(connection);
}

void *linx_call_dbus_method(char *path, char *iface, const char* method) {
    DBusMessage *msg, *reply;
    DBusError error;
    dbus_error_init(&error);

    msg = dbus_message_new_method_call(BLUEZ, path, iface, method);

    if(msg == NULL) {
        fprintf(stderr, "Unable to call %s %s %s", path, iface, method);
        return NULL;
    }

    reply = dbus_connection_send_with_reply_and_block(connection, msg, DBUS_TIMEOUT_USE_DEFAULT, &error);

    if(linx_error_thrown(&error)) {
        return NULL;
    }

    DBusMessageIter iter;
    if(!dbus_message_iter_init(reply, &iter)) {
        printf("Nothing returned\n");
        return NULL;
    }

    linx_parse_reply(&iter, 0);

    dbus_message_unref(msg);
    dbus_message_unref(reply);
    return NULL;
}

static void linx_parse_reply(DBusMessageIter *iter, int tabs) {
    do {
        int type = dbus_message_iter_get_arg_type(iter);
        
        if(type == DBUS_TYPE_ARRAY || type == DBUS_TYPE_DICT_ENTRY || type == DBUS_TYPE_VARIANT) {
            DBusMessageIter sub_iter;
            dbus_message_iter_recurse(iter, &sub_iter);
            if(dbus_message_iter_get_arg_type(&sub_iter) == DBUS_TYPE_INVALID) {
                continue;
            }

            linx_parse_reply(&sub_iter, tabs + 1);
        }
        else if(type == DBUS_TYPE_STRING || type == DBUS_TYPE_OBJECT_PATH) {
            const char *val = NULL;
            dbus_message_iter_get_basic(iter, &val);

            for(int i = 0; i < tabs; i++) printf("\t");

            printf("%s\n", val);
        }

    } while(dbus_message_iter_next(iter));
}


static bool linx_error_thrown(DBusError *error) {
    if(dbus_error_is_set(error)) {
        fprintf(stderr, "D-Bus error: %s\n", error->message);
        dbus_error_free(error);
        return true;
    }

    return false;
}