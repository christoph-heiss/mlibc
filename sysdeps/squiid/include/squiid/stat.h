#ifndef __SQUIID_USER_STAT_H__
#define __SQUIID_USER_STAT_H__

/* SQ_SYS_open flags */
#define SQ_OPEN_WRITEONLY 0x02
#define SQ_OPEN_READWRITE 0x04
#define SQ_OPEN_APPEND    0x08
#define SQ_OPEN_CREATE    0x10
#define SQ_OPEN_EXCLUSIVE 0x20
#define SQ_OPEN_TRUNCATE  0x40
#define SQ_OPEN_NOATIME   0x80

/* SQ_SYS_seek flags */
#define SQ_SEEK_SET      1
#define SQ_SEEK_RELATIVE 2

/* __sq_sys_dirent type */
#define SQ_DT_REG 1
#define SQ_DT_DIR 2
#define SQ_DT_CHR 3
#define SQ_DT_BLK 4
#define SQ_DT_LNK 5

/* SQ_SYS_stat fd */
#define SQ_STAT_FD_RELCWD -1

/* SQ_SYS_stat flags */
#define SQ_STAT_NOPATH     0x1
#define SQ_STAT_DONTFOLLOW 0x2 /* operate on symlink rather than target */

/* __sq_sys_stat mode
 *
 * Basically the same as POSIX specifies, but here's a quick recap:
 *
 * For the file type we allocate bits 12-15 in ->mode.
 *
 * The permissions are stored in the lower 12 bits of ->mode.
 * 01 = execute, 02 = write, 04 = read, 07 = r/w/x
 * bits 0-2: others
 * bits 3-5: group
 * bits 6-8: user
 * bit  9:   sticky
 * bit 10:   set-group-id
 * bit 11:   set-user-id
 */
#define SQ_STAT_FTYPE_MASK 0xf000
#define __SQ_STAT_FTYPE_SHIFT 12
/* same order as SQ_DT_* */
#define SQ_STAT_FTYPE_REG  1
#define SQ_STAT_FTYPE_DIR  2
#define SQ_STAT_FTYPE_CHR  3
#define SQ_STAT_FTYPE_BLK  4
#define SQ_STAT_FTYPE_LNK  5
#define SQ_STAT_FTYPE_SOCK 6
#define SQ_STAT_FTYPE_FIFO 7

#define _SQ_PATH_MAX 2048

#ifndef __ASSEMBLER__

#include "syscalls.h"

struct __sq_timespec {
        __sq_u64 secs;
        __sq_u64 nanosecs;
};

struct __sq_sys_dirent {
        __sq_u32 type;

        __sq_u32 name_len;
        __sq_u8 name[0];
};

struct __sq_sys_stat {
        __sq_u64 device;
        __sq_u64 inode;
        __sq_u64 nr_hardlinks;

        __sq_u32 mode;
        __sq_u32 uid;
        __sq_u32 gid;
        __sq_u32 __pad0;

        __sq_u64 rdevice;
        __sq_u64 size;
        __sq_u64 blksize;
        __sq_u64 nr_blocks;

        struct __sq_timespec access_time;
        struct __sq_timespec modification_time;
        struct __sq_timespec creation_time;

        /* leave some room for easy expansion */
        __sq_u64 __pad1[6];
};

#endif /* __ASSEMBLER__ */

#endif /* __SQUIID_USER_STAT_H__ */
