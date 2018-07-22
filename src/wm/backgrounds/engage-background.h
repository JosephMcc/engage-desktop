#pragma once

#include <meta/screen.h>
#include <meta/meta-background-group.h>
#include <meta/meta-background-image.h>
#include <libgnome-desktop/gnome-bg.h>

G_BEGIN_DECLS

#define ENGAGE_TYPE_BACKGROUND            (engage_background_get_type ())
#define ENGAGE_BACKGROUND(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), ENGAGE_TYPE_BACKGROUND, EngageBackground))
#define ENGAGE_BACKGROUND_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ENGAGE_TYPE_BACKGROUND, EngageBackgroundClass))
#define ENGAGE_IS_BACKGROUND(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ENGAGE_TYPE_BACKGROUND))
#define ENGAGE_IS_BACKGROUND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ENGAGE_TYPE_BACKGROUND))
#define ENGAGE_BACKGROUND_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), ENGAGE_TYPE_BACKGROUND, EngageBackgroundClass))

typedef struct _EngageBackground EngageBackground;
typedef struct _EngageBackgroundClass EngageBackgroundClass;

struct _EngageBackgroundClass
{
    ClutterActorClass parent_class;
};

struct _EngageBackground
{
    MetaBackgroundGroup parent;

    MetaScreen *screen;
    int monitor;

    GSettings *settings;

    ClutterActor *previous_background;
    ClutterActor *current_background;
};

GType engage_background_get_type (void);

EngageBackground *engage_background_new (MetaScreen *screen,
                                         int         monitor);

G_END_DECLS
