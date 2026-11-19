#include "global.h"

#include "heap.h"
#include "overlay_93_thumb_1.h"
#include "overlay_manager.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"

// ov91-93 view of the Wi-Fi Plaza launch record: 0x34 here (embedded by value), the header copy is 0x3c
typedef struct UnkStruct_ov66_02230DBC {
    u32 unk_00;
    UnkStruct_ov66_0222E294 unk_04;
    UnkStruct_ov66_02232068 unk_08;
    BOOL unk_1C;
    struct PlayerProfile *unk_20[4];
    WiFiPlazaSystem *unk_30;
} UnkStruct_ov66_02230DBC;


struct OverlayManager;
struct PlayerProfile;
struct UnkStruct_ov66_0222DFF8_t;
void ov90_02258800(UnkStruct_ov114_0225C76C *param0, BOOL param1, SaveData *saveData, BOOL param3, UnkStruct_ov66_02230DBC *param4);
typedef struct {
    UnkStruct_ov66_02230DBC unk_00;
    SaveData *saveData;
    u8 unk_38;
    u8 unk_39;
    u8 padding[2];
} UnkStruct_ov117_02260440;
typedef struct {
    UnkStruct_ov114_0225C76C unk_00;
    UnkStruct_ov114_0225C9A8 unk_10;
    OverlayManager *appMan;
    u8 unk_2C[4];
    u8 unk_30;
    u8 unk_31;
    u8 padding_32[2];
    int unk_34;
    BOOL unk_38;
    u8 unk_3C;
    u8 unk_3D;
    u8 unk_3E;
    u8 padding_3F;
} UnkStruct_ov117_022605C0;
void ov93_0225C6D8(UnkStruct_ov117_022605C0 *param0, UnkStruct_ov117_02260440 *param1);
BOOL ov90_02258B54(UnkStruct_ov114_0225C76C *param0);
BOOL ov93_0225C730(UnkStruct_ov117_022605C0 *param0);

s32 ov93_0225C6C0(OverlayManager *man) {
    OverlayManager_GetData(man);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_117);
    return 1;
}

void ov93_0225C6D8(UnkStruct_ov117_022605C0 *param0, UnkStruct_ov117_02260440 *param1) {
    int v0, v1, v2, v3;

    v3 = sub_0203769C();
    v2 = 0;
    v1 = 0;

    for (v0 = 0; v0 < 4; v0++) {
        if (sub_02034818(v0) != NULL) {
            param0->unk_2C[v1] = v0;

            if (v0 == v3) {
                v2 = v1;
            }

            v1++;
        } else {
            param0->unk_2C[v0] = 0xffff;
        }
    }

    param0->unk_30 = v1;
    param0->unk_3C = param1->unk_38;

    ov90_02258800(&param0->unk_00, param1->unk_39, param1->saveData, param1->unk_38, &param1->unk_00);
}

BOOL ov93_0225C730(UnkStruct_ov117_022605C0 *param0) {
    if (param0->unk_3D == 0) {
        if (ov90_02258B54(&param0->unk_00) == 1) {
            param0->unk_3D = 1;
        }
    }

    if (param0->unk_3D == 1) {
        if (param0->appMan == NULL) {
            return 1;
        }
    }

    return 0;
}
