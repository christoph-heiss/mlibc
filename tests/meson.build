ansi_test_cases = [
	'sscanf',
	'sprintf',
	'snprintf',
	'utf8',
	'strtol',
	'abs'
]

posix_test_cases = [
	'inet_ntop',
	'inet_pton',
	'pthread_rwlock',
	'getaddrinfo',
	'dprintf',
	'posix_spawn'
]

glibc_test_cases = [
	'getopt'
]

test_sources = [
	crt
]

if static
	test_sources += '../options/internal/gcc-extra/mlibc_crtbegin.S'
	test_sources += '../options/internal' / host_machine.cpu_family() / 'mlibc_crtend.S'
endif

if not disable_ansi_option
	foreach f : ansi_test_cases
		exec = executable('ansi-' + f, ['ansi/' + f + '.c', test_sources],
			link_with: libc, include_directories: libc_include_dirs,
			build_rpath: meson.build_root(),
			c_args: '-no-pie',
			link_args: ['-Wl,--dynamic-linker=' + meson.build_root() + '/ld.so',
				'-no-pie'])
		test('ansi/' + f, exec)
	endforeach
endif

if not disable_posix_option
	foreach f : posix_test_cases
		exec = executable('posix-' + f, ['posix/' + f + '.c', test_sources],
			link_with: libc, include_directories: libc_include_dirs,
			build_rpath: meson.build_root(),
			c_args: '-no-pie',
			link_args: ['-Wl,--dynamic-linker=' + meson.build_root() + '/ld.so',
				'-no-pie'])
		test('posix/' + f, exec)
	endforeach
endif

# We never disable the posix option so glibc is gated behind the posix option here.
if not disable_posix_option
	foreach f : glibc_test_cases
		exec = executable('glibc-' + f, ['glibc/' + f + '.c', test_sources],
			link_with: libc, include_directories: libc_include_dirs,
			build_rpath: meson.build_root(),
			c_args: '-no-pie',
			link_args: ['-Wl,--dynamic-linker=' + meson.build_root() + '/ld.so',
				'-no-pie'])
		test('glibc/' + f, exec)
	endforeach
endif
