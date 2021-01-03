#ifndef __SQUIID_USER_AUXV_H__
#define __SQUIID_USER_AUXV_H__

#define SQ_AT_NULL      0 /* end of auxv */
#define SQ_AT_PHDR      1 /* program headers */
#define SQ_AT_PHENT     2 /* size of program header entry */
#define SQ_AT_PHNUM     3 /* number of program headers */
#define SQ_AT_ENTRY     4 /* entry pointer */
#define SQ_AT_EXECFN    5 /* pointer to exec pathname */
#define SQ_AT_RANDOM    6 /* pointer to random 16-byte value */

#endif /* __SQUIID_USER_AUXV_H__ */
