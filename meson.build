project('os0-libc', 'c', version: '0.0.1', default_options: ['c_std=gnu99'],
	license: 'LGPL-3.0-or-later', meson_version: '>= 0.55.0')
add_project_arguments('-fno-common', language: 'c')

libc_config = configuration_data()
cc = meson.get_compiler('c')

hostcpu = host_machine.cpu_family()
if hostcpu == 'x86'
  libc_config.set('__INT32_T', 'long')
  libc_config.set('__UINT32_T', 'unsigned long')
  libc_config.set('__INT32_SUFFIX', 'L')
  libc_config.set('__UINT32_SUFFIX', 'UL')
  libc_config.set('__INT64_T', 'long long')
  libc_config.set('__UINT64_T', 'unsigned long long')
  libc_config.set('__INT64_SUFFIX', 'LL')
  libc_config.set('__UINT64_SUFFIX', 'ULL')
  libc_config.set('__INTPTR_T', 'int32_t')
  libc_config.set('__UINTPTR_T', 'uint32_t')
elif hostcpu == 'x86_64'
  libc_config.set('__INT32_T', 'int')
  libc_config.set('__UINT32_T', 'unsigned int')
  libc_config.set('__INT32_SUFFIX', '')
  libc_config.set('__UINT32_SUFFIX', 'U')
  libc_config.set('__INT64_T', 'long')
  libc_config.set('__UINT64_T', 'unsigned long')
  libc_config.set('__INT64_SUFFIX', 'L')
  libc_config.set('__UINT64_SUFFIX', 'UL')
  libc_config.set('__INTPTR_T', 'int64_t')
  libc_config.set('__UINTPTR_T', 'uint64_t')
else
  error('Unsupported CPU type: ' + hostcpu)
endif

subdir('include')

libc_inc = include_directories('include', 'private-include')

libc_src = [
  'stdio/ferror.c',
  'stdio/fflush.c',
  'stdio/fio.c',
  'stdio/fprintf.c',
  'stdio/fputc.c',
  'stdio/fputs.c',
  'stdio/setvbuf.c',
  'stdio/stdstr.c',
  'stdio/vfprintf.c',
  'stdlib/abort.c',
  'stdlib/assert.c',
  'stdlib/bitsearch.c',
  'stdlib/ctype.c',
  'stdlib/dlmalloc.c',
  'stdlib/errno.c',
  'stdlib/exit.c',
  'stdlib/init.c',
  'stdlib/isatty.c',
  'stdlib/malloc.c',
  'stdlib/qsort.c',
  'stdlib/signal.c',
  'stdlib/stdmath.c',
  'stdlib/termios.c',
  'string/memcmp.c',
  'string/memset.c',
  'string/strcat.c',
  'string/strcmp.c',
  'string/strcpy.c',
  'string/strerror.c',
  'string/string.c',
  'string/strtok.c',
  'syscall/access.c',
  'syscall/brk.S',
  'syscall/chdir.c',
  'syscall/chmod.c',
  'syscall/chown.c',
  'syscall/close.c',
  'syscall/creat.c',
  'syscall/execve.c',
  'syscall/exit.c',
  'syscall/fcntl.S',
  'syscall/fork.c',
  'syscall/gettimeofday.c',
  'syscall/io.c',
  'syscall/ioctl.S',
  'syscall/kill.c',
  'syscall/link.c',
  'syscall/lseek.c',
  'syscall/mkdir.c',
  'syscall/mknod.c',
  'syscall/open.S',
  'syscall/readlink.c',
  'syscall/rename.c',
  'syscall/sigaction.c',
  'syscall/stat.c',
  'syscall/symlink.c',
  'syscall/syscall.S',
  'syscall/truncate.c',
  'syscall/unlink.c'
]

if get_option('shared')
  libc_targ = 'both_libraries'
else
  libc_targ = 'static_library'
endif

build_target('c', libc_src, install: true, include_directories: libc_inc,
	     target_type: libc_targ, version: '1.0.0',
	     link_args: ['-nostartfiles', '-nostdlib', '-lgcc'])

subdir('rt')
