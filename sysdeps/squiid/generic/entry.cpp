#include <stdint.h>
#include <stdlib.h>
#include <mlibc/elf/startup.h>

#include <squiid/syscalls.h>
#include <squiid/syscall_wrappers_x86_64.h>
#include <squiid/process.h>

// defined by the POSIX library
void __mlibc_initLocale();

extern "C" uintptr_t *__dlapi_entrystack();

extern char **environ;
static mlibc::exec_stack_data __mlibc_stack_data;

struct LibraryGuard {
    LibraryGuard();
};

static LibraryGuard guard;

static void __mlibc_signal_trampoline() {
    __sq_syscall2(SQ_SYS_prctl, SQ_PRCTL_RETURN_FROM_SIGNAL, 0);
    __builtin_unreachable();
}

LibraryGuard::LibraryGuard() {
    __mlibc_initLocale();

    // Parse the exec() stack.
    mlibc::parse_exec_stack(__dlapi_entrystack(), &__mlibc_stack_data);
    mlibc::set_startup_data(__mlibc_stack_data.argc, __mlibc_stack_data.argv,
                            __mlibc_stack_data.envp);

    __sq_syscall2(SQ_SYS_prctl, SQ_PRCTL_SET_SIGNAL_RETURN_TRAMPOLINE,
                  (__sq_u64)&__mlibc_signal_trampoline);
}

extern "C" void __mlibc_entry(int (*main_fn)(int argc, char *argv[], char *env[])) {
	int result = main_fn(__mlibc_stack_data.argc, __mlibc_stack_data.argv, environ);
	exit(result);
}
