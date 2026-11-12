// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/secure_error.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). Its secure_error_func_02033e50 has Black's placeholder name,
// with a prototype.
#include <secure_error.h>
#include <stddef.h>

msl_constraint_handler __msl_constraint_handler;

void func_02094778(void);

void __msl_runtime_constraint_violation_s(const char *msg, void *ptr, int error) {
    if (__msl_constraint_handler != NULL) {
        __msl_constraint_handler(msg, ptr, error);
        return;
    }

    func_02094778();
}

void func_02094778(void) {}
