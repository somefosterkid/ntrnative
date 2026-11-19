#ifndef POKEHEARTGOLD_OVERLAY_27_OV27TASK_H
#define POKEHEARTGOLD_OVERLAY_27_OV27TASK_H

// Ov27Task: the field start menu's SysTask environment (CreateSysTaskAndEnvironment size 0x540). hk65 (11-13)
// built the layout from the asm of every access (asmflow closure of ov27_0225A320's task data): embedded records
// at the shifts the callees take them (G2dRenderer 0x1C, SpriteResourcesHeader 0x204, Windows 0x3D0.. and 0x52C)
// and the pointer/array shapes the accesses show.

#include "bg_window.h"
#include "field_system_def.h"
#include "unk_02009D48.h"

struct BgConfig;
struct GF_2DGfxResMan;
struct GearPhoneRingManager;
struct MessageFormat;
struct MsgData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct String;

// The record at 0x520: the Pokegear phone icon (ov27_0225BDDC fills it in, ov27_0225BDFC updates it, ov27_0225BEB0 tears it down).
typedef struct Ov27PhoneIcon {
    struct BgConfig *bgConfig;
    struct GearPhoneRingManager *gearPhone;
    struct Sprite *sprite;
    Window window;
    s32 unk_1C;
} Ov27PhoneIcon;

// One start-menu icon (unk_470[i]): enabled flag and its label.
typedef struct Ov27TaskIcon {
    u8 unk_0;
    struct String *unk_4;
} Ov27TaskIcon;

typedef struct Ov27Task {
    u32 unk_00;
    struct BgConfig *bgConfig;
    void *task;
    u16 *unk_0C; // &FieldSystem::lastTouchMenuInput
    FieldSystem *fieldSystem;
    s32 unk_14;
    struct SpriteList *unk_18;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *unk_144[4];
    struct SpriteResource *unk_154[11][4];
    SpriteResourcesHeader unk_204[11];
    struct Sprite *unk_390[16];
    Window unk_3D0;
    Window unk_3E0;
    Window unk_3F0[8];
    Ov27TaskIcon unk_470[7];
    struct MsgData *unk_4A8;
    struct MessageFormat *unk_4AC;
    struct String *unk_4B0[7];
    u16 unk_4CC[32];
    u8 unk_50C;
    u8 pad_50D[0x3];
    s32 unk_510;
    u8 unk_514[8];
    u32 unk_51C_0 : 1;
    u32 unk_51C_1 : 4;
    u32 unk_51C_5 : 1;
    u32 unk_51C_6 : 2;
    u32 unk_51C_8 : 24;
    Ov27PhoneIcon unk_520;
} Ov27Task;

// sizeof(Ov27Task) == 0x540

#endif // POKEHEARTGOLD_OVERLAY_27_OV27TASK_H
