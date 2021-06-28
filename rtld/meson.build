rtld_src = [
  'api.c',
  'load.c',
  'lookup.c',
  'main.c',
  'queue.c',
  'reloc.c',
  'remap.c'
]

rtld = static_library('rtld', rtld_src, include_directories: libc_inc)

custom_target('ld.so.2', input: rtld, output: 'ld.so.2',
	      command: [cc_cmd, '-shared', '-o', '@OUTPUT@', '-fPIC',
			'-Wl,--whole-archive', '@INPUT@',
			'-Wl,--no-whole-archive', '-Wl,-soname,ld.so.2',
			'-nostartfiles', '-static'],
	      build_by_default: true, install: true, install_dir: '/lib')

# Build libdl.so
shared_library('dl', 'libdl.c', dependencies: libc_dep, version: '1',
	       install: true)
