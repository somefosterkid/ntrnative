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
UnkStruct_ov22_02259560 *ov41_02248ABC(UnkStruct_ov22_02257F50 *param0, int param1, int param2);

UnkStruct_ov22_02259560 *ov41_02248AFC(UnkStruct_ov22_02257F50 *param0, int param1, int param2, int param3);

UnkStruct_ov22_02259560 *ov41_02248AFC(UnkStruct_ov22_02257F50 *param0, int param1, int param2, int param3) {
    UnkStruct_ov22_02259560 *v0;
    UnkStruct_ov22_02259560 *v1;
    int v2 = 0;

    v0 = ov41_02248ABC(param0, param1, param2);
    v1 = v0->unk_08;

    while (v1 != v0) {
        if (v2 == param3) {
            return v1;
        }

        v2++;
        v1 = v1->unk_08;
    }

    return 0L;
}
