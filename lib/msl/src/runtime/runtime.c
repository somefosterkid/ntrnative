// Metrowerks' C runtime for NITRO as Black links it after the floating-point runtime: the destructor chain of
// __register_atexit, the console of stdin and stdout (the debugger's, empty in a final ROM) and the static
// initializers. No name is known for the file, and the unnamed functions' roles are a guess.
#include <MWException.h>
#include <console_io.h>
#include <stddef.h>

static int __atexit_curr_func;
static DestructorChain __atexit_funcs[64];

void func_0209d724(void) {
}

int __register_atexit(void (*func)(void)) {
    if (__atexit_curr_func == 64) {
        return -1;
    }
    __register_global_object(NULL, func, &__atexit_funcs[__atexit_curr_func++]);
    return 0;
}

void __TRK_write_console(const unsigned char *str) {
}

void WriteUARTN(const unsigned char *str) {
}

int ReadUARTN(void) {
    return 0;
}

int __read_console(unsigned int handle, unsigned char *buffer, size_t *count, void *ref) {
    size_t i;
    size_t limit = *count;

    for (i = 0; i < limit; i++) {
        buffer[i] = ReadUARTN();
        if (buffer[i] == '\r' || buffer[i] == '\n') {
            *count = i + 1;
            break;
        }
    }
    return 0;
}

int __write_console(unsigned int handle, unsigned char *buffer, size_t *count, void *ref) {
    unsigned char last;

    if (*count > 1) {
        last = buffer[*count - 1];
        buffer[*count - 1] = 0;
        WriteUARTN(buffer);
        __TRK_write_console(&last);
    } else {
        __TRK_write_console(buffer);
    }
    return 0;
}

int __close_console(void) {
    return 0;
}

// The static initializers' table, which the link ends with a null (ARM9_CTOR_START in the linker script)
extern void (*ARM9_CTOR_START[])(void);

void __call_static_initializers(void) {
    void (**ctor)(void);

    for (ctor = ARM9_CTOR_START; ctor != NULL && *ctor != NULL; ctor++) {
        (*ctor)();
    }
}
