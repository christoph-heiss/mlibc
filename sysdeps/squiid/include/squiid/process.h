#ifndef __SQUIID_USER_PROCESS_H__
#define __SQUIID_USER_PROCESS_H__

/* SQ_SYS_clone flags */
#define SQ_CLONE_THREAD 0x1

/* SQ_SYS_prctl operations */
#define __SQ_PR_ARCH_BASE 0x400

#define SQ_PR_ARCH_X86_64_SET_FS __SQ_PR_ARCH_BASE
#define SQ_PR_ARCH_X86_64_SET_GS (__SQ_PR_ARCH_BASE+1)

/* SYS_futex operations */
#define SQ_FUTEX_WAIT   1
#define SQ_FUTEX_WAKE   2

#endif /* __SQUIID_USER_PROCESS_H__ */
