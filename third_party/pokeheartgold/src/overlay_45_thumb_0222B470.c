#include "global.h"

#include "constants/heap.h"

#include "save.h"
#include "overlay_45_wifi_plaza_system.h"
#include "ppw_lobby.h"

struct PasswordWordBank;

struct PlayerProfile;

struct UnkStruct_0207E060_t;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_0223177C_t;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef u16 charcode_t;

BOOL ov45_0222AFF8(const WiFiPlazaSystem *param0);

u32 ov45_0222B020(const WiFiPlazaSystem *param0);

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

BOOL ov45_0222D8D4(UnkStruct_ov66_0223177C *param0, u32 param1);

void ov45_0222D8F0(UnkStruct_ov66_0223177C *param0, u32 param1);

s32 ov45_0222E9E0(void);

s32 ov45_0222EAD4(s32 param0);

u32 ov45_0222EC68(s32 param0);

typedef struct PasswordWordBank PasswordWordBank;

void ov45_0222BA3C(WiFiPlazaSystem *param0);

void ov45_0222BD5C(UnkStruct_ov66_0222FC68 *param0);

void ov45_0222BDB0(UnkStruct_ov66_0222FC68 *param0, u32 param1);

void ov45_0222BDE8(UnkStruct_ov66_0222FC68 *param0, u32 param1);

void ov45_0222C944(UnkStruct_ov66_02232068 *param0, u32 param1, BOOL param2);

void ov45_0222C9A0(UnkStruct_ov66_022308A0 *param0, u32 param1, BOOL param2, u32 param3);

BOOL ov45_0222CBD0(UnkStruct_ov66_02230A6C *param0, s32 param1, s32 param2);

void ov45_0222B470(s32 param0, void *param1);

u32 ov45_0222A920(const WiFiPlazaProfile *profile);
u32 ov45_0222AAC8(const WiFiPlazaProfile *profile);
BOOL ov45_0222AADC(const WiFiPlazaProfile *profile);
void ov45_0222C8C8(UnkStruct_ov66_022307D4 *param0, u32 param1, u32 param2);
PPW_LOBBY_RESULT ov45_022320C4(PPW_LobbyTimeInfo *timeInfo);
typedef enum {
    UnkEnum_ov66_02232BEC_00,
    UnkEnum_ov66_02232BEC_01,
    UnkEnum_ov66_02232BEC_02,
    UnkEnum_ov66_02232BEC_03,
    UnkEnum_ov66_02232BEC_04,
    UnkEnum_ov66_02232BEC_05,
    UnkEnum_ov66_02232BEC_06,
    UnkEnum_ov66_02232BEC_07,
    UnkEnum_ov66_02232BEC_08,
} UnkEnum_ov66_02232BEC;
void ov45_0222D500(UnkStruct_ov66_02231428 *param0, const s64 *param1);
void ov45_0222D8BC(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231428 *param1);
u32 ov45_0222ECDC(UnkEnum_ov66_02232BEC param0);
void ov45_0222C370(WiFiPlazaSystem *param0, int param1);
void ov45_0222CA7C(UnkStruct_ov66_022309A4 *param0, u32 param1, u32 param2);
void ov45_0222BAC4(UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);
BOOL ov45_0222BADC(const UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);
void ov45_0222BB00(UnkStruct_ov66_0222F6C4 *param0, const WiFiPlazaProfile *param1, u32 param2);
void ov45_0222B530(s32 param0, const void *param1, void *param2);
void ov45_0222B5A0(PPW_LOBBY_TIME_EVENT param0, void *param1);
void ov45_0222B75C(const void *param0, u32 param1, void *param2);

void ov45_0222B470(s32 param0, void *param1) {
    WiFiPlazaSystem *v0;
    u32 v1, v2;
    BOOL v3;
    s32 v4;

    v0 = param1;

    if (param0 == ov45_0222E9E0()) {
        return;
    }

    v1 = ov45_0222EC68(param0);

    if (ov45_0222AFF8(v0) == 1) {
        if (ov45_0222B020(v0) == v1) {
            ov45_0222BD5C(&v0->unk_1C0);
        }
    }

    v3 = ov45_0222D8D4(v0->unk_04.unk_00, v1);

    if (v3) {
        v4 = ov45_0222EAD4(param0);
        v2 = ov45_0222EC68(v4);

        ov45_0222D8F0(v0->unk_04.unk_00, v2);
    }

    ov45_0222C944(&v0->unk_3CB, v1, 0);
    ov45_0222BDE8(&v0->unk_1C0, v1);
    ov45_0222BDB0(&v0->unk_1C0, v1);

    if (ov45_0222CBD0(&v0->unk_4BC, param0, -1)) {
        ov45_0222BA3C(v0);
    }

    ov45_0222C9A0(&v0->unk_3E4, v1, 0, 0);

    v1 = ov45_0222EC68(param0);
    v0->unk_F8.unk_04 |= 1 << v1;
}

void ov45_0222B530(s32 param0, const void *param1, void *param2) {
    WiFiPlazaSystem *v0;
    u32 v1;
    const WiFiPlazaProfile *v2;
    u32 v3;

    v0 = param2;
    v1 = ov45_0222EC68(param0);

    v0->unk_F8.unk_0C |= 1 << v1;

    v2 = param1;
    v3 = ov45_0222A920(v2);

    switch (v3) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        ov45_0222BDE8(&v0->unk_1C0, v1);
        break;
    default:
        break;
    }

    if (ov45_0222AADC(v2) == 1) {
        ov45_0222C8C8(&v0->unk_3AB, ov45_0222AAC8(v2), v1);
    }
}

void ov45_0222B5A0(PPW_LOBBY_TIME_EVENT param0, void *param1) {
    WiFiPlazaSystem *v0;
    BOOL v1 = 0;
    int v2;

    v0 = param1;

    {
        PPW_LobbyTimeInfo v3;

        ov45_022320C4(&v3);
        ov45_0222D500(&v0->unk_04.unk_D0, &v3.currentTime);

        v0->unk_04.unk_D4 = v3.currentTime;
    }

    switch (param0) {
    case PPW_LOBBY_TIME_EVENT_LOCK:
        ov45_0222D8BC(v0->unk_04.unk_00, &v0->unk_04.unk_D0);
        v0->unk_04.unk_DC = v0->unk_04.unk_D4;
        v0->unk_1FC.unk_00_0 = 1;
        v0->unk_F8.unk_08 |= 1 << 0;
        break;
    case PPW_LOBBY_TIME_EVENT_NEON_A1:
    case PPW_LOBBY_TIME_EVENT_NEON_A2:
    case PPW_LOBBY_TIME_EVENT_NEON_A3:
    case PPW_LOBBY_TIME_EVENT_NEON_A4:
    case PPW_LOBBY_TIME_EVENT_NEON_A5:
        if (v0->unk_04.unk_04 == 0) {
            v1 = 1;
            v2 = 0;

            v0->unk_04.unk_04 = 1;
            v0->unk_1FC.unk_06 = (10 * 30);
        }
    case PPW_LOBBY_TIME_EVENT_NEON_A0:
        v0->unk_1FC.unk_01 = param0;
        v0->unk_F8.unk_08 |= 1 << 1;
        break;
    case PPW_LOBBY_TIME_EVENT_NEON_B1:
    case PPW_LOBBY_TIME_EVENT_NEON_B2:
    case PPW_LOBBY_TIME_EVENT_NEON_B3:
        if (v0->unk_04.unk_04 == 0) {
            v1 = 1;
            v2 = 0;

            v0->unk_04.unk_04 = 1;
            v0->unk_1FC.unk_06 = (10 * 30);
        }
    case PPW_LOBBY_TIME_EVENT_NEON_B0:
        v0->unk_1FC.unk_02 = param0;
        v0->unk_F8.unk_08 |= 1 << 2;
        break;
    case PPW_LOBBY_TIME_EVENT_NEON_C1:
    case PPW_LOBBY_TIME_EVENT_NEON_C2:
    case PPW_LOBBY_TIME_EVENT_NEON_C3:
        if (v0->unk_04.unk_04 == 0) {
            v1 = 1;
            v2 = 0;

            v0->unk_04.unk_04 = 1;
            v0->unk_1FC.unk_06 = (10 * 30);
        }

    case PPW_LOBBY_TIME_EVENT_NEON_C0:
        v0->unk_1FC.unk_03 = param0;
        v0->unk_F8.unk_08 |= 1 << 3;
        break;
    case PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START:
        v1 = 1;
        v2 = 1;

        {
            u32 v4, v5;

            v4 = ov45_0222ECDC(UnkEnum_ov66_02232BEC_06);
            v5 = ov45_0222ECDC(UnkEnum_ov66_02232BEC_07);

            ov45_0222CA7C(&v0->unk_49C, v4, v5);
        }

        v0->unk_1FC.unk_08 = (10 * 30);
        break;
    case PPW_LOBBY_TIME_EVENT_FIRE_WORKS_END:
        v0->unk_1FC.unk_00_2 = 2;
        v0->unk_F8.unk_08 |= 1 << 5;
        break;
    case PPW_LOBBY_TIME_EVENT_PARADE:
        v1 = 1;
        v2 = 2;

        v0->unk_1FC.unk_0A = (10 * 30);
        break;
    case PPW_LOBBY_TIME_EVENT_CLOSE:
        v1 = 1;
        v2 = 4;

        v0->unk_1FC.unk_00_7 = 1;
        v0->unk_1FC.unk_04 = (30 * 30);
        break;
    case PPW_LOBBY_TIME_EVENT_MINIGAME_END:
        v0->unk_1FC.unk_00_5 = 1;
        v0->unk_F8.unk_08 |= 1 << 4;
        break;
    default:
        break;
    }

    if (v1) {
        ov45_0222C370(v0, v2);
    }
}

void ov45_0222B75C(const void *param0, u32 param1, void *param2) {
    WiFiPlazaSystem *v0;
    const WiFiPlazaProfile *v1;

    v0 = param2;
    v1 = param0;

    if (ov45_0222BADC(&v0->unk_108, v0->saveData) == 0) {
        v0->unk_52C = 1;
        return;
    }

    ov45_0222BB00(&v0->unk_108, v1, param1);
    ov45_0222BAC4(&v0->unk_108, v0->saveData);
}
