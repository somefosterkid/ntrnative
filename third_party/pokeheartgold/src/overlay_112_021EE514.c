#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "text.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_112_pokewalker_app.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

s32 ov112_021EC460(Ov112PokewalkerApp *, s32, s32, s32);

s32 ov112_021EE514(void *arg0);

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
int ov112_021EE604(UnkStruct_ov109_021D0F70 *param0);

s32 ov112_021EE514(void *arg0) {
    ov112_021EC460(arg0, 2, 0x1B, 0x39);
    return 0x40;
}

s32 ov112_021EE524(void *arg0);

s32 ov112_021EE524(void *arg0) {
    *(s32 *)((u8 *)arg0 + 4) = 6;
    return 0x42;
}

s32 ov112_021EE52C(void);

s32 ov112_021EE52C(void) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_154);
    return 0x3F;
}

s32 ov112_021EAA10(void *);

s32 ov112_021EE550(void *arg0);

s32 ov112_021EE550(void *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        ov112_021EAA10(arg0);
        *(s32 *)((u8 *)arg0 + 4) = 0;
        return 0x42;
    }
    return 0x3F;
}

s32 ov112_021EE570(Ov112PokewalkerApp *arg0);

s32 ov112_021EE570(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (TextPrinterCheckActive((u8) * &arg0->unk_1E524) == 0) {
        v0 = arg0->unk_C;
        arg0->unk_8 = v0;
        return v0;
    }
    return 0x40;
}

s32 ov112_021EA570(void *);

s32 ov112_021EE594(Ov112PokewalkerApp *arg0);

s32 ov112_021EE594(Ov112PokewalkerApp *arg0) {
    s32 v0;

    if (ManagedSprite_IsAnimated(arg0->sprites[0]) == 0) {
        ov112_021EA570(arg0);
        v0 = arg0->unk_C;
        arg0->unk_8 = v0;
        return v0;
    }
    return 0x41;
}

s32 ov112_021EA64C(void *arg0);

s32 ov112_021EA688(void *, s32);

s32 ov112_021EE5B8(Ov112PokewalkerApp *arg0);

s32 ov112_021EE5B8(Ov112PokewalkerApp *arg0) {
    ov112_021EA64C(arg0);
    ov112_021EA688(arg0, 3);
    ov112_021EA688(arg0, 4);
    ov112_021EA688(arg0, 8);
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 1U);
    return 0x43;
}

int ov112_021EE604(UnkStruct_ov109_021D0F70 *param0) {
    return 2;
}
