configure_file(input: 'stdint.h.in', output: 'stdint.h',
	       configuration: libc_config,
	       install_dir: get_option('includedir'))

install_headers('assert.h',
		'errno.h',
		'fcntl.h',
		'stdio.h',
		'stdlib.h',
		'string.h',
		'time.h',
		'unistd.h')

subdir('sys')
