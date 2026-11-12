// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov173/wifi_p2pmatchroom.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wifi_p2pmatchroom.h"
#include "app/wf2dmap/wf2dmap_judge.h"
#include "app/wifi_p2pmatchroom_map.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/math_util.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/os.h"
#include "nnsys/gfd.h"

// The room of the Wi-Fi Club's match board, where the player walks among the friends who are online. The ROM has no
// name for this file: it is named after the older games' wifi_p2pmatchroom.c, and so are its functions

// How long a friend takes to spin in and out, in frames
#define WIFI_MCR_NPC_KURU_FRAMES 45
// The frames of a friend's jump, and how many of them it stands before jumping
#define WIFI_MCR_NPC_JUMP_FRAMES 28
#define WIFI_MCR_NPC_JUMP_WAIT 16
// A jump is this many frames up and down
#define WIFI_MCR_NPC_JUMP_ONE 6
// How long a friend waits before it turns: at least the minimum, plus up to the random part
#define WIFI_MCR_NPC_TURN_WAIT_MIN 0x100
#define WIFI_MCR_NPC_TURN_WAIT_RANDOM 0x400

// Where the color of a PC's screen is in the BG palettes, in bytes: PC n has color n + 1 of palette 7
#define WIFI_MCR_PCANM_COLOR(pc) (((pc) + 1) * 2 + 7 * 32)
#define WIFI_MCR_PCANM_STEP_NUM 8

// A step of the animation of the PC the player uses
typedef struct {
    // Whether the step ends after wait frames, rather than when asked to
    u8 timed;
    u8 wait;
    // Whether the step changes the color, and the color it changes it to
    u8 trans;
    u8 color;
} MCR_PCANM_STEP;

static BOOL func_ov099_021b9788(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021b97cc(WIFI_MATCHROOM *wk);
static void func_ov099_021b9b80(WIFI_MATCHROOM *wk);
static BOOL func_ov099_021b9ba8(const WIFI_MATCHROOM *wk);
static BOOL func_ov099_021b9bc4(const WIFI_MATCHROOM *wk);
static void func_ov099_021b9c4c(WIFI_MATCHROOM *wk);
static void func_ov099_021b9c9c(WIFI_MATCHROOM *wk);
static void func_ov099_021b9cd0(WIFI_MATCHROOM *wk);
MCR_MOVEOBJ *func_ov099_021b9cf8(WIFI_MATCHROOM *wk, const MCR_MOVEOBJ *obj, u32 way);
static void func_ov099_021b9d30(WIFI_MATCHROOM *wk, u32 heroView, u32 heapId);
void func_ov099_021b9d5c(WIFI_MATCHROOM *wk);
static void func_ov099_021b9d6c(WIFI_MATCHROOM *wk, u32 heapId, ArcTool *arc, u32 mapNo);
static void func_ov099_021b9db0(WIFI_MATCHROOM *wk);
static void func_ov099_021b9dc0(MCR_CLACT *clact, u32 heapId, ArcTool *arc);
static void func_ov099_021b9dd4(MCR_CLACT *clact);
void func_ov127_021c5a4c(WF2DMAP_SCRDRAW *scrDraw);
WF2DMAP_POS func_ov127_021c4ac4(const WF2DMAP_OBJWK *wk);
u32 func_ov127_021c5858(const WF2DMAP_OBJDRAWWK *wk);
u32 func_ov127_021c4864(const WF2DMAP_MAPSYS *sys, u16 x, u16 y);
void func_ov127_021c584c(WF2DMAP_OBJDRAWWK *wk, BOOL flag);
static void func_ov099_021b9f90(WIFI_MATCHROOM *wk, WF2DMAP_POS pos, u32 priority);
s32 func_ov127_021c49a4(const WF2DMAP_OBJWK *wk, u32 id);
u16 func_ov127_021c47f8(const WF2DMAP_MAPSYS *sys);
u16 func_ov127_021c47fc(const WF2DMAP_MAPSYS *sys);
void func_ov127_021c57d8(WF2DMAP_OBJDRAWWK *wk, BOOL flag);
void func_ov127_021c57dc(WF2DMAP_OBJDRAWWK *wk);
void func_ov127_021c5840(WF2DMAP_OBJDRAWWK *wk, BOOL flag);
void func_ov127_021c57e8(WF2DMAP_OBJDRAWWK *wk);
BOOL func_ov127_021c5834(const WF2DMAP_OBJDRAWWK *wk);
void func_ov127_021c5814(WF2DMAP_OBJDRAWWK *wk, WF2DMAP_POS pos);
void func_ov127_021c5898(WF2DMAP_OBJDRAWWK *wk, WF2DMAP_POS pos);
WF2DMAP_POS func_ov127_021c4a54(WF2DMAP_POS pos, u32 way);
static MCR_MOVEOBJ *func_ov099_021ba168(WIFI_MATCHROOM *wk);
void func_ov099_021ba248(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021b9de4(MCR_CLACT *clact, u32 heapId, ArcTool *arc);
static void func_ov099_021b9dec(MCR_CLACT *clact);
static void func_ov099_021b9df4(MCR_CLACT *clact, u32 heapId);
static void func_ov099_021b9dfc(MCR_CLACT *clact);
static void func_ov099_021b9e04(void *unk, u32 heapId);
static void func_ov099_021b9e08(void *unk);
static void func_ov099_021b9e0c(void *unk, u32 heapId, ArcTool *arc, u32 arcId);
static void func_ov099_021b9e50(MCR_CLACT *clact, u32 heapId, ArcTool *arc);
static void func_ov099_021b9ea0(MCR_CLACT *clact);
static void func_ov099_021b9eb8(MCR_CLACT *clact, u32 heapId);
static void func_ov099_021b9f34(MCR_CLACT *clact);
static void func_ov099_021b9f48(WIFI_MATCHROOM *wk);
static void func_ov099_021b9f84(WIFI_MATCHROOM *wk);
void func_ov099_021b9f90(WIFI_MATCHROOM *wk, WF2DMAP_POS pos, u32 priority);
static void func_ov099_021b9fd0(WIFI_MATCHROOM *wk);
static BOOL func_ov099_021b9fdc(const WIFI_MATCHROOM *wk);
void func_ov099_021ba04c(const WIFI_MATCHROOM *wk, WF2DMAP_POS *pos, u32 friendIndex);
void func_ov099_021ba0f8(const WIFI_MATCHROOM *wk, WF2DMAP_POS *pos);
static MCR_MOVEOBJ *func_ov099_021ba168(WIFI_MATCHROOM *wk);
void func_ov099_021ba190(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, BOOL hero);
static void func_ov099_021ba1c4(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, const WF2DMAP_POS *pos, u32 friendNo, u16 view,
                           u8 way, u32 status);
static BOOL func_ov099_021ba1fc(const MCR_MOVEOBJ *obj);
static void func_ov099_021ba228(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021ba264(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021ba2a4(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021ba2d0(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
void func_ov099_021ba2d4(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021ba36c(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static BOOL func_ov099_021ba418(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
BOOL func_ov099_021ba4c8(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static BOOL func_ov099_021ba520(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static BOOL func_ov099_021ba538(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static BOOL func_ov099_021ba578(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static BOOL func_ov099_021ba590(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021ba594(WIFI_MATCHROOM *wk, u32 cmd, u32 way, u32 playId);
static void func_ov099_021ba5b4(WIFI_MATCHROOM *wk, u32 cmd, const MCR_MOVEOBJ *obj);
static BOOL func_ov099_021ba5dc(const MCR_MOVEOBJ *obj);
static void func_ov099_021ba5f0(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, u32 moveId);
static void func_ov099_021ba60c(WIFI_MATCHROOM *wk, MCR_PCANM *anm, ArcTool *arc, u32 arcId);
static void func_ov099_021ba648(WIFI_MATCHROOM *wk, MCR_PCANM *anm);
static void func_ov099_021ba660(WIFI_MATCHROOM *wk, MCR_PCANM *anm);
static void func_ov099_021ba674(MCR_PCANM *anm);
static void func_ov099_021ba6dc(MCR_PCANM *anm);
static const void *func_ov099_021ba764(const MCR_PCANM *anm, u32 color);
static void func_ov099_021ba770(MCR_PCANM *anm);
static void func_ov099_021ba78c(MCR_PCANM *anm);
static void func_ov099_021ba794(MCR_PCANM *anm, u8 pcNo);
static void func_ov099_021ba7bc(MCR_PCANM *anm);

// How long a free PC's screen keeps each of its two colors
// pokebw2's sWifiMcrPcAnmFreeWait
static const u16 data_ov099_021c2878[2] = { 3, 2 };

// pokebw2's sWifiMcrPcAnmSteps
static const MCR_PCANM_STEP data_ov099_021c287c[WIFI_MCR_PCANM_STEP_NUM] = {
    { FALSE, 0, FALSE, 1 }, { TRUE, 2, TRUE, 0 }, { TRUE, 4, TRUE, 1 }, { TRUE, 2, TRUE, 0 },
    { FALSE, 0, FALSE, 1 }, { TRUE, 2, TRUE, 1 }, { TRUE, 4, TRUE, 0 }, { TRUE, 3, TRUE, 1 },
};

void func_ov099_021b95a0(WIFI_MATCHROOM *wk, u32 heapId, ArcTool *arc, u32 heroView, u32 friendNum, u32 arcId,
                   ClActUnit *unit, ClActRenderer *renderer) {
    u32 mapNo = 0;
    WIFI_MCR_MAPSIZE size;

    MI_CpuFill8(wk, 0, sizeof(WIFI_MATCHROOM));

    if (friendNum != 0) {
        mapNo = (friendNum - 1) / WIFI_MCR_BLOCK_FRIEND_NUM;
    }

    wk->heapId = heapId;
    wk->clact.unit = unit;
    wk->clact.renderer = renderer;

    func_ov099_021b9dc0(&wk->clact, heapId, arc);
    func_ov099_021b9e04(wk->unk10, heapId);

    size = func_ov099_021ba810(mapNo);
    wk->mapSys = func_ov127_021c47a0(size.x, size.y, heapId);
    func_ov127_021c4800(wk->mapSys, func_ov099_021ba81c(mapNo));

    wk->objSys = func_ov127_021c4880(WIFI_MCR_OBJ_MAX, heapId);
    func_ov099_021b9d30(wk, heroView, heapId);
    func_ov127_021c59d0(&wk->scroll);
    func_ov099_021b9d6c(wk, heapId, arc, mapNo);
    func_ov099_021b9e0c(wk->unk10, heapId, arc, arcId);
    func_ov099_021ba60c(wk, &wk->pcAnm, arc, arcId);
    wk->cmdQueue = func_ov127_021c5eec(128, heapId);

    wk->init = TRUE;
}

// pokebw2's WIFI_MCR_Dest
void func_ov099_021b9670(WIFI_MATCHROOM *wk) {
    func_ov127_021c5f30(wk->cmdQueue);
    func_ov099_021b9db0(wk);
    func_ov099_021ba648(wk, &wk->pcAnm);
    func_ov099_021b9b80(wk);
    func_ov099_021b9d5c(wk);
    func_ov127_021c48c0(wk->objSys);
    func_ov127_021c47e4(wk->mapSys);
    func_ov099_021b9dd4(&wk->clact);
    func_ov099_021b9e08(wk->unk10);
    MI_CpuFill8(wk, 0, sizeof(WIFI_MATCHROOM));
}

// pokebw2's WIFI_MCR_GetInitFlag
BOOL func_ov099_021b96cc(const WIFI_MATCHROOM *wk) {
    return wk->init;
}

// pokebw2's WIFI_MCR_Main
u32 func_ov099_021b96d0(WIFI_MATCHROOM *wk) {
    // The room runs at 30 frames a second
    if (OS_GetVBlankCount() & 1) {
        func_ov127_021c48e8(wk->objSys);
        func_ov099_021b9c9c(wk);
        func_ov099_021b9c4c(wk);
        func_ov127_021c5994(&wk->scroll, wk->player->obj);
        func_ov127_021c5a60(wk->scrDraw, &wk->scroll);
        func_ov127_021c571c(wk->objDrawSys);
        func_ov099_021b9cd0(wk);
        func_ov099_021b9f84(wk);
    }

    if (func_ov099_021b9ba8(wk) == TRUE) {
        func_ov099_021b97cc(wk);

        if (func_ov099_021b9fdc(wk) == TRUE) {
            func_ov099_021b9f48(wk);
            if (GCTX_HIDGetHeldKeys() & PAD_KEY_DOWN) {
                return WIFI_MCR_RET_CANCEL;
            }
        }

        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            if (func_ov099_021b97f4(wk) != 0) {
                return WIFI_MCR_RET_SELECT;
            }
            if (func_ov099_021b9bc4(wk) == TRUE) {
                return WIFI_MCR_RET_MYSELECT;
            }
        }
    }

    return WIFI_MCR_RET_NONE;
}

// pokebw2's WifiMcr_PlayerKeyCheck
static BOOL func_ov099_021b9788(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    u32 way = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_WAY);

    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        MCR_MOVEOBJ *hitObj = func_ov099_021b9cf8(wk, obj, way);
        u32 friendNo;

        if (hitObj != NULL) {
            if (func_ov099_021ba1fc(hitObj) == TRUE) {
                friendNo = func_ov127_021c49a4(hitObj->obj, WF2DMAP_OBJPM_PLID);
            } else {
                friendNo = 0;
            }
        } else {
            friendNo = 0;
        }
        obj->accessFriend = friendNo;
    }

    return TRUE;
}

// pokebw2's WifiMcr_PlayerKeyCheckAll
static void func_ov099_021b97cc(WIFI_MATCHROOM *wk) {
    int i;

    for (i = 0; i < WIFI_MCR_OBJ_MAX; i++) {
        if (wk->objs[i].obj != NULL) {
            func_ov099_021b9788(wk, &wk->objs[i]);
        }
    }
}

// pokebw2's WIFI_MCR_PlayerSelect
u8 func_ov099_021b97f4(const WIFI_MATCHROOM *wk) {
    return wk->player->accessFriend;
}

u32 func_ov099_021b9800(const WIFI_MATCHROOM *wk) {
    WF2DMAP_POS pos;

    pos = func_ov127_021c4ac4(wk->player->obj);
    return func_ov127_021c4864(wk->mapSys, pos.x / WF2DMAP_GRID_SIZE, pos.y / WF2DMAP_GRID_SIZE);
}

u32 func_ov099_021b984c(const WIFI_MATCHROOM *wk) {
    WF2DMAP_POS pos;

    pos = func_ov127_021c4ac4(wk->player->obj);
    return func_ov127_021c4864(wk->mapSys, pos.x / WF2DMAP_GRID_SIZE, pos.y / WF2DMAP_GRID_SIZE + 1);
}



void func_ov099_021b989c(WIFI_MATCHROOM *wk, const MCR_MOVEOBJ *obj) {
    WF2DMAP_POS pos;
    u32 priority;

    pos = func_ov127_021c4ac4(obj->obj);
    priority = func_ov127_021c5858(obj->draw);
    func_ov099_021b9f90(wk, pos, priority - 2);
    func_ov127_021c584c(obj->draw, FALSE);
}


// pokebw2's WIFI_MCR_ShadowOn
void func_ov099_021b98e4(WIFI_MATCHROOM *wk, const MCR_MOVEOBJ *obj) {
    func_ov127_021c584c(obj->draw, TRUE);
}

// pokebw2's WIFI_MCR_CursorOff
void func_ov099_021b98f0(WIFI_MATCHROOM *wk) {
    func_ov099_021b9fd0(wk);
}

// pokebw2's WIFI_MCR_PlayerMovePause
void func_ov099_021b98f8(WIFI_MATCHROOM *wk, BOOL pause) {
    wk->playerPause = pause;
}

// pokebw2's WIFI_MCR_PlayerMovePauseGet
BOOL func_ov099_021b98fc(const WIFI_MATCHROOM *wk) {
    return wk->playerPause;
}

// pokebw2's WIFI_MCR_SetPlayer
MCR_MOVEOBJ *func_ov099_021b9900(WIFI_MATCHROOM *wk, u32 view) {
    MCR_MOVEOBJ *obj;
    WF2DMAP_POS pos;

    obj = func_ov099_021ba168(wk);
    wk->player = obj;

    func_ov099_021ba0f8(wk, &pos);
    func_ov099_021ba1c4(wk, obj, &pos, 0, view, WF2DMAP_WAY_UP, WF2DMAP_OBJST_NONE);
    func_ov099_021ba228(wk, obj);
    func_ov099_021ba190(wk, obj, TRUE);

    func_ov127_021c5994(&wk->scroll, wk->player->obj);
    func_ov127_021c5a60(wk->scrDraw, &wk->scroll);

    return obj;
}

MCR_MOVEOBJ *func_ov099_021b9968(WIFI_MATCHROOM *wk, u32 view, u32 friendNo) {
    MCR_MOVEOBJ *obj;
    WF2DMAP_POS pos;
    WF2DMAP_POS playerPos;

    obj = func_ov099_021ba168(wk);
    func_ov099_021ba04c(wk, &pos, friendNo - 1);
    if (wk->player != NULL) {
        playerPos = func_ov127_021c4ac4(wk->player->obj);
        if (playerPos.x == pos.x && playerPos.y == pos.y) {
            return NULL;
        }
    }

    func_ov099_021ba1c4(wk, obj, &pos, friendNo, view, WF2DMAP_WAY_DOWN, WF2DMAP_OBJST_BUSY);
    func_ov099_021ba248(wk, obj);
    func_ov099_021ba190(wk, obj, FALSE);
    return obj;
}


// pokebw2's WIFI_MCR_DelPeopleReq
void func_ov099_021b99f4(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_WAY);

    obj->move = func_ov099_021ba520;
    obj->drawFunc = func_ov099_021ba2d4;
    obj->moveCount = 0;
    obj->moveState = WIFI_MCR_MOVESTATE_NPC_OUT;

    func_ov127_021c57d8(obj->draw, FALSE);
    func_ov127_021c57dc(obj->draw);
    func_ov099_021ba5b4(wk, WF2DMAP_OBJST_BUSY, obj);
}

// pokebw2's WIFI_MCR_DelPeople
void func_ov099_021b9a34(MCR_MOVEOBJ *obj) {
    func_ov127_021c5704(obj->draw);
    func_ov127_021c4948(obj->obj);
    MI_CpuFill8(obj, 0, sizeof(MCR_MOVEOBJ));
}

// pokebw2's WIFI_MCR_GetFriendNo
u8 func_ov099_021b9a50(const MCR_MOVEOBJ *obj) {
    return func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_PLID);
}

// pokebw2's WIFI_MCR_GetRetWay
u32 func_ov099_021b9a60(const MCR_MOVEOBJ *obj) {
    return func_ov127_021c4a8c(func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_WAY));
}

// pokebw2's WIFI_MCR_NpcPauseOn
void func_ov099_021b9a70(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, u32 way) {
    WF2DMAP_POS pos;

    obj->moveTmp = obj->move;
    obj->move = func_ov099_021ba590;
    obj->moveStateTmp = obj->moveState;
    obj->moveState = WIFI_MCR_MOVESTATE_NPC_PAUSE;

    func_ov099_021ba594(wk, WF2DMAP_OBJST_NONE, way, func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_PLID));

    func_ov127_021c57d8(obj->draw, FALSE);
    func_ov127_021c5864(obj->draw, way);

    pos.x = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_X);
    pos.y = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_Y);
    func_ov127_021c5814(obj->draw, pos);
}

// pokebw2's WIFI_MCR_NpcPauseOff
void func_ov099_021b9ae0(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    if (obj != NULL) {
        if (obj->moveTmp != NULL) {
            obj->move = obj->moveTmp;
            obj->moveTmp = NULL;
            obj->moveState = obj->moveStateTmp;
            func_ov127_021c57d8(obj->draw, TRUE);
        }
    }
}

// pokebw2's WIFI_MCR_NpcMoveSet
void func_ov099_021b9b00(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, u32 moveId) {
    // A friend that is spinning in takes its move when it has arrived
    if (func_ov099_021ba5dc(obj) == FALSE) {
        obj->moveId = moveId;
        return;
    }

    func_ov099_021ba5f0(wk, obj, moveId);
}

// pokebw2's WIFI_MCR_PCAnmStart
void func_ov099_021b9b24(WIFI_MATCHROOM *wk) {
    if (func_ov099_021b9bc4(wk) == TRUE) {
        s8 pcNo = func_ov099_021b984c(wk);

        pcNo -= WIFI_MCR_MAPPM_BLOCK0;
        func_ov099_021ba794(&wk->pcAnm, pcNo);
    }
}

// pokebw2's WIFI_MCR_PCAnmOff
void func_ov099_021b9b54(WIFI_MATCHROOM *wk) {
    func_ov099_021ba7bc(&wk->pcAnm);
}

// pokebw2's WIFI_MCR_PCAnmMain
void func_ov099_021b9b64(WIFI_MATCHROOM *wk) {
    if (OS_GetVBlankCount() & 1) {
        func_ov099_021ba660(wk, &wk->pcAnm);
    }
}

// pokebw2's WifiMcr_DelPeopleAll
static void func_ov099_021b9b80(WIFI_MATCHROOM *wk) {
    int i;

    for (i = 0; i < WIFI_MCR_OBJ_MAX; i++) {
        if (wk->objs[i].obj != NULL) {
            func_ov099_021b9a34(&wk->objs[i]);
        }
    }
}

// pokebw2's WifiMcr_PlayerMoveEndCheck
static BOOL func_ov099_021b9ba8(const WIFI_MATCHROOM *wk) {
    if (func_ov127_021c49a4(wk->player->obj, WF2DMAP_OBJPM_ST) == WF2DMAP_OBJST_NONE) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov099_021b9bc4(const WIFI_MATCHROOM *wk) {
    WF2DMAP_POS pos;
    WF2DMAP_POS frontPos;
    s32 way;
    u32 param;

    pos = func_ov127_021c4ac4(wk->player->obj);
    way = func_ov127_021c49a4(wk->player->obj, WF2DMAP_OBJPM_WAY);
    frontPos = func_ov127_021c4a54(pos, way);
    param = func_ov127_021c4864(wk->mapSys, frontPos.x / WF2DMAP_GRID_SIZE, frontPos.y / WF2DMAP_GRID_SIZE);
    if (way == WF2DMAP_WAY_UP && param == WIFI_MCR_MAPPM_PC) {
        return TRUE;
    }
    return FALSE;
}


// pokebw2's WifiMcr_ReqCmdMain
static void func_ov099_021b9c4c(WIFI_MATCHROOM *wk) {
    WF2DMAP_REQCMD req;
    WF2DMAP_ACTCMD act;

    while (func_ov127_021c5f80(wk->cmdQueue, &req) == TRUE) {
        if (func_ov127_021c5370(wk->mapSys, wk->objSys, &req, &act) == TRUE) {
            func_ov127_021c48d4(wk->objSys, &act);
        }
    }
}

// pokebw2's WifiMcr_MoveObjMoveAll
static void func_ov099_021b9c9c(WIFI_MATCHROOM *wk) {
    int i;

    for (i = 0; i < WIFI_MCR_OBJ_MAX; i++) {
        if (wk->objs[i].obj != NULL) {
            if (wk->objs[i].move(wk, &wk->objs[i]) == TRUE) {
                func_ov099_021b9a34(&wk->objs[i]);
            }
        }
    }
}

// pokebw2's WifiMcr_MoveObjDrawAll
static void func_ov099_021b9cd0(WIFI_MATCHROOM *wk) {
    int i;

    for (i = 0; i < WIFI_MCR_OBJ_MAX; i++) {
        if (wk->objs[i].obj != NULL) {
            wk->objs[i].drawFunc(wk, &wk->objs[i]);
        }
    }
}

// The character in front of obj, or NULL
// pokebw2's WifiMcr_HitObjGet
MCR_MOVEOBJ *func_ov099_021b9cf8(WIFI_MATCHROOM *wk, const MCR_MOVEOBJ *obj, u32 way) {
    const WF2DMAP_OBJWK *hitObj = func_ov127_021c4ba4(obj->obj, wk->objSys, way);
    int i;

    if (hitObj != NULL) {
        for (i = 0; i < WIFI_MCR_OBJ_MAX; i++) {
            if (wk->objs[i].obj == hitObj) {
                return &wk->objs[i];
            }
        }
    }
    return NULL;
}

// pokebw2's WifiMcr_ObjDrawInit
static void func_ov099_021b9d30(WIFI_MATCHROOM *wk, u32 heroView, u32 heapId) {
    wk->objDrawSys = func_ov127_021c5590(wk->clact.unit, NULL, WIFI_MCR_OBJ_MAX, heroView, 0, 0, heapId);
    func_ov127_021c5664(wk->objDrawSys, 2, heapId);
}

// pokebw2's WifiMcr_ObjDrawExit
void func_ov099_021b9d5c(WIFI_MATCHROOM *wk) {
    func_ov127_021c5608(wk->objDrawSys);
}

// pokebw2's WifiMcr_ScrDrawInit
static void func_ov099_021b9d6c(WIFI_MATCHROOM *wk, u32 heapId, ArcTool *arc, u32 mapNo) {
    // The maps' screens follow each other in the archive
    WF2DMAP_SCRDRAWINIT init = { 0, 0, 0, 28, 0, 0, 3, 0, ARCID_WIFIP2PMATCH, 26, FALSE };

    init.fileId += mapNo;
    wk->scrDraw = func_ov127_021c59f4(wk->clact.renderer, wk->unk10, &init, heapId);
}

static void func_ov099_021b9db0(WIFI_MATCHROOM *wk) {
    func_ov127_021c5a4c(wk->scrDraw);
}


// pokebw2's WifiMcr_ClactInit
static void func_ov099_021b9dc0(MCR_CLACT *clact, u32 heapId, ArcTool *arc) {
    func_ov099_021b9de4(clact, heapId, arc);
    func_ov099_021b9df4(clact, heapId);
}

// pokebw2's WifiMcr_ClactExit
static void func_ov099_021b9dd4(MCR_CLACT *clact) {
    func_ov099_021b9dfc(clact);
    func_ov099_021b9dec(clact);
}

// pokebw2's WifiMcr_ClactResLoad
static void func_ov099_021b9de4(MCR_CLACT *clact, u32 heapId, ArcTool *arc) {
    func_ov099_021b9e50(clact, heapId, arc);
}

// pokebw2's WifiMcr_ClactResFree
static void func_ov099_021b9dec(MCR_CLACT *clact) {
    func_ov099_021b9ea0(clact);
}

// pokebw2's WifiMcr_ClactActorAdd
static void func_ov099_021b9df4(MCR_CLACT *clact, u32 heapId) {
    func_ov099_021b9eb8(clact, heapId);
}

// pokebw2's WifiMcr_ClactActorDel
static void func_ov099_021b9dfc(MCR_CLACT *clact) {
    func_ov099_021b9f34(clact);
}

// pokebw2's WifiMcr_BgInit
static void func_ov099_021b9e04(void *unk, u32 heapId) {
}

// pokebw2's WifiMcr_BgExit
static void func_ov099_021b9e08(void *unk) {
}

// pokebw2's WifiMcr_BgLoad
static void func_ov099_021b9e0c(void *unk, u32 heapId, ArcTool *arc, u32 arcId) {
    GFL_BGSysLoadNCLRDefault(arcId, 7, 0, 0, 0x100, heapId);
    GFL_BGSysResetStdPalette(0, 0);
    GFL_BGSysLoadNCGRStatic(arcId, 16, 0, 0, 0, FALSE, heapId);
}

// pokebw2's WifiMcr_ClactResLoadCore
static void func_ov099_021b9e50(MCR_CLACT *clact, u32 heapId, ArcTool *arc) {
    clact->chars = func_0204a6c8(arc, 17, FALSE, 0, heapId);
    clact->palette = func_0204ac18(arc, 8, 0, 11 * 32, 0, 2, heapId);
    clact->cellAnims = func_0204af28(arc, 20, 32, heapId);
}

// pokebw2's WifiMcr_ClactResFreeCore
static void func_ov099_021b9ea0(MCR_CLACT *clact) {
    func_0204a8d4(clact->chars);
    func_0204ada4(clact->palette);
    func_0204afd8(clact->cellAnims);
}

// pokebw2's WifiMcr_ClactActorAddCore
static void func_ov099_021b9eb8(MCR_CLACT *clact, u32 heapId) {
    ClActorSetup setup;

    setup.x = 1;
    setup.y = 1;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    clact->exitCursor = func_0204b294(clact->unit, clact->chars, clact->palette, clact->cellAnims, &setup, 0, heapId);

    setup.sequence = 2;
    setup.priority = 254;
    setup.bgPriority = 3;
    clact->cursor = func_0204b294(clact->unit, clact->chars, clact->palette, clact->cellAnims, &setup, 0, heapId);

    func_0204b3dc(clact->exitCursor, FALSE);
    func_0204b3dc(clact->cursor, FALSE);
    func_0204b92c(clact->exitCursor, TRUE);
    func_0204b92c(clact->cursor, TRUE);
}

// pokebw2's WifiMcr_ClactActorDelCore
static void func_ov099_021b9f34(MCR_CLACT *clact) {
    func_0204b3b4(clact->exitCursor);
    func_0204b3b4(clact->cursor);
}

// pokebw2's WifiMcr_ExitCursorOn
static void func_ov099_021b9f48(WIFI_MATCHROOM *wk) {
    WF2DMAP_POS pos;
    ClActorPos actorPos;

    func_ov099_021ba0f8(wk, &pos);
    pos.x += 8;
    pos.y += 32;

    actorPos.x = pos.x;
    actorPos.y = pos.y;
    func_0204b528(wk->clact.exitCursor, &actorPos);
    func_0204b3dc(wk->clact.exitCursor, TRUE);
}

// pokebw2's WifiMcr_ExitCursorOff
static void func_ov099_021b9f84(WIFI_MATCHROOM *wk) {
    func_0204b3dc(wk->clact.exitCursor, FALSE);
}

// pokebw2's WifiMcr_CursorSet
void func_ov099_021b9f90(WIFI_MATCHROOM *wk, WF2DMAP_POS pos, u32 priority) {
    ClActorPos actorPos;

    actorPos.x = pos.x + 8;
    actorPos.y = pos.y;
    func_0204b528(wk->clact.cursor, &actorPos);
    func_0204b800(wk->clact.cursor, priority);
    func_0204b3dc(wk->clact.cursor, TRUE);
}

// pokebw2's WifiMcr_CursorHide
static void func_ov099_021b9fd0(WIFI_MATCHROOM *wk) {
    func_0204b3dc(wk->clact.cursor, FALSE);
}

static BOOL func_ov099_021b9fdc(const WIFI_MATCHROOM *wk) {
    WF2DMAP_POS pos;
    s32 way;
    u32 param;

    if (wk->player == NULL) {
        return FALSE;
    }

    pos = func_ov127_021c4ac4(wk->player->obj);
    way = func_ov127_021c49a4(wk->player->obj, WF2DMAP_OBJPM_WAY);
    param = func_ov127_021c4864(wk->mapSys, pos.x / WF2DMAP_GRID_SIZE, pos.y / WF2DMAP_GRID_SIZE);
    if (param == WIFI_MCR_MAPPM_EXIT && way == WF2DMAP_WAY_DOWN) {
        return TRUE;
    }
    return FALSE;
}

void func_ov099_021ba04c(const WIFI_MATCHROOM *wk, WF2DMAP_POS *pos, u32 friendIndex) {
    u32 x;
    u32 y;
    u32 gridX;
    u32 gridY;
    u32 block;
    u32 seat;
    u32 param;

    seat = friendIndex % WIFI_MCR_BLOCK_FRIEND_NUM;
    block = friendIndex / WIFI_MCR_BLOCK_FRIEND_NUM;
    gridX = func_ov127_021c47f8(wk->mapSys);
    gridY = func_ov127_021c47fc(wk->mapSys);

    for (y = 0; y < gridY; y++) {
        for (x = 0; x < gridX; x++) {
            param = func_ov127_021c4864(wk->mapSys, x, y);
            if (param == WIFI_MCR_MAPPM_FRIEND0 + seat) {
                param = func_ov127_021c4864(wk->mapSys, x, y + 1);
                if (param == WIFI_MCR_MAPPM_BLOCK0 + block) {
                    pos->x = x * WF2DMAP_GRID_SIZE;
                    pos->y = y * WF2DMAP_GRID_SIZE;
                    return;
                }
            }
        }
    }
}


void func_ov099_021ba0f8(const WIFI_MATCHROOM *wk, WF2DMAP_POS *pos) {
    u32 x;
    u32 y;
    u32 gridX = func_ov127_021c47f8(wk->mapSys);
    u32 gridY = func_ov127_021c47fc(wk->mapSys);

    for (y = 0; y < gridY; y++) {
        for (x = 0; x < gridX; x++) {
            if (func_ov127_021c4864(wk->mapSys, x, y) == WIFI_MCR_MAPPM_EXIT) {
                pos->x = x * WF2DMAP_GRID_SIZE;
                pos->y = y * WF2DMAP_GRID_SIZE;
                return;
            }
        }
    }
}



// pokebw2's WifiMcr_FreeObjGet
static MCR_MOVEOBJ *func_ov099_021ba168(WIFI_MATCHROOM *wk) {
    int i;

    for (i = 0; i < WIFI_MCR_OBJ_MAX; i++) {
        if (wk->objs[i].obj == NULL) {
            return &wk->objs[i];
        }
    }
    return NULL;
}

void func_ov099_021ba190(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, BOOL hero) {
    obj->draw = func_ov127_021c5680(wk->objDrawSys, obj->obj, hero, wk->heapId);
    if (hero == FALSE) {
        func_ov127_021c57d8(obj->draw, FALSE);
        func_ov127_021c57dc(obj->draw);
        func_ov127_021c5840(obj->draw, FALSE);
    }
}


static void func_ov099_021ba1c4(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, const WF2DMAP_POS *pos, u32 friendNo, u16 view,
                           u8 way, u32 status) {
    WF2DMAP_OBJDATA data;

    data.x = pos->x;
    data.y = pos->y;
    data.playId = friendNo;
    data.status = status;
    data.way = way;
    data.charaId = view;
    obj->obj = func_ov127_021c4918(wk->objSys, &data);
}

// Whether the player can talk to the character
// pokebw2's WifiMcr_ObjAccessCheck
static BOOL func_ov099_021ba1fc(const MCR_MOVEOBJ *obj) {
    switch (obj->moveState) {
    case WIFI_MCR_MOVESTATE_PLAYER:
    case WIFI_MCR_MOVESTATE_NPC_NORMAL:
    case WIFI_MCR_MOVESTATE_NPC_JUMP:
        return TRUE;
    case WIFI_MCR_MOVESTATE_NPC_IN:
    case WIFI_MCR_MOVESTATE_NPC_OUT:
    case WIFI_MCR_MOVESTATE_NPC_PAUSE:
        return FALSE;
    }
    return FALSE;
}

// pokebw2's WifiMcr_PlayerMoveInit
static void func_ov099_021ba228(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    obj->move = func_ov099_021ba418;
    obj->drawFunc = func_ov099_021ba2d0;
    obj->moveState = WIFI_MCR_MOVESTATE_PLAYER;
    func_ov099_021ba5b4(wk, WF2DMAP_OBJST_NONE, obj);
}

// pokebw2's WifiMcr_NpcInMoveInit
void func_ov099_021ba248(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    obj->move = func_ov099_021ba4c8;
    obj->drawFunc = func_ov099_021ba2d4;
    obj->moveCount = WIFI_MCR_NPC_KURU_FRAMES;
    obj->moveState = WIFI_MCR_MOVESTATE_NPC_IN;
}

// pokebw2's WifiMcr_NpcNormalMoveInit
static void func_ov099_021ba264(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    obj->move = func_ov099_021ba538;
    obj->drawFunc = func_ov099_021ba2d0;
    obj->moveCount = GFL_RandomLC(WIFI_MCR_NPC_TURN_WAIT_RANDOM) + WIFI_MCR_NPC_TURN_WAIT_MIN;
    obj->moveState = WIFI_MCR_MOVESTATE_NPC_NORMAL;
    func_ov099_021ba5b4(wk, WF2DMAP_OBJST_NONE, obj);
    func_ov127_021c57d8(obj->draw, TRUE);
}

// pokebw2's WifiMcr_NpcJumpMoveInit
static void func_ov099_021ba2a4(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    obj->move = func_ov099_021ba578;
    obj->drawFunc = func_ov099_021ba36c;
    obj->moveCount = WIFI_MCR_NPC_JUMP_FRAMES;
    obj->moveState = WIFI_MCR_MOVESTATE_NPC_JUMP;
    func_ov099_021ba5b4(wk, WF2DMAP_OBJST_BUSY, obj);
    func_ov127_021c57d8(obj->draw, FALSE);
}

// pokebw2's WifiMcr_DrawNone
static void func_ov099_021ba2d0(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
}

void func_ov099_021ba2d4(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    WF2DMAP_POS pos;
    WF2DMAP_POS shadowPos;

    func_ov127_021c57e8(obj->draw);
    if (func_ov127_021c5834(obj->draw) == FALSE) {
        func_ov127_021c5840(obj->draw, TRUE);
    }

    pos.x = func_ov127_021c49a4(obj->obj, 0);
    pos.y = func_ov127_021c49a4(obj->obj, 1);
    shadowPos = pos;
    pos.y += FX_Mul(func_0203f5fc(obj->moveCount), -192 * FX32_ONE) >> FX32_SHIFT;

    func_ov127_021c5814(obj->draw, pos);
    func_ov127_021c5898(obj->draw, shadowPos);
}


// pokebw2's WifiMcr_DrawJump
static void func_ov099_021ba36c(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    WF2DMAP_POS pos;
    WF2DMAP_POS shadowPos;
    s16 count;

    pos.x = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_X);
    pos.y = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_Y);
    shadowPos = pos;

    count = obj->moveCount - WIFI_MCR_NPC_JUMP_WAIT;
    if (count > 0) {
        u16 degrees;

        count %= WIFI_MCR_NPC_JUMP_ONE;
        degrees = (count * 180) / WIFI_MCR_NPC_JUMP_ONE;
        pos.y -= FX_Mul(func_0203f5fc(degrees), 4 * FX32_ONE) >> FX32_SHIFT;
    }

    func_ov127_021c5814(obj->draw, pos);
    func_ov127_021c5898(obj->draw, shadowPos);
    func_ov127_021c5864(obj->draw, WF2DMAP_WAY_DOWN);
}

// pokebw2's WifiMcr_PlayerMove
static BOOL func_ov099_021ba418(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    u32 way = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_WAY);
    u32 playId = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_PLID);
    u32 cmd;

    if (func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_ST) != WF2DMAP_OBJST_NONE) {
        return FALSE;
    }
    if (wk->playerPause == TRUE) {
        return FALSE;
    }

    cmd = (GCTX_HIDGetHeldKeys() & PAD_BUTTON_B) ? WF2DMAP_OBJST_RUN : WF2DMAP_OBJST_WALK;

    // A key of another way than the one the player faces only turns the player
    if (GCTX_HIDGetHeldKeys() & PAD_KEY_UP) {
        if (way == WF2DMAP_WAY_UP) {
            func_ov099_021ba594(wk, cmd, way, playId);
        } else {
            func_ov099_021ba594(wk, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_UP, playId);
        }
    } else if (GCTX_HIDGetHeldKeys() & PAD_KEY_DOWN) {
        if (way == WF2DMAP_WAY_DOWN) {
            func_ov099_021ba594(wk, cmd, way, playId);
        } else {
            func_ov099_021ba594(wk, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_DOWN, playId);
        }
    } else if (GCTX_HIDGetHeldKeys() & PAD_KEY_LEFT) {
        if (way == WF2DMAP_WAY_LEFT) {
            func_ov099_021ba594(wk, cmd, way, playId);
        } else {
            func_ov099_021ba594(wk, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_LEFT, playId);
        }
    } else if (GCTX_HIDGetHeldKeys() & PAD_KEY_RIGHT) {
        if (way == WF2DMAP_WAY_RIGHT) {
            func_ov099_021ba594(wk, cmd, way, playId);
        } else {
            func_ov099_021ba594(wk, WF2DMAP_OBJST_TURN, WF2DMAP_WAY_RIGHT, playId);
        }
    }

    return FALSE;
}

// pokebw2's WifiMcr_NpcInMove
BOOL func_ov099_021ba4c8(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    obj->moveCount--;
    if (obj->moveCount <= 0) {
        u32 playerId;
        u32 friendNo;

        func_ov127_021c57f4(obj->draw);
        func_ov127_021c57d8(obj->draw, TRUE);

        playerId = func_ov127_021c49a4(wk->player->obj, WF2DMAP_OBJPM_PLID);
        friendNo = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_PLID);
        if (friendNo == playerId) {
            func_ov099_021ba228(wk, obj);
        } else {
            func_ov099_021ba5f0(wk, obj, obj->moveId);
        }
    }
    return FALSE;
}

// pokebw2's WifiMcr_NpcOutMove
static BOOL func_ov099_021ba520(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    obj->moveCount++;
    if (obj->moveCount > WIFI_MCR_NPC_KURU_FRAMES) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiMcr_NpcNormalMove
static BOOL func_ov099_021ba538(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    obj->moveCount--;
    if (obj->moveCount <= 0) {
        u32 way;

        obj->moveCount = GFL_RandomLC(WIFI_MCR_NPC_TURN_WAIT_RANDOM) + WIFI_MCR_NPC_TURN_WAIT_MIN;
        way = GFL_RandomLC(WF2DMAP_WAY_NUM);
        func_ov099_021ba594(wk, WF2DMAP_OBJST_NONE, way, func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_PLID));
    }
    return FALSE;
}

// pokebw2's WifiMcr_NpcJumpMove
static BOOL func_ov099_021ba578(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    obj->moveCount--;
    if (obj->moveCount < 0) {
        obj->moveCount = WIFI_MCR_NPC_JUMP_FRAMES;
    }
    return FALSE;
}

// pokebw2's WifiMcr_NpcPauseMove
static BOOL func_ov099_021ba590(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj) {
    return FALSE;
}

// pokebw2's WifiMcr_ReqCmdSet
static void func_ov099_021ba594(WIFI_MATCHROOM *wk, u32 cmd, u32 way, u32 playId) {
    WF2DMAP_REQCMD req;

    req.cmd = cmd;
    req.way = way;
    req.playId = playId;
    func_ov127_021c5f44(wk->cmdQueue, &req);
}

// Asks for cmd with the way the character faces
// pokebw2's WifiMcr_ReqCmdSetObj
static void func_ov099_021ba5b4(WIFI_MATCHROOM *wk, u32 cmd, const MCR_MOVEOBJ *obj) {
    u32 way = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_WAY);
    u32 playId = func_ov127_021c49a4(obj->obj, WF2DMAP_OBJPM_PLID);

    func_ov099_021ba594(wk, cmd, way, playId);
}

// Whether the character is done spinning
// pokebw2's WifiMcr_NpcMoveSetCheck
static BOOL func_ov099_021ba5dc(const MCR_MOVEOBJ *obj) {
    if (obj->drawFunc != func_ov099_021ba2d4) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiMcr_NpcMoveInit
static void func_ov099_021ba5f0(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, u32 moveId) {
    switch (moveId) {
    case WIFI_MCR_NPC_MOVE_NORMAL:
        func_ov099_021ba264(wk, obj);
        break;
    case WIFI_MCR_NPC_MOVE_JUMP:
        func_ov099_021ba2a4(wk, obj);
        break;
    }
    obj->moveId = moveId;
}

// pokebw2's WifiMcr_PcAnmInit
static void func_ov099_021ba60c(WIFI_MATCHROOM *wk, MCR_PCANM *anm, ArcTool *arc, u32 arcId) {
    MI_CpuFill8(anm, 0, sizeof(MCR_PCANM));

    anm->buffer = GFL_G2DIOReadNCLR(arcId, 9, &anm->palette, wk->heapId);
    anm->frame = 0;
    anm->freeBits = (1 << WIFI_MCR_PC_NUM) - 1;
    anm->wait = data_ov099_021c2878[anm->frame];
}

// pokebw2's WifiMcr_PcAnmExit
static void func_ov099_021ba648(WIFI_MATCHROOM *wk, MCR_PCANM *anm) {
    GFL_HeapFree(anm->buffer);
    MI_CpuFill8(anm, 0, sizeof(MCR_PCANM));
}

// pokebw2's WifiMcr_PcAnmMain
static void func_ov099_021ba660(WIFI_MATCHROOM *wk, MCR_PCANM *anm) {
    func_ov099_021ba674(anm);
    func_ov099_021ba6dc(anm);
}

// The screens of the PCs nobody uses blink
// pokebw2's WifiMcr_PcAnmFreeMain
static void func_ov099_021ba674(MCR_PCANM *anm) {
    anm->wait--;
    if (anm->wait <= 0) {
        u32 color;
        int i;

        anm->frame = (anm->frame + 1) % 2;
        anm->wait = data_ov099_021c2878[anm->frame];

        if (anm->frame == 1) {
            color = 2;
        } else {
            color = 1;
        }

        for (i = 0; i < WIFI_MCR_PC_NUM; i++) {
            if (anm->freeBits & (1 << i)) {
                NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, WIFI_MCR_PCANM_COLOR(i),
                                                   func_ov099_021ba764(anm, color), sizeof(u16));
            }
        }
    }
}

// pokebw2's WifiMcr_PcAnmUseMain
static void func_ov099_021ba6dc(MCR_PCANM *anm) {
    u32 trans = FALSE;
    u32 color = 0;
    BOOL next = FALSE;

    if (data_ov099_021c287c[anm->seq].timed == TRUE) {
        anm->seqWait--;
        if (anm->seqWait <= 0) {
            next = TRUE;
        }
    } else {
        if (anm->nextReq) {
            anm->nextReq = FALSE;
            next = TRUE;
        }
    }

    if (next) {
        anm->seq++;
        if (anm->seq >= WIFI_MCR_PCANM_STEP_NUM) {
            func_ov099_021ba770(anm);
            return;
        }

        anm->seqWait = data_ov099_021c287c[anm->seq].wait;
        trans = data_ov099_021c287c[anm->seq].trans;
        color = data_ov099_021c287c[anm->seq].color;
    }

    if (trans) {
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, WIFI_MCR_PCANM_COLOR(anm->pcNo),
                                           func_ov099_021ba764(anm, color), sizeof(u16));
    }
}

// pokebw2's WifiMcr_PcAnmColorGet
static const void *func_ov099_021ba764(const MCR_PCANM *anm, u32 color) {
    const u16 *colors = anm->palette->pRawData;

    return &colors[color + 1];
}

// The PC goes back to blinking
// pokebw2's WifiMcr_PcAnmUseEnd
static void func_ov099_021ba770(MCR_PCANM *anm) {
    anm->freeBits |= 1 << anm->pcNo;
    anm->pcNo = 0;
    anm->seq = 0;
    anm->seqWait = 0;
    anm->nextReq = FALSE;
}

// pokebw2's WifiMcr_PcAnmNextReq
static void func_ov099_021ba78c(MCR_PCANM *anm) {
    anm->nextReq = TRUE;
}

// pokebw2's WifiMcr_PcAnmUseStart
static void func_ov099_021ba794(MCR_PCANM *anm, u8 pcNo) {
    if (anm->seq != 0) {
        func_ov099_021ba770(anm);
    }

    anm->pcNo = pcNo;
    anm->freeBits ^= 1 << pcNo;
    func_ov099_021ba78c(anm);
}

// pokebw2's WifiMcr_PcAnmUseStop
static void func_ov099_021ba7bc(MCR_PCANM *anm) {
    if (anm->seq != 0) {
        func_ov099_021ba78c(anm);
    }
}
