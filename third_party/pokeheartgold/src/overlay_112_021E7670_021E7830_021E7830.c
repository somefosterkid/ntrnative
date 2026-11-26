#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_3d_loader.h"
#include "overlay_112_pokewalker_app.h"
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
    Ov112PokewalkerApp *data = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished() == 1) {
            *state = 1;
        }
        break;
    case 1:
        *state = ov112_021FF54C[data->unk_0].init(data);
        break;
    case 2:
        *state = ov112_021FF54C[data->unk_0].main(data);
        BgSetPosTextAndCommit(data->bgConfig, (BgPosAdjustOp)(5), (BgPosAdjustOp)(0), data->unk_1F2E0);
        break;
    case 3:
        *state = ov112_021FF54C[data->unk_0].exit(data);
        if (data->unk_4 == 0xB) {
            BeginNormalPaletteFade((FadeMode)(0), (FadeType)(0), (FadeType)(0), 0x7FFF, (FadeMode)(6), (HeapID)(1), (HeapID)(0x9A));
            *state = 4;
        } else {
            data->unk_0 = data->unk_4;
            ov112_021E9C94(data);
        }
        break;
    case 4:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }
    if (data->spriteManager != NULL) {
        SpriteSystem_DrawSprites(data->spriteManager);
    }
    return 0;
}
