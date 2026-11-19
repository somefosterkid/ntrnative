#include "global.h"

#include "player_data.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

s16 ov45_0222BE94(void *arg0);

struct PasswordWordBank;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);
void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *param1, enum HeapID heapID);
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct {
    const PlayerProfile *unk_00;
    const PlayerProfile *unk_04;
    u16 unk_08;
    u16 unk_0A;
} UnkStruct_ov66_0223185C;
void ov45_0222D940(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_0223185C *param1);
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
typedef struct {
    const PlayerProfile *unk_00;
    const PlayerProfile *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u32 unk_0C;
} UnkStruct_ov66_022318AC;
void ov45_0222D990(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_022318AC *param1);
void ov45_0222BE9C(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);
void ov45_0222BF18(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

s16 ov45_0222BE94(void *arg0) {
    return *(s16 *)((u8 *)arg0 + 0x26);
}

void ov45_0222BE9C(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1) {
    const WiFiPlazaProfile *v0;
    int v1;
    u32 v2;
    UnkStruct_ov66_0223185C v3;

    if (param1->unk_10 != 2) {
        return;
    }

    for (v1 = 0; v1 < 2; v1++) {
        v2 = ov45_0222EC68(param1->unk_00[v1]);

        if (v2 != 0xffffffff) {
            v0 = ov45_0222A578(param0, v2);
            ov45_0222A844(v0, param0->unk_04.trainerInfos[v1], param0->heapID);
        } else {
            return;
        }
    }

    v3.unk_00 = param0->unk_04.trainerInfos[0];
    v3.unk_04 = param0->unk_04.trainerInfos[1];
    v3.unk_08 = ov45_0222EC68(param1->unk_00[0]);
    v3.unk_0A = ov45_0222EC68(param1->unk_00[1]);

    ov45_0222D940(param0->unk_04.unk_00, &v3);
}

void ov45_0222BF18(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1) {
    const WiFiPlazaProfile *v0;
    int v1;
    u32 v2;
    UnkStruct_ov66_022318AC v3;

    if (param1->unk_10 != 2) {
        return;
    }

    for (v1 = 0; v1 < 2; v1++) {
        v2 = ov45_0222EC68(param1->unk_00[v1]);

        if (v2 != 0xffffffff) {
            v0 = ov45_0222A578(param0, v2);
            ov45_0222A844(v0, param0->unk_04.trainerInfos[v1], param0->heapID);
        } else {
            return;
        }
    }

    v3.unk_00 = param0->unk_04.trainerInfos[0];
    v3.unk_04 = param0->unk_04.trainerInfos[1];
    v3.unk_08 = ov45_0222EC68(param1->unk_00[0]);
    v3.unk_0A = ov45_0222EC68(param1->unk_00[1]);
    v3.unk_0C = param1->unk_12;

    ov45_0222D990(param0->unk_04.unk_00, &v3);
}
