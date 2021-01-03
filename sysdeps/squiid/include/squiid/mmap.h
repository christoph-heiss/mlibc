#ifndef __SQUIID_USER_MMAP_H__
#define __SQUIID_USER_MMAP_H__

/* SQ_SYS_mmap fd */
#define SQ_MMAP_FD_NONE -1

/* SQ_SYS_mmap protection */
#define SQ_PROT_NONE  0
#define SQ_PROT_READ  0x1
#define SQ_PROT_WRITE 0x2
#define SQ_PROT_EXEC  0x4

/* SQ_SYS_mmap flags */
#define SQ_MMAP_ANONYMOUS   0x1
#define SQ_MMAP_ADJUST_HEAP 0x2
#define SQ_MMAP_UNMAP       0x4
#define SQ_MMAP_FIXED       0x8

#endif /* __SQUIID_USER_MMAP_H__ */
