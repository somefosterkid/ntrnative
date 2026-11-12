// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/demo3d/demo3d_data.h: the lines in use
// so far.
#ifndef BWPORT_APP_DEMO3D_DEMO3D_DATA_H
#define BWPORT_APP_DEMO3D_DEMO3D_DATA_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "nitro/gx.h"
#include "struct_decls.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

// pokebw2's Demo3DData_GetFadeParams
void func_ov203_021be948(u32 demoId, u8 fadeOut, u8 *white, u8 *frames);

// Black's: the 0xa1 that demo3d_cmdsys.s loads, the archive after ARCID_DEMO3D_RESOURCE. Our name
#define ARCID_DEMO3D_2D 161

// Black 2's, not checked against Black
// The commands of demo3d_cmd.c, in the order of its table
enum {
    DEMO3D_CMD_NONE,
    DEMO3D_CMD_SE_PLAY,
    DEMO3D_CMD_SE_STOP,
    DEMO3D_CMD_SE_VOLUME_FADE,
    DEMO3D_CMD_SE_PAN_FADE,
    DEMO3D_CMD_SE_PITCH_FADE,
    DEMO3D_CMD_BGM_PLAY,
    DEMO3D_CMD_BGM_STOP,
    DEMO3D_CMD_BGM_FADE,
    DEMO3D_CMD_BGM_CHANGE,
    DEMO3D_CMD_BRIGHTNESS_FADE,
    DEMO3D_CMD_FLASH,
    DEMO3D_CMD_LIGHT_COLOR,
    DEMO3D_CMD_LIGHT_VECTOR,
    DEMO3D_CMD_MESSAGE,
    DEMO3D_CMD_TELOP,
    DEMO3D_CMD_CAPTURE_START,
    DEMO3D_CMD_CAPTURE_END,
    DEMO3D_CMD_END,
    DEMO3D_CMD_COUNT = DEMO3D_CMD_END
};

// Black 2's, not checked against Black
// The frame of a command that runs when the demo starts, whatever frame it starts at
#define DEMO3D_CMD_FRAME_START (-1)

// Black 2's, not checked against Black
#define DEMO3D_CMD_PARAM_COUNT 8

// Black 2's; twinned code confirms the offsets of type and params in Black
typedef struct {
    u32 type;
    // In frames of the camera's animation
    s32 frame;
    s32 params[DEMO3D_CMD_PARAM_COUNT];
} Demo3DCmd;

// Black 2's; twinned code confirms the offsets of demoId, param, dayPeriod, playerSex and season in Black
// What the engine picks a demo's models with
typedef struct {
    u32 demoId;
    u32 startFrame;
    u8 param;
    u8 dayPeriod;
    u8 playerSex;
    u8 hour;
    u8 minute;
    u8 season;
} Demo3DSceneParam;

// Black 2's; twinned code confirms the values of DEMO3D_BGM_CHANGE_NOW, DEMO3D_BGM_CHANGE_STOP, DEMO3D_BGM_CHANGE_PUSH
// and DEMO3D_BGM_CHANGE_POP in Black
// How DEMO3D_CMD_BGM_CHANGE changes the BGM once the old one has faded out
enum {
    DEMO3D_BGM_CHANGE_NOW,
    DEMO3D_BGM_CHANGE_STOP,
    DEMO3D_BGM_CHANGE_PLAY,
    DEMO3D_BGM_CHANGE_PUSH,
    DEMO3D_BGM_CHANGE_POP,
};

// Black 2's; twinned code confirms the offsets of halfRate, fadeInWhite, fadeInFrames, fadeOutWhite and fadeOutFrames
// in Black
struct Demo3DSetup {
    // The camera's animation in ARCID_DEMO3D_RESOURCE
    u16 cameraFile;
    u16 unk2;
    // The zone whose light the demo takes
    u16 zoneId;
    // The camera's animation has 30 frames a second: every frame takes two
    u16 halfRate : 1;
    u16 dual3D : 1;
    u16 alphaBlend : 1;
    u16 antiAlias : 1;
    u16 fog : 1;
    u16 edgeMarking : 1;
    u16 sortMode : 1;
    u16 bufferMode : 1;
    u16 alphaTest : 1;
    u16 alphaTestRef : 5;
    // The projection's angles in degrees, the sine's and the cosine's
    fx32 fovySin;
    fx32 fovyCos;
    fx32 aspect;
    fx32 near;
    fx32 far;
    fx32 scaleW;
    // How far each screen's view is moved up or down when the demo draws on both
    s16 mainOffset;
    s16 subOffset;
    fx32 polygon1DotDepth;
    GXRgb clearColor;
    u16 clearAlpha;
    u16 clearPolygonId : 15;
    u16 clearDepth;
    GXRgb fogColor;
    u8 fogAlpha;
    u8 fogShift;
    u16 fogOffset : 15;
    u16 fogAlphaMode : 1;
    u8 fogTable[8];
    GXRgb edgeColors[8];
    // Whether the demo fades in from white and how many frames it takes, and the same for its fade-out
    u8 fadeInWhite;
    u8 fadeInFrames;
    u8 fadeOutWhite;
    u8 fadeOutFrames;
};

// Black's: the ids 1 to 18 and 0, which has no demo
#define DEMO3D_ID_COUNT 19

// Black 2's; twinned code confirms the values of DEMO3D_UNIT_BY_SEX, DEMO3D_UNIT_BY_PARAM and DEMO3D_UNIT_BY_SEASON in
// Black
// What picks one of a model's variants
enum {
    DEMO3D_UNIT_SINGLE,
    DEMO3D_UNIT_BY_SEX,
    DEMO3D_UNIT_BY_PARAM,
    DEMO3D_UNIT_BY_SEASON,
};

// A model's files in ARCID_DEMO3D_RESOURCE: the model and then its animations
struct Demo3DFiles {
    const u16 *files;
    u32 count;
};

// A variant of a model: one set of files, or one for each period of the day
struct Demo3DVariant {
    const Demo3DFiles *files;
    u32 count;
};

struct Demo3DUnit {
    const Demo3DVariant *variants;
    u16 count;
    u16 type;
};

// Black's, in no file yet: pokebw2's Demo3DData_CheckId, which the functions here call. The demo's number if it is 1
// to 18, else 0
u32 func_ov203_021be79c(u32 demoId);
// pokebw2's Demo3DData_GetSetup
const Demo3DSetup *func_ov203_021be7a8(u32 demoId);
// Loads the camera's animation of a demo: the file its set-up names, from the demo resource archive
G3DCurve *func_ov203_021be7bc(u32 demoId, HeapID heapId, u32 preloadCount);
// pokebw2's Demo3DData_GetUnitCount
u8 func_ov203_021be7d8(u32 demoId);
// pokebw2's Demo3DData_FreeSceneSetup
void func_ov203_021be85c(G3DSceneSetup *setup);
// pokebw2's Demo3DData_SetSceneSetup
void func_ov203_021be874(G3DSceneSetup *setup, const Demo3DSceneParam *param, u8 unit);
// pokebw2's Demo3DData_GetFrameStep
// How far the camera's animation goes in a frame
fx32 func_ov203_021be920(u32 demoId);
// pokebw2's Demo3DData_IsDual3D
BOOL func_ov203_021be938(u32 demoId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Demo3DData_CreateSceneSetup
// A scene set-up of one actor with up to 5 animations, which Demo3DData_SetSceneSetup fills in for a model
G3DSceneSetup *func_ov203_021be7f0(HeapID heapId);
// pokebw2's Demo3DData_GetCmds
const Demo3DCmd *func_ov203_021be990(u32 demoId);
// pokebw2's Demo3DData_GetEndCmds
// The commands that run when the demo ends
const Demo3DCmd *func_ov203_021be9a4(u32 demoId);

#endif // BWPORT_APP_DEMO3D_DEMO3D_DATA_H
