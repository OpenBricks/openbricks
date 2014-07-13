/*
 * amionline.c
 * Uses dbus-glib to ask ConnMan whether there is an active internet connection
 * Compile with:
 * gcc -o amionline amionline.c `pkg-config --libs --cflags glib-2.0 gthread-2.0 dbus-glib-1`
 * From http://wiki.meego.com/D-Bus/Overview#D-Bus_GLib
 */

#include <glib.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus.h>

#define CONNMAN_SERVICE           "net.connman"
#define CONNMAN_MANAGER_PATH      "/"
#define CONNMAN_MANAGER_INTERFACE CONNMAN_SERVICE ".Manager"

static GMainLoop *main_loop;

static void
_method_call_notify_cb (DBusGProxy *proxy,
                        DBusGProxyCall *call,
                        gpointer user_data)
{
  GError *error = NULL;
  gchar *state = NULL;

  dbus_g_proxy_end_call (proxy,
                         call,
                         &error,
                         G_TYPE_STRING,
                         &state,
                         G_TYPE_INVALID);

  g_print ("Network state is %s\n", state);

  g_main_loop_quit (main_loop);
}

int
main (int    argc,
      char **argv)
{
  g_type_init ();

  DBusGConnection *connection;
  DBusGProxy *proxy;

  main_loop = g_main_loop_new (NULL, TRUE);

  connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, NULL);

  proxy = dbus_g_proxy_new_for_name (connection,
                                     CONNMAN_SERVICE,
                                     CONNMAN_MANAGER_PATH,
                                     CONNMAN_MANAGER_INTERFACE);

  dbus_g_proxy_begin_call (proxy,
                           "GetState",
                           _method_call_notify_cb,
                           main_loop,
                           NULL,
                           G_TYPE_INVALID);

  g_main_loop_run (main_loop);
}
