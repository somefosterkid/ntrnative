#include "global.h"

#include "heap.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "filesystem_files_def.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "bg_window.h"

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SysTask;
struct SysTaskQueue;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
void ov41_022488D8(UnkStruct_ov22_02257F50 *param0, int param1, int param2, int param3, BOOL *param4);
void ov41_02248984(UnkStruct_ov22_02257F50 *param0, int param1, int param2, int param3, int param4);
void DestroySysTaskAndEnvironment(SysTask *task);
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov22_0225886C;
typedef struct {
    UnkStruct_ov22_02257F50 *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    BOOL unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    UnkStruct_ov22_0225886C *unk_28;
    int unk_2C;
} UnkStruct_ov22_022586C4;
UnkStruct_ov22_02259560 *ov41_02248AFC(UnkStruct_ov22_02257F50 *param0, int param1, int param2, int param3);
void ov41_02248D64(UnkStruct_ov22_02259560 *param0, UnkStruct_ov22_0225886C *param1, int param2);
void ov41_02248DA4(UnkStruct_ov22_0225886C *param0, int param1);
int ov41_02248E10(int param0, int param1);
void ov41_02248A08(UnkStruct_ov22_02257F50 *param0, int param1, int param2, int param3);

void ov41_02248BFC(SysTask *param0, void *param1);

void ov41_02248BFC(SysTask *param0, void *param1) {
    UnkStruct_ov22_022586C4 *v0 = param1;
    int v1;
    int v2;

    switch (v0->unk_1C) {
    case 0:
        ov41_02248984(v0->unk_00, v0->unk_0C, v0->unk_10, 0, -132);
        ov41_02248A08(v0->unk_00, v0->unk_0C, v0->unk_10, 1);

        v0->unk_14 = 0;
        v2 = (v0->unk_00->unk_00.unk_40 + 1) % 2;
        ov41_022488D8(v0->unk_00, v2, 2, 5, &v0->unk_14);

        v0->unk_18 = ov41_02248E10(v0->unk_20, 1);
        v0->unk_1C++;
        break;
    case 1:
        for (v1 = 0; v1 < v0->unk_18; v1++) {
            if ((v0->unk_20 - 1) >= 0) {
                UnkStruct_ov22_02259560 *v3;

                v0->unk_20--;
                v3 = ov41_02248AFC(v0->unk_00, v0->unk_04, v0->unk_08, v0->unk_20);
                ov41_02248D64(v3, v0->unk_28, v0->unk_2C);
            }
        }

        if (v0->unk_20 == 0) {
            v0->unk_18 = ov41_02248E10(v0->unk_24, 2);
            v0->unk_1C++;
        }
        break;
    case 2:
        for (v1 = 0; v1 < v0->unk_18; v1++) {
            if ((v0->unk_24 - 1) >= 0) {
                UnkStruct_ov22_02259560 *v4;

                v0->unk_24--;
                v4 = ov41_02248AFC(v0->unk_00, v0->unk_0C, v0->unk_10, v0->unk_24);

                ov41_02248D64(v4, v0->unk_28, v0->unk_2C);
            }
        }

        if (v0->unk_24 == 0) {
            v0->unk_1C++;
            v0->unk_18 = 0;
        }
        break;
    case 3:
        v0->unk_18++;

        if (v0->unk_18 > 3) {
            if (v0->unk_14) {
                v0->unk_1C++;
            }
        }
        break;
    case 4:
        ov41_02248A08(v0->unk_00, v0->unk_04, v0->unk_08, 0);
        ov41_02248984(v0->unk_00, v0->unk_04, v0->unk_08, 0, -132);

        v0->unk_00->unk_00.unk_3C = 0;

        Heap_Free(v0->unk_28);
        DestroySysTaskAndEnvironment(param0);

        return;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    ov41_02248DA4(v0->unk_28, v0->unk_2C);
}
