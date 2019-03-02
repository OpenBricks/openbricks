/*
 * amionline.c
 * Ask ConnMan whether there is an active internet connection
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <dbus/dbus.h>

#define CONNMAN_SERVICE           "net.connman"
#define CONNMAN_MANAGER_PATH      "/"
#define CONNMAN_MANAGER_INTERFACE CONNMAN_SERVICE ".Manager"


struct filterDef {
    const char  *key;
    const char  *fmt;
};


static int parse_result(DBusMessageIter *iter, struct filterDef *filterData)
{
    int arg_type;
    char buf[32];
    const char *str, *outFmt = NULL;

    union {
        dbus_bool_t   b;
        unsigned char c;
        dbus_int32_t  i32;
        dbus_uint16_t u16;
        dbus_uint32_t u32;
        dbus_uint64_t u64;
        double        d;
    } u;

    struct iterNode {
            int parent_type;
            struct iterNode *next;
            DBusMessageIter entry;
    } *tmp, *head;


    head = malloc(sizeof(struct iterNode));
    head->next = NULL;
    head->parent_type = DBUS_TYPE_INVALID;
    dbus_message_iter_recurse(iter, &head->entry);
    iter = &head->entry;

    while (head) {
        str = buf;

        arg_type = dbus_message_iter_get_arg_type(iter);

        switch (arg_type) {
        case DBUS_TYPE_ARRAY:
        case DBUS_TYPE_STRUCT:
        case DBUS_TYPE_VARIANT:
        case DBUS_TYPE_DICT_ENTRY:
            tmp = malloc(sizeof(struct iterNode));
            tmp->next = head;
            tmp->parent_type = arg_type;
            dbus_message_iter_recurse(iter, &tmp->entry);
            dbus_message_iter_next(iter);
            head = tmp;
            iter = &head->entry;
            continue;
        case DBUS_TYPE_INVALID:
            tmp = head;
            if (tmp) {
                head = tmp->next;
                free(tmp);
            }
            iter = &head->entry;
            continue;

        case DBUS_TYPE_STRING:
        case DBUS_TYPE_OBJECT_PATH:
            dbus_message_iter_get_basic(iter, &str);
            break;
        case DBUS_TYPE_BOOLEAN:
            dbus_message_iter_get_basic(iter, &u);
            str = u.b ? "True" : "False";
            break;
        case DBUS_TYPE_BYTE:
            dbus_message_iter_get_basic(iter, &u);
            sprintf(buf, "%d", u.c);
            break;
        case DBUS_TYPE_UINT16:
            dbus_message_iter_get_basic(iter, &u);
            sprintf(buf, "%u", u.u16);
            break;
        case DBUS_TYPE_UINT32:
            dbus_message_iter_get_basic(iter, &u);
            sprintf(buf, "%u", u.u32);
            break;
        case DBUS_TYPE_INT32:
            dbus_message_iter_get_basic(iter, &u);
            sprintf(buf, "%d", u.i32);
            break;
        case DBUS_TYPE_UINT64:
            dbus_message_iter_get_basic(iter, &u);
            sprintf(buf, "%"PRIu64, u.u64);
            break;
        case DBUS_TYPE_DOUBLE:
            dbus_message_iter_get_basic(iter, &u);
            sprintf(buf, "%lf", u.d);
            break;
        default:
            sprintf(buf, "<type %c>", arg_type);
            break;
        }

        if (head->parent_type == DBUS_TYPE_DICT_ENTRY) {
            struct filterDef *f;

            for (f = filterData; f->key; f++)
            {
                if (strcmp(f->key, str) == 0) {
                    outFmt = f->fmt;
                    break;
                }
            }
        }

        if (head->parent_type == DBUS_TYPE_VARIANT) {
            if (outFmt)
                printf(outFmt, str);
            outFmt = NULL;
        }

        dbus_message_iter_next(iter);
    }

    return 0;
}


int main(int argc, char *argv[])
{
    int err;
    DBusMessage *msg;
    DBusError dbus_err;
    DBusConnection *conn;
    DBusMessageIter result;
    DBusPendingCall *pending;

    static struct filterDef filters[] = {
        { "State",          "Network state is %s\n" },
        { "OfflineMode",    NULL },
        { "SessionMode",    NULL },
        { NULL, NULL }
    };

    if (argc > 1 && strncmp(argv[1], "-a", 2) == 0) {
        filters[0].fmt = "State: %s\n";
        filters[1].fmt = "OfflineMode: %s\n";
        filters[2].fmt = "SessionMode: %s\n";
    }

    dbus_error_init(&dbus_err);
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_err);

    if (dbus_error_is_set(&dbus_err)) {
        fprintf(stderr, "Error: %s\n", dbus_err.message);
        dbus_error_free(&dbus_err);
        return EINVAL;
    }

    // create a message
    err = ENOENT;
    msg = dbus_message_new_method_call(
            CONNMAN_SERVICE, CONNMAN_MANAGER_PATH,
            CONNMAN_MANAGER_INTERFACE, "GetProperties");

    if (msg) {
        // send message and get a handle for a reply
        err = EINVAL;
        if (dbus_connection_send_with_reply(
                conn, msg, &pending, -1) && pending) {
            // flush output queue
            dbus_connection_flush(conn);

            // free message
            dbus_message_unref(msg);

            // block until we receive a reply
            dbus_pending_call_block(pending);

            // get the reply message
            msg = dbus_pending_call_steal_reply(pending);
            // free the pending message handle
            dbus_pending_call_unref(pending);

            // read the parameters
            if (msg && dbus_message_iter_init(msg, &result))
                err = parse_result(&result, filters);
        }

        // free reply
        if (msg)
            dbus_message_unref(msg);
    }

    // close connection
    dbus_connection_close(conn);
    // free connection
    dbus_connection_unref(conn);

    return err;
}
