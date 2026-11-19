#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02005D10.h"
#include "bg_window.h"
#include "pokemon_types_def.h"
#include "options.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "player_data.h"
#include "naming_screen.h"
#include "overlay_95_egg_hatch_app.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "overlay_41_sprite_resource_collection.h"
#include "struct_defs/resdat_narc_structs.h"
#include "egg_hatch_particle_system.h"

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
void ManagedSprite_SetAnimationFrame(ManagedSprite *managedSprite, u16 frame);
enum EggAnimFrame {
    ANIM_FRAME_WHOLE_EGG = 0,
    ANIM_FRAME_ONE_CRACK,
    ANIM_FRAME_COUPLE_CRACKS,
    ANIM_FRAME_BIG_CRACKS,
    ANIM_FRAME_GAPING_HOLE,
    ANIM_FRAME_READY_TO_SHATTER,
};
enum EggHatchShakeAnimID {
    EGG_MINOR_SHAKE = 0,
    EGG_BIG_SHAKE,
    EGG_BIGGER_SHAKE,
    EGG_EXPLOSIVE_SHAKE,
};
BOOL ov95_021E5EDC(EggHatchParticleSystem *eps);
int ov95_021E6150(EggHatchCutscene *eggHatch, enum EggHatchShakeAnimID idx);
void ov95_021E5EC0(EggHatchParticleSystem *eps, int resourceID);
enum EggNormalEmitter {
    EMITTER_FIRST_EGG_SHARDS = 0,
    EMITTER_MORE_EGG_SHARDS,
    EMITTER_EGG_EXPLODES,
    EMITTER_SPARKLES,
};
u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);
u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);
void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);
BOOL Pokepic_ResumePaletteFade(Pokepic *monSprite);
NNSG2dImagePaletteProxy *Sprite_GetPaletteProxy(Sprite *paletteProxy);
void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);
void ManagedSprite_GetPositionXY(ManagedSprite *managedSprite, s16 *outX, s16 *outY);
void ManagedSprite_OffsetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);
void ov95_021E62F0(EggHatchCutscene *eggHatch, BOOL hide);
void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);
u32 ObjPlttTransfer_GetPaletteVramOffset(const NNSG2dImagePaletteProxy *paletteProxy, NNS_G2D_VRAM_TYPE vramType);
enum EggManaphyEmitter {
    EMITTER_MANAPHY_GLOW = 0,
    EMITTER_MANAPHY_SPARKLES,
    EMITTER_MANAPHY_RING,
    EMITTER_MANAPHY_GLOW_EGG,
};

BOOL ov95_021E67F0(EggHatchCutscene *eggHatch);
BOOL ov95_021E6838(EggHatchCutscene *eggHatch);
BOOL ov95_021E68A8(EggHatchCutscene *eggHatch);
BOOL ov95_021E6900(EggHatchCutscene *eggHatch);
BOOL ov95_021E6964(EggHatchCutscene *eggHatch);
BOOL ov95_021E6B74(EggHatchCutscene *eggHatch);

BOOL ov95_021E67F0(EggHatchCutscene *eggHatch) {
    switch (eggHatch->subState) {
    case 0:
        ManagedSprite_SetAnimationFrame(eggHatch->eggSprite, ANIM_FRAME_WHOLE_EGG);
        eggHatch->subState++;
        break;
    case 1:
        if (++eggHatch->subStateTimer >= 25) {
            PlaySE(1812);
            eggHatch->subState++;
        }
        break;
    default:
        eggHatch->subState = 0;
        eggHatch->subStateTimer = 0;
        return 0;
    }

    return 1;
}

BOOL ov95_021E6838(EggHatchCutscene *eggHatch) {
    switch (eggHatch->subState) {
    case 0:
        if (ov95_021E5EDC(eggHatch->graphics.eps) == 1) {
            break;
        }

        ManagedSprite_SetAnimationFrame(eggHatch->eggSprite, ANIM_FRAME_ONE_CRACK);
        eggHatch->subState++;
        break;
    case 1:
        if (ov95_021E6150(eggHatch, EGG_MINOR_SHAKE) == 2) {
            PlaySE(1812);

            ManagedSprite_SetAnimationFrame(eggHatch->eggSprite, ANIM_FRAME_COUPLE_CRACKS);
            eggHatch->subState++;
            eggHatch->subStateTimer = 0;
        }
        break;
    default:
        if (ov95_021E6150(eggHatch, EGG_MINOR_SHAKE) == 2) {
            ManagedSprite_SetAnimationFrame(eggHatch->eggSprite, ANIM_FRAME_COUPLE_CRACKS);
            eggHatch->subState = 0;
            eggHatch->subStateTimer = 0;
            return 0;
        }
        break;
    }

    return 1;
}

BOOL ov95_021E68A8(EggHatchCutscene *eggHatch) {
    switch (eggHatch->subState) {
    case 0:
        eggHatch->subState++;
        break;
    case 1: {
        int shakeProgress = ov95_021E6150(eggHatch, EGG_BIG_SHAKE);

        if (shakeProgress == 1) {
            PlaySE(1812);
            ManagedSprite_SetAnimationFrame(eggHatch->eggSprite, ANIM_FRAME_BIG_CRACKS);
            ov95_021E5EC0(eggHatch->graphics.eps, EMITTER_FIRST_EGG_SHARDS);
        }

        if (shakeProgress == 2) {
            eggHatch->subStateTimer = 0;
            eggHatch->subState++;
        }
    } break;
    default:
        eggHatch->subState = 0;
        eggHatch->subStateTimer = 0;
        return 0;
    }

    return 1;
}

BOOL ov95_021E6900(EggHatchCutscene *eggHatch) {
    switch (eggHatch->subState) {
    case 0:
        if (ov95_021E5EDC(eggHatch->graphics.eps) == 1) {
            break;
        }

        eggHatch->subState++;
        break;
    case 1: {
        int shakeProgress = ov95_021E6150(eggHatch, EGG_BIGGER_SHAKE);

        if (shakeProgress == 1) {
            PlaySE(1812);

            ManagedSprite_SetAnimationFrame(eggHatch->eggSprite, ANIM_FRAME_GAPING_HOLE);
            ov95_021E5EC0(eggHatch->graphics.eps, EMITTER_MORE_EGG_SHARDS);
        }

        if (shakeProgress == 2) {
            eggHatch->subStateTimer = 0;
            eggHatch->subState++;
        }
    } break;
    default:
        eggHatch->subState = 0;
        eggHatch->subStateTimer = 0;
        return 0;
    }

    return 1;
}

BOOL ov95_021E6964(EggHatchCutscene *eggHatch) {
    switch (eggHatch->subState) {
    case 0:
        if (ov95_021E5EDC(eggHatch->graphics.eps) == 1) {
            break;
        }

        ManagedSprite_SetAnimationFrame(eggHatch->eggSprite, ANIM_FRAME_GAPING_HOLE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
        eggHatch->subState++;
        break;
    case 1: {
        int shakeProgress = ov95_021E6150(eggHatch, EGG_EXPLOSIVE_SHAKE);

        if (shakeProgress == 1) {
            ManagedSprite_SetAnimationFrame(eggHatch->eggSprite, ANIM_FRAME_READY_TO_SHATTER);
            PlaySE(1799);
            ov95_021E5EC0(eggHatch->graphics.eps, EMITTER_EGG_EXPLODES);
            ov95_021E5EC0(eggHatch->graphics.eps, EMITTER_SPARKLES);
        }

        if (shakeProgress == 2) {
            ManagedSprite_SetDrawFlag(eggHatch->eggSprite, 0);
            eggHatch->subStateTimer = 0;
            eggHatch->subState++;
        }
    } break;
    case 2:
        if (++eggHatch->subStateTimer >= 10) {
            eggHatch->subStateTimer = 0;

            PaletteData_BeginPaletteFade(eggHatch->graphics.plttData, (1 << PLTTBUF_MAIN_BG), 0xFFFF, 0, 0, 16, 0xFFFF);

            NNSG2dImagePaletteProxy *proxy = Sprite_GetPaletteProxy(eggHatch->topBarSprite->sprite);
            int palIndex = ObjPlttTransfer_GetPaletteVramOffset(proxy, NNS_G2D_VRAM_TYPE_2DMAIN);

            PaletteData_BeginPaletteFade(eggHatch->graphics.plttData, (1 << PLTTBUF_MAIN_OBJ), 0xFFFF ^ (1 << palIndex), 0, 0, 16, 0xFFFF);

            Pokepic_StartPaletteFade(eggHatch->monSprite, 0, 16, 0, 0xFFFF);
            eggHatch->subState++;
        }
        break;
    case 3: {
        s16 x, y;
        int barsOffScreen = 0;

        ManagedSprite_GetPositionXY(eggHatch->topBarSprite, &x, &y);

        if (y > -48) {
            ManagedSprite_OffsetPositionXY(eggHatch->topBarSprite, 0, -4);
        } else {
            barsOffScreen++;
        }

        ManagedSprite_GetPositionXY(eggHatch->bottomBarSprite, &x, &y);

        if (y < 192) {
            ManagedSprite_OffsetPositionXY(eggHatch->bottomBarSprite, 0, +4);
        } else {
            barsOffScreen++;
        }

        if (PaletteData_GetSelectedBuffersBitmask(eggHatch->graphics.plttData) != 0 || barsOffScreen != 2 || Pokepic_ResumePaletteFade(eggHatch->monSprite) != 0) {
            break;
        }

        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
        ov95_021E62F0(eggHatch, 0);
        ManagedSprite_SetDrawFlag(eggHatch->eggSprite, 0);
        ManagedSprite_SetDrawFlag(eggHatch->topBarSprite, 0);
        ManagedSprite_SetDrawFlag(eggHatch->bottomBarSprite, 0);

        eggHatch->subState++;
    } break;
    case 4:
        if (++eggHatch->subStateTimer >= 25) {
            eggHatch->subStateTimer = 0;

            Pokepic_StartPaletteFade(eggHatch->monSprite, 16, 0, 0, 0xFFFF);
            PaletteData_BeginPaletteFade(eggHatch->graphics.plttData, (1 << PLTTBUF_MAIN_BG), 0xFFFF, 0, 16, 0, 0xFFFF);
            PaletteData_BeginPaletteFade(eggHatch->graphics.plttData, (1 << PLTTBUF_MAIN_OBJ), 0xFFFF, 0, 16, 0, 0xFFFF);

            eggHatch->subState++;
        }
        break;
    case 5:
        if (PaletteData_GetSelectedBuffersBitmask(eggHatch->graphics.plttData) != 0 || Pokepic_ResumePaletteFade(eggHatch->monSprite) != 0) {
            break;
        }

        eggHatch->subState++;
        break;
    default:
        if (ov95_021E5EDC(eggHatch->graphics.eps) == 1) {
            break;
        }

        eggHatch->subState = 0;
        eggHatch->subStateTimer = 0;

        return 0;
    }

    return 1;
}

BOOL ov95_021E6B74(EggHatchCutscene *eggHatch) {
    switch (eggHatch->subState) {
    case 0:
        ov95_021E5EC0(eggHatch->graphics.eps, EMITTER_MANAPHY_GLOW);
        ov95_021E5EC0(eggHatch->graphics.eps, EMITTER_MANAPHY_SPARKLES);
        ov95_021E5EC0(eggHatch->graphics.eps, EMITTER_MANAPHY_RING);
        ov95_021E5EC0(eggHatch->graphics.eps, EMITTER_MANAPHY_GLOW_EGG);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
        eggHatch->subState++;
        break;
    case 1:
        switch (eggHatch->subStateTimer) {
        case 25:
            PlaySE(2054);
            break;
        case 80:
            PlaySE(1962);
            break;
        case 142:
            PlaySE(2069);
            break;
        }

        if (++eggHatch->subStateTimer >= 160) {
            eggHatch->subStateTimer = 0;
            PaletteData_BeginPaletteFade(eggHatch->graphics.plttData, (1 << PLTTBUF_MAIN_BG), 0xFFFF, 0, 0, 16, 0xFFFF);

            NNSG2dImagePaletteProxy *proxy = Sprite_GetPaletteProxy(eggHatch->topBarSprite->sprite);
            int palIndex = ObjPlttTransfer_GetPaletteVramOffset(proxy, NNS_G2D_VRAM_TYPE_2DMAIN);

            PaletteData_BeginPaletteFade(eggHatch->graphics.plttData, (1 << PLTTBUF_MAIN_OBJ), 0xFFFF ^ (1 << palIndex), 0, 0, 16, 0xFFFF);

            Pokepic_StartPaletteFade(eggHatch->monSprite, 0, 16, 0, 0xFFFF);
            eggHatch->subState++;
        }
        break;
    case 2: {
        s16 x, y;
        int barsOffScreen = 0;

        ManagedSprite_GetPositionXY(eggHatch->topBarSprite, &x, &y);

        if (y > -48) {
            ManagedSprite_OffsetPositionXY(eggHatch->topBarSprite, 0, -4);
        } else {
            barsOffScreen++;
        }

        ManagedSprite_GetPositionXY(eggHatch->bottomBarSprite, &x, &y);

        if (y < 192) {
            ManagedSprite_OffsetPositionXY(eggHatch->bottomBarSprite, 0, +4);
        } else {
            barsOffScreen++;
        }

        if (PaletteData_GetSelectedBuffersBitmask(eggHatch->graphics.plttData) != 0 || barsOffScreen != 2 || Pokepic_ResumePaletteFade(eggHatch->monSprite) != 0) {
            break;
        }

        ov95_021E62F0(eggHatch, 0);

        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
        ManagedSprite_SetDrawFlag(eggHatch->eggSprite, 0);
        ManagedSprite_SetDrawFlag(eggHatch->topBarSprite, 0);
        ManagedSprite_SetDrawFlag(eggHatch->bottomBarSprite, 0);

        eggHatch->subState++;
    } break;
    case 3:
        if (++eggHatch->subStateTimer >= 25) {
            eggHatch->subStateTimer = 0;

            Pokepic_StartPaletteFade(eggHatch->monSprite, 16, 0, 0, 0xFFFF);
            PaletteData_BeginPaletteFade(eggHatch->graphics.plttData, (1 << PLTTBUF_MAIN_BG), 0xFFFF, 0, 16, 0, 0xFFFF);
            PaletteData_BeginPaletteFade(eggHatch->graphics.plttData, (1 << PLTTBUF_MAIN_OBJ), 0xFFFF, 0, 16, 0, 0xFFFF);

            eggHatch->subState++;
        }
        break;
    case 4:
        if (PaletteData_GetSelectedBuffersBitmask(eggHatch->graphics.plttData) != 0 || Pokepic_ResumePaletteFade(eggHatch->monSprite) != 0) {
            break;
        }

        eggHatch->subState++;
        break;
    default:
        eggHatch->subState = 0;
        eggHatch->subStateTimer = 0;
        return 0;
    }

    return 1;
}
