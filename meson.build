project('engage-desktop',
        ['c'],
        version: '0.1',
        license: ['GPLv3'],
        default_options: ['c_std=c11'],
)

meson.add_install_script('meson_post_install.sh')

config_inc_dir = include_directories('.')

gnome_minimum_version = '>= 3.22.0'

dep_glib = dependency('glib-2.0', version: '>= 2.46.0')
dep_mutter = dependency('libmutter-2', version: '>= 3.27')
dep_gnomedesktop = dependency('gnome-desktop-3.0', version: gnome_minimum_version)
dep_gsettings = dependency('gsettings-desktop-schemas', version: gnome_minimum_version)

mutter_typelibdir = dep_mutter.get_pkgconfig_variable('typelibdir')

package_version = meson.project_version()

prefix = get_option('prefix')
datadir = join_paths(prefix, get_option('datadir'))

subdir('src')
