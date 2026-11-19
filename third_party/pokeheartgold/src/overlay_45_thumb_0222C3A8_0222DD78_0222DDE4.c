#include "global.h"

#include "error_handling.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "overlay_45_wifi_plaza_system.h"

struct String;

struct UnkStruct_ov66_0223177C_t;

struct UnkStruct_ov66_02231FB0_t;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

BOOL ov45_0222DD44(const UnkStruct_ov66_0223177C *param0);

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov66_02231D38_sub1;

typedef struct {
    UnkStruct_ov66_02231D38_sub1 unk_00[20];
    u16 unk_50;
    u16 unk_52;
} UnkStruct_ov66_02231D38;

typedef struct UnkStruct_ov66_02231FB0_t {
    u8 unk_00;
    u32 unk_04[3];
    String *unk_10[4];
    u16 unk_20[4];
    u16 unk_28;
    s16 unk_2A;
    struct UnkStruct_ov66_02231FB0_t *unk_2C;
    struct UnkStruct_ov66_02231FB0_t *unk_30;
} UnkStruct_ov66_02231FB0;

u32 ov66_0223245C(const UnkStruct_ov66_02231FB0 *param0, u32 param1);
u32 ov45_0222E540(const UnkStruct_ov66_02231FB0 *param0, u32 param1);

u32 ov66_0223246C(const UnkStruct_ov66_02231FB0 *param0, u32 param1);
u32 ov45_0222E550(const UnkStruct_ov66_02231FB0 *param0, u32 param1);

u32 ov66_0223247C(const UnkStruct_ov66_02231FB0 *param0, u32 param1);
u32 ov45_0222E560(const UnkStruct_ov66_02231FB0 *param0, u32 param1);

u32 ov45_0222E574(const UnkStruct_ov66_02231FB0 *param0, u32 param1);

u32 ov45_0222E584(const UnkStruct_ov66_02231FB0 *param0, u32 param1);

u32 ov66_022324B4(const UnkStruct_ov66_02231FB0 *param0, u32 param1);
u32 ov45_0222E598(const UnkStruct_ov66_02231FB0 *param0, u32 param1);

u32 ov66_022324B8(const UnkStruct_ov66_02231FB0 *param0, u32 param1);
u32 ov45_0222E59C(const UnkStruct_ov66_02231FB0 *param0, u32 param1);

u32 ov66_022324BC(const UnkStruct_ov66_02231FB0 *param0, u32 param1);
u32 ov45_0222E5A0(const UnkStruct_ov66_02231FB0 *param0, u32 param1);

u32 ov45_0222DDE4(const UnkStruct_ov66_0223177C *param0, int param1, u32 param2);

u32 ov45_0222DDE4(const UnkStruct_ov66_0223177C *param0, int param1, u32 param2) {
    UnkStruct_ov66_02231FB0 *v0;
    u32 v1;

    static u32 (*const v2[])(const UnkStruct_ov66_02231FB0 *, u32) = {
        ov45_0222E540,
        ov45_0222E550,
        ov45_0222E560,
        ov45_0222E574,
        ov45_0222E584,
        ov45_0222E584,
        ov45_0222E598,
        ov45_0222E59C,
        ov45_0222E5A0,
    };

    GF_ASSERT(ov45_0222DD44(param0));

    v0 = param0->unk_5C.unk_188.unk_2C;

    if (v0->unk_00 < 9) {
        v1 = v2[v0->unk_00](v0, param2);
    } else {
        GF_ASSERT(FALSE);
        v1 = 20;
    }

    return v1;
}
