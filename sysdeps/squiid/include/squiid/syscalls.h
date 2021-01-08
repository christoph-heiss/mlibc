#ifndef __SQUIID_USER_SYSCALLS_H__
#define __SQUIID_USER_SYSCALLS_H__

#define SQ_SYS_open      0
#define SQ_SYS_close     1
#define SQ_SYS_read      2
#define SQ_SYS_write     3
#define SQ_SYS_seek      4
#define SQ_SYS_exit      5
#define SQ_SYS_execve    6
#define SQ_SYS_clone     7
#define SQ_SYS_getid     8
#define SQ_SYS_mmap      9
#define SQ_SYS_prctl     10
#define SQ_SYS_stat      11
#define SQ_SYS_futex     12

#define __SQ_SYS_LAST   SQ_SYS_futex
#define __SQ_SYS_NUM    (__SQ_SYS_LAST+1)

#ifndef __ASSEMBLER__

typedef signed char        __sq_s8;
typedef unsigned char      __sq_u8;
typedef signed short       __sq_s16;
typedef unsigned short     __sq_u16;
typedef signed int         __sq_s32;
typedef unsigned int       __sq_u32;
typedef signed long long   __sq_s64;
typedef unsigned long long __sq_u64;

#endif /* __ASSEMBLER__ */

#endif /* __SQUIID_USER_SYSCALLS_H__ */
