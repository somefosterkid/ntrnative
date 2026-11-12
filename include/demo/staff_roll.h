// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/demo/staff_roll.h: the lines in use so far.
#ifndef BWPORT_DEMO_STAFF_ROLL_H
#define BWPORT_DEMO_STAFF_ROLL_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx.h"

// Black 2's, not checked against Black
// The parameter of the process, which the game clear event fills (event_game_clear.c)
typedef struct {
    // Whether START skips the credits; they then show no pictures
    BOOL skippable;
    u32 unk04;
    PlayerInfo *playerInfo;
} StaffRollParam;

// The bitmaps the pairs of lines are drawn into ahead of time
#define LINE_BITMAP_MAX 64

// An entry of the list, file 1 of the end archive
typedef struct {
    u32 msgId;
    // The frames to wait after it, or the y of a title
    u16 wait;
    u8 command;
    // The screens of a title or a picture, the picture's index, or how far ahead the line of CMD_LINE_AT is
    u8 arg;
    u8 color;
    u8 font;
    // How the line is aligned, or the screen of a picture
    u16 align;
    // The x of the line, or the frames a picture fades over
    s16 x;
    s16 offset;
} StaffRollLine;

typedef struct {
    GFLBitmap *bitmap;
    u32 line;
} StaffRollLineBitmap;

// The keys of the camera: where it looks from and at
typedef struct {
    VecFx32 pos;
    VecFx32 target;
} StaffRollCameraKey;

// The camera of the 3D background, which moves from key to key
typedef struct {
    const StaffRollCameraKey *keys;
    VecFx32 pos;
    VecFx32 target;
    // How far pos and target move each frame
    VecFx32 posStep;
    VecFx32 targetStep;
    // The moves waiting to start, key 0xffff for a free slot
    struct {
        u16 key;
        u16 frames;
    } queue[16];
    u16 unk_74;
    // The key the camera is at
    u16 current;
    // How far into the move it is, and how many frames the move takes
    u16 frame;
    u16 frames;
    BOOL active;
} StaffRollCamera;

typedef struct {
    StaffRollParam *param;
    TCB *vblankTask;
    G3DManager *g3dManager;
    G3DCamera *g3dCamera;
    G3DLight *g3dLight;
    u32 unk_14;
    u16 scene;
    // Fade of the 3D background
    fx32 backgroundBlend;
    G3DActor *actor;
    u32 actorAnimCount;
    StaffRollCamera camera;
    Font *fonts[2];
    MsgData *msgData;
    StrBuf *strbuf;
    StaffRollLineBitmap lineBitmaps[LINE_BITMAP_MAX];
    u16 lineBitmapCount;
    u8 drawnLineBitmaps;
    u8 shownLineBitmaps;
    BmpWin *topWindow;
    u32 unk_2c0;
    BmpWin *bottomWindow;
    u32 unk_2c8;
    // The windows whose characters are copied at the next VBlank, SCREEN_*
    u8 flushScreens;
    u8 unk231;
    u8 fadeState;
    u8 fadeScreens;
    s8 fadeAlpha;
    u8 fastForwardTimer;
    // Runs the credits four times faster, which nothing sets
    u8 fastForward;
    StaffRollLine *lines;
    u16 lineIndex;
    s16 waitFrames;
    fx32 scrollFraction;
    BOOL scrolling;
    u32 frames;
    u32 seq;
    u32 state;
    u32 nextSeq;
} StaffRollWork;

BOOL func_ov181_021b95f8(StaffRollWork *work);

#endif // BWPORT_DEMO_STAFF_ROLL_H
