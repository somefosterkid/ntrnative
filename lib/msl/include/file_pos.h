// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/file_pos.h
#ifndef _C_FILE_POS_H
#define _C_FILE_POS_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int fseek(FILE *stream, unsigned int offset, int whence);
int _fseek(FILE *stream, unsigned int offset, int whence);
int ftell(FILE *stream);
int _ftell(FILE *stream);

#ifdef __cplusplus
};
#endif

#endif
