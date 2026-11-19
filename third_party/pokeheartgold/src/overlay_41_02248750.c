#include "global.h"

#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "filesystem_files_def.h"
#include "overlay_41_unk_struct_ov22_02259560.h"

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
void ov41_02248724(UnkStruct_ov22_02257F50 *param0);
void ov41_02248B84(UnkStruct_ov22_02257F50 *param0, int param1, int param2, int param3, int param4);

BOOL ov41_02248750(UnkStruct_ov22_02257F50 *param0, int param1, int param2);

BOOL ov41_02248750(UnkStruct_ov22_02257F50 *param0, int param1, int param2) {
    if (param0->unk_00.unk_3C == 0) {
        ov41_02248B84(param0, param0->unk_00.unk_00, param0->unk_00.unk_0C[param0->unk_00.unk_00].unk_08, param1, param2);

        param0->unk_00.unk_00 = param1;
        param0->unk_00.unk_0C[param1].unk_08 = param2;

        ov41_02248724(param0);

        return 1;
    }

    return 0;
}
