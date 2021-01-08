#include <mlibc/all-sysdeps.hpp>
#include <string.h>
#include <bits/ensure.h>
#include <unistd.h>

#include <squiid/syscall_wrappers_x86_64.h>
#include <squiid/mmap.h>
#include <squiid/process.h>
#include <squiid/stat.h>

#define STUB_ONLY { __ensure(!"STUB_ONLY function was called"); __builtin_unreachable(); }

namespace mlibc {

void sys_libc_log(const char *msg) {
    ssize_t written;
    sys_write(STDERR_FILENO, msg, strlen(msg), &written);
    sys_write(STDERR_FILENO, "\n", 1, &written);
}

void sys_libc_panic() {
    sys_libc_log("mlibc panic!\n");
    __builtin_trap();
}

int sys_tcb_set(void *pointer) {
    return __sq_syscall2(SQ_SYS_prctl, SQ_PR_ARCH_X86_64_SET_FS, (__sq_u64)pointer);
}

int sys_futex_wait(int *pointer, int expected) {
    return __sq_syscall3(SQ_SYS_futex, SQ_FUTEX_WAIT, (__sq_u64)pointer, expected);
}

int sys_futex_wake(int *pointer) {
    return __sq_syscall3(SQ_SYS_futex, SQ_FUTEX_WAKE, (__sq_u64)pointer, 0);
}

int sys_close(int fd) {
    return __sq_syscall1(SQ_SYS_close, fd);
}

int sys_open(const char *path, int flags, int *fd) {
    /* TODO: convert flags */
    int ret = __sq_syscall3(SQ_SYS_open, (__sq_u64)path, flags, 0);

    if (ret < 0) {
        return ret;
    }

    *fd = ret;
    return 0;
}

int sys_read(int fd, void *buffer, size_t size, ssize_t *bytes_read) {
    ssize_t ret = __sq_syscall3(SQ_SYS_read, fd, (__sq_u64)buffer, size);

    if (ret < 0) {
        return ret;
    }

    *bytes_read = ret;
    return 0;
}

int sys_write(int fd, const void *buffer, size_t size, ssize_t *bytes_written) {
    ssize_t ret = __sq_syscall3(SQ_SYS_write, fd, (__sq_u64)buffer, size);

    if (ret < 0) {
        return ret;
    }

    *bytes_written = ret;
    return 0;
}

int sys_seek(int fd, off_t offset, int whence, off_t *new_offset) {
    switch (whence) {
    case SEEK_SET:
        whence = SQ_SEEK_SET;
        break;

    case SEEK_CUR:
        whence = SQ_SEEK_RELATIVE;
        break;

    case SEEK_END:
        __ensure(!"SEEK_END not implemented yet");
        break;

    default:
        __ensure(!"Unknown seek-whence value");
        break;
    }

    ssize_t ret = __sq_syscall3(SQ_SYS_seek, fd, (__sq_u64)offset, whence);

    if (ret < 0) {
        return ret;
    }

    *new_offset = ret;
    return 0;
}

int sys_vm_map(void *hint, size_t size, int prot, int flags, int fd,
               off_t offset, void **window
) {
    /* our SQ_PROT_* are the same as defined in abis/mlibc/vm-flags.h,
     * so no need to convert them
     */

    /* this flag is not supported yet */
    __ensure(!(flags & MAP_SHARED));

    int sq_flags = 0;
    if (flags & MAP_FIXED) {
        sq_flags |= SQ_MMAP_FIXED;
    }

    if (flags & MAP_ANONYMOUS) {
        sq_flags |= SQ_MMAP_ANONYMOUS;
    }

    if (flags & MAP_PRIVATE) {
        /* for now is implicit */
    }

    ssize_t ret = __sq_syscall6(SQ_SYS_mmap, fd, (__sq_u64)hint, offset, size, prot, sq_flags);

    if (ret < 0) {
        return ret;
    }

    *window = reinterpret_cast<void*>(ret);
    return 0;
}

int sys_vm_unmap(void *pointer, size_t size) {
    return __sq_syscall6(SQ_SYS_mmap, SQ_MMAP_FD_NONE, (__sq_u64)pointer,
                         0, size, 0, SQ_MMAP_UNMAP | SQ_MMAP_FIXED);
}

int sys_anon_allocate(size_t size, void **pointer) {
    ssize_t ret = __sq_syscall6(SQ_SYS_mmap, SQ_MMAP_FD_NONE, 0, 0,
                                (__sq_u32)size, SQ_PROT_READ | SQ_PROT_WRITE,
                                SQ_MMAP_ANONYMOUS | SQ_MMAP_ADJUST_HEAP);

    if (ret < 0) {
        return ret;
    }

    *pointer = reinterpret_cast<void*>(ret);
    return 0;
}

int sys_anon_free(void *pointer, size_t size) {
    return sys_vm_unmap(pointer, size);
}

#ifndef MLIBC_BUILDING_RTDL

int sys_stat(fsfd_target fsfdt, int fd, const char *path, int flags, struct stat *statbuf) {
    int sq_flags = 0;

    switch (fsfdt) {
    case fsfd_target::path: /* operate on `path` relative to cwd */
        fd = SQ_STAT_FD_RELCWD;
        break;

    case fsfd_target::fd: /* operate on file referred by `fd` solely */
        if (fd == AT_FDCWD) {
            fd = SQ_STAT_FD_RELCWD;
        }
        sq_flags |= SQ_STAT_NOPATH;
        break;

    case fsfd_target::fd_path: /* operate on `path` relative to fd */
        /* nothing to do */
        break;

    default:
        __ensure(!"Invalid fsfdt value!");
    }

    if (flags & AT_EMPTY_PATH) {
        sq_flags |= SQ_STAT_NOPATH;
    }

    if (flags & AT_SYMLINK_NOFOLLOW) {
        sq_flags |= SQ_STAT_DONTFOLLOW;
    }

    __sq_sys_stat sq_statbuf{};
    int ret = __sq_syscall4(SQ_SYS_stat, fd, (__sq_u64)path, flags, (__sq_u64)&sq_statbuf);

    if (ret != 0) {
        return -1; /* TODO: translate errno */
    }

    /* translate our struct __sq_sys_stat to the proper struct stat */
    statbuf->st_dev     = sq_statbuf.device;
    statbuf->st_ino     = sq_statbuf.inode;
    statbuf->st_mode    = sq_statbuf.mode;   /* no need to translate, defined exactly the same */
    statbuf->st_nlink   = sq_statbuf.nr_hardlinks;
    statbuf->st_uid     = sq_statbuf.uid;
    statbuf->st_gid     = sq_statbuf.gid;
    statbuf->st_rdev    = sq_statbuf.rdevice;
    statbuf->st_size    = sq_statbuf.size;
    statbuf->st_blksize = sq_statbuf.blksize;
    statbuf->st_blocks  = sq_statbuf.nr_blocks;

    statbuf->st_atim.tv_sec  = sq_statbuf.access_time.secs;
    statbuf->st_atim.tv_nsec = sq_statbuf.access_time.nanosecs;

    statbuf->st_mtim.tv_sec  = sq_statbuf.modification_time.secs;
    statbuf->st_mtim.tv_nsec = sq_statbuf.modification_time.nanosecs;

    statbuf->st_ctim.tv_sec  = sq_statbuf.creation_time.secs;
    statbuf->st_ctim.tv_nsec = sq_statbuf.creation_time.nanosecs;

    return 0;
}

int sys_fork(pid_t *child) {
    int ret = __sq_syscall1(SQ_SYS_clone, 0);

    if (ret < 0) {
        return ret;
    }

    *child = ret;
    return 0;
}

int sys_execve(const char *path, char *const argv[], char *const envp[]) {
    return __sq_syscall3(SQ_SYS_execve, (__sq_u64)path, (__sq_u64)argv, (__sq_u64)envp);
}

void sys_exit(int code) {
    __sq_syscall1(SQ_SYS_exit, code);
    __builtin_trap();
}

pid_t sys_getpid() {
    return __sq_syscall0(SQ_SYS_getpid);
}

pid_t sys_getppid() {
    return 0;
}

int sys_clock_get(int clock, time_t *secs, long *nanos) {
    *secs = 0;
    *nanos = 0;

    return 0;
}

int sys_isatty(int fd) {
    // TODO: not supported yet
    return 0;
}

int sys_ttyname(int fd, char *buf, size_t size) {
    // TODO: not supported yet
    strncpy(buf, "/dev/console", size);

    return 0;
}

uid_t sys_getuid()
{
    // TODO: not supported yet
    return 0;
}

gid_t sys_getgid()
{
    // TODO: not supported yet
    return 0;
}

uid_t sys_geteuid()
{
    // TODO: not supported yet
    return sys_getuid();
}

gid_t sys_getegid()
{
    // TODO: not supported yet
    return sys_getgid();
}

#endif // !MLIBC_BUILDING_RTDL

} // namespace mlibc
