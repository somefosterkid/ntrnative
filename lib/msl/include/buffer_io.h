// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/buffer_io.h, with Black's
// additions
#ifndef _C_BUFFER_IO_H
#define _C_BUFFER_IO_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    __align_buffer,
    __dont_align_buffer
};

void __convert_from_newlines(unsigned char *p, size_t *n);
void __convert_to_newlines(unsigned char *p, size_t *n);
int __load_buffer(FILE *file, size_t *bytes_loaded, int alignment);
int __flush_buffer(FILE *file, size_t *length);
void __prep_buffer(FILE *file);

#ifdef __cplusplus
}
#endif

#endif
