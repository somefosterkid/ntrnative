#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

#include "game_stats.h"
#include "constants/heap.h"
#include "save.h"
#include "overlay_45_wifi_plaza_system.h"

struct PasswordWordBank;
struct PlayerProfile;
struct UnkStruct_0207E060_t;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;
typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;
typedef u16 charcode_t;
typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;
typedef struct PasswordWordBank PasswordWordBank;
BOOL ov45_0222A3A0(const WiFiPlazaSystem *param0);
void ov45_0222B118(WiFiPlazaSystem *param0, int param1);
int ov45_0222B134(const WiFiPlazaSystem *param0);

void IncrementGameStat47(WiFiPlazaSystem *arg0) {
    GameStats_Inc(Save_GameStats_Get(arg0->saveData), 0x2F);
}

void IncrementGameStat119(WiFiPlazaSystem *arg0) {
    GameStats_Inc(Save_GameStats_Get(arg0->saveData), 0x77);
}

void ov45_0222B118(WiFiPlazaSystem *param0, int param1) {
    ((param1 < 8) ? (void)0 : GF_AssertFail());
    param0->unk_3A3.unk_00[param1] = 1;
}

int ov45_0222B134(const WiFiPlazaSystem *param0) {
    int v0;
    BOOL v1;

    for (v0 = 0; v0 < 8; v0++) {
        if (param0->unk_3A3.unk_00[v0] == 0) {
            switch (v0) {
            case 0:
            case 1:
            case 2:
                if (ov45_0222A394(param0) == 1) {
                    v1 = 0;
                } else {
                    v1 = 1;
                }
                break;
            case 3:
            case 4:
            case 5:
                if (ov45_0222A330(param0) == 1) {
                    v1 = 0;
                } else {
                    v1 = 1;
                }
                break;
            case 6:
                if (ov45_0222A3A0(param0) == 1) {
                    v1 = 0;
                } else {
                    v1 = 1;
                }
                break;
            default:
            case 7:
                v1 = 1;
                break;
            }

            if (v1 == 1) {
                return v0;
            }
        }
    }

    return 8;
}
