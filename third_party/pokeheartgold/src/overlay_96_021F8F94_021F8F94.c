#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02013534.h"

Sprite *ov96_021EB408(enum HeapID *, s32, s32, u8, u8);

enum HeapID ov96_021EB4F4(void *, s32, s32);

s32 ov96_021EB52C(enum HeapID, s32, s32);

s32 ov96_021EB5B8(const SaveData *);

s32 ov96_021F9134(enum HeapID *, enum HeapID *, String *, s32, s32, s32, s32, enum HeapID *);

void ov96_021F8F94(enum HeapID *arg0, PokeathlonCourseData *arg1, s32 arg2);

void ov96_021F8F94(enum HeapID *arg0, PokeathlonCourseData *arg1, s32 arg2) {
    VecFx32 pos;
    s32 i;
    enum HeapID *sp30;
    enum HeapID *sp2C;
    Sprite *sprite;
    String *name;
    enum HeapID *p;
    u8 *row;
    enum HeapID obj;
    s32 j;

    i = 0;
    do {
        sprite = ov96_021EB408(*(void **)((u8 *)arg0 + 4), 0, 2, 0x68, 3);
        pos.z = 0;
        pos.x = (i * 64) << 12;
        pos.y = (arg2 + 0x10) << 12;
        Sprite_SetMatrix(sprite, &pos);
        Sprite_SetAnimActiveFlag(sprite, 1);
        Sprite_SetAnimCtrlSeq(sprite, 0);
        Sprite_SetPalIndexRespectVramOffset(sprite, i);
        Sprite_SetDrawPriority(sprite, 2);
        for (j = 0; j < 3; j++) {
            sprite = ov96_021EB408(*(void **)((u8 *)arg0 + 4), 0, 2, 0x68, 4);
            pos.z = 0;
            pos.x = (i * 64 + 8 + j * 16) << 12;
            pos.y = (arg2 + 0x18) << 12;
            Sprite_SetMatrix(sprite, &pos);
            Sprite_SetAnimActiveFlag(sprite, 1);
            Sprite_SetAnimCtrlSeq(sprite, 1);
            Sprite_SetDrawPriority(sprite, 1);
        }
        i++;
    } while (i < 4);
    i = 0;
    sp30 = (enum HeapID *)((u8 *)arg0 + 0x24);
    row = (u8 *)arg0;
    sp2C = (enum HeapID *)((u8 *)arg0 + 0x34);
    do {
        obj = ov96_021EB4F4(*(void **)((u8 *)arg0 + 4), 0x68, 3);
        *sp30 = obj;
        ov96_021EB52C(obj, 1, 1);
        p = sp30;
        for (j = 0; j < 3; j++) {
            obj = ov96_021EB4F4(*(void **)((u8 *)arg0 + 4), 0x68, 4);
            p[1] = obj;
            ov96_021EB52C(obj, 1, 1);
            p++;
        }
        name = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(arg1, i), *arg0);
        ov96_021F9134(arg0, (enum HeapID *)((u8 *)arg0 + 0xC), name, 2, 0, -0x10, ov96_021EB5B8(*(void **)(row + 0x24)), sp2C);
        String_Delete(name);
        TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)(row + 0x34), 1);
        *(s32 *)(row + 0x48) = arg2 + 0x10;
        row += 0x28;
        sp30 = (enum HeapID *)((u8 *)sp30 + 0x28);
        sp2C = (enum HeapID *)((u8 *)sp2C + 0x28);
        i++;
    } while (i < 4);
}
