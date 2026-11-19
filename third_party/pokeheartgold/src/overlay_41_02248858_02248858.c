#include "global.h"

#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

BOOL ov41_02249AA8(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, NNSG2dCharacterData **param3);

UnkStruct_ov22_02259560 *ov41_02248A94(UnkStruct_ov22_02257F50 *param0);

UnkStruct_ov22_02259560 *ov41_02248858(UnkStruct_ov22_02257F50 *param0, int *param1, int *param2, NNSG2dCharacterData **param3);

UnkStruct_ov22_02259560 *ov41_02248858(UnkStruct_ov22_02257F50 *param0, int *param1, int *param2, NNSG2dCharacterData **param3) {
    UnkStruct_ov22_02259560 *v0;
    UnkStruct_ov22_02259560 *v1;
    BOOL v2;

    v1 = ov41_02248A94(param0);
    v0 = v1->unk_08;

    while (v0 != v1) {
        v2 = 0;

        v2 = ov41_02249AA8(v0, param1, param2, param3);

        if (v2 == 1) {
            return v0;
        }

        v0 = v0->unk_08;
    }

    return NULL;
}
