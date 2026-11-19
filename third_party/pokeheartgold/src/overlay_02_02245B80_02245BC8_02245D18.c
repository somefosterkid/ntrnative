#include "global.h"

#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_02_02245B80.h"

void ov02_02245D18(LegendDemoScene *scene) {
    HeapExp_FndInitAllocator(&scene->allocator, scene->heapID, 0x20);
    Field3dModel_LoadFromFilesystem(&scene->model, NARC_demo_legend, 0, scene->heapID);
    Field3dModelAnimation_LoadFromFilesystem(&scene->animations[0], &scene->model, NARC_demo_legend, 1, scene->heapID, &scene->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&scene->animations[1], &scene->model, NARC_demo_legend, 2, scene->heapID, &scene->allocator);
    Field3dObject_InitFromModel(&scene->object, &scene->model);
    Field3dObject_AddAnimation(&scene->object, &scene->animations[0]);
    Field3dObject_AddAnimation(&scene->object, &scene->animations[1]);
    Field3dObject_SetPosEx(&scene->object, 0x88000, 0, 0xa0000);
    Field3dObject_SetActiveFlag(&scene->object, 1);
}

void ov02_02245DB0(LegendDemoScene *scene) {
    Field3dObject_SetActiveFlag(&scene->object, 0);
    Field3dModelAnimation_Unload(&scene->animations[1], &scene->allocator);
    Field3dModelAnimation_Unload(&scene->animations[0], &scene->allocator);
    Field3dModel_Unload(&scene->model);
}

void ov02_02245DE0(LegendDemoScene *scene) {
    Field3dModelAnimation_FrameAdvanceAndLoop(&scene->animations[0], 0x1000);
    Field3dModelAnimation_FrameAdvanceAndLoop(&scene->animations[1], 0x1000);
    Field3dObject_Draw(&scene->object);
}
