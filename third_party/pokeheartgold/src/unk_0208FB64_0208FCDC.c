#include "global.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

#include "math_util.h"
#include "unk_02035900.h"
#include "unk_0208FB64.h"
#include "overlay_41_sprite_resource_collection.h"
#include "game_stats.h"

void ov73_021E7120(void *, u32, u8);

void sub_0208FCDC(u32 param_1, u32 param_2, u8 *param_3, UnkStruct_020961E8 *param_4) {
    u16 uVar1;

    ov73_021E7120(param_4, 1, *param_3);
    uVar1 = sub_0203769C();
    if (uVar1 == 0) {
        param_4->unk_414 = 0;
    }
    return;
}

void sub_0208FCFC(void) {
    return;
}

void sub_0208FD00(void) {
    return;
}

struct GameStats;

struct JournalEntry;

struct Menu;

struct MsgData;

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_020961E8_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;






typedef struct Menu Menu;




void ov73_021E705C(UnkStruct_020961E8 *param0, int param1, u32 param2);

void sub_0208FD04(int param0, int param1, void *param2, void *param3) {
    UnkStruct_020961E8 *v0 = (UnkStruct_020961E8 *)param3;

    if (sub_0203769C() != 0) {
        ov73_021E705C(v0, 13, 0);
    }
}

void sub_0208FD1C(int param0, int param1, void *param2, void *param3) {
    u8 v0;

    if (sub_0203769C() == 0) {
        v0 = param0;
        sub_02037030(110, &v0, 1);
    }
}

void sub_02036FD8(u32, void *, u32);

void sub_0208FD3C(void *param_1);

void sub_0208FD3C(void *param_1) {
    u16 uVar1;
    s32 iVar2;
    u32 uVar3;
    u32 *puVar4;

    uVar3 = 0;
    puVar4 = (u32 *)(param_1 + 0x388);
    iVar2 = 0;
    do {
        iVar2 = iVar2 + 1;
        uVar3 = uVar3 ^ *puVar4;
        puVar4 = puVar4 + 1;
    } while (iVar2 < 0x2ee);
    *(u32 *)(param_1 + 0xf40) = uVar3;
    uVar1 = LCRandom();
    *(u32 *)(param_1 + 0xf44) = (u32)uVar1;
    sub_02036FD8(0x74, param_1 + 0x388, 0xbc0);
    return;
}

u32 sub_0208FD7C(void) {
    return 0;
}

u32 sub_0208FD80(void) {
    return 1;
}

u32 sub_0208FD84(void) {
    return 4;
}

s32 sub_0208FD88(s32 param_1, s32 param_2) {
    return param_2 + 0xf48 + param_1 * 0xbc0;
}
