#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <bits/ensure.h>
#include <mlibc/allocator.hpp>
#include <mlibc/arch-defs.hpp>
#include <mlibc/debug.hpp>
#include <mlibc/sysdeps.hpp>

unsigned int alarm(unsigned int seconds) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

int chdir(const char *path) {
	if(!mlibc::sys_chdir) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_chdir(path); e) {
		errno = e;
		return -1;
	}
	return 0;
}

int fchdir(int fd) {
	if(!mlibc::sys_fchdir) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_fchdir(fd); e) {
		errno = e;
		return -1;
	}
	return 0;
}

int chown(const char *path, uid_t uid, gid_t gid) {
	mlibc::infoLogger() << "\e[31mmlibc: chown() is not implemented correctly\e[39m"
			<< frg::endlog;
	return 0;
}
ssize_t confstr(int, char *, size_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
char *crypt(const char *, const char *) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
void _exit(int status) {
	mlibc::sys_exit(status);
}
void encrypt(char block[64], int flags) {
	__ensure(!"Not implemented");
}
int execl(const char *path, const char *arg0, ...) {
	// TODO: It's a stupid idea to limit the number of arguments here.
	char *argv[16];
	argv[0] = const_cast<char *>(arg0);

	va_list args;
	int n = 1;
	va_start(args, arg0);
	while(true) {
		__ensure(n < 15);
		auto argn = va_arg(args, const char *);
		argv[n++] = const_cast<char *>(argn);
		if(!argn)
			break;
	}
	va_end(args);
	argv[n] = nullptr;

	return execve(path, argv, environ);
}
int execle(const char *, const char *, ...) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int execlp(const char *, const char *, ...) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

int execv(const char *path, char *const argv[]) {
	return execve(path, argv, environ);
}

int execvp(const char *file, char *const argv[]) {
	if(!mlibc::sys_execve) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}

	if(strchr(file, '/')) {
		int e = mlibc::sys_execve(file, argv, environ);
		__ensure(e && "sys_execve() is supposed to never return with success");
		errno = e;
		return -1;
	}

	frg::string_view dirs;
	if(const char *pv = getenv("PATH"); pv) {
		dirs = pv;
	}else{
		dirs = "/bin:/usr/bin";
	}

	size_t p = 0;
	int res = ENOENT;
	while(p < dirs.size()) {
		size_t s; // Offset of next colon or end of string.
		if(size_t cs = dirs.find_first(':', p); cs != size_t(-1)) {
			s = cs;
		}else{
			s = dirs.size();
		}

		frg::string<MemoryAllocator> path{getAllocator()};
		path += dirs.sub_string(p, s - p);
		path += "/";
		path += file;

		mlibc::infoLogger() << "mlibc: execvp() tries '" << path.data() << "'" << frg::endlog;
		int e = mlibc::sys_execve(path.data(), argv, environ);
		__ensure(e && "sys_execve() is supposed to never return with success");
		switch(e) {
		case ENOENT:
		case ENOTDIR:
			break;
		case EACCES:
			res = EACCES;
			break;
		default:
			errno = e;
			return -1;
		}

		p = s + 1;
	}

	errno = res;
	return -1;
}

int faccessat(int, const char *, int, int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int fchown(int fd, uid_t uid, gid_t gid) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int fchownat(int fd, const char *path, uid_t uid, gid_t gid, int flags) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int fdatasync(int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int fexecve(int, char *const [], char *const []) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
long fpathconf(int, int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int fsync(int fd) {
	if(!mlibc::sys_fsync) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(auto e = mlibc::sys_fsync(fd); e) {
		errno = e;
		return -1;
	}
	return 0;
}
int ftruncate(int fd, off_t size) {
	if(!mlibc::sys_ftruncate) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_ftruncate(fd, size); e) {
		errno = e;
		return -1;
	}
	return 0;
}
char *getcwd(char *buffer, size_t size) {
	__ensure(buffer && "glibc extension for !buffer is not implemented");
	if(!mlibc::sys_getcwd) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return NULL;
	}
	if(int e = mlibc::sys_getcwd(buffer, size); e) {
		errno = e;
		return NULL;
	}
	return buffer;
}
int getgroups(int, gid_t []) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
long gethostid(void) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

int gethostname(char *buffer, size_t bufsize) {
	if(!mlibc::sys_gethostname) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(auto e = mlibc::sys_gethostname(buffer, bufsize); e) {
		errno = e;
		return -1;
	}
	return 0;
}

char *getlogin(void) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int getlogin_r(char *, size_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

// optarg and optind are provided to us by the GLIBC part of the mlibc.

static char *scan = NULL; /* Private scan pointer. */

int getopt(int argc, char *const argv[], const char *optstring) {
	char c;
	char *place;

	optarg = NULL;

	if (!scan || *scan == '\0') {
		if (optind == 0)
			optind++;

		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
			return EOF;
		if (argv[optind][1] == '-' && argv[optind][2] == '\0') {
			optind++;
			return EOF;
		}

		scan = argv[optind]+1;
		optind++;
	}

	c = *scan++;
	place = strchr(optstring, c);

	if (!place || c == ':') {
		fprintf(stderr, "%s: unknown option -%c\n", argv[0], c);
		return '?';
	}

	place++;
	if (*place == ':') {
		if (*scan != '\0') {
			optarg = scan;
			scan = NULL;
		} else if( optind < argc ) {
			optarg = argv[optind];
			optind++;
		} else {
			fprintf(stderr, "%s: option requires argument -%c\n", argv[0], c);
			return ':';
		}
	}

	return c;
}

pid_t getpgid(pid_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
pid_t getpgrp(void) {
	mlibc::infoLogger() << "mlibc: getpgrp() returns invalid PID" << frg::endlog;
	return 0;
}
pid_t getsid(pid_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int lchown(const char *path, uid_t uid, gid_t gid) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int link(const char *, const char *) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int linkat(int, const char *, int, const char *, int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int lockf(int, int, off_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int nice(int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
long pathconf(const char *, int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int pause(void) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

int pipe(int *fds) {
	if(!mlibc::sys_pipe) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_pipe(fds, 0); e) {
		errno = e;
		return -1;
	}
	return 0;
}

int pipe2(int *fds, int flags) {
	if(!mlibc::sys_pipe) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_pipe(fds, flags); e) {
		errno = e;
		return -1;
	}
	return 0;
}

ssize_t pread(int, void *, size_t, off_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
ssize_t pwrite(int, const void *, size_t, off_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
ssize_t readlink(const char *__restrict path, char *__restrict buffer, size_t max_size) {
	ssize_t length;
	if(!mlibc::sys_readlink) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_readlink(path, buffer, max_size, &length); e) {
		errno = e;
		return -1;
	}
	return length;
}
ssize_t readlinkat(int, const char *__restrict, char *__restrict, size_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int rmdir(const char *) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int setegid(gid_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int seteuid(uid_t) {
	mlibc::infoLogger() << "\e[31mmlibc: seteuid() is a no-op\e[39m" << frg::endlog;
	return 0;
}
int setgid(gid_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int setpgid(pid_t, pid_t) {
	mlibc::infoLogger() << "mlibc: setpgid() fails with ENOSYS" << frg::endlog;
	errno = ENOSYS;
	return -1;
}
pid_t setpgrp(void) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int setregid(gid_t, gid_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int setreuid(uid_t, uid_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
pid_t setsid(void) {
	mlibc::infoLogger() << "\e[31mmlibc: setsid() is a no-op\e[39m" << frg::endlog;
	return 1;
}
int setuid(uid_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
void swab(const void *__restrict, void *__restrict, ssize_t) {
	__ensure(!"Not implemented");
}
int symlink(const char *target_path, const char *link_path) {
	if(!mlibc::sys_symlink) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_symlink(target_path, link_path); e) {
		errno = e;
		return -1;
	}
	return 0;
}
int symlinkat(const char *, int, const char *) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
void sync(void) {
	__ensure(!"Not implemented");
}
unsigned long sysconf(int number) {
	if(number == _SC_PAGE_SIZE) {
		return mlibc::page_size;
	}else{
		mlibc::panicLogger() << "\e[31mmlibc: sysconf() call is not implemented\e[39m"
				<< frg::endlog;
		__builtin_unreachable();
	}
}
pid_t tcgetpgrp(int fd) {
	(void)fd;
	mlibc::infoLogger() << "mlibc: tcgetpgrp() fails with ENOSYS" << frg::endlog;
	errno = ENOSYS;
	return -1;
}
int tcsetpgrp(int, pid_t) {
	mlibc::infoLogger() << "mlibc: tcsetpgrp() fails with ENOSYS" << frg::endlog;
	errno = ENOSYS;
	return -1;
}
int truncate(const char *, off_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

char *ttyname(int fd) {
	const size_t size = 128;
	static thread_local char buf[size];
	if(!mlibc::sys_ttyname) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return nullptr;
	}
	if(int e = mlibc::sys_ttyname(fd, buf, size); e) {
		errno = e;
		return nullptr;
	}
	return buf;
}

int ttyname_r(int, char *, size_t) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}
int unlink(const char *path) {
	if(!mlibc::sys_unlink) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_unlink(path); e) {
		errno = e;
		return -1;
	}
	return 0;
}
int unlinkat(int, const char *, int) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

int getpagesize() {
	return mlibc::page_size;
}

char *get_current_dir_name(void) {
	__ensure(!"Not implemented");
	__builtin_unreachable();
}

// This is a Linux extension
pid_t gettid(void) {
	mlibc::infoLogger() << "\e[31mmlibc: gettid() is not implemented correctly\e[39m"
			<< frg::endlog;
	return mlibc::sys_getpid();
}


ssize_t write(int fd, const void *buf, size_t count) {
	ssize_t bytes_written;
	if(int e = mlibc::sys_write(fd, buf, count, &bytes_written); e) {
		errno = e;
		return (ssize_t)-1;
	}
	return bytes_written;
}

ssize_t read(int fd, void *buf, size_t count) {
	ssize_t bytes_read;
	if(int e = mlibc::sys_read(fd, buf, count, &bytes_read); e) {
		errno = e;
		return (ssize_t)-1;
	}
	return bytes_read;
}

off_t lseek(int fd, off_t offset, int whence) {
	off_t new_offset;
	if(int e = mlibc::sys_seek(fd, offset, whence, &new_offset); e) {
		errno = e;
		return (off_t)-1;
	}
	return new_offset;
}

int close(int fd) {
	return mlibc::sys_close(fd);
}

unsigned int sleep(unsigned int secs) {
	time_t seconds = secs;
	long nanos = 0;
	if(!mlibc::sys_sleep) {
		MLIBC_MISSING_SYSDEP();
		__ensure(!"Cannot continue without sys_sleep()");
	}
	mlibc::sys_sleep(&seconds, &nanos);
	return seconds;
}

// In contrast to sleep() this functions returns 0/-1 on success/failure.
int usleep(useconds_t usecs) {
	time_t seconds = 0;
	long nanos = usecs * 1000;
	if(!mlibc::sys_sleep) {
		MLIBC_MISSING_SYSDEP();
		__ensure(!"Cannot continue without sys_sleep()");
	}
	return mlibc::sys_sleep(&seconds, &nanos);
}

int dup(int fd) {
	int newfd;
	if(!mlibc::sys_dup) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_dup(fd, 0, &newfd); e) {
		errno = e;
		return -1;
	}
	return newfd;
}

int dup2(int fd, int newfd) {
	if(!mlibc::sys_dup2) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_dup2(fd, 0, newfd); e) {
		errno = e;
		return -1;
	}
	return newfd;
}

pid_t fork(void) {
	pid_t child;
	if(!mlibc::sys_fork) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_fork(&child); e) {
		errno = e;
		return -1;
	}
	return child;
}

int execve(const char *path, char *const argv[], char *const envp[]) {
	if(!mlibc::sys_execve) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	int e = mlibc::sys_execve(path, argv, envp);
	__ensure(e && "sys_execve() is expected to fail if it returns");
	errno = e;
	return -1;
}

gid_t getgid(void) {
	if(!mlibc::sys_getgid) {
		MLIBC_MISSING_SYSDEP();
		__ensure(!"Cannot continue without sys_getgid()");
	}
	return mlibc::sys_getgid();
}
gid_t getegid(void) {
	if(!mlibc::sys_getegid) {
		MLIBC_MISSING_SYSDEP();
		__ensure(!"Cannot continue without sys_getegid()");
	}
	return mlibc::sys_getegid();
}

uid_t getuid(void) {
	if(!mlibc::sys_getuid) {
		MLIBC_MISSING_SYSDEP();
		__ensure(!"Cannot continue without sys_getuid()");
	}
	return mlibc::sys_getuid();
}
uid_t geteuid(void) {
	if(!mlibc::sys_geteuid) {
		MLIBC_MISSING_SYSDEP();
		__ensure(!"Cannot continue without sys_geteuid()");
	}
	return mlibc::sys_geteuid();
}

pid_t getpid(void) {
	if(!mlibc::sys_getpid) {
		MLIBC_MISSING_SYSDEP();
		__ensure(!"Cannot continue without sys_getpid()");
	}
	return mlibc::sys_getpid();
}

pid_t getppid(void) {
	if(!mlibc::sys_getppid) {
		MLIBC_MISSING_SYSDEP();
		__ensure(!"Cannot continue without sys_getppid()");
	}
	return mlibc::sys_getppid();
}

int access(const char *path, int mode) {
	if(!mlibc::sys_access) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_access(path, mode); e) {
		errno = e;
		return -1;
	}
	return 0;
}

int isatty(int fd) {
	if(!mlibc::sys_isatty) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return 0; // isatty() returns 0 on failure.
	}
	if(int e = mlibc::sys_isatty(fd); e) {
		errno = e;
		return 0;
	}
	return 1;
}

int chroot(const char *ptr) {
	if(!mlibc::sys_chroot) {
		MLIBC_MISSING_SYSDEP();
		errno = ENOSYS;
		return -1;
	}
	if(int e = mlibc::sys_chroot(ptr); e) {
		errno = e;
		return -1;
	}
	return 0;
}


