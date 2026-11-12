// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/stdarg.h
#ifndef _C_STDARG_H
#define _C_STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char *va_list;

#define __fourbytealign(n) ((((unsigned int) (n)) + 3U) & ~3U)
#define __va_start(parm) ((va_list) ((char *) ((unsigned int) (&parm) & ~3U) + __fourbytealign(sizeof(parm))))

#define va_start(ap, parm) ((ap) = __va_start(parm))
#define va_arg(ap, type) (*(type *) ((ap += __fourbytealign(sizeof(type))) - __fourbytealign(sizeof(type))))
#define va_end(ap) ((void) 0)
#define va_copy(dest, src) dest = src

#ifdef __cplusplus
}
#endif

#endif
