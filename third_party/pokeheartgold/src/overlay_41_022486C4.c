#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "sys_task.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"

void ov41_02248B20(UnkStruct_ov22_02257F50 *, void *, s32, s32);

void ov41_02249A50(UnkStruct_ov22_02259560 *, UnkStruct_ov22_02259560 *);

void ov41_022486C4(UnkStruct_ov22_02257F50 *arg0, s32 arg1, s32 arg2, UnkStruct_ov22_02259560 *arg3);

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SysTask;
struct SysTaskQueue;
void ov41_02249A60(UnkStruct_ov22_02259560 *arg0);
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
void ov41_02249A70(UnkStruct_ov22_02259560 *param0);
void ov41_022486F0(void *arg0);
void ov41_022486F8(UnkStruct_ov22_02257F50 *param0);

void ov41_022486C4(UnkStruct_ov22_02257F50 *arg0, s32 arg1, s32 arg2, UnkStruct_ov22_02259560 *arg3) {
    ov41_02249A50(arg3, arg0->unk_00.unk_0C[arg1].unk_00 + arg2);
    ov41_02248B20(arg0, arg3->unk_00, arg1, arg2);
}

void ov41_022486F0(void *arg0) {
    ov41_02249A60(arg0);
}

void ov41_022486F8(UnkStruct_ov22_02257F50 *param0) {
    int v0, v1;

    for (v0 = 0; v0 < 3; v0++) {
        for (v1 = 0; v1 < param0->unk_00.unk_0C[v0].unk_04; v1++) {
            ov41_02249A70(param0->unk_00.unk_0C[v0].unk_00 + v1);
        }
    }
}
