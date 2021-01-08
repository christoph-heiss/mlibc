#ifndef __SQUIID_USER_SYSCALL_WRAPPERS_H__
#define __SQUIID_USER_SYSCALL_WRAPPERS_H__

#include "syscalls.h"

#define __sq_always_inline __attribute__((always_inline)) inline

static __sq_always_inline __sq_u64 __sq_syscall0(__sq_u64 nr)
{
        __sq_u64 result;

        asm volatile (
                "syscall"
                : "=a"(result)
                : "a"(nr)
                : "cc", "memory", "rcx", "r11", "rdi", "rsi", "rdx", "r10", "r8", "r9"
        );

        return result;
}

static __sq_always_inline __sq_u64 __sq_syscall1(__sq_u64 nr, __sq_u64 arg1)
{
        __sq_u64 result;

        asm volatile (
                "syscall"
                : "=a"(result)
                : "a"(nr), "D"(arg1)
                : "cc", "memory", "rcx", "r11", "rsi", "rdx", "r10", "r8", "r9"
        );

        return result;
}

static __sq_always_inline __sq_u64 __sq_syscall2(__sq_u64 nr, __sq_u64 arg1, __sq_u64 arg2)
{
        __sq_u64 result;

        asm volatile (
                "syscall"
                : "=a"(result)
                : "a"(nr), "D"(arg1), "S"(arg2)
                : "cc", "memory", "rcx", "r11", "rdx", "r10", "r8", "r9"
        );

        return result;
}

static __sq_always_inline __sq_u64 __sq_syscall3(
        __sq_u64 nr, __sq_u64 arg1, __sq_u64 arg2, __sq_u64 arg3
) {
        __sq_u64 result;

        asm volatile (
                "syscall"
                : "=a"(result)
                : "a"(nr), "D"(arg1), "S"(arg2), "d"(arg3)
                : "cc", "memory", "rcx", "r11", "r10", "r8", "r9"
        );

        return result;
}

static __sq_always_inline __sq_u64 __sq_syscall4(
        __sq_u64 nr,
        __sq_u64 arg1, __sq_u64 arg2, __sq_u64 arg3, __sq_u64 arg4
) {
        __sq_u64 result;

        register __sq_u64 r10 asm("r10") = arg4;

        asm volatile (
                "syscall"
                : "=a"(result)
                : "a"(nr), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10)
                : "cc", "memory", "rcx", "r11", "r8", "r9"
        );

        return result;
}

static __sq_always_inline __sq_u64 __sq_syscall6(
        __sq_u64 nr,
        __sq_u64 arg1, __sq_u64 arg2, __sq_u64 arg3,
        __sq_u64 arg4, __sq_u64 arg5, __sq_u64 arg6
) {
        __sq_u64 result;

        register __sq_u64 r10 asm("r10") = arg4;
        register __sq_u64 r8 asm("r8") = arg5;
        register __sq_u64 r9 asm("r9") = arg6;

        asm volatile (
                "syscall"
                : "=a"(result)
                : "a"(nr), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10), "r"(r8), "r"(r9)
                : "cc", "memory", "rcx", "r11"
        );

        return result;
}

#endif /* __SQUIID_USER_SYSCALL_WRAPPERS_H__ */
