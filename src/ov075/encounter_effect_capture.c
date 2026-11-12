// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov148/encounter_effect_capture.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "field/encounter_effect_capture.h"
#include "types.h"
#include "field/encounter_effect.h"
#include "field/field.h"
#include "gfl/bg_sys.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nnsys/gfd.h"
#include "system/game_event.h"
#include "system/game_system.h"

// The capture is a 256x256 direct color texture in a whole VRAM bank
#define CAPTURE_TEX_SIZE (256 * 256 * sizeof(u16))
// The size of a VRAM bank of A to D in the texture image slots
#define TEX_BANK_SIZE 0x20000

static GameEventReturnCode func_ov075_021f536c(GameEvent *event, u32 *state, void *data);
static void func_ov075_021f54a0(EncEffCaptureWork *work);
static void func_ov075_021f54c0(TCB *tcb, void *data);
static NNSGfdTexKey func_ov075_021f54e0(void);
static GameEventReturnCode func_ov075_021f5518(GameEvent *event, u32 *state, void *data);

GameEvent *func_ov075_021f5260(GameSystem *gsys, const VecFx32 *pos, GameEvent *(*init)(GameSystem *gsys),
                               void (*render)(EncEffGrid *grid)) {
    GameEvent *event;
    EncEffCaptureWork *work;

    event = GameEvent_Create(gsys, NULL, func_ov075_021f536c, sizeof(EncEffCaptureWork));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(EncEffCaptureWork));
    work->camPos = *pos;
    work->init = init;
    work->render = render;
    return event;
}

GameEvent *func_ov075_021f529c(GameSystem *gsys, const VecFx32 *pos, GameEvent *(*init)(GameSystem *gsys),
                               void (*render)(EncEffGrid *grid)) {
    GameEvent *event;
    EncEffCaptureWork *work;

    event = GameEvent_Create(gsys, NULL, func_ov075_021f5518, sizeof(EncEffCaptureWork));
    work = GameEvent_GetData(event);
    MI_CpuFill8(work, 0, sizeof(EncEffCaptureWork));
    work->camPos = *pos;
    work->init = init;
    work->render = render;
    return event;
}

// Sets up the camera and the captured screen's texture, then calls the effect's render function
// pokebw2's EncEffCapture_Draw
void func_ov075_021f52d8(void *data) {
    EncEffCaptureWork *work = data;

    G3i_PerspectiveW_(FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)), FX32_ONE * 4 / 3, FX32_ONE, FX32_CONST(1024),
                   FX32_ONE, TRUE, NULL);
    G3X_Reset();
    {
        VecFx32 camUp = { 0, FX32_ONE, 0 };
        VecFx32 target = { 0, 0, 0 };
        VecFx32 camPos = work->camPos;

        G3i_LookAt_(&camPos, &camUp, &target, TRUE, NULL);
    }
    G3_TexImageParam(GX_TEXFMT_DIRECT, GX_TEXGEN_TEXCOORD, GX_TEXSIZE_S256, GX_TEXSIZE_T256, GX_TEXREPEAT_NONE,
                     GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, NNS_GfdGetTexKeyAddr(work->texKey));
    work->render(work->effectWork);
}

// pokebw2's EncEffCaptureFlash_Callback
static GameEventReturnCode func_ov075_021f536c(GameEvent *event, u32 *state, void *data) {
    EncEffCaptureWork *work = data;
    GameSystem *gsys;
    Field *field;
    GameEvent *next;

    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    switch (*state) {
    case 0:
        GFL_FadeSet(FADE_ENGINE_A_WHITE, 0, 16, 0);
        (*state)++;
        // Fall through to check the fade in the same frame
    case 1:
        if (!GFL_FadeIsRunning()) {
            GFL_FadeSet(FADE_ENGINE_A_WHITE, 16, 0, 0);
            (*state)++;
        }
        break;
    case 2:
        if (!GFL_FadeIsRunning()) {
            work->flashes++;
            if (work->flashes < 2) {
                GFL_FadeSet(FADE_ENGINE_A_WHITE, 0, 16, 0);
                *state = 1;
            } else {
                (*state)++;
            }
        }
        break;
    case 3:
        func_ov075_021f54a0(work);
        (*state)++;
        break;
    case 4:
        if (work->captured) {
            GFL_BGSysSetBGPriority(0, 0);
            GFL_BGSysSetBGPriority(2, 3);
            work->renderMode = Field_GetRenderMode(field);
            work->texBanks = GX_GetBankForTex();
            work->addedTexBank = FALSE;
            if (!(work->texBanks & GX_VRAM_D)) {
                work->texBanks |= GX_VRAM_D;
                work->addedTexBank = TRUE;
            }
            GX_DisableBankForLCDC();
            GX_SetBankForTex(work->texBanks);
            work->texKey = func_ov075_021f54e0();
            (*state)++;
        }
        break;
    case 5:
        GFL_BGSysSetBGEnabled(2, TRUE);
        GFL_BGSysSetBGEnabled(0, TRUE);
        Field_SetRenderMode(field, 2);
        next = work->init(gsys);
        work->effectWork = EncEff_GetWorkArea(Field_GetEncEff(field));
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    case 6:
        if (work->addedTexBank == TRUE) {
            work->texBanks &= ~GX_VRAM_D;
            GX_SetBankForTex(work->texBanks);
        }
        Field_SetRenderMode(field, work->renderMode);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

// Turns bank D over to the LCDC and captures the 3D screen to it at the next V-blank
// pokebw2's EncEffCapture_StartCapture
static void func_ov075_021f54a0(EncEffCaptureWork *work) {
    work->captured = FALSE;
    GX_SetBankForLCDC(GX_VRAM_D);
    GFL_VBlankTCBAdd(func_ov075_021f54c0, &work->captured, 0);
}

// pokebw2's EncEffCapture_CaptureTask
static void func_ov075_021f54c0(TCB *tcb, void *data) {
    BOOL *captured = data;

    GX_SetCapture(GX_CAPTURE_SIZE_256x192, GX_CAPTURE_MODE_A, GX_CAPTURE_SRCA_3D, GX_CAPTURE_SRCB_VRAM_0x00000,
                  GX_CAPTURE_DEST_VRAM_D_0x00000, 16, 0);
    GFL_TCBRemove(tcb);
    if (captured != NULL) {
        *captured = TRUE;
    }
}

// The key of bank D in the texture VRAM: the texture image slots are the texture banks in order
// pokebw2's EncEffCapture_GetTexKey
static NNSGfdTexKey func_ov075_021f54e0(void) {
    u32 texBanks = GX_GetBankForTex();
    u32 offset = 0;

    // BUG: `|=` for `&`, which is always true, so banks A to C are all counted, texture banks or not. This is right
    // only while they all hold textures (system/screentex.c has the same bug)
#ifdef BUGFIX
    if (texBanks & GX_VRAM_C) {
#else
    if (texBanks |= GX_VRAM_C) {
#endif
        offset += TEX_BANK_SIZE;
    }
#ifdef BUGFIX
    if (texBanks & GX_VRAM_B) {
#else
    if (texBanks |= GX_VRAM_B) {
#endif
        offset += TEX_BANK_SIZE;
    }
#ifdef BUGFIX
    if (texBanks & GX_VRAM_A) {
#else
    if (texBanks |= GX_VRAM_A) {
#endif
        offset += TEX_BANK_SIZE;
    }
    return NNS_GfdMakeTexKey(offset, CAPTURE_TEX_SIZE, FALSE);
}

// pokebw2's EncEffCapture_Callback
static GameEventReturnCode func_ov075_021f5518(GameEvent *event, u32 *state, void *data) {
    EncEffCaptureWork *work = data;
    GameSystem *gsys;
    Field *field;
    GameEvent *next;

    gsys = GameEvent_GetGameSystem(event);
    field = GSYS_GetField(gsys);
    switch (*state) {
    case 0:
        func_ov075_021f54a0(work);
        (*state)++;
        break;
    case 1:
        if (work->captured) {
            GFL_BGSysSetBGPriority(0, 0);
            GFL_BGSysSetBGPriority(2, 3);
            work->renderMode = Field_GetRenderMode(field);
            work->texBanks = GX_GetBankForTex();
            work->addedTexBank = FALSE;
            if (!(work->texBanks & GX_VRAM_D)) {
                work->texBanks |= GX_VRAM_D;
                work->addedTexBank = TRUE;
            }
            GX_DisableBankForLCDC();
            GX_SetBankForTex(work->texBanks);
            work->texKey = func_ov075_021f54e0();
            (*state)++;
        }
        break;
    case 2:
        GFL_BGSysSetBGEnabled(2, TRUE);
        GFL_BGSysSetBGEnabled(0, TRUE);
        Field_SetRenderMode(field, 2);
        next = work->init(gsys);
        work->effectWork = EncEff_GetWorkArea(Field_GetEncEff(field));
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    case 3:
        if (work->addedTexBank == TRUE) {
            work->texBanks &= ~GX_VRAM_D;
            GX_SetBankForTex(work->texBanks);
        }
        Field_SetRenderMode(field, work->renderMode);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}
