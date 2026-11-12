// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov266/staff_roll_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/psel_graphic.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "demo/staff_roll.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/calctool.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "save/player_info.h"
#include "system/gf_font.h"
#include "system/main.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "nitro/fx/fx_trig.h"

// The credits: lines of names that scroll up from the bottom screen to the top one, and titles that fade in and out,
// over a 3D background with a moving camera, all read from a list in the end archive. The names are ours

// Black's credits are on black, White's on white
#if GAME_VERSION == VERSION_WHITE
#define FADE_BRIGHTNESS(value) (-(value))
#define BACKGROUND_COLOR 0xffff
#define WIPE_COLOR WIPE_COLOR_WHITE
#else
#define FADE_BRIGHTNESS(value) (value)
#define BACKGROUND_COLOR 0
#define WIPE_COLOR WIPE_COLOR_BLACK
#endif
#define COPYRIGHT_PALETTE 2

// The screens a title is shown on
#define SCREEN_TOP (1 << 0)
#define SCREEN_BOTTOM (1 << 1)

// The steps of the staff roll
enum {
    SEQ_INIT,
    SEQ_WAIT_WIPE,
    SEQ_EXIT,
    SEQ_LOGO,
    SEQ_ROLL,
    SEQ_END,
};

// The states of the roll, which the commands of the list set
enum {
    ROLL_READ,
    ROLL_WAIT,
    ROLL_FADE_IN,
    ROLL_FADE_OUT,
    ROLL_UNUSED,
    ROLL_END,
};

// The commands of the list
enum {
    CMD_LINE,
    CMD_FADE_IN,
    CMD_FADE_OUT,
    CMD_SCROLL_START,
    CMD_SCROLL_STOP,
    CMD_END,
    CMD_YIELD,
    CMD_BG_ON,
    CMD_BG_OFF,
    CMD_CAMERA,
    CMD_LINE_AT,
};

// How a line is aligned on its x
enum {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
};


typedef u32 (*StaffRollSeqFunc)(StaffRollWork *work);
typedef BOOL (*StaffRollCmdFunc)(StaffRollWork *work, StaffRollLine *line);

BOOL func_ov181_021b95f8(StaffRollWork *work);
u32 func_ov181_021b9630(StaffRollWork *work);
static u32 func_ov181_021b96b0(StaffRollWork *work);
u32 func_ov181_021b96c8(StaffRollWork *work);
u32 func_ov181_021b974c(StaffRollWork *work);
u32 func_ov181_021b97bc(StaffRollWork *work);
static void func_ov181_021b98f4(void);
void func_ov181_021b9908(void);
static void func_ov181_021b99cc(void);
void func_ov181_021b99f8(void);
void func_ov181_021b9a80(StaffRollWork *work, BOOL flag);
static void func_ov181_021b9af8(StaffRollWork *work);
static void func_ov181_021b9b10(StaffRollWork *work);
void func_ov181_021b9b1c(StaffRollWork *work);
static void func_ov181_021b9b6c(StaffRollWork *work);
void func_ov181_021b9c80(StaffRollWork *work);
void func_ov181_021b9cc8(StaffRollWork *work);
static void func_ov181_021b9d04(StaffRollWork *work);
static void func_ov181_021b9d20(StaffRollWork *work);
void func_ov181_021b9d34(StaffRollWork *work);
void func_ov181_021b9e14(StaffRollWork *work);
static void func_ov181_021b9f1c(StaffRollWork *work);
void func_ov181_021b9f40(StaffRollWork *work);
void func_ov181_021b9f9c(StaffRollCamera *camera);
static void func_ov181_021ba000(StaffRollCamera *camera);
void func_ov181_021ba128(StaffRollWork *work);
u32 func_ov181_021ba2ac(StaffRollWork *work, u32 nextSeq);
static void func_ov181_021ba2d8(StaffRollWork *work);
void func_ov181_021ba318(StaffRollWork *work);
void func_ov181_021ba390(StaffRollWork *work);
BOOL func_ov181_021baa00(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021baa54(StaffRollWork *work, StaffRollLine *line);
void func_ov181_021ba594(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021baa98(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021baaa4(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021baab0(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021baabc(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021baac0(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021baad0(StaffRollWork *work, StaffRollLine *line);

void func_ov181_021b9ad4(TCB *tcb, void *data);
void func_ov181_021b9b98(StaffRollWork *work);
void func_ov181_021b9c30(StaffRollWork *work);
void func_ov181_021b9c58(StaffRollWork *work, GFLBitmap *bitmap, u32 index);
void func_ov181_021b9db0(StaffRollWork *work);
void func_ov181_021b9dcc(StaffRollWork *work, u32 screens);
void func_ov181_021b9ddc(StaffRollWork *work);
void func_ov181_021b9fd8(StaffRollCamera *camera, u16 key, u16 frames);
static void func_ov181_021ba2a8(StaffRollCamera *camera, BOOL active);
void func_ov181_021ba4ac(StaffRollWork *work, StaffRollLine *line, GFLBitmap *bitmap, int y);
void func_ov181_021ba558(StaffRollWork *work, StaffRollLine *line);
u32 func_ov181_021ba83c(StaffRollWork *work);
BOOL func_ov181_021baa84(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021baae0(StaffRollWork *work, StaffRollLine *line);
BOOL func_ov181_021bab00(StaffRollWork *work, StaffRollLine *line);
static void StaffRoll_InitVRAM(void);
static void StaffRoll_InitBG(void);
static void StaffRoll_FreeBG(void);
static void StaffRoll_LoadGraphics(ArcTool *arc);
static void StaffRoll_VBlank(TCB *tcb, void *data);
static void StaffRoll_InitVBlank(StaffRollWork *work);
static void StaffRoll_FreeVBlank(StaffRollWork *work);
static void StaffRoll_InitMsg(StaffRollWork *work);
static void StaffRoll_FreeMsg(StaffRollWork *work);
static void StaffRoll_CreateLineBitmaps(StaffRollWork *work);
static void StaffRoll_FreeLineBitmaps(StaffRollWork *work);
static void StaffRoll_DrawLineBitmap(StaffRollWork *work, GFLBitmap *bitmap, u32 index);
static void StaffRoll_DrawNextLineBitmap(StaffRollWork *work);
static void StaffRoll_ShowLineBitmap(StaffRollWork *work);
static void StaffRoll_LoadLines(StaffRollWork *work);
static void StaffRoll_FreeLines(StaffRollWork *work);
static void StaffRoll_InitWindows(StaffRollWork *work);
static void StaffRoll_FreeWindows(StaffRollWork *work);
static void StaffRoll_RequestFlush(StaffRollWork *work, u32 screens);
static void StaffRoll_Flush(StaffRollWork *work);
static u32 StaffRoll_StartWipeIn(StaffRollWork *work, u32 nextSeq);
static u32 StaffRoll_StartWipeOut(StaffRollWork *work, u32 nextSeq);
void func_ov181_021ba354(u32 *tile, u32 *above, u32 rows);
static void StaffRoll_Scroll(StaffRollWork *work);
static void StaffRoll_DrawLine(StaffRollWork *work, StaffRollLine *line, GFLBitmap *bitmap, int y);
static void StaffRoll_DrawLineBelow(StaffRollWork *work, StaffRollLine *line);
static void StaffRoll_DrawTitle(StaffRollWork *work, StaffRollLine *line);
static BOOL StaffRoll_FadeIn(StaffRollWork *work);
static BOOL StaffRoll_FadeOut(StaffRollWork *work);
static BOOL StaffRollCmd_Line(StaffRollWork *work, StaffRollLine *line);
static BOOL StaffRollCmd_FadeIn(StaffRollWork *work, StaffRollLine *line);
static BOOL StaffRollCmd_FadeOut(StaffRollWork *work, StaffRollLine *line);
static BOOL StaffRollCmd_ScrollStart(StaffRollWork *work, StaffRollLine *line);
static BOOL StaffRollCmd_ScrollStop(StaffRollWork *work, StaffRollLine *line);
static BOOL StaffRollCmd_End(StaffRollWork *work, StaffRollLine *line);
static BOOL StaffRollCmd_Yield(StaffRollWork *work, StaffRollLine *line);
static BOOL StaffRollCmd_LineAt(StaffRollWork *work, StaffRollLine *line);


// The background's model has two animations
static const G3DSceneAnimationSetup data_ov181_021bab30[] = { { 1, 0 }, { 2, 0 } };

static const LightSetup data_ov181_021baba8[4] = {
#include "asset/staff_roll_main_data_ov181_021baba8.inc"
};

static const LightSetupList data_ov181_021bab38 = { data_ov181_021baba8, NELEMS(data_ov181_021baba8) };

static const G3DSceneActorSetup data_ov181_021bab60[] = {
    { 0, 0, 0, 0, data_ov181_021bab30, NELEMS(data_ov181_021bab30) },
};

// The colors of the lines, as the DS draws them
static const u16 data_ov181_021bab40[8] = {
#include "asset/staff_roll_main_data_ov181_021bab40.inc"
};

static const G3DSceneResourceSetup data_ov181_021bab84[] = {
#include "asset/staff_roll_main_data_ov181_021bab84.inc"
};

static const G3DSceneSetup data_ov181_021bab50 = { data_ov181_021bab84, NELEMS(data_ov181_021bab84),
                                                    data_ov181_021bab60, NELEMS(data_ov181_021bab60) };

// pokebw2's sStaffRollSeqFuncs
static const StaffRollSeqFunc data_ov181_021bab70[] = {
    func_ov181_021b9630, func_ov181_021b96b0, func_ov181_021b96c8, func_ov181_021b974c, func_ov181_021b97bc,
};

// pokebw2's sCmdFuncs
static const StaffRollCmdFunc data_ov181_021babd0[] = {
    func_ov181_021baa00, func_ov181_021baa54, func_ov181_021baa84, func_ov181_021baa98,
    func_ov181_021baaa4, func_ov181_021baab0, func_ov181_021baabc, func_ov181_021baac0,
    func_ov181_021baad0, func_ov181_021baae0, func_ov181_021bab00,
};

// The 3D background is drawn in place
static const SRTMatrix data_ov181_021bac2c = {
    { 0, 0, 0 },
    { FX32_ONE, FX32_ONE, FX32_ONE },
    { { { FX32_ONE, 0, 0 }, { 0, FX32_ONE, 0 }, { 0, 0, FX32_ONE } } },
};

// The camera of the background moves through these
static const StaffRollCameraKey data_ov181_021bace4[7] = {
#include "asset/staff_roll_main_data_ov181_021bace4.inc"
};

static const BGSysVRAMConfig data_ov181_021babfc = {
    GX_VRAM_D,          GX_VRAM_BGEXTPLTT_NONE, GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_NONE,   GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_NONE,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_AB,  GX_VRAM_E,              GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K, // Black's
};

BOOL func_ov181_021ba5e8(StaffRollWork *work);
BOOL func_ov181_021ba6f0(StaffRollWork *work);

BOOL func_ov181_021b95f8(StaffRollWork *work) {
    func_ov181_021ba318(work);
    work->seq = data_ov181_021bab70[work->seq](work);
    if (work->seq == SEQ_END) {
        return FALSE;
    }
    func_ov181_021b9c80(work);
    func_ov181_021b9f40(work);
    return TRUE;
}

u32 func_ov181_021b9630(StaffRollWork *work) {
    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetDisplayLayout(1);
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    func_ov181_021b98f4();
    func_ov181_021b9908();
    func_ov181_021b99f8();
    func_ov181_021b9b1c(work);
    func_ov181_021b9d34(work);
    func_ov181_021b9e14(work);
    func_ov181_021b9d04(work);
    func_ov181_021b9af8(work);
    GFL_SndBGMStop(0x3f8);
    return SEQ_LOGO;
}



static u32 func_ov181_021b96b0(StaffRollWork *work) {
    if (GFL_WipeIsFinished() == TRUE) {
        return work->nextSeq;
    }
    return SEQ_WAIT_WIPE;
}

u32 func_ov181_021b96c8(StaffRollWork *work) {
    if (GFL_SndBGMIsFading() == TRUE) {
        return SEQ_EXIT;
    }
    if (GFL_SndBGMIsPlaying() == TRUE) {
        func_02005d28();
    }
    func_ov181_021b9b10(work);
    func_ov181_021b9d20(work);
    func_ov181_021b9f1c(work);
    func_ov181_021b9db0(work);
    func_ov181_021b9b6c(work);
    func_ov181_021b99cc();
    GX_SetMasterBrightness(FADE_BRIGHTNESS(-16));
    GXS_SetMasterBrightness(FADE_BRIGHTNESS(-16));
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GFL_OvlUnload(OVERLAY_ID(67));
    return SEQ_END;
}

u32 func_ov181_021b974c(StaffRollWork *work) {
    switch (work->state) {
    case 0:
        if (work->frames >= 41) {
            work->frames = 0;
            work->state++;
        } else if (work->fastForward == TRUE) {
            work->frames += 4;
        } else {
            work->frames++;
        }
        break;
    case 1:
        if (!func_ov181_021ba83c(work)) {
            work->state++;
        }
        break;
    case 2:
        work->frames = 0;
        work->state = 0;
        return SEQ_ROLL;
    }
    return SEQ_LOGO;
}

u32 func_ov181_021b97bc(StaffRollWork *work) {
    StaffRollLine *line;

    if (work->state == ROLL_READ) {
        while (TRUE) {
            line = &work->lines[work->lineIndex];
            if (work->lineIndex == work->lineBitmaps[work->shownLineBitmaps].line) {
                func_ov181_021b9cc8(work);
                work->shownLineBitmaps++;
                work->waitFrames = line->wait;
                work->state = ROLL_WAIT;
                work->lineIndex += 2;
                break;
            }
            if (data_ov181_021babd0[line->command](work, line) == TRUE) {
                work->lineIndex++;
                break;
            }
            work->lineIndex++;
        }
    }
    switch (work->state) {
    case ROLL_WAIT:
        if (work->waitFrames <= 0) {
            work->waitFrames = 0;
            work->state = ROLL_READ;
        } else if (work->fastForward == TRUE) {
            work->waitFrames -= 4;
        } else {
            work->waitFrames -= 1;
        }
        break;
    case ROLL_FADE_IN:
        if (!func_ov181_021ba5e8(work)) {
            work->state = ROLL_READ;
        }
        break;
    case ROLL_FADE_OUT:
        if (!func_ov181_021ba6f0(work)) {
            work->state = ROLL_READ;
        }
        break;
    case ROLL_END:
        if (GFL_SndBGMIsPlaying() == TRUE) {
            GFL_SndBGMFadeOut(0x40);
        }
        return func_ov181_021ba2ac(work, 2);
    }
    func_ov181_021ba390(work);
    return SEQ_ROLL;
}




void func_ov181_021b98f4(void) {
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov181_021babfc);
}

void func_ov181_021b9908(void) {
    GFL_BGSysCreate(0x8e);
    {
        BGSysLCDConfig lcdConfig = { 1, 3, 0, 1 };

        GFL_BGSysSetLCDConfig(&lcdConfig);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, 1, 0, 0x1f, 0, 0x8000, 0, 0, 0, 0 };

        GFL_BGSysCreateBG(1, &setup, 0);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, 1, 1, 0x1e, 4, 0x10000, 1, 1, 0, 0 };

        GFL_BGSysCreateBG(3, &setup, 2);
    }
    {
        MtxFx22 mtx;

        MAT2_SetScaleRot(&mtx, 0, 0x9000, 0x9000, 0);
        GFL_BGSysSetBGTransform(3, &mtx, 0x80, 0x60);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, 1, 0, 0x1f, 0, 0x8000, 0, 0, 0, 0 };

        GFL_BGSysCreateBG(5, &setup, 0);
    }
    GFL_BGSysSetBGEnabledA(2, 1);
    GFL_BGSysSetBGEnabledB(2, 1);
}


void func_ov181_021b99cc(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysFree();
}

// Loads the logo and the palettes of the text
void func_ov181_021b99f8(void) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(0x1a, 0x808e);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 3, 0, 0, TRUE, 0x8e);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 1, 3, 0, 0, TRUE, 0x8e);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(0xdc, 0x808e);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0xc, 0, 0x1e0, 0x20, 0x8e);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0xc, 4, 0x1e0, 0x20, 0x8e);
    GFL_ArcToolFree(arc);
    GFL_BGSysResetStdPalette(1, BACKGROUND_COLOR);
    GFL_BGSysResetStdPalette(5, BACKGROUND_COLOR);
}


void func_ov181_021b9a80(StaffRollWork *work, BOOL flag) {
    ArcTool *arc;

    if (flag == TRUE) {
        arc = GFL_ArcSysCreateFileHandle(0x1a, 0x808e);
        GFL_G2DIOLoadArcNCLRDefault(arc, 2, 0, 0, 0, 0x8e);
    } else {
        arc = GFL_ArcSysCreateFileHandle(0xdc, 0x808e);
        GFL_G2DIOLoadArcNCLRDefault(arc, 0xc, 0, 0x1e0, 0x20, 0x8e);
    }
    GFL_ArcToolFree(arc);
    GFL_BGSysResetStdPalette(1, BACKGROUND_COLOR);
}



void func_ov181_021b9ad4(TCB *tcb, void *data) {
    func_ov181_021b9ddc(data);
    GFL_BGSysUpdate();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}




static void func_ov181_021b9af8(StaffRollWork *work) {
    work->vblankTask = GFL_VBlankTCBAdd(func_ov181_021b9ad4, work, 0);
}



static void func_ov181_021b9b10(StaffRollWork *work) {
    GFL_TCBRemove(work->vblankTask);
}

void func_ov181_021b9b1c(StaffRollWork *work) {
    work->msgData = GFL_MsgSysLoadData(0, 2, 0xb1, 0x8e);
    work->strbuf = GFL_StrBufCreate(0x80, 0x8e);
    work->fonts[0] = GFL_FontCreate(0x17, 0, 1, FALSE, 0x8e);
    work->fonts[1] = GFL_FontCreate(0x17, 2, 1, FALSE, 0x8e);
}


void func_ov181_021b9b6c(StaffRollWork *work) {
    GFL_FontFree(work->fonts[1]);
    GFL_FontFree(work->fonts[0]);
    GFL_StrBufFree(work->strbuf);
    GFL_MsgDataFree(work->msgData);
}

// A bitmap for each pair of lines that scroll together, after the background is shown
void func_ov181_021b9b98(StaffRollWork *work) {
    StaffRollLine *line;
    BOOL started = FALSE;
    u16 i = 0;

    while (TRUE) {
        line = &work->lines[i];
        if (!started) {
            if (line->command == CMD_BG_ON) {
                started = TRUE;
            }
        } else {
            if (line->command == CMD_END) {
                return;
            }
            if (line->command == CMD_LINE && line->msgId != 0xffff) {
                line = &work->lines[i + 1];
                if (line->wait == 0 && line->command == CMD_LINE && line->msgId != 0xffff) {
                    work->lineBitmaps[work->lineBitmapCount].bitmap = GFL_BitmapCreate(32, 2, 32, 0x8e);
                    work->lineBitmaps[work->lineBitmapCount].line = i;
                    work->lineBitmapCount++;
                    if (work->lineBitmapCount == LINE_BITMAP_MAX) {
                        return;
                    }
                    i++;
                }
            }
        }
        i++;
    }
}



void func_ov181_021b9c30(StaffRollWork *work) {
    u32 i;

    for (i = 0; i < work->lineBitmapCount; i++) {
        GFL_BitmapFree(work->lineBitmaps[i].bitmap);
    }
}

void func_ov181_021b9c58(StaffRollWork *work, GFLBitmap *bitmap, u32 index) {
    StaffRollLine *line = &work->lines[index];

    if (line->font == 0) {
        func_ov181_021ba4ac(work, line, bitmap, 0);
    } else {
        func_ov181_021ba4ac(work, line, bitmap, 2);
    }
}

void func_ov181_021b9c80(StaffRollWork *work) {
    if (work->drawnLineBitmaps < work->lineBitmapCount) {
        func_ov181_021b9c58(work, work->lineBitmaps[work->drawnLineBitmaps].bitmap,
                            work->lineBitmaps[work->drawnLineBitmaps].line);
        func_ov181_021b9c58(work, work->lineBitmaps[work->drawnLineBitmaps].bitmap,
                            work->lineBitmaps[work->drawnLineBitmaps].line + 1);
        work->drawnLineBitmaps++;
    }
}


void func_ov181_021b9cc8(StaffRollWork *work) {
    GFL_BitmapCopyArea(work->lineBitmaps[work->shownLineBitmaps].bitmap, BmpWin_GetBitmap(work->bottomWindow), 0, 0, 0,
                       192, 256, 16, 0);
}




static void func_ov181_021b9d04(StaffRollWork *work) {
    work->lines = GFL_ArcSysReadHeapNew(220, 1, 0x8e);
    func_ov181_021b9b98(work);
}

static void func_ov181_021b9d20(StaffRollWork *work) {
    func_ov181_021b9c30(work);
    GFL_HeapFree(work->lines);
}

void func_ov181_021b9d34(StaffRollWork *work) {
    BmpWin *window;

    BmpWin_InitAllocator(0x8e);
    work->topWindow = BmpWin_CreateDynamic(1, 0, 0, 0x20, 0x19, 0xf, 1);
    work->bottomWindow = BmpWin_CreateDynamic(5, 0, 0, 0x20, 0x1c, 0xf, 1);
    window = work->topWindow;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    window = work->bottomWindow;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
}


void func_ov181_021b9db0(StaffRollWork *work) {
    BmpWin_Free(work->topWindow);
    BmpWin_Free(work->bottomWindow);
    BmpWin_FreeAllocator();
}


void func_ov181_021b9dcc(StaffRollWork *work, u32 screens) {
    u8 s = screens;
    work->flushScreens |= s;
}

void func_ov181_021b9ddc(StaffRollWork *work) {
    if (work->flushScreens & SCREEN_TOP) {
        BmpWin_FlushChar(work->topWindow);
    }
    if (work->flushScreens & SCREEN_BOTTOM) {
        BmpWin_FlushChar(work->bottomWindow);
    }
    work->flushScreens = 0;
}

void func_ov181_021b9e14(StaffRollWork *work) {
    u32 i = 0;

    GFL_G3DSysCreate(FALSE, 2, FALSE, 2, 0x1000, 0x8e, NULL);
    work->g3dManager = GFL_G3DMgrCreate(3, 1, 0x8e);
    work->scene = GFL_G3DMgrNewScene(work->g3dManager, &data_ov181_021bab50);
    work->actor = GFL_G3DMgrGetActor(work->g3dManager, work->scene);
    work->actorAnimCount = GFL_G3DActorGetAnmCount(work->actor);
    for (; i < work->actorAnimCount; i++) {
        GFL_G3DActorBindAnm(work->actor, i);
    }
    work->g3dLight = GFL_G3DLightCreate(&data_ov181_021bab38, 0x8e);
    GFL_G3DLightFlush(work->g3dLight);
    {
        VecFx32 up = { 0, FX32_ONE, 0 };

        work->g3dCamera = GFL_G3DCameraCreate(0, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)), 0x1555, 0, 0x1000, 0x400000, 0,
                                              &data_ov181_021bace4[0].pos, &up, &data_ov181_021bace4[0].target, 0x8e);
    }
    GFL_G3DCameraFlush(work->g3dCamera);
    G3X_AntiAlias(TRUE);
    GFL_BGSysSet3DBGPriority(2);
    func_ov181_021b9f9c(&work->camera);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, FADE_BRIGHTNESS(-16));
}




void func_ov181_021b9f1c(StaffRollWork *work) {
    GFL_G3DCameraFree(work->g3dCamera);
    GFL_G3DLightFree(work->g3dLight);
    GFL_G3DMgrDeleteScene(work->g3dManager, work->scene);
    GFL_G3DMgrFree(work->g3dManager);
    GFL_G3DSysFree();
}

// Draws the 3D background
void func_ov181_021b9f40(StaffRollWork *work) {
    u32 i;

    if (work->param->skippable != TRUE && (GFL_BGSysGetEnabledBGsA() & GX_PLANEMASK_BG0)) {
        func_ov181_021ba2d8(work);
        func_ov181_021ba128(work);
        for (i = 0; i < work->actorAnimCount; i++) {
            GFL_G3DActorStepAnmFrameLoop(work->actor, i, FX32_ONE);
        }
        GFL_G3DSysReset();
        GFL_G3DSysMtxViewFlush();
        GFL_G3DSysDrawObj(work->actor, &data_ov181_021bac2c);
        GFL_G3DSysReqSwapBuffers();
    }
}





void func_ov181_021b9f9c(StaffRollCamera *camera) {
    u32 i;

    camera->keys = data_ov181_021bace4;
    camera->unk_74 = 7;
    camera->frame = 0;
    camera->current = 0;
    camera->active = FALSE;
    for (i = 0; i < 16; i++) {
        camera->queue[i].key = 0xffff;
        camera->queue[i].frames = 0;
    }
}

// Queues a move of the camera to a key
void func_ov181_021b9fd8(StaffRollCamera *camera, u16 key, u16 frames) {
    u32 i;

    for (i = 0; i < 16; i++) {
        if (camera->queue[i].key == 0xffff) {
            camera->queue[i].key = key;
            camera->queue[i].frames = frames;
            return;
        }
    }
}

// Drops the first move of the queue
void func_ov181_021ba000(StaffRollCamera *camera) {
    u32 i;

    for (i = 0; i < 15; i++) {
        camera->queue[i].key = camera->queue[i + 1].key;
        camera->queue[i].frames = camera->queue[i + 1].frames;
    }
    camera->queue[15].key = 0xffff;
    camera->queue[15].frames = 0;
}

// How far a point moves each frame to reach another in frames, in 1/256ths of a unit
void func_ov181_021ba028(VecFx32 *step, const VecFx32 *from, const VecFx32 *to, u32 frames) {
    VecFx32 result;

    result.x = (MATH_ABS(from->x - to->x) << 8) / frames;
    if (from->x > to->x) {
        result.x *= -1;
    }
    result.y = (MATH_ABS(from->y - to->y) << 8) / frames;
    if (from->y > to->y) {
        result.y *= -1;
    }
    result.z = (MATH_ABS(from->z - to->z) << 8) / frames;
    if (from->z > to->z) {
        result.z *= -1;
    }
    *step = result;
}

// Where a point is after frames of its step
void func_ov181_021ba0ac(VecFx32 *pos, const VecFx32 *from, const VecFx32 *step, u32 frame) {
    VecFx32 result = *from;
    u32 distance;

    distance = (MATH_ABS(step->x) * frame) >> 8;
    if (step->x > 0) {
        result.x += distance;
    } else {
        result.x -= distance;
    }
    distance = (MATH_ABS(step->y) * frame) >> 8;
    if (step->y > 0) {
        result.y += distance;
    } else {
        result.y -= distance;
    }
    distance = (MATH_ABS(step->z) * frame) >> 8;
    if (step->z > 0) {
        result.z += distance;
    } else {
        result.z -= distance;
    }
    *pos = result;
}

// Moves the camera along its queue of keys
void func_ov181_021ba128(StaffRollWork *work) {
    StaffRollCamera *camera = &work->camera;
    VecFx32 current;
    VecFx32 posStep;
    VecFx32 targetStep;
    VecFx32 pos;
    VecFx32 target;

    if (!camera->active) {
        return;
    }
    if (camera->frame == 0) {
        camera->frames = camera->queue[0].frames;
        GFL_G3DCameraGetLookatPos(work->g3dCamera, &current);
        func_ov181_021ba028(&posStep, &current, &camera->keys[camera->queue[0].key].pos, camera->frames);
        camera->posStep = posStep;
        GFL_G3DCameraGetLookatTarget(work->g3dCamera, &current);
        func_ov181_021ba028(&targetStep, &current, &camera->keys[camera->queue[0].key].target, camera->frames);
        camera->targetStep = targetStep;
    }
    if (camera->frame >= camera->frames) {
        camera->pos = camera->keys[camera->queue[0].key].pos;
        camera->target = camera->keys[camera->queue[0].key].target;
        camera->current = camera->queue[0].key;
        camera->frame = 0;
        func_ov181_021ba000(camera);
        if (camera->queue[0].key == 0xffff) {
            func_ov181_021ba2a8(camera, FALSE);
        }
    } else {
        func_ov181_021ba0ac(&pos, &camera->keys[camera->current].pos, &camera->posStep, camera->frame);
        camera->pos = pos;
        func_ov181_021ba0ac(&target, &camera->keys[camera->current].target, &camera->targetStep, camera->frame);
        camera->target = target;
        if (work->fastForward == TRUE) {
            camera->frame += 4;
        } else {
            camera->frame++;
        }
    }
    GFL_G3DCameraSetLookatPos(work->g3dCamera, &camera->pos);
    GFL_G3DCameraSetLookatTarget(work->g3dCamera, &camera->target);
    GFL_G3DCameraFlush(work->g3dCamera);
}







static void func_ov181_021ba2a8(StaffRollCamera *camera, BOOL active) {
    camera->active = active;
}

u32 func_ov181_021ba2ac(StaffRollWork *work, u32 nextSeq) {
    GFL_WipeSet(0, 0, 0, WIPE_COLOR, 6, 1, 0x8e);
    work->nextSeq = nextSeq;
    return SEQ_WAIT_WIPE;
}

// Brightens the 3D background from the credits' color
void func_ov181_021ba2d8(StaffRollWork *work) {
    if (work->backgroundBlend <= 16 * FX32_ONE) {
        if (work->fastForward == TRUE) {
            work->backgroundBlend += FX32_ONE / 4;
        } else {
            work->backgroundBlend += FX32_ONE / 16;
        }
#if GAME_VERSION == VERSION_WHITE
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, 16 - work->backgroundBlend / FX32_ONE);
#else
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, work->backgroundBlend / FX32_ONE - 16);
#endif
    }
}

// Fast-forwards while A or B is held, checked every fourth frame
void func_ov181_021ba318(StaffRollWork *work) {
    if (work->param->skippable) {
        if (work->fastForwardTimer == 0) {
            if (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                work->fastForward = TRUE;
            } else {
                work->fastForward = FALSE;
            }
        }
        work->fastForwardTimer = (work->fastForwardTimer + 1) & 3;
    }
}










static void StaffRoll_InitVRAM(void) {
    GFL_BGSysInitVRAM(0);
    GFL_BGSysSetVRAMBanks(&data_ov181_021babfc);
}

static void StaffRoll_FreeBG(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysFree();
}

static void StaffRoll_FreeVBlank(StaffRollWork *work) {
    GFL_TCBRemove(work->vblankTask);
}

static void StaffRoll_FreeMsg(StaffRollWork *work) {
    GFL_FontFree(work->fonts[1]);
    GFL_FontFree(work->fonts[0]);
    GFL_StrBufFree(work->strbuf);
    GFL_MsgDataFree(work->msgData);
}

static inline void StaffRoll_DrawLineBitmap(StaffRollWork *work, GFLBitmap *bitmap, u32 index) {
    StaffRollLine *line = &work->lines[index];

    if (line->font == 0) {
        StaffRoll_DrawLine(work, line, bitmap, 0);
    } else {
        StaffRoll_DrawLine(work, line, bitmap, 2);
    }
}

// Draws one pair of lines a frame, ahead of their turn
static inline void StaffRoll_DrawNextLineBitmap(StaffRollWork *work) {
    if (work->drawnLineBitmaps < work->lineBitmapCount) {
        StaffRoll_DrawLineBitmap(work, work->lineBitmaps[work->drawnLineBitmaps].bitmap,
                                 work->lineBitmaps[work->drawnLineBitmaps].line);
        StaffRoll_DrawLineBitmap(work, work->lineBitmaps[work->drawnLineBitmaps].bitmap,
                                 work->lineBitmaps[work->drawnLineBitmaps].line + 1);
        work->drawnLineBitmaps++;
    }
}

// Copies the next pair of lines below the bottom screen, from where it scrolls up
static inline void StaffRoll_ShowLineBitmap(StaffRollWork *work) {
    GFL_BitmapCopyArea(work->lineBitmaps[work->shownLineBitmaps].bitmap, BmpWin_GetBitmap(work->bottomWindow), 0, 0, 0,
                       192, 256, 16, 0);
}

static void StaffRoll_FreeWindows(StaffRollWork *work) {
    BmpWin_Free(work->topWindow);
    BmpWin_Free(work->bottomWindow);
    func_ov014_02177704();
}

static inline void StaffRoll_RequestFlush(StaffRollWork *work, u32 screens) {
    work->flushScreens |= screens;
}

// Moves the rows of a 16 color tile up, the top rows into the bottom of the tile above
// pokebw2's StaffRoll_ScrollTile
void func_ov181_021ba354(u32 *tile, u32 *above, u32 rows) {
    u32 i;

    if (above != NULL) {
        for (i = 0; i < rows; i++) {
            above[8 - rows + i] = tile[i];
        }
    }
    for (i = 0; i < 8 - rows; i++) {
        tile[i] = tile[rows + i];
    }
}

void func_ov181_021ba390(StaffRollWork *work) {
    u8 *top;
    u8 *bottom;
    u32 rows;
    u32 dst;
    u32 src;
    u16 i;
    u16 y;
    u16 y2;

    if (work->scrolling) {
        work->scrollFraction += FX32_ONE;
        if (work->scrollFraction >= FX32_ONE) {
            work->scrollFraction -= FX32_ONE;
            rows = 1;
            if (work->fastForward) {
                rows = 4;
            }
            top = GFL_BitmapGetPixelData(BmpWin_GetBitmap(work->topWindow));
            bottom = GFL_BitmapGetPixelData(BmpWin_GetBitmap(work->bottomWindow));
            dst = 0;
            for (i = 0; i < 0x20; i++) {
                func_ov181_021ba354((u32 *)(top + dst), NULL, rows);
                dst += 0x20;
            }
            for (y = 1; y < 0x19; y++) {
                dst = y << 10;
                src = (y - 1) << 10;
                for (i = 0; i < 0x20; i++) {
                    func_ov181_021ba354((u32 *)(top + dst), (u32 *)(top + src), rows);
                    dst += 0x20;
                    src += 0x20;
                }
            }
            dst = 0;
            src = 0x6000;
            for (i = 0; i < 0x20; i++) {
                func_ov181_021ba354((u32 *)(bottom + dst), (u32 *)(top + src), rows);
                dst += 0x20;
                src += 0x20;
            }
            for (y2 = 1; y2 < 0x1c; y2++) {
                dst = y2 << 10;
                src = (y2 - 1) << 10;
                for (i = 0; i < 0x20; i++) {
                    func_ov181_021ba354((u32 *)(bottom + dst), (u32 *)(bottom + src), rows);
                    dst += 0x20;
                    src += 0x20;
                }
            }
            func_ov181_021b9dcc(work, 3);
        }
    }
}


void func_ov181_021ba4ac(StaffRollWork *work, StaffRollLine *line, GFLBitmap *bitmap, int y) {
    int x;

    GFL_MsgDataLoadStrbuf(work->msgData, line->msgId, work->strbuf);
    if (line->align == 0) {
        x = line->x + line->offset;
    } else if (line->align == 1) {
        x = line->x + line->offset - GFL_FontGetBlockWidth(work->strbuf, work->fonts[line->font], 0);
    } else if (line->align == 2) {
        x = line->x + line->offset - GFL_FontGetBlockWidth(work->strbuf, work->fonts[line->font], 0) / 2;
    }
    GFL_TextRendererDrawToBitmapEx(bitmap, x, y, work->strbuf, work->fonts[line->font], data_ov181_021bab40[line->color]);
}


void func_ov181_021ba558(StaffRollWork *work, StaffRollLine *line) {
    if (line->font == 0) {
        func_ov181_021ba4ac(work, line, BmpWin_GetBitmap(work->bottomWindow), 192);
    } else {
        func_ov181_021ba4ac(work, line, BmpWin_GetBitmap(work->bottomWindow), 194);
    }
}

void func_ov181_021ba594(StaffRollWork *work, StaffRollLine *line) {
    GFLBitmap *bitmap;
    int y;

    if (line->wait < 0x100) {
        bitmap = BmpWin_GetBitmap(work->topWindow);
        y = line->wait;
        work->fadeScreens |= SCREEN_TOP;
    } else {
        bitmap = BmpWin_GetBitmap(work->bottomWindow);
        y = line->wait - 0x100;
        work->fadeScreens |= SCREEN_BOTTOM;
    }
    func_ov181_021ba4ac(work, line, bitmap, y);
}

BOOL func_ov181_021ba5e8(StaffRollWork *work) {
    int eva, evb;

    switch (work->fadeState) {
    case 0:
        if (work->fadeScreens & SCREEN_TOP) {
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 0, 16);
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 0, 16);
        }
        work->flushScreens = work->fadeScreens;
        work->fadeState++;
        break;
    case 1:
        if (work->fastForward == TRUE) {
            work->fadeAlpha += 16;
        } else {
            work->fadeAlpha += 4;
        }
        if (work->fadeAlpha > 16) {
            eva = 16;
            evb = 0;
        } else {
            evb = 16 - work->fadeAlpha;
            eva = work->fadeAlpha;
        }
        if (work->fadeScreens & SCREEN_TOP) {
            G2_ChangeBlendAlpha(eva, evb);
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_ChangeBlendAlpha(eva, evb);
        }
        if (work->fadeAlpha >= 16) {
            if (work->fadeScreens & SCREEN_TOP) {
                G2_BlendNone();
            }
            if (work->fadeScreens & SCREEN_BOTTOM) {
                G2S_BlendNone();
            }
            work->fadeAlpha = 0;
            work->fadeState = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL func_ov181_021ba6f0(StaffRollWork *work) {
    int eva, evb;

    switch (work->fadeState) {
    case 0:
        if (work->fadeScreens & SCREEN_TOP) {
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 16, 0);
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 16, 0);
        }
        work->fadeState++;
    case 1:
        if (work->fastForward == TRUE) {
            work->fadeAlpha += 16;
        } else {
            work->fadeAlpha += 4;
        }
        if (work->fadeAlpha > 16) {
            eva = 0;
            evb = 16;
        } else {
            eva = 16 - work->fadeAlpha;
            evb = work->fadeAlpha;
        }
        if (work->fadeScreens & SCREEN_TOP) {
            G2_ChangeBlendAlpha(eva, evb);
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_ChangeBlendAlpha(eva, evb);
        }
        if (work->fadeAlpha >= 16) {
            if (work->fadeScreens & SCREEN_TOP) {
                GFL_BitmapFill(BmpWin_GetBitmap(work->topWindow), 0);
            }
            if (work->fadeScreens & SCREEN_BOTTOM) {
                GFL_BitmapFill(BmpWin_GetBitmap(work->bottomWindow), 0);
            }
            work->flushScreens = work->fadeScreens;
            work->fadeAlpha = 0;
            work->fadeState++;
        }
        break;
    case 2:
        if (work->fadeScreens & SCREEN_TOP) {
            G2_BlendNone();
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_BlendNone();
        }
        work->fadeScreens = 0;
        work->fadeState = 0;
        return FALSE;
    }
    return TRUE;
}



u32 func_ov181_021ba83c(StaffRollWork *work) {
    MtxFx22 mtx;
    s32 scale;

    switch (work->fadeState) {
    case 0:
        func_ov181_021b9a80(work, 1);
        G2_SetBlendAlpha(8, 0x20, 0, 0x10);
        GFL_BGSysSetBGEnabledA(8, 1);
        GX_SetMasterBrightness(0);
        GXS_SetMasterBrightness(0);
        work->fadeAlpha = 0;
        work->fadeState++;
        break;
    case 1:
        if (work->fastForward == 1) {
            work->fadeAlpha += 8;
        } else {
            work->fadeAlpha += 2;
        }
        if (work->fadeAlpha >= 0x10) {
            reg_G2_BLDALPHA = 0x10;
            MAT2_SetScaleRot(&mtx, 0, 0x1000, 0x1000, 0);
            GFL_BGSysSetBGTransform(3, &mtx, 0x80, 0x60);
            work->fadeAlpha = 0;
            work->waitFrames = 0xa6;
            work->fadeState++;
            break;
        }
        reg_G2_BLDALPHA = work->fadeAlpha | ((0x10 - work->fadeAlpha) << 8);
        scale = (8 - work->fadeAlpha / 2) << 12;
        MAT2_SetScaleRot(&mtx, 0, scale, scale, 0);
        GFL_BGSysSetBGTransform(3, &mtx, 0x80, 0x60);
        break;
    case 2:
        if (work->fastForward == 1) {
            work->waitFrames -= 4;
        } else {
            work->waitFrames -= 1;
        }
        if (work->waitFrames <= 0) {
            work->waitFrames = 0;
            work->fadeState++;
        }
        break;
    case 3:
        if (work->fastForward == 1) {
            work->fadeAlpha += 8;
        } else {
            work->fadeAlpha += 2;
        }
        if (work->fadeAlpha >= 0x10) {
            reg_G2_BLDCNT = 0;
            GFL_BGSysSetBGEnabledA(8, 0);
            MAT2_SetScaleRot(&mtx, 0, 0x1000, 0x1000, 0);
            GFL_BGSysSetBGTransform(3, &mtx, 0x80, 0x60);
            func_ov181_021b9a80(work, 0);
            work->fadeAlpha = 0;
            work->fadeState = 0;
            return 0;
        }
        reg_G2_BLDALPHA = ((0x10 - work->fadeAlpha)) | (work->fadeAlpha << 8);
        scale = (work->fadeAlpha / 2 + 1) << 12;
        MAT2_SetScaleRot(&mtx, 0, scale, scale, 0);
        GFL_BGSysSetBGTransform(3, &mtx, 0x80, 0x60);
        break;
    }
    return 1;
}

BOOL func_ov181_021baa00(StaffRollWork *work, StaffRollLine *line) {
    if (line->msgId == 0xffff) {
        work->waitFrames = line->wait;
        work->state = ROLL_WAIT;
        return TRUE;
    }
    if (work->waitFrames == 0) {
        work->waitFrames = line->wait;
    }
    func_ov181_021ba558(work, line);
    if (line[1].command != CMD_LINE || line[1].wait != 0) {
        work->state = ROLL_WAIT;
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov181_021baa54(StaffRollWork *work, StaffRollLine *line) {
    if (work->state == ROLL_READ) {
        work->unk231 = line->arg;
        work->state = ROLL_FADE_IN;
    }
    func_ov181_021ba594(work, line);
    if (line[1].command != CMD_FADE_IN) {
        return TRUE;
    }
    return FALSE;
}








BOOL func_ov181_021baa84(StaffRollWork *work, StaffRollLine *line) {
    work->unk231 = line->arg;
    work->state = ROLL_FADE_OUT;
    return TRUE;
}


BOOL func_ov181_021baa98(StaffRollWork *work, StaffRollLine *line) {
    work->scrolling = TRUE;
    return FALSE;
}

BOOL func_ov181_021baaa4(StaffRollWork *work, StaffRollLine *line) {
    work->scrolling = FALSE;
    return FALSE;
}

BOOL func_ov181_021baab0(StaffRollWork *work, StaffRollLine *line) {
    work->state = ROLL_END;
    return TRUE;
}




BOOL func_ov181_021baabc(StaffRollWork *work, StaffRollLine *line) {
    return TRUE;
}

// Shows the 3D background
BOOL func_ov181_021baac0(StaffRollWork *work, StaffRollLine *line) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    return TRUE;
}

BOOL func_ov181_021baad0(StaffRollWork *work, StaffRollLine *line) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, FALSE);
    return TRUE;
}

BOOL func_ov181_021baae0(StaffRollWork *work, StaffRollLine *line) {
    StaffRollCamera *camera = &work->camera;

    func_ov181_021b9fd8(camera, line->arg, line->wait);
    func_ov181_021ba2a8(camera, TRUE);
    return TRUE;
}

BOOL func_ov181_021bab00(StaffRollWork *work, StaffRollLine *line) {
#if GAME_VERSION == VERSION_WHITE
    line += line->arg + 1;
#else
    line += line->arg;
#endif
    func_ov181_021ba558(work, line);
    work->waitFrames = line->wait;
    work->state = ROLL_WAIT;
    return TRUE;
}






// Scrolls the text up, from the bottom window into the top one
static inline void StaffRoll_Scroll(StaffRollWork *work) {
    u8 *top;
    u8 *bottom;
    u32 rows;
    u32 offset;
    u32 aboveOffset;
    u16 x;
    u16 y;

    if (work->scrolling) {
        work->scrollFraction += FX32_ONE;
        if (work->scrollFraction >= FX32_ONE) {
            work->scrollFraction -= FX32_ONE;
            rows = 1;
            if (work->fastForward) {
                rows = 4;
            }
            top = GFL_BitmapGetPixelData(BmpWin_GetBitmap(work->topWindow));
            bottom = GFL_BitmapGetPixelData(BmpWin_GetBitmap(work->bottomWindow));
            offset = 0;
            for (x = 0; x < 32; x++) {
                func_ov181_021ba354((u32 *)(top + offset), NULL, rows);
                offset += 0x20;
            }
            for (y = 1; y < 25; y++) {
                offset = y * 0x400;
                aboveOffset = (y - 1) * 0x400;
                for (x = 0; x < 32; x++) {
                    func_ov181_021ba354((u32 *)(top + offset), (u32 *)(top + aboveOffset), rows);
                    offset += 0x20;
                    aboveOffset += 0x20;
                }
            }
            offset = 0;
            aboveOffset = 24 * 0x400;
            for (x = 0; x < 32; x++) {
                func_ov181_021ba354((u32 *)(bottom + offset), (u32 *)(top + aboveOffset), rows);
                offset += 0x20;
                aboveOffset += 0x20;
            }
            for (y = 1; y < 28; y++) {
                offset = y * 0x400;
                aboveOffset = (y - 1) * 0x400;
                for (x = 0; x < 32; x++) {
                    func_ov181_021ba354((u32 *)(bottom + offset), (u32 *)(bottom + aboveOffset), rows);
                    offset += 0x20;
                    aboveOffset += 0x20;
                }
            }
            StaffRoll_RequestFlush(work, SCREEN_TOP | SCREEN_BOTTOM);
        }
    }
}

static inline void StaffRoll_DrawLine(StaffRollWork *work, StaffRollLine *line, GFLBitmap *bitmap, int y) {
    int x;

    GFL_MsgDataLoadStrbuf(work->msgData, line->msgId, work->strbuf);
    if (line->align == ALIGN_LEFT) {
        x = line->x + line->offset;
    } else if (line->align == ALIGN_RIGHT) {
        u32 width = GFL_FontGetBlockWidth(work->strbuf, work->fonts[line->font], 0);

        x = line->x + line->offset - width;
    } else if (line->align == ALIGN_CENTER) {
        // Any other alignment leaves x unset; the original tests the third value too (an else is 8 bytes shorter)
        u32 width = GFL_FontGetBlockWidth(work->strbuf, work->fonts[line->font], 0);

        x = line->x + line->offset - width / 2;
    }
    GFL_TextRendererDrawToBitmapEx(bitmap, x, y, work->strbuf, work->fonts[line->font], data_ov181_021bab40[line->color]);
}

// Draws a line below the bottom screen
static inline void StaffRoll_DrawLineBelow(StaffRollWork *work, StaffRollLine *line) {
    if (line->font == 0) {
        StaffRoll_DrawLine(work, line, BmpWin_GetBitmap(work->bottomWindow), 192);
    } else {
        StaffRoll_DrawLine(work, line, BmpWin_GetBitmap(work->bottomWindow), 194);
    }
}

// Draws a title at its y, from the top of the top screen to the bottom of the bottom one
static inline void StaffRoll_DrawTitle(StaffRollWork *work, StaffRollLine *line) {
    GFLBitmap *bitmap;
    int y;

    if (line->wait < 256) {
        bitmap = BmpWin_GetBitmap(work->topWindow);
        y = line->wait;
        work->fadeScreens |= SCREEN_TOP;
    } else {
        bitmap = BmpWin_GetBitmap(work->bottomWindow);
        y = line->wait - 256;
        work->fadeScreens |= SCREEN_BOTTOM;
    }
    StaffRoll_DrawLine(work, line, bitmap, y);
}

// Fades the titles in, and returns FALSE once done
static inline BOOL StaffRoll_FadeIn(StaffRollWork *work) {
    int eva, evb;

    switch (work->fadeState) {
    case 0:
        if (work->fadeScreens & SCREEN_TOP) {
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 0, 16);
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 0, 16);
        }
        work->flushScreens = work->fadeScreens;
        work->fadeState++;
        break;
    case 1:
        if (work->fastForward == TRUE) {
            work->fadeAlpha += 16;
        } else {
            work->fadeAlpha += 4;
        }
        if (work->fadeAlpha > 16) {
            eva = 16;
            evb = 0;
        } else {
            evb = 16 - work->fadeAlpha;
            eva = work->fadeAlpha;
        }
        if (work->fadeScreens & SCREEN_TOP) {
            G2_ChangeBlendAlpha(eva, evb);
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_ChangeBlendAlpha(eva, evb);
        }
        if (work->fadeAlpha >= 16) {
            if (work->fadeScreens & SCREEN_TOP) {
                G2_BlendNone();
            }
            if (work->fadeScreens & SCREEN_BOTTOM) {
                G2S_BlendNone();
            }
            work->fadeAlpha = 0;
            work->fadeState = 0;
            return FALSE;
        }
        break;
    }
    return TRUE;
}

// Fades the titles out and clears them, and returns FALSE once done
static inline BOOL StaffRoll_FadeOut(StaffRollWork *work) {
    int eva, evb;

    switch (work->fadeState) {
    case 0:
        if (work->fadeScreens & SCREEN_TOP) {
            G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 16, 0);
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BD, 16, 0);
        }
        work->fadeState++;
    case 1:
        if (work->fastForward == TRUE) {
            work->fadeAlpha += 16;
        } else {
            work->fadeAlpha += 4;
        }
        if (work->fadeAlpha > 16) {
            eva = 0;
            evb = 16;
        } else {
            eva = 16 - work->fadeAlpha;
            evb = work->fadeAlpha;
        }
        if (work->fadeScreens & SCREEN_TOP) {
            G2_ChangeBlendAlpha(eva, evb);
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_ChangeBlendAlpha(eva, evb);
        }
        if (work->fadeAlpha >= 16) {
            if (work->fadeScreens & SCREEN_TOP) {
                GFL_BitmapFill(BmpWin_GetBitmap(work->topWindow), 0);
            }
            if (work->fadeScreens & SCREEN_BOTTOM) {
                GFL_BitmapFill(BmpWin_GetBitmap(work->bottomWindow), 0);
            }
            work->flushScreens = work->fadeScreens;
            work->fadeAlpha = 0;
            work->fadeState++;
        }
        break;
    case 2:
        if (work->fadeScreens & SCREEN_TOP) {
            G2_BlendNone();
        }
        if (work->fadeScreens & SCREEN_BOTTOM) {
            G2S_BlendNone();
        }
        work->fadeScreens = 0;
        work->fadeState = 0;
        return FALSE;
    }
    return TRUE;
}

// A line that scrolls alone, or the end of a block when its message is 0xffff
static inline BOOL StaffRollCmd_Line(StaffRollWork *work, StaffRollLine *line) {
    if (line->msgId == 0xffff) {
        work->waitFrames = line->wait;
        work->state = ROLL_WAIT;
        return TRUE;
    }
    if (work->waitFrames == 0) {
        work->waitFrames = line->wait;
    }
    StaffRoll_DrawLineBelow(work, line);
    if (line[1].command != CMD_LINE || line[1].wait != 0) {
        work->state = ROLL_WAIT;
        return TRUE;
    }
    return FALSE;
}

// A title, which fades in with the titles after it
static inline BOOL StaffRollCmd_FadeIn(StaffRollWork *work, StaffRollLine *line) {
    if (work->state == ROLL_READ) {
        work->unk231 = line->arg;
        work->state = ROLL_FADE_IN;
    }
    StaffRoll_DrawTitle(work, line);
    if (line[1].command != CMD_FADE_IN) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL StaffRollCmd_FadeOut(StaffRollWork *work, StaffRollLine *line) {
    work->unk231 = line->arg;
    work->state = ROLL_FADE_OUT;
    return TRUE;
}

static inline BOOL StaffRollCmd_ScrollStart(StaffRollWork *work, StaffRollLine *line) {
    work->scrolling = TRUE;
    return FALSE;
}

static inline BOOL StaffRollCmd_ScrollStop(StaffRollWork *work, StaffRollLine *line) {
    work->scrolling = FALSE;
    return FALSE;
}

static inline BOOL StaffRollCmd_End(StaffRollWork *work, StaffRollLine *line) {
    work->state = ROLL_END;
    return TRUE;
}

static inline BOOL StaffRollCmd_Yield(StaffRollWork *work, StaffRollLine *line) {
    return TRUE;
}

static inline BOOL StaffRollCmd_LineAt(StaffRollWork *work, StaffRollLine *line) {
#ifdef BLACK2
    line += line->arg;
#else
    line += line->arg + 1;
#endif
    StaffRoll_DrawLineBelow(work, line);
    work->waitFrames = line->wait;
    work->state = ROLL_WAIT;
    return TRUE;
}

