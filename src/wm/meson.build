install_data(
    'engage-wm.desktop',
    install_dir: join_paths(datadir, 'applications')
)

engage_wm_sources = [
    'engage-wm.c',
    'main.c',
    'backgrounds/engage-background.c',
]

engage_wm_deps = [
    dep_mutter,
    dep_gnomedesktop,
    dep_gsettings,
]

executable(
    'engage-wm', engage_wm_sources,
    dependencies: engage_wm_deps,
    include_directories: config_inc_dir,
    c_args: [
        '-DGNOME_DESKTOP_USE_UNSTABLE_API',
    ],
    install: true,
    install_rpath: mutter_typelibdir,
)
