// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/console_io.h, with the count a
// size_t, as runtime/runtime.c has it
#ifndef _C_CONSOLE_IO_H
#define _C_CONSOLE_IO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int __read_console(unsigned int handle, unsigned char *buffer, size_t *count, void *ref);
extern int __write_console(unsigned int handle, unsigned char *buffer, size_t *count, void *ref);
extern int __close_console();

#ifdef __cplusplus
}
#endif

#endif
