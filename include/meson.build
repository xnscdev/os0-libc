configure_file(input: 'stdint.h.in', output: 'stdint.h',
	       configuration: libc_config,
	       install_dir: get_option('includedir'))

install_headers('assert.h',
		'errno.h',
		'fcntl.h',
		'math.h',
		'signal.h',
		'stdio.h',
		'stdlib.h',
		'string.h',
		'termios.h',
		'time.h',
		'unistd.h')

subdir('bits/types')
subdir('sys')
