#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "easy_chat.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02013534.h"
#include "overlay_89_unk_struct_ov113_0225_dbcc_t.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "yes_no_prompt.h"
#include "unk_02013534_font_system.h"
#include "gf_gfx_planes.h"

typedef struct YesNoButtonAnimData YesNoButtonAnimData;

typedef struct YesNoButtonSprite YesNoButtonSprite;

typedef struct UnkStruct_ov113_0225DBCC_t UnkStruct_ov113_0225DBCC;

struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
};

struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
};

void ov89_0225A1D8(UnkStruct_ov113_0225DBCC *param0, s32 param1);

void ov89_0225A1D8(UnkStruct_ov113_0225DBCC *param0, s32 param1) {
    s32 v1;
    u16 *v0;
    u16 *v2;

    v2 = PaletteData_GetUnfadedBuf(param0->unk_0C, PLTTBUF_MAIN_OBJ);
    v0 = PaletteData_GetFadedBuf(param0->unk_0C, PLTTBUF_MAIN_OBJ);
    MIi_CpuCopy16(v2 + 17, v0 + 17, 0xCU);
    v1 = param1 + 0x11;
    BlendPalette(&v2[v1], &v0[v1], 1U, 6U, 0U);
}
