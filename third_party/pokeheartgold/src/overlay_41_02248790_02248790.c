#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

extern BOOL ov41_02248750(UnkStruct_ov22_02257F50 *param0, int param1, int param2);

int ov41_0224895C(UnkStruct_ov22_02257F50 *param0, int param1);

extern UnkStruct_ov22_02259560 *ov41_02248ABC(UnkStruct_ov22_02257F50 *param0, int param1, int param2);

BOOL ov41_02248790(UnkStruct_ov22_02257F50 *param0, int param1, int param2);

BOOL ov41_02248790(UnkStruct_ov22_02257F50 *param0, int param1, int param2) {
    UnkStruct_ov22_02259560 *v0;
    int v1;
    int v3;
    int v4;
    BOOL v5;

    v1 = ov41_0224895C(param0, param1);

    for (v3 = 1; v3 < param0->unk_00.unk_0C[param1].unk_04; v3++) {
        if (param2 == 0) {
            v4 = (v3 + v1) % param0->unk_00.unk_0C[param1].unk_04;
        } else {
            v4 = v1 - v3;

            if (v4 < 0) {
                v4 += param0->unk_00.unk_0C[param1].unk_04;
            }
        }

        v0 = ov41_02248ABC(param0, param1, v4);

        if (v0->unk_08 != v0) {
            v5 = ov41_02248750(param0, param1, v4);
            return v5;
        }
    }

    return 0;
}
