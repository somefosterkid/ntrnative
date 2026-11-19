#include "global.h"

#include "pm_string.h"

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef struct UnkStruct_ov66_02231FB0_t UnkStruct_ov66_02231FB0;

typedef struct {
    u8 pad_0[4];
    u8 unk_4;
    u8 unk_5;
    u16 unk_6;
    u32 unk_8;
} UnkStruct_ov66_02231428;

typedef struct {
    u8 unk_00[20];
} UnkStruct_ov66_02232068;

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
    UnkStruct_ov66_02231FB0 unk_00[8];
    UnkStruct_ov66_02231FB0 unk_188;
} UnkStruct_ov66_02231E94;

struct UnkStruct_ov66_0223177C_t {
    struct { u8 unk_0_anon[4]; } unk_00;
    BOOL unk_04;
    UnkStruct_ov66_02231D38 unk_08;
    UnkStruct_ov66_02231E94 unk_5C;
};

typedef struct {
    u32 unk_00;
} UnkStruct_ov66_02231B24;

void *ov45_0222E04C(UnkStruct_ov66_02231E94 *, u16);

s32 ov45_0222E0A4(UnkStruct_ov66_02231E94 *, u8 *);

s32 ov45_0222E0E0(u8 *, u32, s32, s32, void *, void *, void *, void *, u16, u16, u16, u16, s32, s32, s32);

void ov45_0222DC08(UnkStruct_ov66_0223177C *param0, UnkStruct_ov66_02231B24 *param1, UnkStruct_ov66_02232068 *param2);

void ov45_0222DC08(UnkStruct_ov66_0223177C *param0, UnkStruct_ov66_02231B24 *param1, UnkStruct_ov66_02232068 *param2) {
    void *v1;
    u32 v0;

    v0 = param1->unk_00;
    if ((v0 < 0x14U) && (param2->unk_00[v0] != 0)) {
        v1 = ov45_0222E04C(&param0->unk_5C, 1);
        if (v1 != 0) {
            ov45_0222E0E0(v1, param1->unk_00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x708, 1, 7);
            ov45_0222E0A4(&param0->unk_5C, v1);
        }
    }
}
