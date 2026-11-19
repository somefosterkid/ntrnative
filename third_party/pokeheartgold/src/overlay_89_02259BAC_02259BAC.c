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
#include "overlay_45_wifi_plaza_system.h"
#include "yes_no_prompt.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "gf_gfx_planes.h"

typedef struct YesNoButtonAnimData YesNoButtonAnimData;

typedef struct YesNoButtonSprite YesNoButtonSprite;

typedef struct UnkStruct_ov113_0225DBCC_t UnkStruct_ov113_0225DBCC;

typedef struct UnkStruct_ov113_0225E6B8_t UnkStruct_ov113_0225E6B8;

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

typedef struct {
    BOOL unk_00;
    s32 unk_04;
} UnkStruct_ov113_022607EC;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov113_02260818;

typedef struct {
    WiFiPlazaSystem *unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_ov66_0223127C;

typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_0225FE30;

typedef struct {
    VecFx32 unk_00;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FEE0;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0225FF8C;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260064;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_0226013C;

typedef struct {
    CameraAngle cameraAngle;
    fx32 unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E[2];
} UnkStruct_ov113_02260218;

typedef struct {
    fx32 unk_00;
    s16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov113_022602E4;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_02260394;

typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    fx32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E[2];
} UnkStruct_ov113_0226046C;

typedef struct {
    CameraAngle cameraAngle;
    s32 unk_08;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkStruct_ov113_02260544;

typedef struct {
    UnkStruct_ov113_0225FE30 unk_00;
    UnkStruct_ov113_0225FEE0 unk_08;
    UnkStruct_ov113_0225FF8C unk_18;
    UnkStruct_ov113_02260064 unk_28;
    UnkStruct_ov113_0226013C unk_38;
    UnkStruct_ov113_02260218 unk_48;
    UnkStruct_ov113_022602E4 unk_58;
    UnkStruct_ov113_02260394 unk_60;
    UnkStruct_ov113_0226046C unk_80;
    UnkStruct_ov113_02260544 unk_A0;
} UnkStruct_ov113_0225EB20_sub1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
} UnkStruct_ov113_0225EDCC;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
} UnkStruct_ov113_0225FD80;

typedef struct {
    UnkStruct_ov113_0225EB20_sub1 unk_00;
    UnkStruct_ov113_0225FD80 unk_B0;
    UnkStruct_ov113_0225E6B8 *unk_B4[128];
    UnkStruct_ov113_0225E6B8 *unk_2B4[128];
    NNSG3dResTex *unk_4B4[32];
    u32 unk_534[2];
    UnkStruct_ov113_0225EDCC unk_53C[128];
    u8 unk_73C;
    u8 unk_73D;
    u8 unk_73E;
    u8 unk_73F;
    u8 unk_740;
    s16 unk_742;
} UnkStruct_ov113_0225EB20;

typedef struct {
    Field3dModel unk_00;
    Field3dObject unk_10;
} UnkStruct_ov113_0225DBCC_sub1;

typedef struct {
    TextOBJ *unk_00;
    UnkStruct_02021AC8 unk_04;
    u16 unk_10;
} UnkStruct_ov113_0225E250;

struct UnkStruct_ov113_0225DBCC_t {
    UnkStruct_ov66_0223127C *unk_00;
    SaveData *saveData;
    BgConfig *unk_08;
    PaletteData *unk_0C;
    UnkStruct_02012744 *unk_10;
    GF3DVramMan *unk_14;
    SysTask *unk_18;
    SpriteSystem *unk_1C;
    SpriteManager *unk_20;
    YesNoPrompt *unk_24;
    u8 unk_28;
    u8 unk_29;
    u8 pad_2A[2];
    MessageFormat *unk_2C;
    MsgData *unk_30;
    Window unk_34[8];
    Window unk_B4;
    String *unk_C4;
    u8 unk_C8;
    u8 pad_C9[3];
    Camera *camera;
    fx32 unk_D0;
    fx32 unk_D4;
    UnkStruct_ov113_0225DBCC_sub1 unk_D8;
    NARC *unk_160;
    NARC *unk_164;
    UnkStruct_ov113_022607EC unk_168;
    s32 unk_170[8];
    int unk_190;
    UnkStruct_ov113_0225EB20 unk_194;
    UnkStruct_ov113_02260818 unk_8D8[6];
    u8 unk_920;
    u8 unk_921;
    u8 pad_922[2];
    ManagedSprite *unk_924[6];
    ManagedSprite *unk_93C[6];
    ManagedSprite *unk_954[6];
    ManagedSprite *unk_96C;
    ManagedSprite *unk_970[8];
    ManagedSprite *unk_990[3];
    u16 unk_99C[8];
    u16 unk_9AC[8];
    int unk_9BC;
    u16 unk_9C0[2048];
    UnkStruct_ov113_0225E250 unk_19C0;
    int unk_19D4;
    int unk_19D8;
    int unk_19DC;
    BOOL unk_19E0;
};

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
} UnkStruct_ov113_0225EDA4;

typedef struct {
    s32 unk_00;
    UnkStruct_ov113_0225EDA4 unk_04;
} UnkStruct_ov113_0225EF0C;

typedef struct {
    int unk_00;
    fx32 unk_04;
    u8 unk_08;
    u8 unk_09;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225EF78;

typedef struct {
    Field3dObject unk_00[3];
    u16 unk_168;
    u8 unk_16A;
    UnkStruct_ov113_0225EDA4 unk_16C[4];
} UnkStruct_ov113_0225F060;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225F1E4;

typedef struct {
    int unk_00;
    fx32 unk_04;
    int unk_08;
    fx32 unk_0C;
    UnkStruct_ov113_0225EDA4 unk_10;
} UnkStruct_ov113_0225F368;

typedef struct {
    Field3dObject unk_00[3];
    u16 unk_168;
    u8 unk_16A;
    u8 unk_16B;
    u8 unk_16C;
    u8 unk_16D;
    UnkStruct_ov113_0225EDA4 unk_170;
} UnkStruct_ov113_0225F51C;

typedef struct {
    int unk_00;
    fx32 unk_04;
    fx32 unk_08;
    UnkStruct_ov113_0225EDA4 unk_0C;
} UnkStruct_ov113_0225F728;

typedef struct {
    fx32 unk_00;
    u16 unk_04;
    u8 unk_06;
    UnkStruct_ov113_0225EDA4 unk_08;
} UnkStruct_ov113_0225F818;

typedef struct {
    Field3dObject unk_00[2];
    fx32 unk_F0;
    u16 unk_F4;
    u8 unk_F6;
    u8 unk_F7;
    UnkStruct_ov113_0225EDA4 unk_F8[3];
} UnkStruct_ov113_0225F890;

typedef struct {
    Field3dObject unk_00[2];
    fx32 unk_F0;
    u16 unk_F4;
    u8 unk_F6;
    u8 unk_F7;
    UnkStruct_ov113_0225EDA4 unk_F8[3];
} UnkStruct_ov113_0225FAB0;

typedef struct {
    fx32 unk_00;
    u16 unk_04;
    u8 unk_06;
    UnkStruct_ov113_0225EDA4 unk_08;
} UnkStruct_ov113_0225FCD0;

typedef union {
    UnkStruct_ov113_0225EF0C val1;
    UnkStruct_ov113_0225EF78 val2;
    UnkStruct_ov113_0225F060 val3;
    UnkStruct_ov113_0225F1E4 val4;
    UnkStruct_ov113_0225F368 val5;
    UnkStruct_ov113_0225F51C val6;
    UnkStruct_ov113_0225F728 val7;
    UnkStruct_ov113_0225F818 val8;
    UnkStruct_ov113_0225F890 val9;
    UnkStruct_ov113_0225FAB0 val10;
    UnkStruct_ov113_0225FCD0 val11;
} UnkStruct_ov113_0225E6B8_sub1;

struct UnkStruct_ov113_0225E6B8_t {
    UnkStruct_ov113_02260818 unk_00;
    Field3dModel unk_0C;
    Field3dObject unk_1C;
    UnkStruct_ov113_0225E6B8_sub1 unk_94;
    u32 unk_240_0 : 4;
    u32 unk_240_4 : 4;
    u32 unk_240_8 : 8;
    u32 unk_240_16 : 8;
    u32 unk_240_24 : 8;
};

s32 sub_02018030(UnkStruct_ov113_0225DBCC_sub1 *, NARC *, s32, s32);

s32 sub_020181B0(Field3dObject *, UnkStruct_ov113_0225DBCC_sub1 *);

s32 sub_020182A0(Field3dObject *, s32);

s32 sub_020182A8(Field3dObject *, s32, s32, s32);

s32 sub_020182C4(Field3dObject *, s32, s32, s32);

void ov89_02259BAC(UnkStruct_ov113_0225DBCC *param0, NARC *param1);

void ov89_02259BAC(UnkStruct_ov113_0225DBCC *param0, NARC *param1) {
    s32 v0;

    if (param0->unk_00->unk_04 == 0) {
        v0 = 0x1C;
    } else {
        v0 = 0x1B;
    }
    sub_02018030(&param0->unk_D8, param1, v0, 0x7D);
    sub_020181B0(&param0->unk_D8.unk_10, &param0->unk_D8);
    sub_020182A8(&param0->unk_D8.unk_10, 0, 0, 0);
    sub_020182C4(&param0->unk_D8.unk_10, 0x1000, 0x1000, 0x1000);
    sub_020182A0(&param0->unk_D8.unk_10, 1);
}
