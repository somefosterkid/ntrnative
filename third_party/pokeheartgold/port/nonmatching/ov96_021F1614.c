/* PORTOK hk42 */
#include "bg_window.h"
#include "error_handling.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "unk_02005D10.h"
void *ov96_021E60C0(PokeathlonCourseData *, s32, s32);

void *ov96_021E60D8(PokeathlonCourseData *, s32, s32);

s32 ov96_021E658C(PokeathlonCourseData *, u8, s32);

Sprite *ov96_021E8BAC(s32);

s32 ov96_021EAA04(s32, u8);

s32 ov96_021EAA20(s32);

s32 ov96_021EAB38(s32, s32);

s32 ov96_021EAB74(s32, s32);
s32 ov96_021EABE0(s32, s32);

s32 ov96_021EAC0C(s32, u8);

s32 ov96_021EAED4(s32, s32, s16, s32, s32);

s32 ov96_021EAF60(s32, u8, s32, void *);

s32 ov96_021EB01C(s32, s32, s32, s32);

s32 ov96_021EB06C(s32, s32, s32, s32 *, s32 *);

s32 ov96_021EB52C(s32, s32, s32);

Sprite *ov96_021EB5B8(s32);

s32 ov96_021F295C(u8, BgConfig *);

s32 ov96_021F2B68(void *, u8, u8, PokeathlonCourseData *);

s32 ov96_021F2D98(void *);

s32 ov96_021F2E2C(void *, u8);

s32 ov96_021F2E80(PokeathlonCourseData *, void *, s32, u8, s32);

s32 ov96_021F2FEC(void *, u8 *);

s32 ov96_021F3180(u8, u16, u16, s32 *);

s32 ov96_021F4364(s32, u32);

s32 ov96_021F4390(s32, u8, u32);

s32 ov96_021F45F4(s32, u8, u8 *);

s32 ov96_021F4688(s32, s32, s32);

/* invented */ typedef struct { u16 v[8]; } Ov96TileRect;
/* invented */ typedef struct { f32 v[6]; } Ov96ScaleTable;
/* invented */ typedef struct { s32 v[6]; } Ov96OffsetTable;

extern const Ov96TileRect ov96_0221BC8C;
extern const Ov96ScaleTable ov96_0221BC9C;
extern const Ov96OffsetTable ov96_0221BCB4;

void ov96_021F1614(PokeathlonCourseData *arg0) {
    u8 *alloc;
    u8 *e;
    Sprite *sprite;
    u8 sync;
    Sprite *sprite2;
    u8 player;
    s32 w;
    s32 hilite;
    s32 page;
    u8 stage;
    u8 playSE;
    u8 seq;
    u8 arg58;
    u8 *data;
    u8 level;
    u8 arg54;
    u8 count;
    u8 oldPage;
    u8 mode;
    u8 blink;
    u8 kind;
    int i;
    int row;
    int col;
    int x;
    int y;
    s32 extra;
    s32 obj;
    s32 obj2;
    s32 outX;
    s32 outY;
    s32 posX;
    s32 posY;
    s32 unk78;
    u8 flags[12];
    Ov96TileRect tiles;
    Ov96OffsetTable offsets;
    Ov96ScaleTable scales2;
    Ov96OffsetTable offsets2;
    VecFx32 scale;
    VecFx32 one;
    Ov96ScaleTable scales;

    alloc = PokeathlonCourse_GetHeapAllocPtr4(arg0);
    data = ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0) + 0xF0);
    player = ov96_021E5F24(arg0);
    w = *(s32 *)(data + 0x1C);
    hilite = (w >> 28) & 0xF;
    page = (w >> 24) & 0xF;
    stage = *(s32 *)(data + 0x18) >> 24;
    w = *(s32 *)(data + 0x20);
    playSE = (w >> 13) & 1;
    sync = (w >> 14) & 1;
    seq = (w >> 15) & 7;
    arg58 = (w >> 26) & 3;
    arg54 = w >> 18;
    count = (u8)((w >> 28) & 0xF) + 1;
    if (sync == 0) {
#ifdef __MWERKS__
        ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0))[8] = 0;
#else
        ((PokeathlonTouchRecord *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(arg0)))->unk_08 = 0;
#endif
    }
    oldPage = *(u16 *)(alloc + 0x730);
    if (oldPage != (u8)page) {
        *(u16 *)(alloc + 0x730) = (u8)page;
        for (i = 0; i < 12; i++) {
            alloc[0x73C + i] = 0;
        }
        ov96_021F2B68(alloc + 0x7AC, oldPage, page, arg0);
    }
    if (stage == 0) {
        u8 *tilemap = *(u8 **)(alloc + 0x738);
        BG_LoadScreenTilemapData(*(BgConfig **)alloc, 1, tilemap + 0xC, *(u32 *)(tilemap + 8));
        ScheduleBgTilemapBufferTransfer(*(BgConfig **)alloc, 1);
        *(s32 *)(alloc + 0x7A8) = 0;
    } else if (stage < 0x2B) {
        if (*(s32 *)(alloc + 0x7A8) == 0) {
            if (count <= 1 && stage >= 0x20) {
                PlaySE(0x6D7);
                *(s32 *)(alloc + 0x7A8) = 1;
            } else if (count > 1 && stage >= 0x1C) {
                PlaySE(0x6D7);
                *(s32 *)(alloc + 0x7A8) = 1;
            } else if (count > 3 && stage >= 0x15) {
                PlaySE(0x6D7);
                *(s32 *)(alloc + 0x7A8) = 1;
            }
        }
        ov96_021F295C(stage, *(BgConfig **)alloc);
    } else {
        tiles = ov96_0221BC8C;
        LoadRectToBgTilemapRect(*(BgConfig **)alloc, 1, &tiles, 14, 2, 4, 2);
        ov96_021F295C(0x2A, *(BgConfig **)alloc);
    }
    if (playSE != 0) {
        PlaySE(0x8CF);
    }
    ov96_021F4364(*(s32 *)(alloc + 0x774), count);
    scales = ov96_0221BC9C;
    offsets = ov96_0221BCB4;
    for (i = 0; i < 12; i++) {
        col = i % 3;
        row = i / 3;
        e = alloc + 0x20 + row * 0x1B0 + col * 0x90;
        sprite = ov96_021E8BAC(ov96_021EAA20(*(s32 *)e));
        mode = (*(s32 *)(data + 0x1C) >> (i * 2)) & 3;
        blink = (*(s32 *)(data + 0x20) >> i) & 1;
        if (mode == 0) {
            extra = 0;
            if (*(u16 *)(alloc + i * 4 + 0x778) == 0) {
                *(u16 *)(alloc + i * 4 + 0x778) = 1;
            } else if (*(u16 *)(alloc + i * 4 + 0x778) == 1) {
                scales2 = scales;
                offsets2 = offsets;
                scale.x = 4096.0f * scales2.v[*(u16 *)(alloc + i * 4 + 0x77A) >> 1];
                scale.y = 4096.0f * scales2.v[*(u16 *)(alloc + i * 4 + 0x77A) >> 1];
                scale.z = 4096.0f * scales2.v[*(u16 *)(alloc + i * 4 + 0x77A) >> 1];
                ov96_021EB06C(*(s32 *)e, data[i], data[i + 0xC], &outX, &outY);
                if (outY >= 0xB0) {
                    extra = offsets2.v[*(u16 *)(alloc + i * 4 + 0x77A) >> 1];
                }
                *(u16 *)(alloc + i * 4 + 0x77A) += 1;
                ov96_021EABE0(*(s32 *)e, 1);
                Sprite_SetAffineScale(sprite, &scale);
                if (*(u16 *)(alloc + i * 4 + 0x77A) >= 12) {
                    *(u16 *)(alloc + i * 4 + 0x778) = 2;
                }
            } else {
                ov96_021EAB38(*(s32 *)e, 0);
            }
            ov96_021EB52C(*(s32 *)(e + 8), 1, 0);
            ov96_021EAB74(*(s32 *)e, 0);
            ov96_021EB01C(*(s32 *)e, data[i], extra + data[i + 0xC], 1);
            flags[i] = 1;
        } else {
        *(u16 *)(alloc + i * 4 + 0x778) = 0;
        *(u16 *)(alloc + i * 4 + 0x77A) = 0U;
            one.x = FX32_ONE;
            one.y = FX32_ONE;
            one.z = FX32_ONE;
            ov96_021EABE0(*(s32 *)e, 0);
            if (blink != 0) {
                ov96_021EB52C(*(s32 *)(e + 8), 1, 1);
                if (row == player && IsSEPlaying(0x890) == 0) {
                    PlaySE(0x890);
                }
            } else {
                ov96_021EB52C(*(s32 *)(e + 8), 1, 0);
            }
            Sprite_SetAffineScale(sprite, &one);
            ov96_021EAB38(*(s32 *)e, 1);
            if (mode == 2 && alloc[0x73C + i] == 0) {
                alloc[0x73C + i] = 1;
                ov96_021EAF60(*(s32 *)(alloc + 0x76C), i, 1, e + 0x68);
            } else if (mode == 1 && alloc[0x73C + i] == 1) {
                alloc[0x73C + i] = 0;
                ov96_021EAF60(*(s32 *)(alloc + 0x76C), i, 1, e + 0x48);
            }
            flags[i] = 0;
            x = data[i];
            y = data[i + 0xC];
            level = (*(s32 *)(data + 0x18) >> (i * 2)) & 3;
            *(s32 *)(e + 0xC) = x << 12;
            *(s32 *)(e + 0x10) = y << 12;
            {
                VecFx32 pos = {0, 0, 0};
                pos.x = *(s32 *)(e + 0xC);
                pos.y = *(s32 *)(e + 0x10);
                Sprite_SetMatrix(ov96_021EB5B8(*(s32 *)(e + 4)), &pos);
            }
            {
                VecFx32 pos2 = {0, 0, 0};
                pos2.x = *(s32 *)(e + 0xC);
                pos2.y = *(s32 *)(e + 0x10);
                switch (*((u8 *)ov96_021E60C0(arg0, i / 3, i % 3) + 7)) {
                case 1:
                    pos2.y -= 0x8000;
                    break;
                case 2:
                    pos2.y -= 0x8000;
                    break;
                case 3:
                    pos2.y -= 0x10000;
                    break;
                default:
                    GF_AssertFail();
                    break;
                }
                Sprite_SetMatrix(ov96_021EB5B8(*(s32 *)(e + 8)), &pos2);
            }
            obj = ov96_021EAA04(*(s32 *)(alloc + 0x76C), i);
            kind = level + 1;
            ov96_021EAC0C(obj, kind);
            ov96_021EB01C(obj, x, y, 1);
            if (row == player && *((u8 *)ov96_021E60D8(arg0, player, i % 3) + 2) != 0) {
                ov96_021EAED4(obj, 0, *(s16 *)(alloc + 0x7D0), kind, 0);
            }
            if (sync != 0) {
                obj2 = ov96_021EAA04(*(s32 *)(alloc + 0x76C), i);
                Sprite_SetAnimActiveFlag(sprite, 0);
                alloc[0x7AF] = 1;
                ov96_021F2E80(arg0, alloc + 0x7AC, obj2, i, kind);
            } else {
                Sprite_SetAnimActiveFlag(sprite, 1);
            }
            if (hilite == i) {
                if (seq >= 1 && seq <= 4) {
                    sprite2 = ov96_021EB5B8(*(s32 *)(e + 4));
                    ov96_021EB52C(*(s32 *)(e + 4), 1, 1);
                    Sprite_SetAnimCtrlSeq(sprite2, seq + 2);
                    if (seq <= 2) {
                        PlaySE(0x88D);
                    } else if (seq == 3) {
                        PlaySE(0x88E);
                    } else {
                        PlaySE(0x88F);
                    }
                    ov96_021F4390(*(s32 *)(alloc + 0x774), row, seq);
                    ov96_021F2E2C(alloc + 0x7AC, i);
                } else {
                    GF_AssertFail();
                }
            }
            if (row == player) {
                ov96_021EB06C(*(s32 *)e, x, y, &posX, &posY);
                if (mode == 1 && ov96_021F3180(*(u16 *)(alloc + 0x730), posX, posY, &unk78) != 0) {
                    ov96_021E658C(arg0, col, 1);
                } else {
                    ov96_021E658C(arg0, col, 0);
                }
            }
        }
    }
    ov96_021F2FEC(alloc, data);
    if (alloc[0x7AF] != 0 && sync == 0) {
        ov96_021F2D98(alloc + 0x7AC);
    }
    ov96_021F4688(*(s32 *)(alloc + 0x774), arg54, arg58);
    ov96_021F45F4(*(s32 *)(alloc + 0x774), sync, flags);
}

