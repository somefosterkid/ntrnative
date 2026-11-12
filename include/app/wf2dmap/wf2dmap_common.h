// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_common.h: the lines in
// use so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_COMMON_H
#define BWPORT_APP_WF2DMAP_WF2DMAP_COMMON_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"

// A position in pixels
typedef struct {
    s16 x;
    s16 y;
} WF2DMAP_POS;

enum {
    WF2DMAP_WAY_UP,
    WF2DMAP_WAY_DOWN,
    WF2DMAP_WAY_LEFT,
    WF2DMAP_WAY_RIGHT,
    WF2DMAP_WAY_NUM,
};

// Black 2's; twinned code confirms the values of WF2DMAP_OBJST_NONE, WF2DMAP_OBJST_TURN, WF2DMAP_OBJST_WALK,
// WF2DMAP_OBJST_RUN, WF2DMAP_OBJST_BUSY, WF2DMAP_OBJST_WALLWALK, WF2DMAP_OBJST_SLOWWALK, WF2DMAP_OBJST_WALK4,
// WF2DMAP_OBJST_STAYWALK2, WF2DMAP_OBJST_STAYWALK4, WF2DMAP_OBJST_STAYWALK8 and WF2DMAP_OBJST_STAYWALK16 in Black
// What an object is doing. A command (WF2DMAP_REQCMD, WF2DMAP_ACTCMD) asks for one of them by the same number. The
// number in a name is how many frames it lasts
enum {
    WF2DMAP_OBJST_NONE,
    // Face another way, 2 frames
    WF2DMAP_OBJST_TURN,
    // Move one square, in 8, 4 and 16 frames
    WF2DMAP_OBJST_WALK,
    WF2DMAP_OBJST_RUN,
    // Stand and take no requests but WF2DMAP_OBJST_NONE
    WF2DMAP_OBJST_BUSY,
    // Walk on the spot against a wall or another object, 16 frames
    WF2DMAP_OBJST_WALLWALK,
    WF2DMAP_OBJST_SLOWWALK,
    // Move one square in 4 frames, drawn as WF2DMAP_OBJST_STAYWALK4
    WF2DMAP_OBJST_WALK4,
    // Walk on the spot
    WF2DMAP_OBJST_STAYWALK2,
    WF2DMAP_OBJST_STAYWALK4,
    WF2DMAP_OBJST_STAYWALK8,
    WF2DMAP_OBJST_STAYWALK16,
    WF2DMAP_OBJST_NUM,
};

// A player's request, which wf2dmap_judge.c turns into an action
typedef struct {
    u16 cmd;
    u8 way;
    u8 playId;
} WF2DMAP_REQCMD;

// An action for an object, which starts from pos
typedef struct {
    WF2DMAP_POS pos;
    u16 cmd;
    u8 way;
    u8 playId;
} WF2DMAP_ACTCMD;

// The side of a grid square, in pixels
#define WF2DMAP_GRID_SIZE 16

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_COMMON_H
