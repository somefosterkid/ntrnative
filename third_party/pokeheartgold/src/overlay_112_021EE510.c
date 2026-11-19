#include "global.h"

typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233268_sub1_sub1;
typedef struct {
    u32 unk_00_0 : 8;
    u32 unk_00_8 : 4;
    u32 unk_00_12 : 4;
    u32 unk_00_16 : 16;
} UnkStruct_ov97_02233268_sub1_sub2;
typedef struct {
    UnkStruct_ov97_02233268_sub1_sub1 unk_00;
    UnkStruct_ov97_02233268_sub1_sub2 unk_04;
} UnkStruct_ov97_02233268_sub1;
typedef struct {
    u16 unk_00[12];
} UnkStruct_ov97_02233268_sub2_sub1;
typedef struct {
    UnkStruct_ov97_02233268_sub2_sub1 unk_00;
    UnkStruct_ov97_02233268_sub2_sub1 unk_18;
} UnkStruct_ov97_02233268_sub2;
typedef struct {
    UnkStruct_ov97_02233268_sub1 unk_00;
    UnkStruct_ov97_02233268_sub2 unk_08;
} UnkStruct_ov97_02233268;

u32 ov112_021EE510(void);

u32 ov112_021EE510(void) {
    return sizeof(UnkStruct_ov97_02233268);
}
