#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "overlay_97_course_sub_screen.h"

// DS form: p/dst/spr walk the stat rows as byte offsets; the typed form below has the same effect but mwcc then
// folds the 0x1DC offset into the base first and allocates r5 instead of r4 (not byte-equal).
#if !defined(SDK_PORT) && !defined(NONMATCHING)
void ov97_0221F5F8(u8 *arg0, u8 *arg1);

void ov97_0221F5F8(u8 *arg0, u8 *arg1) {
    u8 i;
    u8 j;
    u8 *src;
    u8 *dst;
    u8 *spr;
    u8 *p;

    for (i = 0; i < 5; i++) {
        src = arg1 + i * 8;
        if (*(s8 *)(src + 0x28) > 0) {
            p = arg0 + i * 0x18;
            Sprite_SetDrawFlag(*(Sprite **)(p + 0x1DC), 1);
            Sprite_SetAnimCtrlSeq(*(Sprite **)(p + 0x1DC), 4);
        } else if (*(s8 *)(src + 0x28) < 0) {
            p = arg0 + i * 0x18;
            Sprite_SetDrawFlag(*(Sprite **)(p + 0x1DC), 1);
            Sprite_SetAnimCtrlSeq(*(Sprite **)(p + 0x1DC), 5);
        } else {
            Sprite_SetDrawFlag(((Sprite * (*)[6])(arg0 + 0x1DC))[i][0], 0);
        }
        dst = arg0 + i * 0x18;
        for (j = 0; j < 5; j++) {
            switch (src[0x29 + j]) {
            case 0:
                Sprite_SetDrawFlag(*(Sprite **)(dst + j * 4 + 0x1E0), 0);
                break;
            case 1:
                spr = dst + j * 4;
                Sprite_SetDrawFlag(*(Sprite **)(spr + 0x1E0), 1);
                Sprite_SetAnimCtrlSeq(*(Sprite **)(spr + 0x1E0), 3);
                break;
            case 2:
                spr = dst + j * 4;
                Sprite_SetDrawFlag(*(Sprite **)(spr + 0x1E0), 1);
                Sprite_SetAnimCtrlSeq(*(Sprite **)(spr + 0x1E0), 0);
                break;
            case 3:
                spr = dst + j * 4;
                Sprite_SetDrawFlag(*(Sprite **)(spr + 0x1E0), 1);
                Sprite_SetAnimCtrlSeq(*(Sprite **)(spr + 0x1E0), 1);
                break;
            case 4:
                spr = dst + j * 4;
                Sprite_SetDrawFlag(*(Sprite **)(spr + 0x1E0), 1);
                Sprite_SetAnimCtrlSeq(*(Sprite **)(spr + 0x1E0), 2);
                break;
            default:
                GF_AssertFail();
                break;
            }
        }
    }
}
#else
void ov97_0221F5F8(Ov97CourseSubScreenSprites *arg0, u8 *arg1);

void ov97_0221F5F8(Ov97CourseSubScreenSprites *arg0, u8 *arg1) {
    u8 i;
    u8 j;
    u8 *src;
    Ov97StatRowSprites *dst;
    Ov97StatRowSprites *p;

    for (i = 0; i < 5; i++) {
        src = arg1 + i * 8;
        if (*(s8 *)(src + 0x28) > 0) {
            p = &arg0->statRows[i];
            Sprite_SetDrawFlag(p->arrow, 1);
            Sprite_SetAnimCtrlSeq(p->arrow, 4);
        } else if (*(s8 *)(src + 0x28) < 0) {
            p = &arg0->statRows[i];
            Sprite_SetDrawFlag(p->arrow, 1);
            Sprite_SetAnimCtrlSeq(p->arrow, 5);
        } else {
            Sprite_SetDrawFlag(arg0->statRows[i].arrow, 0);
        }
        dst = &arg0->statRows[i];
        for (j = 0; j < 5; j++) {
            switch (src[0x29 + j]) {
            case 0:
                Sprite_SetDrawFlag(dst->marks[j], 0);
                break;
            case 1:
                Sprite_SetDrawFlag(dst->marks[j], 1);
                Sprite_SetAnimCtrlSeq(dst->marks[j], 3);
                break;
            case 2:
                Sprite_SetDrawFlag(dst->marks[j], 1);
                Sprite_SetAnimCtrlSeq(dst->marks[j], 0);
                break;
            case 3:
                Sprite_SetDrawFlag(dst->marks[j], 1);
                Sprite_SetAnimCtrlSeq(dst->marks[j], 1);
                break;
            case 4:
                Sprite_SetDrawFlag(dst->marks[j], 1);
                Sprite_SetAnimCtrlSeq(dst->marks[j], 2);
                break;
            default:
                GF_AssertFail();
                break;
            }
        }
    }
}
#endif
