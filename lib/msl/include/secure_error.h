// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/secure_error.h
#ifndef _C_SECURE_ERROR_H
#define _C_SECURE_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*msl_constraint_handler)(const char *msg, void *ptr, int error);

void __msl_runtime_constraint_violation_s(const char *msg, void *ptr, int error);

#ifdef __cplusplus
}
#endif

#endif
