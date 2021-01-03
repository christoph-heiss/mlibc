#ifndef __SQUIID_USER_STAT_H__
#define __SQUIID_USER_STAT_H__

#include "syscalls.h"

/* open() flags */
#define SQ_OPEN_WRITEONLY 0x02
#define SQ_OPEN_READWRITE 0x04
#define SQ_OPEN_APPEND    0x08
#define SQ_OPEN_CREATE    0x10
#define SQ_OPEN_EXCLUSIVE 0x20
#define SQ_OPEN_TRUNCATE  0x40
#define SQ_OPEN_NOATIME   0x80

/* seek() flags */
#define SQ_SEEK_SET      1
#define SQ_SEEK_RELATIVE 2

/* sys_dirent type */
#define SQ_DT_REG 0
#define SQ_DT_DIR 1
#define SQ_DT_CHR 2
#define SQ_DT_BLK 3

#define _SQ_PATH_MAX 1024

#ifndef __ASSEMBLER__

struct sq_sys_dirent {
        __sq_u32 type;

        __sq_u32 name_len;
        __sq_u8 name[0];
};

#endif /* __ASSEMBLER__ */

#endif /* __SQUIID_USER_STAT_H__ */
