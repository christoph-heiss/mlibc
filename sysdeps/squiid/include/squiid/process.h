#ifndef __SQUIID_USER_PROCESS_H__
#define __SQUIID_USER_PROCESS_H__

/* SQ_SYS_clone flags */
#define SQ_CLONE_THREAD 0x1

/* SQ_SYS_getid flags */
#define SQ_GETID_PID  1
#define SQ_GETID_PPID 2
#define SQ_GETID_UID  3
#define SQ_GETID_GID  4

/* SQ_SYS_prctl operations */
#define SQ_PRCTL_SET_SIGNALACTION             1
#define SQ_PRCTL_SET_SIGNAL_RETURN_TRAMPOLINE 2
#define SQ_PRCTL_RETURN_FROM_SIGNAL           3

#define __SQ_PRCTL_ARCH_BASE        0x1000
#define SQ_PRCTL_ARCH_X86_64_SET_FS (__SQ_PRCTL_ARCH_BASE  )
#define SQ_PRCTL_ARCH_X86_64_SET_GS (__SQ_PRCTL_ARCH_BASE+1)

/* __sq_sys_signalaction flags */
#define SQ_PRCTL_SIGNALACTION_WITH_INFO 0x1

/* SYS_futex operations */
#define SQ_FUTEX_WAIT   1
#define SQ_FUTEX_WAKE   2

#ifndef __ASSEMBLER__

#include "syscalls.h"

struct __sq_sys_signalaction {
        union {
                __sq_addr handler;
                __sq_addr info_handler;
        };

        __sq_u64  signal_mask;
        __sq_u64  flags;
};

struct __sq_sys_set_signalaction_info {
        __sq_u32  signal_nr;
        __sq_u32  __pad0;

        __sq_addr action;
        __sq_addr old_action;
};

#endif

#endif /* __SQUIID_USER_PROCESS_H__ */
