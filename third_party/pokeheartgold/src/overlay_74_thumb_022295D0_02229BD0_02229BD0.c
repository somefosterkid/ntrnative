#include "global.h"

#include "unk_02034354.h"
#include "unk_02035900.h"
#include "struct_defs/resdat_narc_structs.h"
#include "mystery_gift.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_mystery_gift_app_data.h"

struct MysteryGiftAppData;

struct PlayerProfile;

typedef void (*UnkFuncPtr_ov97_0222D0A4)(void);

typedef struct {
    MysteryGiftAppData *unk_00;
    MATHRandContext32 unk_04;
    SysTask *unk_1C;
    UnkFuncPtr_ov97_0222D0A4 unk_20;
    u16 unk_24;
    u8 unk_26;
    u8 unk_27;
    PlayerProfile *unk_28[16];
    WonderCard unk_68;
    u8 unk_3C0;
    u8 unk_3C1[16];
} UnkStruct_ov97_0223F1A4;

void ov74_02229C0C(void);

extern UnkStruct_ov97_0223F1A4 *ov74_0223D0A4;

void ov74_02229BC0(UnkFuncPtr_ov97_0222D0A4 param0, s16 param1);

void ov74_02229BD0(void);

void ov74_02229BD0(void) {
    int v0;

    if (!sub_020373B4(sub_0203769C())) {
        return;
    }

    for (v0 = 0; v0 < 16; v0++) {
        ov74_0223D0A4->unk_3C1[v0] = 0;
    }

    sub_02034434();
    ov74_02229BC0(ov74_02229C0C, 0);
}
