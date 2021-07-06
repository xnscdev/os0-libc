project('os0-libc', 'c', version: '0.0.1', default_options: ['c_std=gnu99'],
	license: 'LGPL-3.0-or-later', meson_version: '>= 0.55.0')
add_project_arguments('-fno-common', '-U_FILE_OFFSET_BITS', language: 'c')

libc_config = configuration_data()
libc_config.set_quoted('LIBC_VERSION', meson.project_version())

cc = meson.get_compiler('c')
hostcpu = host_machine.cpu_family()

if hostcpu == 'x86'
  bits = 32
  libc_config.set10('ARCH_I386', true)
elif hostcpu == 'x86_64'
  bits = 64
  libc_config.set10('ARCH_X86_64', true)
else
  error('Unsupported CPU type: ' + hostcpu)
endif

if bits == 32
  # stdint.h macros
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

  # inttypes.h macros
  libc_config.set('__FMT32', 'l')
  libc_config.set('__FMT64', 'll')
else
  # stdint.h macros
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

  # inttypes.h macros
  libc_config.set('__FMT32', '')
  libc_config.set('__FMT64', 'l')
endif

configure_file(input: 'config.h.in', output: 'config.h',
	       configuration: libc_config)

subdir('include')

libc_inc = include_directories('.', 'include', 'private-include')

libc_src = [
  'locale/localeconv.c',
  'locale/posix.c',
  'locale/setlocale.c',
  'locale/uselocale.c',
  'stdio/ferror.c',
  'stdio/fflush.c',
  'stdio/fgetc.c',
  'stdio/fgets.c',
  'stdio/fgetwc.c',
  'stdio/fgetws.c',
  'stdio/fio.c',
  'stdio/flockfile.c',
  'stdio/fopen.c',
  'stdio/fpos.c',
  'stdio/fputc.c',
  'stdio/fputs.c',
  'stdio/fputwc.c',
  'stdio/fputws.c',
  'stdio/fwide.c',
  'stdio/getdelim.c',
  'stdio/perror.c',
  'stdio/popen.c',
  'stdio/printf.c',
  'stdio/scanf.c',
  'stdio/setvbuf.c',
  'stdio/stdstr.c',
  'stdio/tmpnam.c',
  'stdio/vscanf.c',
  'stdlib/abort.c',
  'stdlib/assert.c',
  'stdlib/bitsearch.c',
  'stdlib/bsearch.c',
  'stdlib/dirent.c',
  'stdlib/dlmalloc.c',
  'stdlib/env.c',
  'stdlib/errno.c',
  'stdlib/exec.c',
  'stdlib/exit.c',
  'stdlib/getopt.c',
  'stdlib/grp.c',
  'stdlib/init.c',
  'stdlib/isatty.c',
  'stdlib/killpg.c',
  'stdlib/locks.c',
  'stdlib/login.c',
  'stdlib/longjmp.S',
  'stdlib/lsearch.c',
  'stdlib/malloc.c',
  'stdlib/mktemp.c',
  'stdlib/passwd.c',
  'stdlib/qsort.c',
  'stdlib/rand.c',
  'stdlib/realpath.c',
  'stdlib/setjmp.c',
  'stdlib/signal.c',
  'stdlib/sigset.c',
  'stdlib/sigsetjmp.S',
  'stdlib/sleep.c',
  'stdlib/statvfs.c',
  'stdlib/stdmath.c',
  'stdlib/sysconf.c',
  'stdlib/system.c',
  'stdlib/time.c',
  'string/atol.c',
  'string/bset.c',
  'string/fstrtol_l.c',
  'string/memcmp.c',
  'string/memcpy.c',
  'string/memset.c',
  'string/strcasestr.c',
  'string/strcat.c',
  'string/strcmp.c',
  'string/strcpy.c',
  'string/strerror.c',
  'string/strftime.c',
  'string/string.c',
  'string/strpbrk.c',
  'string/strsignal.c',
  'string/strspn.c',
  'string/strstr.c',
  'string/strtod.c',
  'string/strtod_l.c',
  'string/strtok.c',
  'string/strtol.c',
  'string/strtol_l.c',
  'syscall/access.c',
  'syscall/brk.S',
  'syscall/chdir.c',
  'syscall/chmod.c',
  'syscall/chown.c',
  'syscall/close.c',
  'syscall/creat.c',
  'syscall/dup.c',
  'syscall/execve.c',
  'syscall/exit.c',
  'syscall/fcntl.S',
  'syscall/fork.c',
  'syscall/getcwd.c',
  'syscall/getrusage.c',
  'syscall/gettimeofday.c',
  'syscall/io.c',
  'syscall/ioctl.S',
  'syscall/itimer.c',
  'syscall/kill.c',
  'syscall/link.c',
  'syscall/lseek.c',
  'syscall/mkdir.c',
  'syscall/mknod.c',
  'syscall/mman.c',
  'syscall/nanosleep.c',
  'syscall/open.S',
  'syscall/pgid.c',
  'syscall/pipe.c',
  'syscall/readlink.c',
  'syscall/rename.c',
  'syscall/sigaction.c',
  'syscall/signal.S',
  'syscall/signal.c',
  'syscall/stat.c',
  'syscall/statfs.c',
  'syscall/symlink.c',
  'syscall/sync.c',
  'syscall/syscall.S',
  'syscall/time.c',
  'syscall/truncate.c',
  'syscall/ugid.c',
  'syscall/umask.c',
  'syscall/unlink.c',
  'syscall/wait.c',
  'syscall/xattr.c',
  'termios/cfmakeraw.c',
  'termios/cfspeed.c',
  'termios/tcattr.c',
  'termios/tcflush.c',
  'termios/tcpgrp.c',
  'termios/ttyname.c',
  'wchar/btowc.c',
  'wchar/ctype.c',
  'wchar/mbrtowc.c',
  'wchar/wcrtomb.c',
  'wchar/wctob.c',
  'wchar/wctype.c'
]

if get_option('shared')
  lib_targ = 'both_libraries'
else
  lib_targ = 'static_library'
endif
lib_ldflags = ['-nostdlib', '-lgcc']

libc = build_target('c', libc_src, install: true,
		    include_directories: libc_inc, target_type: lib_targ,
		    version: '1', link_args: lib_ldflags)

libc_dep = declare_dependency(include_directories: libc_inc, link_with: libc)

# Build libm
subdir('math')

cc_cmd = meson.get_compiler('c').cmd_array()
if get_option('debug')
  cc_cmd += ['-g']
endif

# Build crt0.o
custom_target('crt0.o', input: 'stdlib/crt0.S', output: 'crt0.o',
	      command: [cc_cmd, '-c', '-o', '@OUTPUT@', '@INPUT@'],
	      build_by_default: true, install: true,
	      install_dir: get_option('libdir'))

# Build dynamic linker if shared libc enabled
if get_option('shared')
  subdir('rtld')
endif
