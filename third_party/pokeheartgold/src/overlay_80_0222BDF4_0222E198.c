#include "global.h"

#include "frontier/frontier.h"

#include "player_data.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "frontier/overlay_80_0222BDF4.h"
#include "overlay_80_wfcfacility_selector.h"

typedef struct UnkStruct_FrtCmd_207 {
    void *unk_0;
    u8 unk_4[0x18];
    u8 *unk_1C;
} UnkStruct_FrtCmd_207;

typedef struct UnkStruct_FrtCmd_206 {
    void *unk_0;
    u8 unk_4[0x18];
    u8 *unk_1C;
} UnkStruct_FrtCmd_206;

s32 sub_02096BF8(WFCFacilitySelector *selector, u16 streakDeletionChoice);

s32 ov80_0222E198(void *arg0);

s32 FrtCmd_204(FrontierScriptContext *arg0);
s32 FrtCmd_205(void **arg0);

s32 ov80_0222F5D0(void *, BOOL);
s32 FrtCmd_206(UnkStruct_FrtCmd_206 *arg0);
s32 ov80_0222F5EC(s32, BOOL);
s32 FrtCmd_207(UnkStruct_FrtCmd_207 *arg0);

s32 ov80_0222E198(void *arg0) {
    s32 v2;
    u16 v1;
    void *v0;

    if (sub_02037454() < 2) {
        v2 = 1;
    } else {
        v2 = sub_02037B38((u8) *((u16 *)&((FrontierScriptContext *)arg0)->data[0]));
    }
    if (v2 == 0) {
        v0 = Frontier_GetData(*(void **)(*(void **)arg0));
        if ((v0 != NULL) && (((WFCFacilitySelector *)v0)->msgsReceived != 0)) {
            v1 = ((WFCFacilitySelector *)v0)->partnersStreakDeletionChoice;
            if (v1 == 0x1 || v1 == 0xFFFE) {
                v2 = 1;
                *(s8 *)((u8 *)(*(void **)arg0) + 0x39) = 1;
            } else if (v1 == 0x0) {
                if (PlayerProfile_GetVersion(sub_02034818(sub_0203769C() ^ 1)) == 0xC) {
                    ((WFCFacilitySelector *)v0)->msgsReceived = 0U;
                    sub_02096BF8(v0, 0);
                }
            }
        }
    }
    return v2;
}

s32 FrtCmd_204(FrontierScriptContext *ctx) {
    *FrontierScript_ReadVarPtr(ctx) = (u16)ctx->frontierSystem->unk4[0x35];
    return 1;
}

s32 FrtCmd_205(void **arg0) {
    *(s8 *)((u8 *)(*arg0) + 0x39) = 0;
    return 1;
}

s32 FrtCmd_206(UnkStruct_FrtCmd_206 *arg0) {
    u8 *new_var;
    u8 *v0;
    new_var = ((u8 *)(arg0->unk_0)) + 0x60;
    v0 = arg0->unk_1C;
    arg0->unk_1C = (u8 *)(v0 + 1);
    ov80_0222F5D0(*((void **)new_var), *v0);
    return 1;
}

s32 FrtCmd_207(UnkStruct_FrtCmd_207 *arg0) {
    u8 *v0;
    s32 *new_var;
    new_var = &(*((s32 *)(((u8 *)(arg0->unk_0)) + 0x60)));
    v0 = arg0->unk_1C;
    arg0->unk_1C = (u8 *)(v0 + 1);
    ov80_0222F5EC(*new_var, *v0);
    return 1;
}
