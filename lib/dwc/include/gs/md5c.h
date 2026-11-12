#ifndef GS_MD5C_H
#define GS_MD5C_H

#include "types.h"

typedef struct {
    u32 state[4];
    u32 count[2];
    unsigned char buffer[64];
} MD5_CTX;

void func_ov009_02184938(u32 *param_1);
void func_ov009_0218497c(s32 *param_1, void *param_2, u32 param_3);
void func_ov009_02184a34(s32 param_1, s32 *param_2);


void MD5Digest(const unsigned char *input, unsigned int len, char *output);

#endif
