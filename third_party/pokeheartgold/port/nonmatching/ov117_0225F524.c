#include "bg_window.h"
#include "brightness.h"
#include "constants/heap.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_01_encounter_effect.h"
#include "overlay_117.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite.h"
#include "text.h"
#include "unk_02009D48.h"

#define EFFECT_BLEND_TARGETS (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD)

// The task and its EncounterEffect arrive as pointers (the callers ov117_0225F4D4..ov117_0225F514 pass them on), and
// the effect's work is an ExecutiveEncounterEffect in encEffect->param: every field is reached by name, because the
// pointers in these structs are 8 bytes on a 64-bit host.
void ov01_021EFCDC(EncounterEffect *encEffect, SysTask *effectTask);
void ov01_021EFCF8(u32 screen, u32 screenFlashColor, u32 otherScreenFlashColor, BOOL *done, u32 numFlashes);
void ov01_021EFEC8(QuadraticInterpolationTaskFX32 *task, fx32 startValue, fx32 endValue, fx32 initialRate, int numSteps);
BOOL ov01_021EFF28(QuadraticInterpolationTaskFX32 *task);
void ov01_021F0454(NARC *narc, u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, BgConfig *bgConfig, u32 param7);
void ov01_021F05C4(UnkStruct_ov5_021DE47C *renderer, s32 param1, s32 param2);
void ov01_021F05F4(UnkStruct_ov5_021DE47C *renderer);
void ov01_021F06EC(UnkStruct_ov5_021DE47C *renderer, UnkStruct_ov5_021DE5A4 *resources);
Sprite *ov01_021F0718(UnkStruct_ov5_021DE47C *renderer, UnkStruct_ov5_021DE5A4 *resources, fx32 x, fx32 y, fx32 z, int param5);
void ov01_021F12D0(ScreenShakeEffect *screenShake);
void ov01_021F1310(ScreenShakeEffect *screenShake, u32 interval);

void ov117_0225F524(SysTask *task, EncounterEffect *encEffect, const ExecutiveEncounterParam *param) {
    BOOL transitionDone;
    Sprite *sprite;
    String *string;
    int iVar5;
    ExecutiveEncounterEffect *eff;
    enum HeapID heapId = HEAP_ID_FIELD1;

    eff = encEffect->param;
    switch (encEffect->state) {
    case 0:
        encEffect->param = Heap_Alloc(heapId, sizeof(ExecutiveEncounterEffect));
        memset(encEffect->param, 0, sizeof(ExecutiveEncounterEffect));
        eff = encEffect->param;
        ov01_021F05C4(&eff->renderer, 1, 1);
        ov01_021F0614(encEffect->narc, &eff->renderer, &eff->resources, param->mugshotPlttIdx, 1, param->mugshotTileIdx, param->mugshotCellIdx, param->mugshotAnimIdx, 600000);
        sprite = ov01_021F0718(&eff->renderer, &eff->resources, 0x140000, 0x80000, 0, 0);
        eff->sprite = sprite;
        Sprite_SetDrawFlag(sprite, 0);
        Sprite_SetOamMode(eff->sprite, GX_OAM_MODE_XLU);
        Sprite_SetPriority(eff->sprite, 0);
        GfGfx_EngineATogglePlanes(0x10, 1);
        eff->spritePos.x = 0x140000;
        eff->spritePos.y = 0x80000;
        eff->spritePos.z = 0;
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, EFFECT_BLEND_TARGETS, 0xc, 4);
        SetBgPriority(2, 0);
        SetBgPriority(3, 1);
        SetBgPriority(1, 2);
        SetBgPriority(0, 3);
        ToggleBgLayer(1, 0);
        ToggleBgLayer(2, 0);
        ToggleBgLayer(3, 0);
        ov01_021F0454(encEffect->narc, 0xd9, 0xd8, 0xd7, 0, 2, encEffect->fieldSystem->bgConfig, 1);
        GfGfxLoader_GXLoadPalFromOpenNarc(encEffect->narc, 0x10, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_2_OFFSET, 0x20, heapId);
        AddWindowParameterized(encEffect->fieldSystem->bgConfig, &eff->nameWindow, 2, 0, 0x14, 0x10, 2, 2, 1);
        FillWindowPixelBuffer(&eff->nameWindow, 0);
        string = ov117_0225F470(param->trainerID, heapId);
        AddTextPrinterParameterizedWithColor(&eff->nameWindow, FONT_SYSTEM, string, 0, 0, 0, 0x10200, NULL);
        String_Delete(string);
        ov01_021F0454(encEffect->narc, 0xda, 0xd8, 0xd7, 0, 2, encEffect->fieldSystem->bgConfig, 3);
        G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG3, TRUE);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_OBJ, TRUE);
        G2_SetWnd0Position(0, 0x60, 255, 0x60);
        ov01_021F12B4(&eff->screenShake, heapId);
        ov01_021EFCF8(1, 0x10, 0xfffffff0, &encEffect->effectComplete, 1);
        encEffect->state++;
        break;
    case 1:
        ToggleBgLayer(1, 1);
        eff->timer = 0xc;
        encEffect->state++;
        // fallthrough
    case 2:
        BgSetPosTextAndCommit(encEffect->fieldSystem->bgConfig, 1, BG_POS_OP_ADD_ROT, 0x18);
        eff->timer--;
        if (eff->timer <= 0) {
            encEffect->state++;
        }
        break;
    case 3:
        GX_SetVisibleWnd(GX_WNDMASK_W0);
        ToggleBgLayer(3, 1);
        ov01_021EFEC8(&eff->interpTask, 0, 0x60000, 0x4000, 4);
        encEffect->state++;
        // fallthrough
    case 4:
        BgSetPosTextAndCommit(encEffect->fieldSystem->bgConfig, 1, BG_POS_OP_ADD_ROT, 0x18);
        iVar5 = ov01_021EFF28(&eff->interpTask);
        if (iVar5 == 0) {
            iVar5 = eff->interpTask.currentValue >> 0xc;
            G2_SetWnd0Position(0, 0x60 - iVar5, 255, 0x60 + iVar5);
        } else {
            GX_SetVisibleWnd(GX_WNDMASK_NONE);
            BgSetPosTextAndCommit(encEffect->fieldSystem->bgConfig, 1, BG_POS_OP_SET_ROT, 0);
            eff->timer = 0xd;
            encEffect->state++;
        }
        break;
    case 5:
        eff->timer--;
        if (eff->timer <= 0) {
            encEffect->state++;
        }
        break;
    case 6:
        ov117_0225F420(encEffect, 1, 0xdd);
        SetBgPriority(1, 1);
        SetBgPriority(3, 2);
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, EFFECT_BLEND_TARGETS, 0, 0x10);
        Sprite_SetDrawFlag(eff->sprite, 1);
        eff->timer = 0;
        encEffect->state++;
        // fallthrough
    case 7:
        transitionDone = FALSE;
        if (eff->timer <= 0x10) {
            eff->timer += 4;
            if (eff->timer > 0x10) {
                eff->timer = 0x10;
                transitionDone = TRUE;
            }
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, EFFECT_BLEND_TARGETS, eff->timer, 0x10 - eff->timer);
        }
        eff->spritePos.x += ((0x80000 - eff->spritePos.x) * 2) / 3;
        if (eff->spritePos.x >> 0xc <= 0x82) {
            eff->spritePos.x = 0x80000;
            if (transitionDone == TRUE) {
                StartBrightnessTransition(8, 0, 0x10, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG1), 1);
                BG_SetMaskColor(0, 0x14a5);
                ToggleBgLayer(0, 0);
                encEffect->state++;
            }
        }
        if (4 < eff->timer) {
            ToggleBgLayer(2, 1);
        }
        ScheduleSetBgPosText(encEffect->fieldSystem->bgConfig, 2, BG_POS_OP_SET_ROT, -((eff->spritePos.x >> 0xc) + 8));
        Sprite_SetMatrix(eff->sprite, &eff->spritePos);
        break;
    case 8:
        if (IsBrightnessTransitionActive(1) != 0) {
            eff->timer = 0x10;
            encEffect->state++;
        }
        break;
    case 9:
        eff->timer--;
        if (eff->timer <= 0) {
            ov01_021EFEC8(&eff->interpTask, eff->spritePos.x, -0x60000, 0x2000, 8);
            eff->timer = 8;
            ToggleBgLayer(3, 0);
            encEffect->state++;
        }
        break;
    case 10:
        eff->spritePos.x = eff->interpTask.currentValue;
        iVar5 = -((eff->spritePos.x >> 0xc) + 8);
        if (iVar5 < 0) {
            ScheduleSetBgPosText(encEffect->fieldSystem->bgConfig, 2, BG_POS_OP_SET_ROT, iVar5);
        } else {
            ToggleBgLayer(2, 0);
        }
        Sprite_SetMatrix(eff->sprite, &eff->spritePos);
        iVar5 = ov01_021EFF28(&eff->interpTask);
        if (iVar5 != 0) {
            GXS_DispOff();
            BG_SetMaskColor(4, 0);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, 4, 1, heapId);
            encEffect->state++;
        }
        break;
    case 0xb:
        if (IsPaletteFadeFinished() != 0) {
            encEffect->state++;
        }
        break;
    case 0xc:
        if (encEffect->done != NULL) {
            *encEffect->done = TRUE;
        }
        ov01_021F12D0(&eff->screenShake);
        eff->shaking = FALSE;
        Sprite_Delete(eff->sprite);
        ov01_021F06EC(&eff->renderer, &eff->resources);
        ov01_021F05F4(&eff->renderer);
        RemoveWindow(&eff->nameWindow);
        BG_ClearCharDataRange(3, 0x20, 0, heapId);
        BG_ClearCharDataRange(1, 0x20, 0, heapId);
        BgClearTilemapBufferAndCommit(encEffect->fieldSystem->bgConfig, 3);
        BgClearTilemapBufferAndCommit(encEffect->fieldSystem->bgConfig, 1);
        ov01_021EFCDC(encEffect, task);
        // the effect and its work are freed: nothing below applies (shaking was just cleared, state is 0xc)
        return;
    }
    if (eff->shaking == 1) {
        ov01_021F1310(&eff->screenShake, 2);
    }
    if (encEffect->state != 0xc) {
        SpriteList_RenderAndAnimateSprites(eff->renderer.unk_00);
    }
}
