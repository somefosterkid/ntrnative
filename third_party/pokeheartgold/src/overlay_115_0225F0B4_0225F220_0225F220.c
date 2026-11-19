#include "global.h"

#include "constants/heap.h"

#include "field/hblank_system.h"

#include "bg_window.h"
#include "brightness.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_115.h"
#include "overlay_120.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sys_task.h"
#include "text.h"
#include "unk_02009D48.h"
#include "overlay_01_encounter_effect.h"
#include "field/hblank_system_priv.h"
#include "linear_interpolation_task.h"

BOOL ov01_021EFE30(EncounterEffect *);

void ov01_021F0454(NARC *, u32, u32, u32, u32, u32, BgConfig *, u32);

UnkStruct_ov5_021DED04 *ov01_021F0B44(void);

void ov01_021F0B78(EncounterEffect *, UnkStruct_ov5_021DED04 *, u32, u32, u32, u32, u32);

BOOL ov115_0225F220(EncounterEffect *encEffect, enum HeapID heapID, GymLeaderEncounterParam *param) {
    GymLeaderEncounterEffect *eff;
    String *name;
    VecFx32 pos;

    eff = encEffect->param;
    switch (encEffect->state) {
    case 0:
        encEffect->param = Heap_Alloc(heapID, sizeof(GymLeaderEncounterEffect));
        memset(encEffect->param, 0, sizeof(GymLeaderEncounterEffect));
        eff = encEffect->param;
        GfGfxLoader_GXLoadPalFromOpenNarc(encEffect->narc, 0x10, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, 0x20, heapID);
        GfGfx_EngineATogglePlanes(4, 0);
        AddWindowParameterized(encEffect->fieldSystem->bgConfig, &eff->nameWindow, 2, 0, 0xa, 0x10, 2, 2, 1);
        FillWindowPixelBuffer(&eff->nameWindow, 0);
        if (param->trainerClass == 0x17) {
            name = ov115_0225F1BC(encEffect->fieldSystem->saveData, heapID);
        } else {
            name = ov115_0225F158(param->trainerID, heapID);
        }
        AddTextPrinterParameterizedWithColor(&eff->nameWindow, FONT_SYSTEM, name, 0, 0, 0, 0x10200, NULL);
        String_Delete(name);
        ov01_021F05C4(&eff->renderer, 8, 3);
        ov01_021F0614(encEffect->narc, &eff->renderer, &eff->resources[0], param->mugshotPlttIdx, 1, param->mugshotTileIdx, param->mugshotCellIdx, param->mugshotAnimIdx, 600000);
        ov01_021F0614(encEffect->narc, &eff->renderer, &eff->resources[1], 0x3b, 1, 0x3c, 0x3d, 0x3e, 0x927c1);
        eff->mugshotSprite = ov01_021F0718(&eff->renderer, &eff->resources[0], 0x110000, 0x42000, 0, 0);
        Sprite_SetDrawFlag(eff->mugshotSprite, 0);
        ov115_0225F020(&eff->scaleSprites, &eff->renderer, &eff->resources[1], 0x48000, 0x4a000, heapID);
        ov115_02260254(eff->mugshotSprite, heapID, param->mugshotPlttIdx, 0xe, 0);
        eff->bannerEffect = ov01_021F0B44();
        encEffect->state++;
        break;
    case 1:
        ov01_021EFCF8(1, 0x10, 0x10, &encEffect->effectComplete, 1);
        encEffect->state++;
        break;
    case 2:
        if (encEffect->effectComplete != 0) {
            encEffect->state++;
        }
        break;
    case 3:
        ov01_021F0454(encEffect->narc, param->bannerTilemapIdx, param->bannerTileIdx, param->bannerPlttIdx, 0, 1, encEffect->fieldSystem->bgConfig, 3);
        eff->bannerScrollEnabled = 1;
        ov01_021F0B78(encEffect, eff->bannerEffect, 6, 8, 0x10, 0x1f, 0x17);
        SetBgPriority(2, 0);
        SetBgPriority(3, 1);
        SetBgPriority(0, 2);
        GfGfx_EngineATogglePlanes(8, 1);
        GfGfx_EngineATogglePlanes(2, 0);
        encEffect->state++;
        break;
    case 4:
        if (ov01_021EFE30(encEffect) != 0) {
            encEffect->state++;
            ov01_021F0B5C(eff->bannerEffect);
            eff->timer = 10;
        }
        break;
    case 5:
        eff->timer--;
        if (eff->timer < 0) {
            GfGfx_EngineATogglePlanes(0x10, 1);
            if (ov115_0225F0B4(&eff->scaleSprites) == 1) {
                encEffect->state++;
            }
        }
        break;
    case 6:
        ov01_021EFEC8(&eff->mugshotXTask, 0x110000, param->endX, 0xfffc0000, 4);
        Sprite_SetDrawFlag(eff->mugshotSprite, 1);
        Sprite_SetPriority(eff->mugshotSprite, 1);
        pos = ov01_021F074C(eff->mugshotXTask.currentValue, 0x42000, 0);
        Sprite_SetMatrix(eff->mugshotSprite, &pos);
        encEffect->state++;
        break;
    case 7: {
        BOOL done = ov01_021EFF28(&eff->mugshotXTask);
        pos = ov01_021F074C(eff->mugshotXTask.currentValue, 0x42000, 0);
        Sprite_SetMatrix(eff->mugshotSprite, &pos);
        if (done == 1) {
            encEffect->state++;
        }
        break;
    }
    case 8:
        ov01_021EFE34(&eff->brightnessTask, 0, 0x10, 3);
        eff->timer = 10;
        encEffect->state++;
        break;
    case 9:
        eff->timer--;
        if (eff->timer < 0) {
            BOOL done = ov01_021EFE44(&eff->brightnessTask);
            ov01_021F0DC8(&eff->brightnessTask.currentValue);
            if (done == 1) {
                ov115_02260254(eff->mugshotSprite, heapID, param->mugshotPlttIdx, 0, 0);
                SetBlendBrightness(-0xe, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_BG0), 1);
                ScheduleSetBgPosText(encEffect->fieldSystem->bgConfig, 2, BG_POS_OP_SET_ROT, -((eff->mugshotXTask.currentValue >> 0xc) - 0x5c));
                GfGfx_EngineATogglePlanes(4, 1);
                encEffect->state++;
            }
        }
        break;
    case 10:
        ov01_021EFE34(&eff->brightnessTask, 0x10, 0, 3);
        encEffect->state++;
        break;
    case 0xb:
    {
        BOOL done = ov01_021EFE44(&eff->brightnessTask);
        ov01_021F0DC8(&eff->brightnessTask.currentValue);
        if (done == 1) {
            encEffect->state++;
            eff->timer = 0x1a;
        }
        break;
    }
    case 0xc:
        eff->timer--;
        if (eff->timer < 0) {
            encEffect->state++;
        }
        break;
    case 0xd:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, 0xf, 1, HEAP_ID_FIELD1);
        encEffect->state++;
        break;
    case 0xe:
        if (IsPaletteFadeFinished() != 0) {
            encEffect->state++;
        }
        break;
    case 0xf:
        sub_0200FBF4(PM_LCD_BOTTOM, 0x7fff);
        if (encEffect->done != NULL) {
            *encEffect->done = 1;
        }
        Sprite_Delete(eff->mugshotSprite);
        ov115_0225F09C(&eff->scaleSprites);
        ov01_021F06EC(&eff->renderer, &eff->resources[0]);
        ov01_021F06EC(&eff->renderer, &eff->resources[1]);
        ov01_021F05F4(&eff->renderer);
        RemoveWindow(&eff->nameWindow);
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffff1fff;
        SetBlendBrightness(0, GX_BLEND_PLANEMASK_NONE, 1);
        BgSetPosTextAndCommit(encEffect->fieldSystem->bgConfig, 2, BG_POS_OP_SET_ROT, 0);
        return 1;
    }
    if (eff->bannerScrollEnabled == 1) {
        ScheduleSetBgPosText(encEffect->fieldSystem->bgConfig, 3, BG_POS_OP_SET_ROT, eff->bannerScroll);
        eff->bannerScroll = (eff->bannerScroll + 0x1e) % 512;
    }
    if (encEffect->state != 0xf) {
        SpriteList_RenderAndAnimateSprites(eff->renderer.unk_00);
    }
    return 0;
}
