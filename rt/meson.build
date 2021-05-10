crt_cmd = meson.get_compiler('c').cmd_array()
if get_option('debug')
  crt_cmd += ['-g']
endif
crt_cmd += ['-c', '-o', '@OUTPUT@', '@INPUT@']

custom_target('crt0.o', input: 'crt0.S', output: 'crt0.o',
	      command: crt_cmd, build_by_default: true, install: true,
	      install_dir: get_option('libdir'))
