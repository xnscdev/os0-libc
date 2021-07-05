configure_file(input: 'stdint.h.in', output: 'stdint.h',
	       configuration: libc_config,
	       install_dir: get_option('includedir'))
configure_file(input: 'inttypes.h.in', output: 'inttypes.h',
	       configuration: libc_config,
	       install_dir: get_option('includedir'))

install_headers('alloca.h',
		'assert.h',
		'complex.h',
		'ctype.h',
		'dirent.h',
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

if get_option('shared')
  install_headers('dlfcn.h')
endif

# Install a limits.h that provides more macros
install_data('_limits.h', install_dir: get_option('includedir'),
	     rename: 'limits.h')

subdir('bits')
subdir('sys')
