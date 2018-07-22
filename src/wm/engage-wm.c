#include <meta/meta-plugin.h>

#include "engage-wm.h"
#include "backgrounds/engage-background.h"

G_DEFINE_TYPE (EngageWm, engage_wm, META_TYPE_PLUGIN)

static const MetaPluginInfo *
engage_wm_plugin_info (MetaPlugin *plugin)
{
    static const MetaPluginInfo info = {
        .name = "Engage window manager",
        .version = "0.1",
        .author = "JosephMcc",
        .license = "GPLv3",
        .description = "A mutter based window manager"
    };

    return &info;
}

static void
on_monitors_changed (MetaScreen *screen,
                     gpointer    user_data)
{
    EngageWm *wm = ENGAGE_WM (user_data);
    ClutterActor *background;

    clutter_actor_destroy_all_children (wm->background_group);

    for (int i = 0; i < meta_screen_get_n_monitors (screen); i++)
    {
        background = CLUTTER_ACTOR (engage_background_new (screen, i));
        clutter_actor_add_child (wm->background_group, background);
        clutter_actor_show (background);
    }
}

static void
engage_wm_start (MetaPlugin *plugin)
{
    EngageWm *wm = ENGAGE_WM (plugin);
    MetaScreen *screen;

    screen = meta_plugin_get_screen (plugin);
    wm->stage = meta_get_stage_for_screen (screen);

    wm->ui_group = clutter_actor_new ();
    clutter_actor_add_child (wm->stage, wm->ui_group);

    wm->window_group = meta_get_window_group_for_screen (screen);
    g_object_ref (G_OBJECT (wm->window_group));
    clutter_actor_remove_child (wm->stage, wm->window_group);
    clutter_actor_add_child (wm->ui_group, wm->window_group);
    g_object_unref (G_OBJECT (wm->window_group));

    wm->background_group = meta_background_group_new ();
    clutter_actor_insert_child_below (wm->window_group, wm->background_group, NULL);
    g_signal_connect (screen, "monitors-changed", G_CALLBACK (on_monitors_changed), wm);
    on_monitors_changed (screen, wm);

    wm->top_window_group = meta_get_top_window_group_for_screen (screen);
    g_object_ref (G_OBJECT (wm->top_window_group));
    clutter_actor_remove_child (wm->stage, wm->top_window_group);
    clutter_actor_add_child (wm->ui_group, wm->top_window_group);
    g_object_unref (G_OBJECT (wm->top_window_group));

    clutter_actor_show (wm->stage);
}

static void
engage_wm_dispose (GObject *object)
{
    G_OBJECT_CLASS (engage_wm_parent_class)->dispose (object);
}

static void
engage_wm_class_init (EngageWmClass *klass)
{
    GObjectClass *g_object_class;
    MetaPluginClass *meta_plugin_class;

    g_object_class = G_OBJECT_CLASS (klass);
    g_object_class->dispose = engage_wm_dispose;

    meta_plugin_class = META_PLUGIN_CLASS (klass);
    meta_plugin_class->start = engage_wm_start;
    meta_plugin_class->plugin_info = engage_wm_plugin_info;
}

static void
engage_wm_init (EngageWm *wm)
{

}
