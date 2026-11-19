#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

struct PasswordWordBank;

struct SaveData;

struct PlayerProfile;

struct UnkStruct_0207E060_t;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_0223177C_t;

struct WiFiPlazaProfile;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef u16 charcode_t;

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

void ov45_0222D500(UnkStruct_ov66_02231428 *param0, const s64 *param1);

void ov45_0222ECB8(s64 *param0);

typedef struct PasswordWordBank PasswordWordBank;

void ov45_0222B840(WiFiPlazaSystem *param0);

void ov45_0222BB60(UnkStruct_ov66_0222FB64 *param0, UnkStruct_ov66_0222FA80 *param1, UnkStruct_ov66_022302B0 *param2, UnkStruct_ov66_022309A4 *param3);

void ov45_0222BD30(UnkStruct_ov66_0222FC58 *param0);

void ov45_0222BE5C(UnkStruct_ov66_0222FC68 *param0);

void ov45_0222C3B0(UnkStruct_ov66_022302B0 *param0);

void ov45_0222CAA0(UnkStruct_ov66_022309A4 *param0);

WiFiPlazaSystem *ov45_02229FF4(SaveData *saveData, enum HeapID heapID);

void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *trainerInfo, enum HeapID heapID);

void ov45_0222B8A0(UnkStruct_ov66_0222F6C4 *param0, SaveData *saveData, enum HeapID heapID);

void ov66_0222FBF0(UnkStruct_ov66_0222FBF0 *param0, enum HeapID heapID);

void ov45_0222BCE4(UnkStruct_ov66_0222FBF0 *param0, const UnkStruct_ov66_0222F6C4 *param1, enum HeapID heapID);

void ov45_0222D8A4(u8 *arg0);

void ov45_0222BB58(void *arg0);

void ov45_0222BCB8(s16 *arg0);

u16 ov45_0222CCDC(void *arg0);

void ov45_0222A15C(WiFiPlazaSystem *param0);

void ov45_0222A15C(WiFiPlazaSystem *param0) {
    ov45_0222D8A4((u8 *)(param0->unk_04.unk_00));

    {
        s64 v0;

        ov45_0222ECB8(&v0);
        param0->unk_04.unk_D4 = v0;
        ov45_0222D500(&param0->unk_04.unk_D0, &v0);
    }

    {
        ov45_0222B840(param0);
    }

    {
        ov45_0222BE5C(&param0->unk_1C0);
    }

    ov45_0222BD30(&param0->unk_1E8);
    ov45_0222BB58(&param0->unk_F8);
    ov45_0222BB60(&param0->unk_1FC, &param0->unk_F8, &param0->unk_20C, &param0->unk_49C);
    ov45_0222BCB8((short *)(&param0->unk_3A0));
    ov45_0222C3B0(&param0->unk_20C);
    ov45_0222CAA0(&param0->unk_49C);

    {
        BOOL v1;

        v1 = ov45_0222CCDC(&param0->unk_4BC);

        if (v1 == 1) {
            param0->unk_52C = 1;
        }
    }
}
