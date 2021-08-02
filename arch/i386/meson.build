# Build crt0.o
custom_target('crt0.o', input: 'crt0.S', output: 'crt0.o',
	      command: [cc_cmd, '-c', '-o', '@OUTPUT@', '@INPUT@'],
	      build_by_default: true, install: true,
	      install_dir: get_option('libdir'))

# Build dynamic linker if shared libc enabled
if get_option('shared')
  subdir('rtld')
endif
