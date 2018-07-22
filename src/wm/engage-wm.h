#ifndef __ENGAGE_WM_H__
#define __ENGAGE_WM_H__

#include <glib.h>
#include <meta/meta-plugin.h>

G_BEGIN_DECLS

typedef struct _EngageWm      EngageWm;
typedef struct _EngageWmClass EngageWmClass;

#define ENGAGE_WM_TYPE            (engage_wm_get_type ())
#define ENGAGE_WM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), ENGAGE_WM_TYPE, EngageWm))
#define ENGAGE_WM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  ENGAGE_WM_TYPE, EngageWmClass))
#define IS_ENGAGE_WM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), ENGAGE_WM_TYPE))
#define IS_ENGAGE_WM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  ENGAGE_WM_TYPE))
#define ENGAGE_WM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), ENGAGE_WM_TYPE, EngageWmClass))

struct _EngageWm
{
    MetaPlugin parent;

    ClutterActor *ui_group;
    ClutterActor *stage;
    ClutterActor *window_group;
    ClutterActor *top_window_group;
    ClutterActor *background_group;
};

struct _EngageWmClass
{
    MetaPluginClass parent_class;
};

GType engage_wm_get_type (void);

G_END_DECLS

#endif /* __ENGAGE_WM_H__ */
