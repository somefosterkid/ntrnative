// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ui/ui_scene.h: the lines in use so far.
#ifndef BWPORT_APP_UI_UI_SCENE_H
#define BWPORT_APP_UI_UI_SCENE_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/clact.h"

// The OBJ resources of a set of files, and where to load them from
typedef struct {
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u32 vramType;
} UIObjRes;

typedef struct {
    u32 vramType;
    // Bit 0 loads the whole palette, bit 1 means the characters are compressed
    u32 flags;
    u32 arcId;
    u32 paletteFile;
    u32 charFile;
    u32 cellFile;
    u32 animFile;
    u8 paletteOffset;
    u8 paletteStart;
    u8 paletteCount;
} UIObjResSetup;

// pokebw2's UIObjRes_Load
void func_ov067_021b6208(UIObjRes *res, UIObjResSetup *setup, ClActUnit *unit, HeapID heapId);
// pokebw2's UIObjRes_CreateActor
// Creates an actor of the resources at (x, y), playing an animation
ClActor *func_ov067_021b629c(UIObjRes *res, ClActUnit *unit, u8 x, u8 y, u8 anim, HeapID heapId);
// pokebw2's UIObjRes_Free
void func_ov067_021b6284(UIObjRes *res);

// ui_scene.c: A scene steps through a table of steps, each with callbacks for its parts. A callback that returns a
// BOOL says whether its part is done; the steps go on to the step set with UIScene_SetNext, or end with UI_SCENE_END
typedef struct UIScene UIScene;

// The next step that ends the scene
#define UI_SCENE_END 0xfffe

// Black's
typedef BOOL (*UISceneFunc)(UIScene *scene, void *param);
typedef void (*UISceneHook)(UIScene *scene, void *param);

typedef struct {
    UISceneFunc begin;
    UISceneHook enter;
    UISceneFunc run;
    UISceneHook leave;
    UISceneFunc end;
} UISceneStep;

struct UIScene {
    const UISceneStep *steps;
    void *param;
    u16 step;
    u16 next;
    u8 flag;
    u8 state;
    u8 counter;
};

// pokebw2's UIScene_Create
UIScene *func_ov067_021b6100(HeapID heapId, const UISceneStep *steps, u8 flag, u16 first, void *param);
// pokebw2's UIScene_Main
// Runs the current step, and returns TRUE once the scene has ended
BOOL func_ov067_021b6144(UIScene *scene);
// pokebw2's UIScene_GetCounter
u8 func_ov067_021b61fc(UIScene *scene);
// pokebw2's UIScene_IncCounter
void func_ov067_021b6200(UIScene *scene);

// Black's
#define OVERLAY_APP_UI OVERLAY_ID(67)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's UIScene_Free
void func_ov067_021b613c(UIScene *scene);
// pokebw2's UIScene_SetNext
void func_ov067_021b61f8(UIScene *scene, u16 next);

#endif // BWPORT_APP_UI_UI_SCENE_H
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ui/ui_scene.h: the lines in use so far.
#ifndef BWPORT_APP_UI_UI_SCENE_H

#include "types.h"
#include "gfl/overlay.h"

// Black's

// Black's

// pokebw2's UIObjRes_Load
// pokebw2's UIObjRes_CreateActor
// Creates an actor of the resources at (x, y), playing an animation
// pokebw2's UIObjRes_Free

// Black's

#endif // BWPORT_APP_UI_UI_SCENE_H
