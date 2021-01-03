.section .text

.global _start
_start:
    movq $main, %rdi
    callq __mlibc_entry
