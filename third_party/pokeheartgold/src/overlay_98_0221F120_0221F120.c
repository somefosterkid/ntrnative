#include "global.h"

#include "constants/gx.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_98.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02013FDC.h"

struct GraphicsBanks;

struct OamCharTransferParam;

struct OamManagerParam;

struct Ov98Rodata;

struct UnkStruct_02014E30;

typedef struct Ov98Rodata {
    u32 vramTypes[2];
    UnkStruct_02014E30 monPicRect;
    OamCharTransferParam oamTransfer;
    OamManagerParam oamManager;
    TouchscreenHitbox hitbox;
    u32 powersOfTen[5];
    u16 statThresholds[10][2];
    s32 rankThresholds[10];
    GraphicsBanks banks;
} Ov98Rodata;

extern const Ov98Rodata _0221F194;

u32 ov98_0221F120(u32 value, u32 scaleIdx);

u32 ov98_0221F120(u32 value, u32 scaleIdx) {
    u32 prevIdx;

    GF_ASSERT(scaleIdx != 0 && scaleIdx < 5);
    prevIdx = scaleIdx - 1;
    return (value % _0221F194.powersOfTen[scaleIdx]) / _0221F194.powersOfTen[prevIdx];
}
