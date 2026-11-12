// MSL's direct_io.c: only fread and __fread are in Black. The file's name is pokediamond's object's
// (MSL_Common_direct_io.o), its functions' pokebw2's.
#include <ansi_files.h>
#include <buffer_io.h>
#include <critical_regions.h>
#include <mem.h>
#include <stdio.h>

size_t fread(void *ptr, size_t memb_size, size_t num_memb, FILE *file) {
    size_t retval;
    int crtrgn;

    if (file == stdin) {
        crtrgn = stdin_access;
    } else {
        crtrgn = files_access;
    }

    __begin_critical_region(crtrgn);
    retval = __fread(ptr, memb_size, num_memb, file);
    __end_critical_region(crtrgn);
    return retval;
}

size_t __fread(void *ptr, size_t memb_size, size_t num_memb, FILE *file) {
    unsigned char *read_ptr;
    size_t num_bytes, bytes_to_go, bytes_read;
    int ioresult, always_buffer;

    if (fwide(file, 0) == 0) {
        fwide(file, -1);
    }

    bytes_to_go = memb_size * num_memb;

    if (!bytes_to_go || file->state.error || file->mode.file_kind == __closed_file) {
        return 0;
    }

    always_buffer = !file->mode.binary_io || file->mode.buffer_mode == _IOFBF;

    if (file->state.io_state == __neutral) {
        if (file->mode.io_mode & __read) {
            file->state.io_state = __reading;
            file->buffer_len     = 0;
        }
    }

    if (file->state.io_state < __reading) {
        set_error(file);
        return 0;
    }

    if (file->mode.buffer_mode & _IOLBF) {
        if (__flush_line_buffered_output_files()) {
            set_error(file);
            return 0;
        }
    }

    read_ptr   = (unsigned char *) ptr;
    bytes_read = 0;

    if (bytes_to_go && file->state.io_state >= __rereading) {
        do {
            if (fwide(file, 0) == 1) {
                *(wchar_t *) read_ptr = file->ungetwc_buffer[file->state.io_state - __rereading];
                read_ptr += sizeof(wchar_t);
                bytes_read += sizeof(wchar_t);
                bytes_to_go -= sizeof(wchar_t);
            } else {
                *read_ptr++ = file->ungetc_buffer[file->state.io_state - __rereading];
                bytes_read++;
                bytes_to_go--;
            }
            file->state.io_state--;
        } while (bytes_to_go && file->state.io_state >= __rereading);

        if (file->state.io_state == __reading) {
            file->buffer_len = file->saved_buffer_len;
        }
    }

    if (bytes_to_go && (file->buffer_len || always_buffer)) {
        do {
            if (!file->buffer_len) {
                ioresult = __load_buffer(file, NULL, __align_buffer);
                if (ioresult) {
                    if (ioresult == __io_error) {
                        set_error(file);
                    } else {
                        file->state.io_state = __neutral;
                        set_eof(file);
                    }
                    bytes_to_go = 0;
                    break;
                }
            }

            num_bytes = file->buffer_len;
            if (num_bytes > bytes_to_go) {
                num_bytes = bytes_to_go;
            }

            memcpy(read_ptr, file->buffer_ptr, num_bytes);

            read_ptr += num_bytes;
            bytes_read += num_bytes;
            bytes_to_go -= num_bytes;

            file->buffer_ptr += num_bytes;
            file->buffer_len -= num_bytes;
        } while (bytes_to_go && always_buffer);
    }

    if (bytes_to_go && !always_buffer) {
        unsigned char *save_buffer = file->buffer;
        size_t save_size           = file->buffer_size;

        file->buffer      = read_ptr;
        file->buffer_size = bytes_to_go;

        ioresult = __load_buffer(file, &num_bytes, __dont_align_buffer);
        if (ioresult) {
            if (ioresult == __io_error) {
                set_error(file);
            } else {
                file->state.io_state = __neutral;
                set_eof(file);
            }
        }

        bytes_read += num_bytes;

        file->buffer      = save_buffer;
        file->buffer_size = save_size;

        __prep_buffer(file);

        file->buffer_len = 0;
    }

    return bytes_read / memb_size;
}
