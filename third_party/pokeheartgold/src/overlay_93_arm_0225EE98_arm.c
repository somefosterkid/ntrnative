#include "global.h"

#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"

void ov93_0225EE98(void);

void ov93_0225EE98(void) {
    u8 shininess[128];
    u8 *p = shininess;
    u8 i;

    for (i = 0; i < 0x7F; i++) {
        s32 n = i * 2 + 1;
        p[i] = (u8)(((s64)n * n * n * n) >> 24);
    }

    p[0x7F] = 0xFF;
    NNS_G3dGeBufferOP_N(G3OP_SHININESS, (u32 *)shininess, G3OP_SHININESS_NPARAMS);
}
