// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/file_io.h, as it is (tools/port_lib.py)
#ifndef _C_FILE_IO_H
#define _C_FILE_IO_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int fclose(FILE *file);
int fflush(FILE *file);

#ifdef __cplusplus
}
#endif

#endif
