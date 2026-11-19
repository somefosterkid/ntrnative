#include "global.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "field_bgm.h"
#include "overlay_85.h"
#include "palette.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sprite_system.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "vram_transfer_manager.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "game_stats.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void ov85_021E7194(UnkStruct_ov109_021D0F70 *param0, u32 param1);

int ov85_021E6674(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E6674(UnkStruct_ov109_021D0F70 *param0) {
    if (param0->unk_18 == 0) {
        sub_02037AC0(202);
    }

    ov85_021E7194(param0, 10);
    param0->unk_00 = 49;

    return 0;
}

int ov85_021E6694(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E6694(UnkStruct_ov109_021D0F70 *param0) {
    if (param0->unk_18 == 0) {
        if (sub_02037B38(202)) {
            sub_020398D4(0, 0);
            sub_02038C1C(1);
            sub_02037FF0();
            param0->unk_00 = 50;
        }
    } else {
        param0->unk_0C++;

        if (param0->unk_0C >= (30 * 4)) {
            param0->unk_0C = 0;

            if (param0->unk_D0->unk_3C == 0) {
                sub_020398D4(0, 0);
                sub_02038C1C(1);
                sub_02037FF0();
            }

            param0->unk_00 = 50;
        }
    }

    return 0;
}

int ov85_021E66F4(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E66F4(UnkStruct_ov109_021D0F70 *param0) {
    if (sub_02037454() <= 1) {
        param0->unk_00 = 51;
        return 1;
    }

    return 0;
}

s32 ov85_021E670C(UnkStruct_ov109_021D0F70 *arg0);

s32 ov85_021E670C(UnkStruct_ov109_021D0F70 *arg0) {
    s32 seqNo;

    BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 8, 1, HEAP_ID_102);
    seqNo = arg0->unk_1C;
    if (seqNo != GF_GetCurrentPlayingBGM()) {
        sub_02055198(NULL, (u16)seqNo);
    }
    arg0->unk_00 = 0x34;
    return 0;
}

int ov85_021E6748(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E6748(UnkStruct_ov109_021D0F70 *param0) {
    if (IsPaletteFadeFinished()) {
        param0->unk_00 = 53;
        return 1;
    }

    return 0;
}

int ov85_021E6760(UnkStruct_ov109_021D0F70 *param0);

int ov85_021E6760(UnkStruct_ov109_021D0F70 *param0) {
    return 2;
}

void ov85_021E6764(void *param0) {
    UnkStruct_ov109_021D0F70 *v0 = param0;

    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(v0->unk_D9C);
    DoScheduledBgGpuUpdates(v0->unk_D84);
}

s32 ov85_021E6854(void);

s32 ov85_021E6874(UnkStruct_ov109_021D0F70 *);

s32 ov85_021E6980(void *);

s32 ov85_021E6A2C(void *);

s32 ov85_021E6B68(void *);

s32 ov85_021E6E14(void *);

s32 ov85_021E6ECC(void *);

s32 ov85_021E6F6C(void *);

s32 ov85_021E705C(void *);

s32 ov85_021E7424(void *);

void ov85_021E678C(void *arg0) {
    ov85_021E6854();
    ov85_021E6980(arg0);
    ov85_021E7424(arg0);
    ov85_021E6A2C(arg0);
    ov85_021E6E14(arg0);
    ov85_021E6ECC(arg0);
    ov85_021E6B68(arg0);
    ov85_021E705C(arg0);
    ov85_021E6F6C(arg0);
    ov85_021E6874(arg0);
}

s32 ov85_021E692C(UnkStruct_ov109_021D0F70 *param0);

s32 ov85_021E6D68(UnkStruct_ov109_021D0F70 *);

s32 ov85_021E6E6C(void *);

s32 ov85_021E7024(void *);

s32 ov85_021E7148(void *);

s32 ov85_021E749C(void *);

void ov85_021E67CC(void *arg0);

void ov85_021E67CC(void *arg0) {
    ov85_021E692C(arg0);
    ov85_021E7024(arg0);
    ov85_021E7148(arg0);
    ov85_021E6D68(arg0);
    ov85_021E6E6C(arg0);
    ov85_021E749C(arg0);
}
