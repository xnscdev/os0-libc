configure_file(input: 'stdint.h.in', output: 'stdint.h',
	       configuration: libc_config,
	       install_dir: get_option('includedir'))

install_headers('assert.h',
		'ctype.h',
		'errno.h',
		'fcntl.h',
		'locale.h',
		'math.h',
		'signal.h',
		'stdio.h',
		'stdlib.h',
		'string.h',
		'termios.h',
		'time.h',
		'unistd.h')

# Install a limits.h that provides more macros
install_data('_limits.h', install_dir: get_option('includedir'),
	     rename: 'limits.h')

subdir('bits/types')
subdir('sys')
