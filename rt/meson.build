cc_cmd = meson.get_compiler('c').cmd_array()
if get_option('debug')
  cc_cmd += ['-g']
endif

crt_cmd = [cc_cmd, '-c', '-o', '@OUTPUT@', '@INPUT@']
custom_target('crt0.o', input: 'crt0.S', output: 'crt0.o',
	      command: crt_cmd, build_by_default: true, install: true,
	      install_dir: get_option('libdir'))

if get_option('shared')
  # Build dynamic linker
  rtld_cmd = [cc_cmd, '-I../include', '-shared', '-nostdlib', '-o', '@OUTPUT@',
	      '@INPUT@']
  custom_target('ld.so.2', input: ['rtld.c', 'rtld-syscall.S'],
		output: 'ld.so.2', command: rtld_cmd, build_by_default: true,
		install: true, install_dir: '/lib')
endif
