// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_p2pmatchroom.h: the lines in use so
// far.
#ifndef BWPORT_APP_WIFI_P2PMATCHROOM_H
#define BWPORT_APP_WIFI_P2PMATCHROOM_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "app/wf2dmap/wf2dmap_cmdq.h"
#include "app/wf2dmap/wf2dmap_map.h"
#include "app/wf2dmap/wf2dmap_obj.h"
#include "app/wf2dmap/wf2dmap_objdraw.h"
#include "app/wf2dmap/wf2dmap_scrdraw.h"
#include "app/wf2dmap/wf2dmap_scroll.h"
#include "gfl/clact.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"

// The match board's graphics archive, which arc.h has no name for yet
// Black's (near twins)
#define ARCID_WIFIP2PMATCH 33

// How many characters the room holds: the player and the friends
#define WIFI_MCR_OBJ_MAX 48

// How many friends a block of the room seats, and so how many each of the room's maps adds
#define WIFI_MCR_BLOCK_FRIEND_NUM 8

// The PCs of the room, one per block
#define WIFI_MCR_PC_NUM 4

// Black 2's; twinned code confirms the values of WIFI_MCR_MAPPM_PC, WIFI_MCR_MAPPM_BLOCK0, WIFI_MCR_MAPPM_BLOCK3,
// WIFI_MCR_MAPPM_FRIEND0 and WIFI_MCR_MAPPM_EXIT in Black
// What a square of the room's maps is (its WF2DMAP_MAP param)
enum {
    WIFI_MCR_MAPPM_NONE,
    WIFI_MCR_MAPPM_WALL,
    WIFI_MCR_MAPPM_PC,
    // The square under a friend's seat tells the block it belongs to
    WIFI_MCR_MAPPM_BLOCK0,
    WIFI_MCR_MAPPM_BLOCK1,
    WIFI_MCR_MAPPM_BLOCK2,
    WIFI_MCR_MAPPM_BLOCK3,
    // The seats of a block's friends
    WIFI_MCR_MAPPM_FRIEND0,
    WIFI_MCR_MAPPM_FRIEND1,
    WIFI_MCR_MAPPM_FRIEND2,
    WIFI_MCR_MAPPM_FRIEND3,
    WIFI_MCR_MAPPM_FRIEND4,
    WIFI_MCR_MAPPM_FRIEND5,
    WIFI_MCR_MAPPM_FRIEND6,
    WIFI_MCR_MAPPM_FRIEND7,
    // Where the player comes in and leaves
    WIFI_MCR_MAPPM_EXIT,
};

// What WIFI_MCR_Main returns
enum {
    WIFI_MCR_RET_NONE,
    // Down was held on the exit
    WIFI_MCR_RET_CANCEL,
    // A was pressed in front of a friend: WIFI_MCR_PlayerSelect tells which
    WIFI_MCR_RET_SELECT,
    // A was pressed in front of a PC
    WIFI_MCR_RET_MYSELECT,
};

// How a friend's character behaves
enum {
    // Turns to a random way now and then
    WIFI_MCR_NPC_MOVE_NORMAL,
    // Jumps on the spot
    WIFI_MCR_NPC_MOVE_JUMP,
};

// What a character of the room is doing
enum {
    WIFI_MCR_MOVESTATE_PLAYER,
    WIFI_MCR_MOVESTATE_NPC_IN,
    WIFI_MCR_MOVESTATE_NPC_OUT,
    WIFI_MCR_MOVESTATE_NPC_NORMAL,
    WIFI_MCR_MOVESTATE_NPC_JUMP,
    WIFI_MCR_MOVESTATE_NPC_PAUSE,
};

typedef struct WIFI_MATCHROOM WIFI_MATCHROOM;
typedef struct MCR_MOVEOBJ MCR_MOVEOBJ;

// Black 2's, not checked against Black
// Returns TRUE when the character is to be deleted
typedef BOOL (*MCR_MOVEFUNC)(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);

// Black 2's, not checked against Black
typedef void (*MCR_DRAWFUNC)(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);

// A character of the room
struct MCR_MOVEOBJ {
    WF2DMAP_OBJWK *obj;
    WF2DMAP_OBJDRAWWK *draw;
    s16 moveCount;
    // The friend the player pressed A in front of, or 0
    u8 accessFriend;
    // WIFI_MCR_NPC_MOVE_*
    u8 moveId;
    // WIFI_MCR_MOVESTATE_*, and what it was before the pause
    u16 moveState;
    u16 moveStateTmp;
    MCR_MOVEFUNC move;
    // What move was before the pause
    MCR_MOVEFUNC moveTmp;
    MCR_DRAWFUNC drawFunc;
};

// The cell actors of the room: the arrow on the exit and the cursor over a friend
typedef struct {
    ClActUnit *unit;
    ClActRenderer *renderer;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    ClActor *exitCursor;
    ClActor *cursor;
} MCR_CLACT;

// The palette animation of the PCs' screens
typedef struct {
    void *buffer;
    NNSG2dPaletteData *palette;
    // A bit per PC that nobody uses, whose screen blinks
    u8 freeBits;
    u8 frame;
    s16 wait;
    // The PC the player is using, and the step of its animation
    u8 pcNo;
    u8 seq;
    s16 seqWait;
    // Set to go on from a step that waits
    u8 nextReq;
} MCR_PCANM;

// Black 2's; matching code confirms the offsets of init, heapId, playerPause, unk10, clact, objs, player, pcAnm,
// mapSys, objSys, objDrawSys, scroll, scrDraw and cmdQueue, and its size in Black
struct WIFI_MATCHROOM {
    BOOL init;
    u32 heapId;
    u32 unk8;
    // Whether the player's keys are ignored
    BOOL playerPause;
    // Never set
    void *unk10;
    MCR_CLACT clact;
    MCR_MOVEOBJ objs[WIFI_MCR_OBJ_MAX];
    MCR_MOVEOBJ *player;
    MCR_PCANM pcAnm;
    WF2DMAP_MAPSYS *mapSys;
    WF2DMAP_OBJSYS *objSys;
    WF2DMAP_OBJDRAWSYS *objDrawSys;
    WF2DMAP_SCROLL scroll;
    WF2DMAP_SCRDRAW *scrDraw;
    WF2DMAP_REQCMDQ *cmdQueue;
};

// pokebw2's WIFI_MCR_Dest
void func_ov099_021b9670(WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_Main
// Returns a WIFI_MCR_RET_*
u32 func_ov099_021b96d0(WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_PlayerSelect
// The friend number the player pressed A in front of, or 0
u8 func_ov099_021b97f4(const WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_GetPlayerOnMapParam
// The WIFI_MCR_MAPPM_* of the square the player stands on, and of the square below it
u32 func_ov099_021b9800(const WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_GetPlayerOnUnderMapParam
u32 func_ov099_021b984c(const WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_CursorOn
// Shows the cursor over a character, whose shadow it hides
void func_ov099_021b989c(WIFI_MATCHROOM *wk, const MCR_MOVEOBJ *obj);
// pokebw2's WIFI_MCR_ShadowOn
void func_ov099_021b98e4(WIFI_MATCHROOM *wk, const MCR_MOVEOBJ *obj);
// pokebw2's WIFI_MCR_SetPlayer
MCR_MOVEOBJ *func_ov099_021b9900(WIFI_MATCHROOM *wk, u32 view);
// pokebw2's WIFI_MCR_SetNpc
// friendNo is the friend's index in the friend list, plus 1. Returns NULL when the player stands on the friend's seat
MCR_MOVEOBJ *func_ov099_021b9968(WIFI_MATCHROOM *wk, u32 view, u32 friendNo);
// pokebw2's WIFI_MCR_DelPeopleReq
// Makes the character spin away, after which it is deleted
void func_ov099_021b99f4(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
// pokebw2's WIFI_MCR_DelPeople
void func_ov099_021b9a34(MCR_MOVEOBJ *obj);
// pokebw2's WIFI_MCR_GetFriendNo
u8 func_ov099_021b9a50(const MCR_MOVEOBJ *obj);
// pokebw2's WIFI_MCR_GetRetWay
// The way opposite to the one the character faces
u32 func_ov099_021b9a60(const MCR_MOVEOBJ *obj);
// pokebw2's WIFI_MCR_NpcPauseOn
void func_ov099_021b9a70(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, u32 way);
// pokebw2's WIFI_MCR_NpcPauseOff
void func_ov099_021b9ae0(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj);
// pokebw2's WIFI_MCR_NpcMoveSet
// moveId is a WIFI_MCR_NPC_MOVE_*
void func_ov099_021b9b00(WIFI_MATCHROOM *wk, MCR_MOVEOBJ *obj, u32 moveId);
// pokebw2's WIFI_MCR_PCAnmStart
// The animation of the PC in front of the player
void func_ov099_021b9b24(WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_PCAnmOff
void func_ov099_021b9b54(WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_PCAnmMain
void func_ov099_021b9b64(WIFI_MATCHROOM *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WIFI_MCR_Init
// heroView is the player's character (a WF_2DC_CHARA_*), friendNum the number of friends in the friend list, which
// picks the map, and arcId the archive of arc
void func_ov099_021b95a0(WIFI_MATCHROOM *wk, u32 heapId, ArcTool *arc, u32 heroView, u32 friendNum, u32 arcId,
                   ClActUnit *unit, ClActRenderer *renderer);
// pokebw2's WIFI_MCR_GetInitFlag
BOOL func_ov099_021b96cc(const WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_CursorOff
void func_ov099_021b98f0(WIFI_MATCHROOM *wk);
// pokebw2's WIFI_MCR_PlayerMovePause
void func_ov099_021b98f8(WIFI_MATCHROOM *wk, BOOL pause);
// pokebw2's WIFI_MCR_PlayerMovePauseGet
BOOL func_ov099_021b98fc(const WIFI_MATCHROOM *wk);

#endif // BWPORT_APP_WIFI_P2PMATCHROOM_H
