arch_src = [
    'brk.S',
    'fcntl.S',
    'ioctl.S',
    'longjmp.S',
    'memchr.S',
    'memcmp.S',
    'memcpy.S',
    'memmove.S',
    'memset.S',
    'open.S',
    'sigsetjmp.S',
    'syscall.S'
]

# Build crt0.o
custom_target('crt0.o', input: 'crt0.S', output: 'crt0.o',
	      command: [cc_cmd, '-c', '-o', '@OUTPUT@', '@INPUT@'],
	      build_by_default: true, install: true,
	      install_dir: get_option('libdir'))
