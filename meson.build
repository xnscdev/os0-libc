project('os0-libc', 'c', version: '0.0.1', default_options: ['c_std=gnu99'],
	license: 'LGPL-3.0-or-later', meson_version: '>= 0.55.0')
add_project_arguments('-ffreestanding', '-fno-common', language: 'c',
		      native: false)

subdir('include')
