// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov139/ui_scene.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// ui_scene.c: scenes that step through a table of steps, and the OBJ resources of the lower screen's parts. The names
// are ours

#include "app/ui/ui_scene.h"
#include "types.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/std.h"

enum {
    UI_SCENE_STATE_BEGIN,
    UI_SCENE_STATE_ENTER,
    UI_SCENE_STATE_RUN,
    UI_SCENE_STATE_LEAVE,
    UI_SCENE_STATE_NEXT,
};

// No next step set
#define UI_SCENE_NONE 0xffff

// pokebw2's UIScene_Create
UIScene *func_ov067_021b6100(HeapID heapId, const UISceneStep *steps, u8 flag, u16 first, void *param) {
    UIScene *scene = GFL_HeapAllocate(heapId, sizeof(UIScene), FALSE, "ui_scene.c", 78);

    MI_CpuFill8(scene, 0, sizeof(UIScene));
    scene->steps = steps;
    scene->flag = flag;
    scene->param = param;
    scene->step = first;
    scene->next = UI_SCENE_NONE;
    return scene;
}

// pokebw2's UIScene_Free
void func_ov067_021b613c(UIScene *scene) {
    GFL_HeapFree(scene);
}

// pokebw2's UIScene_Main
BOOL func_ov067_021b6144(UIScene *scene) {
    const UISceneStep *step = &scene->steps[scene->step];

    switch (scene->state) {
    case UI_SCENE_STATE_BEGIN:
        if (step->begin == NULL) {
            scene->state = UI_SCENE_STATE_ENTER;
        } else {
            if (!step->begin(scene, scene->param)) {
                break;
            }
            scene->counter = 0;
            scene->state = UI_SCENE_STATE_ENTER;
            break;
        }
        // fall through
    case UI_SCENE_STATE_ENTER:
        if (step->enter != NULL) {
            step->enter(scene, scene->param);
        }
        scene->state = UI_SCENE_STATE_RUN;
        // fall through
    case UI_SCENE_STATE_RUN:
        if (step->run(scene, scene->param)) {
            scene->counter = 0;
            if (step->leave != NULL) {
                step->leave(scene, scene->param);
            }
            scene->state = UI_SCENE_STATE_LEAVE;
        }
        break;
    case UI_SCENE_STATE_LEAVE:
        if (step->end == NULL) {
            scene->state = UI_SCENE_STATE_NEXT;
        } else {
            if (!step->end(scene, scene->param)) {
                break;
            }
            scene->counter = 0;
            scene->state = UI_SCENE_STATE_NEXT;
            break;
        }
        // fall through
    case UI_SCENE_STATE_NEXT: {
        u16 next = scene->next;

        scene->next = UI_SCENE_NONE;
        scene->step = next;
        if (next == UI_SCENE_END) {
            return TRUE;
        }
        scene->state = UI_SCENE_STATE_BEGIN;
        scene->counter = 0;
        break;
    }
    }
    return FALSE;
}

// pokebw2's UIScene_SetNext
void func_ov067_021b61f8(UIScene *scene, u16 next) {
    scene->next = next;
}

// pokebw2's UIScene_GetCounter
u8 func_ov067_021b61fc(UIScene *scene) {
    return scene->counter;
}

// pokebw2's UIScene_IncCounter
void func_ov067_021b6200(UIScene *scene) {
    scene->counter++;
}

// pokebw2's UIObjRes_Load
void func_ov067_021b6208(UIObjRes *res, UIObjResSetup *setup, ClActUnit *unit, HeapID heapId) {
    u32 compressed = setup->flags & 2;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(setup->arcId, heapId);

    if (setup->flags & 1) {
        res->palette = func_0204acec(arc, setup->paletteFile, setup->vramType, setup->paletteOffset << 5, heapId);
    } else {
        res->palette = func_0204ac18(arc, setup->paletteFile, setup->vramType, setup->paletteOffset << 5,
                                     setup->paletteStart, setup->paletteCount, heapId);
    }
    res->chars = func_0204a6c8(arc, setup->charFile, compressed, setup->vramType, heapId);
    res->cellAnims = func_0204af28(arc, setup->cellFile, setup->animFile, heapId);
    res->vramType = setup->vramType;
    GFL_ArcToolFree(arc);
}

// pokebw2's UIObjRes_Free
void func_ov067_021b6284(UIObjRes *res) {
    func_0204ada4(res->palette);
    func_0204a8d4(res->chars);
    func_0204afd8(res->cellAnims);
}

// pokebw2's UIObjRes_CreateActor
ClActor *func_ov067_021b629c(UIObjRes *res, ClActUnit *unit, u8 x, u8 y, u8 anim, HeapID heapId) {
    ClActorSetup setup = {0};
    ClActor *actor;

    setup.x = x;
    setup.y = y;
    actor = func_0204b294(unit, res->chars, res->palette, res->cellAnims, &setup, res->vramType, heapId);
    func_0204b878(actor, anim);
    return actor;
}
