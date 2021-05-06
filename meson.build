project('os0-libc', 'c', version: '0.0.1', default_options: ['c_std=gnu99'],
	license: 'LGPL-3.0-or-later', meson_version: '>= 0.55.0')
add_project_arguments('-ffreestanding', '-fno-common', language: 'c',
		      native: false)

libc_config = configuration_data()
cc = meson.get_compiler('c')

if cc.sizeof('int') == 4
  libc_config.set('__INT32_T', 'int')
  libc_config.set('__UINT32_T', 'unsigned int')
  libc_config.set('__INT32_SUFFIX', '')
  libc_config.set('__UINT32_SUFFIX', '')
else
  libc_config.set('__INT32_T', 'long')
  libc_config.set('__UINT32_T', 'unsigned long')
  libc_config.set('__INT32_SUFFIX', 'L')
  libc_config.set('__UINT32_SUFFIX', 'UL')
endif

if cc.sizeof('long') == 8
  libc_config.set('__INT64_T', 'long')
  libc_config.set('__UINT64_T', 'unsigned long')
  libc_config.set('__INT64_SUFFIX', 'L')
  libc_config.set('__UINT64_SUFFIX', 'UL')
else
  libc_config.set('__INT64_T', 'long long')
  libc_config.set('__UINT64_T', 'unsigned long long')
  libc_config.set('__INT64_SUFFIX', 'LL')
  libc_config.set('__UINT64_SUFFIX', 'ULL')
endif

ptrsize = cc.sizeof('void *')
if ptrsize == 8
  libc_config.set('__INTPTR_T', 'int64_t')
  libc_config.set('__UINTPTR_T', 'uint64_t')
elif ptrsize == 4
  libc_config.set('__INTPTR_T', 'int32_t')
  libc_config.set('__UINTPTR_T', 'uint32_t')
else
  error('Unsupported pointer width: ' + ptrsize)
endif

subdir('include')
