#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

void sub_0209515C(UnkStruct_020932E0 *arg0) {
    Sprite *sprite;
    Sprite *v0;
    Sprite *v1;
    Sprite *v2;
    Sprite *v3;
    Sprite *v4;
    Sprite *v5;
    Sprite *v6;
    Sprite *v7;
    s32 v12;
    s32 v14;
    s32 v15;
    s32 v16;
    u8 v8;

    v14 = 0;
    do {
        sprite = arg0->unk_198[v14].unk_0;
        if (sprite != NULL) {
            Sprite_Delete(sprite);
        }
        v14 += 1;
    } while (v14 < 0x1E);
    v15 = 0;
    if ((s32)arg0->unk_D > 0) {
        do {
            v0 = arg0->unk_7E8[v15].unk_0;
            if (v0 != NULL) {
                Sprite_Delete(v0);
            }
            v15 += 1;
        } while (v15 < (s32)arg0->unk_D);
    }
    v16 = 0;
    if ((s32)arg0->unk_D > 0) {
        do {
            v1 = arg0->unk_8D0[v16].unk_0;
            if (v1 != NULL) {
                Sprite_Delete(v1);
            }
            v16 += 1;
        } while (v16 < (s32)arg0->unk_D);
    }
    v2 = arg0->unk_8B4;
    if (v2 != NULL) {
        Sprite_Delete(v2);
    }
    v3 = arg0->unk_8BC;
    if (v3 != NULL) {
        Sprite_Delete(v3);
    }
    v4 = arg0->unk_8B8;
    if (v4 != NULL) {
        Sprite_Delete(v4);
    }
    v5 = arg0->unk_8C4;
    if (v5 != NULL) {
        Sprite_Delete(v5);
    }
    v6 = arg0->unk_8C0;
    if (v6 != NULL) {
        Sprite_Delete(v6);
    }
    v12 = 0;
    v8 = 3 - arg0->unk_D;
    if ((s32)v8 > 0) {
        do {
            v7 = arg0->unk_8C8[v12];
            if (v7 != NULL) {
                Sprite_Delete(v7);
            }
            v12 += 1;
        } while (v12 < (s32)v8);
    }
}
