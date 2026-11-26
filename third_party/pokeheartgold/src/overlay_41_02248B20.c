#include "global.h"

#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "filesystem_files_def.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "touchscreen.h"
#include "constants/heap.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

void ov41_02246008(UnkStruct_ov22_02255040 *param0, BOOL param1);

void ov41_02248B20(UnkStruct_ov22_02257F50 *param0, UnkStruct_ov22_02255040 *param1, int param2, int param3);

void ov41_02248B48(int param0, int *param1, int *param2);

void ov41_02248B20(UnkStruct_ov22_02257F50 *param0, UnkStruct_ov22_02255040 *param1, int param2, int param3) {
    if ((param0->unk_00.unk_00 != param2) || (param0->unk_00.unk_0C[param2].unk_08 != param3)) {
        ov41_02246008(param1, 0);
    } else {
        ov41_02246008(param1, 1);
    }
}

void ov41_02248B48(int param0, int *param1, int *param2) {
    int v0;
    int v1;

    param0 %= 9;

    v0 = param0 / 3;
    v1 = param0 % 3;

    *param2 = 8 * (v0 + 1);
    *param2 += 32 * v0;
    *param2 += 16;
    *param1 = 8 * (v1 + 1);
    *param1 += 24 * v1;
    *param1 += 8;
}
