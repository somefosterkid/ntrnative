#include "global.h"

#include "bag.h"
#include "bag_view.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_02013534.h"
#include "overlay_41_unk_struct_ov22_0225a154.h"
#include "overlay_14_box_funcs.h"
#include "overlay_41_unk_struct_ov22_02258A48.h"
#include "touchscreen.h"
#include "constants/heap.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

struct CharTransferAllocation;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

struct Sprite;

struct SpriteAnimFrame;

struct SysTask;

struct SysTaskQueue;

struct TouchHitboxController;

struct PlayerProfile;

struct UnkStruct_02029C88_t;

struct UnkStruct_ov22_02258A48_t;

struct UnkStruct_ov22_02259C58_tag;

struct YesNoPrompt;

typedef struct UnkStruct_02029C88_t UnkStruct_02029C88;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

enum PokemonContestRank {
    CONTEST_RANK_NORMAL = 0,
    CONTEST_RANK_GREAT = 1,
    CONTEST_RANK_ULTRA = 2,
    CONTEST_RANK_MASTER = 3,
    CONTEST_NUM_RANKS = 3,
    CONTEST_MAX_RANKS = 4,
    CONTEST_RANK_LINK = 4,
};

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;


typedef struct UnkStruct_ov22_02259C58_tag {
    Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58_tag *param0, void *param1);
} UnkStruct_ov22_02259C58;

typedef struct TouchHitboxController TouchHitboxController;

typedef struct {
    UnkStruct_ov22_02259C58 unk_00[4];
    UnkStruct_ov22_0225A154 unk_40;
    BOOL unk_60;
    UnkStruct_02012744 *unk_64;
    TouchHitboxController *unk_68;
    TouchscreenHitbox unk_6C[5];
} UnkStruct_ov22_02259D2C;


void ov41_0224A5D4(u8 *arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4);

typedef struct YesNoPrompt YesNoPrompt;

void ov22_02256AE4(UnkStruct_ov22_02259C58 *param0, void *param1);

void ov22_02256B04(UnkStruct_ov22_02259C58 *param0, void *param1);

void ov22_02256B24(UnkStruct_ov22_02259C58 *param0, void *param1);

void ov22_02256B44(UnkStruct_ov22_02259C58 *param0, void *param1);

void ov22_02256B78(UnkStruct_ov22_02259C58 *param0, void *param1);

void ov22_02256FD8(UnkStruct_02029C88 *param0, UnkStruct_ov22_02257964 *param1, enum PokemonContestRank contestRank, const PlayerProfile *trainerInfo);

extern u8 ov14_021F7D14[];

extern u8 gOverlayManagerTemplate_Bag[];

FS_EXTERN_OVERLAY(OVY_15);

int ov14_021E8ACC(void **param0) {
    void **args;
    Bag *bag;

    args = (void **)param0[0];
    bag = Save_Bag_Get((SaveData *)args[0]);
    param0[6] = Bag_CreateView(bag, ov14_021F7D14, HEAP_ID_9);
    args = (void **)param0[0];
    BagView_Init((BagView *)param0[6], (SaveData *)args[0], 1, NULL, (MenuInputStateMgr *)args[1]);
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_15), OVY_LOAD_ASYNC);
    param0[5] = OverlayManager_New((const OverlayManagerTemplate *)gOverlayManagerTemplate_Bag, param0[6], HEAP_ID_9);
    return 0;
}
