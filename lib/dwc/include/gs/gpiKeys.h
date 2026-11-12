#ifndef GS_GPIKEYS_H
#define GS_GPIKEYS_H

#include "types.h"
#include "gs/darray.h"
#include "gs/gp.h"

// gpiKeys.c: a profile's key/value store (not written yet; the three below as their callers use them)
void func_ov009_0218d7cc(s32 *param_1);
// Black's: the keys array's creation (GP_NO_ERROR, or the memory error it sets) and its freeing
GPResult func_ov009_0218d7f4(GPConnection *connection);
void func_ov009_0218d840(GPConnection *connection);
BOOL func_ov009_0218d9a8(GPConnection *connection, DArray keys, const char *key, int *found);
GPResult func_ov009_0218d878(GPConnection *connection, DArray keys, const char *keyName, const char *keyValue);
GPResult func_ov009_0218d900(GPConnection *connection, DArray keys, const char *keyName, const char *newValue);

#endif
