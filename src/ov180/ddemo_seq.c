// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov265/ddemo_seq.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "demo/ddemo_main.h"
#include "demo/ddemo_obj.h"
#include "demo/ddemo_seq.h"
#include "demo/dendou_demo.h"
#include "gfl/bg_sys.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/key.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "system/mcss.h"
#include "system/wipe.h"

// The Hall of Fame entry demo's sequences: the set-up, the first scene, which shows the team's Pokémon one by one,
// and the second, which shows the trainer with the team until a button is pressed (overlay 265). The file's name is
// a guess, after the Hall of Fame PC's dpc_seq.c

// The sequences, which return the one to run next
enum {
    SEQ_INIT,
    SEQ_RELEASE,
    SEQ_WIPE,
    SEQ_WAIT,
    SEQ_1ST_INIT,
    SEQ_1ST_MAIN,
    SEQ_1ST_EXIT,
    SEQ_2ND_INIT,
    SEQ_2ND_MAIN,
    SEQ_2ND_EXIT,
    SEQ_END,
};

typedef int (*DDemoSeqFunction)(DDemoWork *wk);

static int func_ov180_021b6a60(DDemoWork *wk);
static int func_ov180_021b6ab0(DDemoWork *wk);
static int func_ov180_021b6afc(DDemoWork *wk);
static int func_ov180_021b6b18(DDemoWork *wk);
static int func_ov180_021b6b34(DDemoWork *wk);
static int func_ov180_021b6b70(DDemoWork *wk);
static int func_ov180_021b6bac(DDemoWork *wk);
static int func_ov180_021b6f34(DDemoWork *wk);
static int func_ov180_021b6f80(DDemoWork *wk);
static int func_ov180_021b6fac(DDemoWork *wk);
static int func_ov180_021b7210(DDemoWork *wk, int next);
static int func_ov180_021b723c(DDemoWork *wk, int next, int division);
static int func_ov180_021b7268(DDemoWork *wk, int wait);
static void func_ov180_021b727c(DDemoWork *wk);
static BOOL func_ov180_021b72b0(DDemoWork *wk, u32 id);
static void func_ov180_021b7300(DDemoWork *wk);

// pokebw2's sDDemoSeqFunctions
static const DDemoSeqFunction data_ov180_021b814c[] = {
    func_ov180_021b6a60,    func_ov180_021b6ab0, func_ov180_021b6afc,    func_ov180_021b6b18,    func_ov180_021b6b34,
    func_ov180_021b6bac, func_ov180_021b6b70, func_ov180_021b6f34, func_ov180_021b6fac, func_ov180_021b6f80,
};

// pokebw2's DDemoSeq_Main
BOOL func_ov180_021b6a10(DDemoWork *wk) {
    wk->mainSeq = data_ov180_021b814c[wk->mainSeq](wk);
    if (wk->mainSeq == SEQ_END) {
        return FALSE;
    }
    if (wk->scene == DDEMO_SCENE_1ST) {
        func_ov180_021b7d5c(wk);
        func_ov180_021b6704(wk);
    } else if (wk->scene == DDEMO_SCENE_2ND) {
        func_ov180_021b7f30(wk);
        func_ov180_021b66e4(wk);
        func_ov180_021b7300(wk);
    }
    return TRUE;
}

// pokebw2's DDemoSeq_Init
static int func_ov180_021b6a60(DDemoWork *wk) {
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    func_ov180_021b6210();
    func_ov180_021b6418(wk);
    func_ov180_021b64ac(wk);
    func_ov180_021b64dc(wk);
    return SEQ_1ST_INIT;
}

// pokebw2's DDemoSeq_Release
static int func_ov180_021b6ab0(DDemoWork *wk) {
    func_ov180_021b64d0(wk);
    func_ov180_021b6474(wk);
    func_ov180_021b621c();
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    return SEQ_END;
}

// pokebw2's DDemoSeq_Wipe
static int func_ov180_021b6afc(DDemoWork *wk) {
    if (GFL_WipeIsFinished() == TRUE) {
        return wk->nextSeq;
    }
    return SEQ_WIPE;
}

// pokebw2's DDemoSeq_Wait
static int func_ov180_021b6b18(DDemoWork *wk) {
    if (wk->count == 0) {
        return wk->nextSeq;
    }
    wk->count--;
    return SEQ_WAIT;
}

// pokebw2's DDemoSeq_Init1st
static int func_ov180_021b6b34(DDemoWork *wk) {
    func_ov180_021b61d4(0);
    func_ov180_021b6224();
    func_ov180_021b66a0(wk);
    func_ov180_021b6764();
    func_ov180_021b6750();
    func_ov180_021b7318(wk, 0);
    func_ov180_021b7cf0(wk);
    func_ov180_021b63e8();
    func_ov180_021b6158(wk);
    wk->scene = DDEMO_SCENE_1ST;
    return SEQ_1ST_MAIN;
}

// pokebw2's DDemoSeq_Exit1st
static int func_ov180_021b6b70(DDemoWork *wk) {
    func_ov180_021b6188(wk);
    G2_BlendNone();
    G2S_BlendNone();
    func_ov180_021b7d44(wk);
    func_ov180_021b7390(wk);
    func_ov180_021b675c();
    func_ov180_021b6770();
    func_ov180_021b66d4(wk);
    wk->scene = DDEMO_SCENE_NONE;
    return SEQ_2ND_INIT;
}

// Shows a Pokémon: its sprites slide in from the left, the title and name plates from both sides, each shaking as
// it stops, then the Pokémon cries and all slide out again
// pokebw2's DDemoSeq_Main1st
static int func_ov180_021b6bac(DDemoWork *wk) {
    switch (wk->subSeq) {
    case 0:
        wk->subSeq++;
        return func_ov180_021b7268(wk, 32);
    case 1:
        GFL_SndSEPlay(SEQ_SE_DDEMO_02A);
        wk->subSeq++;
        break;
    case 2:
        wk->count++;
        if (wk->count == 48) {
            GFL_SndSEPlay(SEQ_SE_DDEMO_02B);
            wk->count = 0;
            wk->subSeq++;
        }
        break;
    case 3:
        if (GFL_SndIsPlaying(SEQ_SE_DDEMO_02B) == FALSE) {
            GFL_SndSEPlay(SEQ_SE_DDEMO_01);
            func_ov180_021b654c(wk);
            func_ov180_021b65a0(wk);
            func_ov180_021b6778(wk);
            func_ov180_021b67ec(wk);
            func_ov180_021b761c(wk);
            func_ov180_021b78e0(wk);
            func_ov180_021b7c08(wk);
            func_ov180_021b7750(wk);
            wk->subSeq++;
            return func_ov180_021b7210(wk, SEQ_1ST_MAIN);
        }
        break;
    case 4:
        if (wk->count == 22) {
            wk->subSeq++;
            return func_ov180_021b7268(wk, 8);
        }
        func_ov180_021b7438(wk, DDEMOOBJ_POKE_BACK, -16, 0);
        wk->count++;
        break;
    case 5:
        func_ov180_021b67c4(wk, 0);
        func_ov180_021b67c4(wk, 1);
        wk->subSeq++;
        return func_ov180_021b7268(wk, 8);
    case 6:
        func_ov180_021b682c(wk, 1);
        wk->subSeq++;
        // fallthrough
    case 7: {
        s16 x, y;

        func_ov180_021b7408(wk, DDEMOOBJ_PLATE_TITLE, &x, &y);
        if (x != 168) {
            func_ov180_021b7438(wk, DDEMOOBJ_PLATE_TITLE, 16, 0);
        } else {
            GFL_SndSEPlay(SEQ_SE_ROTATION_B);
            func_ov180_021b727c(wk);
            wk->shakeBaseX = x;
            wk->shakeBaseY = y;
            wk->subSeq++;
        }
        func_ov180_021b7c08(wk);
        break;
    }
    case 8:
        func_ov180_021b682c(wk, 0);
        wk->subSeq++;
        // fallthrough
    case 9: {
        BOOL shaking = func_ov180_021b72b0(wk, DDEMOOBJ_PLATE_TITLE);
        s16 x, y;

        func_ov180_021b7408(wk, DDEMOOBJ_PLATE_NAME, &x, &y);
        if (x != 88) {
            func_ov180_021b7438(wk, DDEMOOBJ_PLATE_NAME, -16, 0);
        } else if (shaking == FALSE) {
            GFL_SndSEPlay(SEQ_SE_ROTATION_B);
            func_ov180_021b727c(wk);
            wk->shakeBaseX = x;
            wk->shakeBaseY = y;
            wk->subSeq++;
        }
        func_ov180_021b7c08(wk);
        break;
    }
    case 10: {
        BOOL shaking = func_ov180_021b72b0(wk, DDEMOOBJ_PLATE_NAME);
        s16 x, y;

        func_ov180_021b7408(wk, DDEMOOBJ_POKE_FRONT, &x, &y);
        if (x != 48) {
            func_ov180_021b7438(wk, DDEMOOBJ_POKE_FRONT, 16, 0);
        } else if (shaking == FALSE) {
            GFL_SndSEPlay(SEQ_SE_ROTATION_B);
            func_ov180_021b727c(wk);
            wk->shakeBaseX = x;
            wk->shakeBaseY = y;
            wk->subSeq++;
        }
        func_ov180_021b7c08(wk);
        break;
    }
    case 11:
        if (func_ov180_021b72b0(wk, DDEMOOBJ_POKE_FRONT) == FALSE) {
            PokeVoice_StartPlayback(wk->voice);
            func_ov180_021b746c(wk, DDEMOOBJ_FLASH, TRUE);
            func_ov180_021b7488(wk, DDEMOOBJ_FLASH, 6);
            wk->subSeq++;
            return func_ov180_021b7268(wk, 128);
        }
        break;
    case 12:
        if (PokeVoice_IsPlaying(wk->voice) == FALSE && func_ov180_021b74b4(wk, DDEMOOBJ_FLASH) == FALSE) {
            wk->subSeq++;
        }
        break;
    case 13: {
        s16 x, y;

        func_ov180_021b7408(wk, DDEMOOBJ_POKE_FRONT, &x, &y);
        if (x != -48) {
            func_ov180_021b7438(wk, DDEMOOBJ_POKE_FRONT, -16, 0);
        } else {
            wk->subSeq++;
        }
        break;
    }
    case 14: {
        s16 x, y;

        func_ov180_021b7408(wk, DDEMOOBJ_PLATE_NAME, &x, &y);
        if (x != 264) {
            func_ov180_021b7438(wk, DDEMOOBJ_PLATE_NAME, 16, 0);
        } else {
            wk->subSeq++;
        }
        func_ov180_021b7c08(wk);
        break;
    }
    case 15: {
        s16 x, y;

        func_ov180_021b7408(wk, DDEMOOBJ_PLATE_TITLE, &x, &y);
        if (x != -8) {
            func_ov180_021b7438(wk, DDEMOOBJ_PLATE_TITLE, -16, 0);
            func_ov180_021b7c08(wk);
            break;
        }
        wk->subSeq++;
        return func_ov180_021b723c(wk, SEQ_1ST_MAIN, 6);
    }
    case 16:
        func_ov180_021b681c(wk);
        func_ov180_021b67b4(wk);
        wk->monIndex++;
        if (wk->monIndex == wk->monCount) {
            wk->monIndex = 0;
            wk->subSeq = 0;
            return SEQ_1ST_EXIT;
        }
        wk->subSeq = 3;
        break;
    }
    return SEQ_1ST_MAIN;
}

// pokebw2's DDemoSeq_Init2nd
static int func_ov180_021b6f34(DDemoWork *wk) {
    GFL_BGSysSetDisplayLayout(GX_DISP_SELECT_SUB_MAIN);
    func_ov180_021b61d4(1);
    func_ov180_021b6224();
    func_ov180_021b6254();
    func_ov180_021b6360();
    func_ov180_021b66a0(wk);
    func_ov180_021b68a8(wk);
    func_ov180_021b7318(wk, 1);
    func_ov180_021b7d80(wk);
    func_ov180_021b6170(wk);
    wk->scene = DDEMO_SCENE_2ND;
    return func_ov180_021b7210(wk, SEQ_2ND_MAIN);
}

// pokebw2's DDemoSeq_Exit2nd
static int func_ov180_021b6f80(DDemoWork *wk) {
    func_ov180_021b6188(wk);
    func_ov180_021b7f0c(wk);
    func_ov180_021b7390(wk);
    func_ov180_021b6340();
    func_ov180_021b6914(wk);
    func_ov180_021b66d4(wk);
    wk->scene = DDEMO_SCENE_NONE;
    return SEQ_RELEASE;
}

// The trainer drops in on both screens and the plates open, then the team's Pokémon cross the screen in turn, again
// and again, until A, B or a touch ends the demo
// pokebw2's DDemoSeq_Main2nd
static int func_ov180_021b6fac(DDemoWork *wk) {
    if (wk->end == TRUE && GFL_WipeIsFinished() == TRUE && GFL_SndBGMIsFading() == FALSE) {
        func_02005d28();
        return SEQ_2ND_EXIT;
    }
    switch (wk->subSeq) {
    case 0:
        wk->subSeq++;
        return func_ov180_021b7268(wk, 32);
    case 1:
        GFL_SndSEPlay(SEQ_SE_DDEMO_03);
        wk->subSeq++;
        break;
    case 2:
        if (wk->count != 51) {
            func_ov180_021b7438(wk, DDEMOOBJ_TRAINER_MAIN, 0, 8);
            func_ov180_021b7438(wk, DDEMOOBJ_TRAINER_SUB, 0, 8);
            wk->count++;
        } else {
            GFL_SndSEPlay(SEQ_SE_ROTATION_B);
            wk->subSeq++;
            return func_ov180_021b7268(wk, 32);
        }
        break;
    case 3:
        func_ov180_021b746c(wk, DDEMOOBJ_PLATE_UPPER, TRUE);
        func_ov180_021b746c(wk, DDEMOOBJ_PLATE_LOWER, TRUE);
        func_ov180_021b7488(wk, DDEMOOBJ_PLATE_UPPER, 2);
        func_ov180_021b7488(wk, DDEMOOBJ_PLATE_LOWER, 2);
        wk->subSeq++;
        break;
    case 4:
        if (func_ov180_021b74b4(wk, DDEMOOBJ_PLATE_UPPER) == FALSE) {
            GFL_SndSEPlay(SEQ_SE_DDEMO_04);
            wk->subSeq++;
            return func_ov180_021b7268(wk, 16);
        }
        break;
    case 5:
        func_ov180_021b7c60(wk, DDEMO_FOAM_PLAYER, TRUE);
        wk->subSeq++;
        return func_ov180_021b7268(wk, 32);
    case 6:
        func_ov180_021b7c60(wk, DDEMO_FOAM_TITLE_2ND, TRUE);
        wk->subSeq++;
        break;
    case 7:
        if (wk->count == 300) {
            wk->count = 0;
            wk->subSeq++;
        } else {
            wk->count++;
        }
        break;
    case 8: {
        BOOL start = FALSE;

        if (wk->monIndex == 0) {
            if (wk->count == 256) {
                start = TRUE;
            }
        } else {
            if (wk->count == 128) {
                start = TRUE;
            }
        }
        if (start == TRUE) {
            func_ov180_021b692c(wk);
            wk->count = 0;
            wk->subSeq++;
        } else {
            wk->count++;
        }
        break;
    }
    case 9:
        if (wk->count != 20) {
            func_ov180_021b699c(wk, -8);
            wk->count++;
        } else {
            func_02015e4c(wk->mcss);
            wk->count = 0;
            wk->subSeq++;
        }
        break;
    case 10:
        if (wk->count != 128) {
            wk->count++;
        } else {
            func_ov180_021b69f8(wk);
            wk->count = 0;
            wk->subSeq++;
        }
        break;
    case 11:
        if (wk->mcssAnmEnd != FALSE) {
            if (wk->count != 24) {
                func_ov180_021b699c(wk, -8);
                wk->count++;
            } else {
                func_ov180_021b6984(wk);
                wk->count = 0;
                wk->monIndex++;
                if (wk->monIndex == wk->monCount) {
                    wk->monIndex = 0;
                }
                wk->subSeq = 8;
            }
        }
        break;
    }
    if (wk->subSeq >= 6) {
        func_ov180_021b7f38(wk);
    }
    if (wk->subSeq >= 8 && wk->end == FALSE) {
        if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60() == TRUE) {
            wk->end = TRUE;
            GFL_SndBGMFadeOut(220);
            func_ov180_021b723c(wk, SEQ_2ND_EXIT, 188);
        }
    }
    return SEQ_2ND_MAIN;
}

// pokebw2's DDemoSeq_SetFadeIn
static int func_ov180_021b7210(DDemoWork *wk, int next) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, HEAPID_DENDOU_DEMO);
    wk->nextSeq = next;
    return SEQ_WIPE;
}

// pokebw2's DDemoSeq_SetFadeOut
static int func_ov180_021b723c(DDemoWork *wk, int next, int division) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, division, 1,
                HEAPID_DENDOU_DEMO);
    wk->nextSeq = next;
    return SEQ_WIPE;
}

// Waits for the given frames, then goes on with the sequence that runs
// pokebw2's DDemoSeq_SetWait
static int func_ov180_021b7268(DDemoWork *wk, int wait) {
    wk->count = wait;
    wk->nextSeq = wk->mainSeq;
    return SEQ_WAIT;
}

// pokebw2's DDemoSeq_InitShake
static void func_ov180_021b727c(DDemoWork *wk) {
    u32 i;

    wk->shakeIndex = 0;
    for (i = 0; i < DDEMO_SHAKE_COUNT; i++) {
        wk->shakeX[i] = GFL_RandomLC(8) - 4;
        wk->shakeY[i] = GFL_RandomLC(8) - 4;
    }
}

// Moves an actor to the next place of the shake, or back in place and returns FALSE once it is over
// pokebw2's DDemoSeq_Shake
static BOOL func_ov180_021b72b0(DDemoWork *wk, u32 id) {
    if (wk->shakeIndex == DDEMO_SHAKE_COUNT) {
        func_ov180_021b73e4(wk, id, wk->shakeBaseX, wk->shakeBaseY);
        return FALSE;
    }
    func_ov180_021b73e4(wk, id, wk->shakeBaseX + wk->shakeX[wk->shakeIndex], wk->shakeBaseY + wk->shakeY[wk->shakeIndex]);
    wk->shakeIndex++;
    return TRUE;
}

// pokebw2's DDemoSeq_RotateBg
static void func_ov180_021b7300(DDemoWork *wk) {
    GFL_BGSysRotateBGReq(3, BG_ROTATE_ADD, 1);
    GFL_BGSysRotateBGReq(BGSYS_BG_SUB + 3, BG_ROTATE_ADD, 1);
}
