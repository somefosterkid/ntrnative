#include "global.h"

#include "pm_string.h"

struct UnkStruct_ov66_0223177C_t;
typedef struct {
    u8 pad_0[4];
    u8 unk_4;
    u8 unk_5;
    u16 unk_6;
    u32 unk_8;
} UnkStruct_ov66_02231428;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
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
void ov41_02249CC4(UnkStruct_ov66_02231E94 *param0);

void ov41_02248940(UnkStruct_ov66_0223177C *param0);

void ov41_02248940(UnkStruct_ov66_0223177C *param0) {
    ov41_02249CC4(&param0->unk_5C);
}
