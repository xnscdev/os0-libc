rtld_src = [
  'load.c',
  'main.c',
  'print.c',
  'syscall.S'
]

rtld = static_library('rtld', rtld_src, include_directories: libc_inc)

custom_target('ld.so.2', input: rtld, output: 'ld.so.2',
	      command: [cc_cmd, '-shared', '-o', '@OUTPUT@', '-fPIC',
			'-Wl,--whole-archive', '@INPUT@',
			'-Wl,--no-whole-archive'],
	      build_by_default: true, install: true, install_dir: '/lib')
