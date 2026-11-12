// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/ansi_files.h, with Black's
// additions
#ifndef _C_ANSI_FILES_H
#define _C_ANSI_FILES_H

#include <file_struc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define console_buff_mode _IOLBF
#define console_buff_size 256

typedef unsigned char console_buff[console_buff_size];

int __flush_line_buffered_output_files(void);
int __flush_all(void);

#ifdef __cplusplus
}
#endif

#endif
