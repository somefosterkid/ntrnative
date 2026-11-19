#include "global.h"

#include "error_handling.h"
#include "save.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"
#include "unk_0202C034.h"
#include "unk_struct_ov66_022342_f4.h"

struct PPW_LobbyVipRecord;

struct WiFiHistory;

struct WiFiList;

typedef struct WiFiHistory WiFiHistory;

typedef struct {
    PPW_LobbyQuestionnaire lobbyQuestionnaire;
    u32 unk_2D8;
} UnkStruct_ov66_0223439C;

const PPW_LobbySystemProfile *ov45_0222F954(const UnkStruct_ov66_022336C4 *param0, s32 param1);

void ov45_0222EA4C(s32 param0, s64 *param1);

s32 ov45_0222EAD4(s32 param0);

void ov45_0222EA4C(s32 param0, s64 *param1) {
    GF_ASSERT(_022577C0 != NULL);

    const PPW_LobbySystemProfile *sysProfile = ov45_0222F954(_022577C0, param0);
    *param1 = sysProfile->enterTime;
}

s32 ov45_0222FB5C(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);

s32 ov45_0222EA78(void);

s32 ov45_0222EA78(void) {
    int v0;
    s32 v1 = -1;
    s64 v2 = 0xfffffffffffffff;

    ((_022577C0 != 0L) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < 20; v0++) {
        {
            s32 v3;
            s64 v4;

            v3 = ov45_0222FB5C(_022577C0, v0, 0);

            if (v3 != -1) {
                ov45_0222EA4C(v3, &v4);

                if (v2 > v4) {
                    v2 = v4;
                    v1 = v3;
                }
            }
        }
    }

    return v1;
}

s32 ov45_0222EAD4(s32 param0) {
    int v0;
    s32 v1 = -1;
    s64 v2 = 0xfffffffffffffff;

    ((_022577C0 != 0L) ? (void)0 : GF_AssertFail());

    for (v0 = 0; v0 < 20; v0++) {
        {
            s32 v3;
            s64 v4;

            v3 = ov45_0222FB5C(_022577C0, v0, 0);

            if ((v3 != -1) && (v3 != param0)) {
                ov45_0222EA4C(v3, &v4);

                if (v2 > v4) {
                    v2 = v4;
                    v1 = v3;
                }
            }
        }
    }

    return v1;
}
