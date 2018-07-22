#include <stdlib.h>
#include <meta/main.h>

#include "engage-wm.h"

#define GAIL_OPT "NO_GAIL"
#define AT_BRIDGE_OPT "NO_AT_BRIDGE"
#define GNOME_KBDS "Mutter,GNOME Shell"
#define WMNAME "Mutter (Engage)"

static gboolean
print_version_and_exit (const gchar *name,
                        const gchar *value,
                        gpointer     user_data,
                        GError     **error)
{
    g_print ("%s\n", "Engage window manager 0.01");
    exit (EXIT_SUCCESS);
    return TRUE;
}

static const GOptionEntry options[] = {
    { "version", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
      print_version_and_exit, "Show the application's version and exit", NULL },
    { NULL }
};

int
main (int argc, char **argv)
{
    GOptionContext *context;
    GError *error = NULL;

    context = meta_get_option_context ();
    g_option_context_add_main_entries (context, options, NULL);

    if (!g_option_context_parse (context, &argc, &argv, &error))
    {
        g_printerr ("%s\n", error->message);

        g_clear_error (&error);
        g_option_context_free (context);

        return 1;
    }

    g_option_context_free (context);

    meta_plugin_manager_set_plugin_type (engage_wm_get_type ());
    meta_set_gnome_wm_keybindings (GNOME_KBDS);
    meta_set_wm_name (WMNAME);

    g_setenv (GAIL_OPT, "1", TRUE);
    g_setenv (AT_BRIDGE_OPT, "1", TRUE);

    meta_init ();

    g_unsetenv (GAIL_OPT);
    g_unsetenv (AT_BRIDGE_OPT);

    meta_register_with_session ();

    return meta_run ();
}