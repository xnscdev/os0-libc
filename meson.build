project('os0-libc', 'c', version: '0.0.1', default_options: ['c_std=gnu99'],
	license: 'LGPL-3.0-or-later', meson_version: '>= 0.55.0')
add_project_arguments('-fno-common', language: 'c', native: false)

libc_config = configuration_data()
cc = meson.get_compiler('c')

hostcpu = host_machine.cpu_family()
if hostcpu == 'x86'
  libc_config.set('__INT64_T', 'long long')
  libc_config.set('__UINT64_T', 'unsigned long long')
  libc_config.set('__INT64_SUFFIX', 'LL')
  libc_config.set('__UINT64_SUFFIX', 'ULL')
  libc_config.set('__INTPTR_T', 'int32_t')
  libc_config.set('__UINTPTR_T', 'uint32_t')
elif hostcpu == 'x86_64'
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

libc_src = [
  'stdlib/assert.c'
]

static_library('c', libc_src, install: true, include_directories: libc_inc)
