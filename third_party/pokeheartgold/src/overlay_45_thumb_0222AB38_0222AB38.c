#include "global.h"

#include "constants/heap.h"

#include "easy_chat.h"
#include "pm_string.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_02231_e94.h"
#include "overlay_45_unk_struct_ov66_0223177_c_t.h"
#include "overlay_45_wifi_plaza_system.h"

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

typedef struct UnkStruct_ov66_02231FB0_t UnkStruct_ov66_02231FB0;

typedef u16 charcode_t;

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

void ov45_0222AB38(WiFiPlazaSystem *param0, UnkStruct_ov66_02232068 *param1);

void ov45_0222AB38(WiFiPlazaSystem *param0, UnkStruct_ov66_02232068 *param1) {
    MI_CpuCopy8(&param0->unk_3CB, param1, 0x14U);
}
