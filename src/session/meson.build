session_data = configuration_data()
session_data.set('PACKAGE_VERSION', package_version)

gnome_session_components = [
    'org.gnome.SettingsDaemon.A11ySettings',
    'org.gnome.SettingsDaemon.Clipboard',
    'org.gnome.SettingsDaemon.Color',
    'org.gnome.SettingsDaemon.Datetime',
    'org.gnome.SettingsDaemon.Housekeeping',
    'org.gnome.SettingsDaemon.Keyboard',
    'org.gnome.SettingsDaemon.MediaKeys',
    'org.gnome.SettingsDaemon.Mouse',
    'org.gnome.SettingsDaemon.Power',
    'org.gnome.SettingsDaemon.PrintNotifications',
    'org.gnome.SettingsDaemon.Rfkill',
    'org.gnome.SettingsDaemon.ScreensaverProxy',
    'org.gnome.SettingsDaemon.Sharing',
    'org.gnome.SettingsDaemon.Smartcard',
    'org.gnome.SettingsDaemon.Sound',
    'org.gnome.SettingsDaemon.Wacom',
    'org.gnome.SettingsDaemon.XSettings',
]

engage_components = [
    'engage-wm',
]

dep_gsd = dependency('gnome-settings-daemon', version: '>= 3.27.90')
session_components = engage_components + gnome_session_components

session_data.set('SESSION_COMPONENTS', ';'.join(session_components))
session_data.set('prefix', join_paths(get_option('prefix')))

configure_file(
    input: 'engage-desktop.session.in',
    output: 'engage-desktop.session',
    configuration: session_data,
    install_dir: join_paths(datadir, 'gnome-session', 'sessions'),
)

configure_file(
    input: 'engage-desktop.in',
    output: 'engage-desktop',
    configuration: session_data,
    install_dir: join_paths(get_option('prefix'), get_option('bindir')),
)

configure_file(
    input: 'engage-desktop.desktop.in',
    output: 'engage-desktop.desktop',
    configuration: session_data,
    install_dir: join_paths(datadir, 'xsessions'),
)
