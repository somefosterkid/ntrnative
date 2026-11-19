/* PORTOK hk42 */
#include "sprite.h"
#include "unk_02005D10.h"

// 12-byte entries of the array at work+0x8D0
typedef struct {
    Sprite *sprite;
    s32 id;
    u32 val;
} UnkEntry_02093E7C;

void sub_02094668(void *, s32, u32, u32);

void sub_02093E7C(void *work, u8 idx) {
    u8 *p = (u8 *)work;
    u8 i;
    UnkEntry_02093E7C *e;
    VecFx32 pos;
    u8 *slot;

    PlaySE(0x5EA);
    *(u32 *)(*(u8 **)(p + 0x46A4) + idx * 8 + 4) = 1;
    sub_02094668(work, idx, *(u32 *)(p + 0x4684) & 0xFF, 0);
    for (i = 0; i < *(u8 *)(p + 0xD); i++) {
        e = *(UnkEntry_02093E7C **)(p + 0x8D0) + i;
        if (e->id == -1) {
            pos = *(VecFx32 *)(p + 0x4688);
            pos.x -= 0x8000;
            pos.y -= 0x4000;
            Sprite_SetMatrix(e->sprite, &pos);
            Sprite_SetDrawFlag(e->sprite, 1);
            e->id = *(s32 *)(p + 0x4684);
            e->val = *(u8 *)(p + 0xF);
            *(u8 *)(p + 0xE) = *(u8 *)(p + 0xE) + 1;
            Sprite_SetOamMode(*(Sprite **)(p + *(s32 *)(p + 0x4684) * 0x34 + 0x198), GX_OAM_MODE_XLU);
            slot = p + idx * 16;
            *(s32 *)(slot + 0x888) = *(s32 *)(p + 0x4684);
            *(u32 *)(slot + 0x884) = *(u8 *)(p + 0xF);
            *(u32 *)(slot + 0x88C) = i;
            *(u32 *)(slot + 0x890) = 1;
            *(s32 *)(p + 0x46B0) = idx;
            return;
        }
    }
}
