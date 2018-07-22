#include <meta/meta-background.h>
#include <meta/meta-background-actor.h>

#include "engage-background.h"

#define BACKGROUND_SCHEMA  "org.gnome.desktop.background"
#define PICTURE_URI        "picture-uri"
#define PRIMARY_COLOR      "primary-color"
#define SECONDARY_COLOR    "secondary-color"
#define COLOR_SHADING_TYPE "color-shading-type"
#define BACKGROUND_STYLE   "picture-options"

#define BACKGROUND_TIMEOUT 500

enum
{
    PROP_0,
    PROP_SCREEN,
    PROP_MONITOR,
    N_PROPERTIES
};

static GParamSpec *obj_props[N_PROPERTIES] = { NULL };

G_DEFINE_TYPE (EngageBackground, engage_background, META_TYPE_BACKGROUND_GROUP)

static void
remove_previous (ClutterActor *actor,
                 gpointer      user_data)
{
    EngageBackground *bg = ENGAGE_BACKGROUND (user_data);

    clutter_actor_destroy (actor);
    bg->previous_background = NULL;
}

static void
begin_remove_previous (ClutterActor *actor,
                       gpointer      user_data)
{
    EngageBackground *bg = ENGAGE_BACKGROUND (user_data);

    if (bg->previous_background && bg->previous_background != bg->current_background)
    {
        g_signal_connect (bg->previous_background, "transitions-completed",
                          G_CALLBACK (remove_previous), user_data);

        clutter_actor_save_easing_state (bg->previous_background);
        clutter_actor_set_easing_mode (bg->previous_background, CLUTTER_EASE_OUT_QUAD);
        clutter_actor_set_easing_duration (bg->previous_background, BACKGROUND_TIMEOUT);
        clutter_actor_set_opacity (bg->previous_background, 0);
        clutter_actor_restore_easing_state (bg->previous_background);
    }
}

static void
on_changed (MetaBackground *background,
            gpointer        user_data)
{
    EngageBackground *bg = ENGAGE_BACKGROUND (user_data);

    clutter_actor_save_easing_state (bg->current_background);
    g_signal_connect (bg->current_background, "transitions-completed",
                      G_CALLBACK (begin_remove_previous), user_data);

    clutter_actor_set_easing_mode (bg->current_background, CLUTTER_EASE_IN_EXPO);
    clutter_actor_set_easing_duration (bg->current_background, BACKGROUND_TIMEOUT);
    clutter_actor_set_opacity (bg->current_background, 255);
    clutter_actor_restore_easing_state (bg->current_background);
}

static void
set_image_background (gchar                   *current_background_filename,
                      MetaBackground          *background,
                      GDesktopBackgroundStyle *style)
{
    GFile *current_background_file = NULL;

    current_background_file = g_file_new_for_uri (current_background_filename);
    g_free (current_background_filename);

    meta_background_set_file (background, current_background_file, *style);

    g_object_unref (current_background_file);
}

static void
set_color_background (GSettings      *settings,
                      MetaBackground *background)
{
    GDesktopBackgroundShading shading_direction;
    ClutterColor primary_color;
    ClutterColor secondary_color;
    gchar *color_str = NULL;

    shading_direction = g_settings_get_enum (settings, COLOR_SHADING_TYPE);

    color_str = g_settings_get_string (settings, PRIMARY_COLOR);
    if (color_str)
    {
        clutter_color_from_string (&primary_color, color_str);
        g_free (color_str);
        color_str = NULL;
    }

    color_str = g_settings_get_string (settings, SECONDARY_COLOR);
    if (color_str)
    {
        clutter_color_from_string (&secondary_color, color_str);
        g_free (color_str);
        color_str = NULL;
    }

    if (shading_direction == G_DESKTOP_BACKGROUND_SHADING_SOLID)
    {
        meta_background_set_color (background, &primary_color);
    }
    else
    {
        meta_background_set_gradient (background, shading_direction, &primary_color, &secondary_color);
    }
}

static void
update (EngageBackground *bg)
{
    gchar *current_background_filename = NULL;
    MetaRectangle rectangle;
    ClutterActor *actor;
    MetaBackground *background;
    GDesktopBackgroundStyle style;

    actor = meta_background_actor_new (bg->screen, bg->monitor);
    background = meta_background_new (bg->screen);
    meta_background_actor_set_background (META_BACKGROUND_ACTOR (actor), background);
    g_object_unref (background);

    meta_screen_get_monitor_geometry (bg->screen, bg->monitor, &rectangle);
    clutter_actor_set_size (actor, rectangle.width, rectangle.height);
    clutter_actor_insert_child_at_index (CLUTTER_ACTOR (bg), actor, -1);
    clutter_actor_set_opacity (actor, 0);
    clutter_actor_show (actor);
    g_signal_connect (background, "changed", G_CALLBACK (on_changed), bg);

    if (bg->current_background)
    {
        bg->previous_background = bg->current_background;
    }

    bg->current_background = actor;

    style = g_settings_get_enum (bg->settings, BACKGROUND_STYLE);
    current_background_filename = g_settings_get_string (bg->settings, PICTURE_URI);

    if (style == G_DESKTOP_BACKGROUND_STYLE_NONE)
    {
        set_color_background (bg->settings, background);
    }
    else
    {
        set_image_background (current_background_filename, background, &style);
    }
}

static void
on_settings_changed (GSettings   *settings,
                     const gchar *key,
                     gpointer     user_data)
{
    update (ENGAGE_BACKGROUND (user_data));
}

static void
engage_background_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
    EngageBackground *bg = ENGAGE_BACKGROUND (object);

    switch (prop_id)
    {
        case PROP_SCREEN:
            bg->screen = g_value_get_pointer (value);
            break;
        case PROP_MONITOR:
            bg->monitor = g_value_get_int (value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void
engage_background_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
    EngageBackground *bg = ENGAGE_BACKGROUND (object);

    switch (prop_id)
    {
        case PROP_SCREEN:
            g_value_set_pointer (value, bg->screen);
            break;
        case PROP_MONITOR:
            g_value_set_int (value, bg->monitor);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void
engage_background_dispose (GObject *object)
{
    EngageBackground *bg = ENGAGE_BACKGROUND (object);

    g_clear_object (&bg->settings);

    G_OBJECT_CLASS (engage_background_parent_class)->dispose (object);
}

static GObject *
engage_background_constructor (GType                  type,
                               guint                  n_props,
                               GObjectConstructParam *props)
{
    GObject *object;
    EngageBackground *bg;
    MetaRectangle rectangle;

    object = G_OBJECT_CLASS (engage_background_parent_class)->constructor (type, n_props, props);
    bg = ENGAGE_BACKGROUND (object);

    meta_screen_get_monitor_geometry (bg->screen, bg->monitor, &rectangle);
    clutter_actor_set_position (CLUTTER_ACTOR (bg), rectangle.x, rectangle.y);
    clutter_actor_set_size (CLUTTER_ACTOR (bg), rectangle.width, rectangle.height);

    g_signal_connect (bg->settings, "changed", G_CALLBACK (on_settings_changed), bg);

    update (bg);

    return object;
}

static void
engage_background_init (EngageBackground *bg)
{
    bg->settings = g_settings_new (BACKGROUND_SCHEMA);

    clutter_actor_set_background_color (CLUTTER_ACTOR (bg), CLUTTER_COLOR_DarkGreen);
}

static void
engage_background_class_init (EngageBackgroundClass *klass)
{
    GObjectClass *obj_class = G_OBJECT_CLASS (klass);

    obj_class->dispose = engage_background_dispose;
    obj_class->constructor = engage_background_constructor;
    obj_class->get_property = engage_background_get_property;
    obj_class->set_property = engage_background_set_property;

    obj_props[PROP_SCREEN] =
        g_param_spec_pointer ("screen",
                              "Screen",
                              "The screen for this background",
                              G_PARAM_CONSTRUCT | G_PARAM_WRITABLE);

    obj_props[PROP_MONITOR] =
        g_param_spec_int ("monitor",
                          "Monitor",
                          "The index of this backgrounds monitor",
                          0, 100, 0, G_PARAM_CONSTRUCT | G_PARAM_WRITABLE);

    g_object_class_install_properties (obj_class, N_PROPERTIES, obj_props);
}

EngageBackground *
engage_background_new (MetaScreen *screen,
                       int         monitor)
{
    return g_object_new (ENGAGE_TYPE_BACKGROUND,
                         "screen", screen,
                         "monitor", monitor,
                         NULL);
}
