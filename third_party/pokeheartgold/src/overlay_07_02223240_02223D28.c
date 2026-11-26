#include "global.h"
#include "overlay_07_swagger_sprite_context.h"
struct SwaggerSpriteContext;

s32 ov07_02223CAC(void *, s16, s16);

BOOL ov07_02223CE4(SwaggerSpriteContext *ctx);

s32 ov07_02223D28(struct SwaggerSpriteContext *arg0);

s32 ov07_02223D28(SwaggerSpriteContext *arg0) {
    s32 v1;
    s32 v2;
    u32 v0;

    v0 = *((u32 *)&arg0->popState);
    v2 = 0;
    switch (v0) {
    case 0:
        ov07_02223CAC(arg0, (s16)(arg0->defenderX + (0x18 * *((s32 *)&arg0->dir))), (s16)(arg0->defenderY - 0x10));
        *((u32 *)&arg0->popState) = (u32)(*((u32 *)&arg0->popState) + 1);
        break;
    case 1:
        if (ov07_02223CE4(arg0) != 0) {
            *((u32 *)&arg0->popState) = (u32)(*((u32 *)&arg0->popState) + 1);
        }
        break;
    case 2:
        v1 = *((s32 *)&arg0->delay) - 1;
        *((s32 *)&arg0->delay) = v1;
        if (v1 < 0) {
            *((u32 *)&arg0->popState) = (u32)(*((u32 *)&arg0->popState) + 1);
            ov07_02223CAC(arg0, (s16)(arg0->defenderX - (0x18 * *((s32 *)&arg0->dir))), (s16)(arg0->defenderY - 0x18));
        }
        break;
    case 3:
        if (ov07_02223CE4(arg0) != 0) {
            *((u32 *)&arg0->popState) = (u32)(*((u32 *)&arg0->popState) + 1);
        }
        break;
    case 4:
        v2 = 1;
        break;
    }
    return v2;
}
