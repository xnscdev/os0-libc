configure_file(input: 'stdint.h.in', output: 'stdint.h',
	       configuration: libc_config,
	       install_dir: get_option('includedir'))

install_headers('alloca.h',
		'assert.h',
		'ctype.h',
		'errno.h',
		'fcntl.h',
		'getopt.h',
		'locale.h',
		'math.h',
		'pwd.h',
		'setjmp.h',
		'signal.h',
		'stdio.h',
		'stdlib.h',
		'string.h',
		'strings.h',
		'termios.h',
		'time.h',
		'unistd.h',
		'wchar.h',
		'wctype.h')

# Install a limits.h that provides more macros
install_data('_limits.h', install_dir: get_option('includedir'),
	     rename: 'limits.h')

subdir('bits')
subdir('sys')
