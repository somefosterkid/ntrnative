#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_3d_loader.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

typedef enum HeapID HeapID;

typedef enum BgPosAdjustOp BgPosAdjustOp;

typedef enum FadeType FadeType;

typedef enum FadeMode FadeMode;

struct Background;

struct BgConfig;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResHeader;

struct GF_2DGfxResMan;

struct GF_2DGfxResObjList;

struct OverlayManager;

struct OverlayManagerTemplate;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceHeaderList;

struct SpriteResourcesHeader;

struct UnkStruct_ov112_021E7830_Table;

typedef int (*UnkFunc_ov112_021E7830)(void *data);

typedef struct UnkStruct_ov112_021E7830_Table {
    UnkFunc_ov112_021E7830 init;
    UnkFunc_ov112_021E7830 main;
    UnkFunc_ov112_021E7830 exit;
} UnkStruct_ov112_021E7830_Table;

extern UnkStruct_ov112_021E7830_Table ov112_021FF54C[];

void ov112_021E9C94(void *data);

BOOL ov112_021E7830(OverlayManager *man, int *state);

BOOL ov112_021E7830(OverlayManager *man, int *state) {
    u8 *data = OverlayManager_GetData(man);
    u32 *words = (u32 *)data;

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished() == 1) {
            *state = 1;
        }
        break;
    case 1:
        *state = ov112_021FF54C[words[0]].init(data);
        break;
    case 2:
        *state = ov112_021FF54C[words[0]].main(data);
        BgSetPosTextAndCommit((BgConfig *)words[0x18 / 4], (BgPosAdjustOp)(5), (BgPosAdjustOp)(0), *(u16 *)(data + 0x1F2E0));
        break;
    case 3:
        *state = ov112_021FF54C[words[0]].exit(data);
        if (words[1] == 0xB) {
            BeginNormalPaletteFade((FadeMode)(0), (FadeType)(0), (FadeType)(0), 0x7FFF, (FadeMode)(6), (HeapID)(1), (HeapID)(0x9A));
            *state = 4;
        } else {
            words[0] = words[1];
            ov112_021E9C94(data);
        }
        break;
    case 4:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }
    if (*(u32 *)(data + 0x1E52C) != 0) {
        SpriteSystem_DrawSprites(*(SpriteManager **)(data + 0x1E52C));
    }
    return 0;
}
