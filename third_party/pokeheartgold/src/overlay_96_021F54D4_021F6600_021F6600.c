#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"
#include "overlay_96_unk_struct_ov96_021_f74a4.h"

u8 ov96_021F65D8(UnkStruct_ov96_021F5018_Ball *arg0);

void ov96_021E8228(PokeathlonCourseData *, u32, u8, s32, s32);

void ov96_021EAF78(void *, s32, s32, s32 *, s32 *, s32 *);

void ov96_021EB0A4(void *, s32, s32, s32 *, s32 *);

u32 ov96_021F6060(VecFx32 *, s32, VecFx32 *, s32);

void ov96_021F6798(PokeathlonCourseData *, UnkStruct_ov96_021F5018 *, s32);

void ov96_021F75BC(UnkStruct_ov96_021F74A4 *, s32);

VecFx32 *ov96_021F75D4(UnkStruct_ov96_021F74A4 *, u8);

u32 ov96_021F75E0(UnkStruct_ov96_021F74A4 *, u8);

void ov96_021F75E8(UnkStruct_ov96_021F74A4 *, u8, u8);

s32 sub_02005944(s32, u16, s32);

s32 sub_0200606C(s32, s32);

extern s32 ov96_0221C110[];

extern u8 ov96_0221DC18[4];

void ov96_021F6600(PokeathlonCourseData *arg0, UnkStruct_ov96_021F5018 *arg1);

void ov96_021F6600(PokeathlonCourseData *arg0, UnkStruct_ov96_021F5018 *arg1) {
    s32 posZ;
    s32 tileX;
    s32 tileY;
    VecFx32 pos;
    u8 hitBall;
    s32 player;
    s32 i;
    s32 j;
    void *sprites;

    player = (u8)ov96_021E5F24(arg0);
    for (i = 0; i < 3; i++) {
        ov96_021F6798(arg0, arg1, (u8)i);
    }
    for (i = 0; i < 3; i++) {
        sprites = arg1->unk_90[i].unk_00;
        ov96_021EB0A4(sprites, arg1->unk_90[i].unk_1C / 0x1000, (0x120000 - (arg1->unk_FAC[i].unk_08 << 6)) / 0x1000, &tileX, &tileY);
        ov96_021EAF78(sprites, tileX << 0xC, tileY << 0xC, &pos.x, &pos.y, &posZ);
        pos.z = 0;
        for (j = 0; j < 0x1D; j++) {
            if (ov96_021F75E0(arg1->unk_8C, j) != 0 && ov96_021F6060(&pos, posZ << 0xC, ov96_021F75D4(arg1->unk_8C, j), 0x8000) != 0) {
                ov96_021F75BC(arg1->unk_8C, (u8)j);
                hitBall = ov96_021F65D8(&arg1->unk_90[i]);
                ov96_021F75E8(arg1->unk_8C, j, hitBall);
                arg1->unk_142 = 1;
                sub_0200606C(0x88D, ov96_0221DC18[i]);
                sub_02005944(ov96_0221DC18[i], 0xFFFF, ov96_0221C110[hitBall - 1]);
                ov96_021E8228(arg0, player, i, 3, 1);
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (arg1->unk_90[i].unk_26 != 0) {
            arg1->unk_90[i].unk_27--;
            if (arg1->unk_90[i].unk_27 == 0) {
                arg1->unk_90[i].unk_27 = 0;
                arg1->unk_90[i].unk_26 = 0;
            }
        }
    }
}
