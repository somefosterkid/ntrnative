#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_0203A3B0.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_struct_02089688.h"
#include "touch_hitbox_controller.h"

struct OverlayManager;
struct TextOBJ;
struct TouchHitboxController;
struct UnkStruct_02012744_t;
typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;
extern WMLinkLevel DWC_GetLinkLevel(void);
typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchHitboxController *unk_14;
    TouchscreenHitbox unk_18[28];
    BOOL unk_88;
    UnkStruct_02012744 *unk_8C;
    FontOAM *unk_90[2];
    UnkStruct_02021AC8 unk_98[2];
    Window unk_B0;
} UnkStruct_02089688_sub1;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02089688_sub2;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_0208AF44_sub1;
typedef struct {
    int unk_00;
    int unk_04;
    BOOL unk_08;
    ManagedSprite *unk_0C;
    TouchscreenHitbox *unk_10;
    UnkStruct_0208AF44_sub1 unk_14;
} UnkStruct_0208AF44;
typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);
void *OverlayManager_GetData(OverlayManager *appMan);
BOOL sub_02085BEC(UnkStruct_02089688 *param0);

int sub_0208524C(OverlayManager *appMan, int *param1);

int sub_0208524C(OverlayManager *appMan, int *param1) {
    BOOL v0;
    UnkStruct_02089688 *v1 = OverlayManager_GetData(appMan);
    v0 = sub_02085BEC(v1);

    if (v1->unk_3B8.unk_30 != 0) {
        sub_0203A930(WM_LINK_LEVEL_3 - DWC_GetLinkLevel());
    }

    return v0 ? 1 : 0;
}
