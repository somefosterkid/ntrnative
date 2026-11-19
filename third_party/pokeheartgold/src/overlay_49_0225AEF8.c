#include "global.h"

#include "save.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct PlayerProfile;

struct UnkStruct_ov114_0225D678_t;

struct UnkStruct_ov114_0225E854_t;

struct UnkStruct_ov115_0226095C_t;

struct UnkStruct_ov115_02260BE4_t;

struct UnkStruct_ov115_02260D78_t;

struct UnkStruct_ov66_0222DFF8_t;

typedef struct UnkStruct_ov115_0226095C_t UnkStruct_ov115_0226095C;

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

typedef struct UnkStruct_ov115_02260BE4_t UnkStruct_ov115_02260BE4;

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;

typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;

typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;

void ListMenuItems_AddItem(UnkStruct_ov115_02260D78 *param0);

struct UnkStruct_ov115_0226095C_t {
    UnkStruct_ov114_0225D678 *unk_00;
    UnkStruct_ov114_0225E854 *unk_04;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225C9A8 unk_18;
    UnkStruct_ov115_02260BE4 *unk_30;
    UnkStruct_ov115_02260D78 *unk_34;
    BOOL unk_38;
    BOOL unk_3C;
    BOOL unk_40;
    u16 unk_44;
    u16 unk_46;
    UnkStruct_ov115_02262F50 unk_48;
    u32 unk_70;
    u32 unk_74;
    u32 unk_78;
    u32 unk_7C;
    BOOL unk_80;
    u8 unk_84[4];
};

void ov49_0225AEF8(void *param0);

s32 ov49_0225AF04(void *arg0);

void ov49_0225AEF8(void *param0) {
    UnkStruct_ov115_0226095C *v0 = param0;

    ListMenuItems_AddItem(v0->unk_34);
}

s32 ov49_0225AF04(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0x34);
}
