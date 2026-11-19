#include "global.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "filesystem_files_def.h"
#include "overlay_41_unk_struct_ov22_02259560.h"

void ov41_02248724(s32 *);

void ov41_02248A18(s32 *, u32);

void ov41_022487F8(s32 *param_1, s32 param_2, s32 param_3);

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;
BOOL TouchscreenHitbox_TouchHeldIsIn(const TouchscreenHitbox *hitTable);
BOOL TouchscreenHitbox_PointIsIn(const TouchscreenHitbox *hitTable, u32 touchX, u32 touchY);
BOOL ov41_02248820(UnkStruct_ov22_02257F50 *param0);
BOOL ov41_0224883C(UnkStruct_ov22_02257F50 *param0, int param1, int param2);

void ov41_022487F8(s32 *param_1, s32 param_2, s32 param_3) {
    ov41_02248A18(param_1, 0);
    *param_1 = param_2;
    param_1[param_2 * 3 + 5] = param_3;
    ov41_02248A18(param_1, 1);
    ov41_02248724(param_1);
    return;
}

BOOL ov41_02248820(UnkStruct_ov22_02257F50 *param0) {
    TouchscreenHitbox v0;

    v0.rect.top = (16 + 2);
    v0.rect.bottom = (16 + 2) + (129 - (2 * 2));
    v0.rect.left = (8 + 2);
    v0.rect.right = (8 + 2) + (112 - (2 * 2));

    return TouchscreenHitbox_TouchHeldIsIn(&v0);
}

BOOL ov41_0224883C(UnkStruct_ov22_02257F50 *param0, int param1, int param2) {
    TouchscreenHitbox v0;

    v0.rect.top = (16 + 2);
    v0.rect.bottom = (16 + 2) + (129 - (2 * 2));
    v0.rect.left = (8 + 2);
    v0.rect.right = (8 + 2) + (112 - (2 * 2));

    return TouchscreenHitbox_PointIsIn(&v0, param1, param2);
}
