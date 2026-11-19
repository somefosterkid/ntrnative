#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"
#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bg_window.h"
#include "camera.h"
#include "easy_chat.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02013534.h"
#include "unk_02014DA0.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "unk_02013534_font_system.h"

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

typedef struct UnkStruct_ov66_02231FB0_t UnkStruct_ov66_02231FB0;

typedef u16 charcode_t;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02231D38_sub1;

typedef struct {
    UnkStruct_ov66_02231D38_sub1 unk_00[20];
    u16 unk_50;
    u16 unk_52;
} UnkStruct_ov66_02231D38;

struct UnkStruct_ov66_02231FB0_t {
    u8 unk_00;
    u32 unk_04[3];
    String *unk_10[4];
    u16 unk_20[4];
    u16 unk_28;
    s16 unk_2A;
    struct UnkStruct_ov66_02231FB0_t *unk_2C;
    struct UnkStruct_ov66_02231FB0_t *unk_30;
};

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
} UnkStruct_ov117_022626B0_sub1_sub1;

typedef struct {
    UnkStruct_ov117_022626B0_sub1_sub1 unk_00;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_08;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_10;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_18;
} UnkStruct_ov117_022626B0_sub1;

typedef struct {
    UnkStruct_ov117_022626B0_sub1_sub1 unk_00;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_08;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_10;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_18;
} UnkStruct_ov117_022626B0_sub2;

struct UnkStruct_ov117_022626B0_t {
    NNSG3dResFileHeader *unk_00;
    NNSG3dResTex *unk_04;
    u16 unk_08;
    UnkStruct_ov117_022626B0_sub1 unk_0C[8];
    UnkStruct_ov117_022626B0_sub2 unk_10C[8];
    int unk_20C;
    int unk_210;
    int unk_214;
    int unk_218;
    int unk_21C;
    int unk_220;
    int unk_224;
    int unk_228;
    int unk_22C;
    fx32 unk_230;
    ManagedSprite *unk_234;
    int unk_238;
    int unk_23C;
    int unk_240;
    int unk_244;
    fx32 unk_248;
    u16 *unk_24C;
    u16 *unk_250;
    u32 unk_254;
    u32 unk_258;
    s32 unk_25C;
    fx32 unk_260;
    fx32 unk_264;
    s16 unk_268;
    s16 unk_26A;
    s16 unk_26C;
    int unk_270;
    u8 unk_274;
    u8 unk_275;
    u8 unk_276;
};

struct UnkStruct_0207E060_t {
    u16 unk_00[6];
    u16 unk_0C[6];
    u8 unk_18;
    u8 language;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E;
    u8 unk_1F;
    u8 unk_20;
    u8 unk_21;
    u8 unk_22;
};

s32 ov93_02261FC8(UnkStruct_ov117_02265EC8 *);

s32 ov93_022620D4(UnkStruct_ov117_02265EC8 *);

s32 ov93_02262108(UnkStruct_ov117_02265EC8 *);

s32 ov93_02262130(UnkStruct_ov117_02265EC8 *);

void ov93_02262034(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265EC8 *param1);

void ov93_02262034(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265EC8 *param1) {
    s32 v2;
    s8 v1;
    u8 v0;

    v1 = param1->unk_19E;
    if ((s32)v1 > 0) {
        param1->unk_19E = v1 - 1;
        return;
    }
    v0 = param1->unk_1A0;
    switch (v0) {
    case 0:
        if (ov93_02262108(param1) == 1) {
            ov93_022620D4(param1);
            return;
        }
        param1->unk_1A0 += 1;
    case 1:
        v2 = ov93_02262130(param1);
        ov93_02261FC8(param1);
        if (v2 == 1) {
            param1->unk_19E = 0xF;
            param1->unk_1A0 -= 1;
        }
        return;
    }
}
