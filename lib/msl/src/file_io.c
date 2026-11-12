// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/file_io.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py), with the headers of the functions it calls. fclose and
// __msl_strnicmp, which zeldaret-st lacks, are MSL's.
#include <ansi_files.h>
#include <buffer_io.h>
#include <ctype.h>
#include <stdio.h>

int fclose(FILE *file) {
    int flush_result, close_result;

    if (file == NULL) {
        return -1;
    }
    if (file->mode.file_kind == __closed_file) {
        return 0;
    }

    flush_result = fflush(file);
    close_result = (*file->close_proc)(file->handle);

    file->mode.file_kind = __closed_file;
    file->handle         = 0;

    if (file->state.free_buffer) {
        return -1;
    }

    return (flush_result || close_result) ? -1 : 0;
}

int fflush(FILE *file) {
    if (file == NULL) {
        return __flush_all();
    }

    if (file->state.error != 0 || file->mode.file_kind == __closed_file) {
        return -1;
    }

    if (file->mode.io_mode == 1) {
        return 0;
    }

    if (file->state.io_state >= 3) {
        file->state.io_state = 2;
    }

    if (file->state.io_state == 2) {
        file->buffer_len = 0;
    }

    if (file->state.io_state != 1) {
        file->state.io_state = 0;
        return 0;
    }

    if (__flush_buffer(file, 0) != 0) {
        file->state.error = 1;
        file->buffer_len  = 0;
        return -1;
    }

    file->state.io_state = 0;
    file->position       = 0;
    file->buffer_len     = 0;
    return 0;
}

int __msl_strnicmp(const char *s1, const char *s2, int n) {
    int i;
    unsigned char c1, c2;

    for (i = 0; i < n; i++) {
        c1 = tolower((unsigned char) *s1++);
        c2 = tolower((unsigned char) *s2++);
        if (c1 < c2) {
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }
        if (c1 == 0) {
            return 0;
        }
    }
    return 0;
}
