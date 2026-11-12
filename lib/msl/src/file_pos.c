// MSL's file_pos.c. The file's name is pokediamond's object's (MSL_Common_file_pos.o), its functions' pokebw2's.
#include <buffer_io.h>
#include <critical_regions.h>
#include <errno.h>
#include <stdio.h>

int _ftell(FILE *file) {
    int charsInUndoBuffer = 0;
    unsigned int position;
    unsigned char tmp_kind = file->mode.file_kind;

    if (!(tmp_kind == __disk_file || tmp_kind == __console_file) || file->state.error) {
        errno = EFPOS;
        return -1;
    }

    if (file->state.io_state == __neutral) {
        return file->position;
    }

    position = file->buffer_pos + (file->buffer_ptr - file->buffer);

    if (file->state.io_state >= __rereading) {
        charsInUndoBuffer = file->state.io_state - __rereading + 1;
        position -= charsInUndoBuffer;
    }

    return position;
}

int ftell(FILE *file) {
    int retval;
    int crtrgn;

    if (file == stdin) {
        crtrgn = stdin_access;
    } else if (file == stdout) {
        crtrgn = stdout_access;
    } else if (file == stderr) {
        crtrgn = stderr_access;
    } else {
        crtrgn = files_access;
    }

    __begin_critical_region(crtrgn);
    retval = _ftell(file);
    __end_critical_region(crtrgn);
    return retval;
}

int _fseek(FILE *file, unsigned int offset, int file_mode) {
    __pos_proc pos_proc;
    unsigned char tmp_kind = file->mode.file_kind;

    if (!(tmp_kind == __disk_file) || file->state.error) {
        errno = EFPOS;
        return -1;
    }

    if (file->state.io_state == __writing) {
        if (__flush_buffer(file, NULL) != __no_io_error) {
            set_error(file);
            errno = EFPOS;
            return -1;
        }
    }

    if (file_mode == SEEK_CUR) {
        file_mode = SEEK_SET;
        offset += _ftell(file);
    }

    if ((file_mode != SEEK_END) && (file->mode.io_mode != __read_write) &&
        ((file->state.io_state == __reading) || (file->state.io_state == __rereading))) {
        if ((offset >= file->position) || (offset < file->buffer_pos)) {
            file->state.io_state = __neutral;
        } else {
            file->buffer_ptr     = file->buffer + (offset - file->buffer_pos);
            file->buffer_len     = file->position - offset;
            file->state.io_state = __reading;
        }
    } else {
        file->state.io_state = __neutral;
    }

    if (file->state.io_state == __neutral) {
        if ((pos_proc = file->position_proc) != NULL && (*pos_proc)(file->handle, &offset, file_mode, file->ref_con)) {
            set_error(file);
            errno = EFPOS;
            return -1;
        } else {
            file->state.eof  = 0;
            file->position   = offset;
            file->buffer_len = 0;
        }
    }

    return 0;
}

int fseek(FILE *file, unsigned int offset, int file_mode) {
    int retval;
    int crtrgn;

    if (file == stdin) {
        crtrgn = stdin_access;
    } else if (file == stdout) {
        crtrgn = stdout_access;
    } else if (file == stderr) {
        crtrgn = stderr_access;
    } else {
        crtrgn = files_access;
    }

    __begin_critical_region(crtrgn);
    retval = _fseek(file, offset, file_mode);
    __end_critical_region(crtrgn);
    return retval;
}

void rewind(FILE *file) {
    file->state.error = 0;
    fseek(file, 0, SEEK_SET);
    file->state.error = 0;
}
