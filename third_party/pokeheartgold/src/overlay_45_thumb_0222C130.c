#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

#include "player_data.h"

#include "constants/heap.h"

#include "save.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

struct PasswordWordBank;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *param1, enum HeapID heapID);
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct {
    u32 unk_00;
    const PlayerProfile *unk_04;
    const PlayerProfile *unk_08;
    const PlayerProfile *unk_0C;
    const PlayerProfile *unk_10;
    u16 unk_14;
    u16 unk_16;
    u16 unk_18;
    u16 unk_1A;
} UnkStruct_ov66_02231A58;
void ov45_0222DAE0(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231A58 *param1);
u32 ov45_0222EC68(s32 param0);
typedef struct {
    s32 unk_00[4];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13_0 : 7;
    u8 unk_13_7 : 1;
} UnkStruct_ov66_0222E990;
typedef struct PasswordWordBank PasswordWordBank;
void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *trainerInfo, enum HeapID heapID);
void ov45_0222DB3C(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231A58 *param1);

void ov45_0222C130(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222C1EC(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov45_0222C130(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1) {
    const WiFiPlazaProfile *v0;
    int v1;
    PlayerProfile *v2[4];
    u32 v3;
    UnkStruct_ov66_02231A58 v4;

    if ((param1->unk_10 > 4) || (param1->unk_10 <= 0)) {
        return;
    }

    for (v1 = 0; v1 < 4; v1++) {
        if (v1 < param1->unk_10) {
            v3 = ov45_0222EC68(param1->unk_00[v1]);

            if (v3 != 0xffffffff) {
                v0 = ov45_0222A578(param0, v3);
                ov45_0222A844(v0, param0->unk_04.trainerInfos[v1], param0->heapID);
                v2[v1] = param0->unk_04.trainerInfos[v1];
            } else {
                return;
            }
        } else {
            v2[v1] = 0L;
        }
    }

    v4.unk_00 = param1->unk_10;
    v4.unk_04 = v2[0];
    v4.unk_08 = v2[1];
    v4.unk_0C = v2[2];
    v4.unk_10 = v2[3];
    v4.unk_14 = ov45_0222EC68(param1->unk_00[0]);
    v4.unk_16 = ov45_0222EC68(param1->unk_00[1]);
    v4.unk_18 = ov45_0222EC68(param1->unk_00[2]);
    v4.unk_1A = ov45_0222EC68(param1->unk_00[3]);

    ov45_0222DAE0(param0->unk_04.unk_00, &v4);
}

void ov45_0222C1EC(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1) {
    const WiFiPlazaProfile *v0;
    int v1;
    PlayerProfile *v2[4];
    u32 v3;
    UnkStruct_ov66_02231A58 v4;

    if ((param1->unk_10 > 4) || (param1->unk_10 <= 0)) {
        return;
    }

    for (v1 = 0; v1 < 4; v1++) {
        if (v1 < param1->unk_10) {
            v3 = ov45_0222EC68(param1->unk_00[v1]);

            if (v3 != 0xffffffff) {
                v0 = ov45_0222A578(param0, v3);
                ov45_0222A844(v0, param0->unk_04.trainerInfos[v1], param0->heapID);
                v2[v1] = param0->unk_04.trainerInfos[v1];
            } else {
                return;
            }
        } else {
            v2[v1] = 0L;
        }
    }

    v4.unk_00 = param1->unk_10;
    v4.unk_04 = v2[0];
    v4.unk_08 = v2[1];
    v4.unk_0C = v2[2];
    v4.unk_10 = v2[3];
    v4.unk_14 = ov45_0222EC68(param1->unk_00[0]);
    v4.unk_16 = ov45_0222EC68(param1->unk_00[1]);
    v4.unk_18 = ov45_0222EC68(param1->unk_00[2]);
    v4.unk_1A = ov45_0222EC68(param1->unk_00[3]);

    ov45_0222DB3C(param0->unk_04.unk_00, &v4);
}
