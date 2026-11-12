// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/buffer_io.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). __convert_to_newlines and __load_buffer, which zeldaret-st
// lacks, are MSL's.
#include <buffer_io.h>

void __convert_from_newlines(unsigned char *p, size_t *n) {}

void __convert_to_newlines(unsigned char *p, size_t *n) {}

void __prep_buffer(FILE *file) {
    file->buffer_ptr = file->buffer;
    file->buffer_len = file->buffer_size;
    file->buffer_len = file->buffer_len - (file->position & file->buffer_alignment);
    file->buffer_pos = file->position;
    return;
}

int __load_buffer(FILE *file, size_t *bytes_loaded, int alignment) {
    int ioresult;

    __prep_buffer(file);

    if (alignment == __dont_align_buffer) {
        file->buffer_len = file->buffer_size;
    }

    ioresult = (*file->read_proc)(file->handle, file->buffer, &file->buffer_len, file->ref_con);

    if (ioresult == __io_EOF) {
        file->buffer_len = 0;
    }

    if (bytes_loaded != NULL) {
        *bytes_loaded = file->buffer_len;
    }

    if (ioresult) {
        return ioresult;
    }

    file->position += file->buffer_len;

    if (!file->mode.binary_io) {
        __convert_to_newlines(file->buffer, &file->buffer_len);
    }

    return 0;
}

int __flush_buffer(FILE *file, size_t *length) {
    size_t bufferLen;
    int writeCode;

    bufferLen = file->buffer_ptr - file->buffer;
    if (bufferLen) {
        file->buffer_len = bufferLen;

        if (file->mode.binary_io == 0) {
            __convert_from_newlines(file->buffer, &file->buffer_len);
        }

        writeCode = file->write_proc(file->handle, file->buffer, &file->buffer_len, file->ref_con);

        if (length) {
            *length = file->buffer_len;
        }

        if (writeCode) {
            return writeCode;
        }

        file->position += file->buffer_len;
    }

    __prep_buffer(file);
    return 0;
}
