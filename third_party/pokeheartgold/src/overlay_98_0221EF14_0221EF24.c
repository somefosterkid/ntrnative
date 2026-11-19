#include "global.h"

#include "constants/gx.h"

#include "bg_window.h"
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

struct SysTask;

struct SysTaskQueue;

struct System;

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

extern const Ov98Rodata sRodata;

extern const TouchscreenHitbox ov98_0221F1E0;

BOOL ov98_0221EF24(void);

BOOL ov98_0221EF24(void) {
    TouchscreenHitbox hitbox = ov98_0221F1E0;
    BOOL result = FALSE;

    if (TouchscreenHitbox_TouchNewIsIn(&hitbox) || (gSystem.newKeys & PAD_BUTTON_B)) {
        result = TRUE;
    }
    return result;
}
