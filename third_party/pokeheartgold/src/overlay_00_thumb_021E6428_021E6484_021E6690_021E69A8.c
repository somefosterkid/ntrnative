#include "global.h"

#include "overlay_00_thumb.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "overlay_00_nintendo_wfcmanager.h"

s32 ov00_021E77F4(s32, s32, s32);

s32 ov00_021E7A1C(s32 *);

s32 sub_02034084(int param0);

extern void *_0221A680;

extern s32 ov00_021E6994;

void ov00_021E69A8(s32 arg0) {
    s32 v0;
    s32 v2;
    s32 v3;
    u32 v1;

    v3 = 1;
    v0 = sub_02034084(sub_0203993C());
    if (v0 != 0) {
        v3 = sub_02037454() - 1;
        if (v3 < 1) {
            v3 = 1;
        }
        *((s16 *)&((NintendoWFCManager *)_0221A680)->inVoiceChatLobby) = 1;
        *((s32 *)&((NintendoWFCManager *)_0221A680)->otherPlayersEnabledVoiceChat) = 1;
        *((s16 *)&((NintendoWFCManager *)_0221A680)->voiceChatEnabled_Battles) = 1;
    }
    if (*((s32 *)&((NintendoWFCManager *)_0221A680)->voiceChatActive) == 0) {
        v1 = *((u32 *)&((NintendoWFCManager *)_0221A680)->voiceChatCodec);
        switch (v1) {
        case 2:
            v2 = 1;
            break;
        case 3:
            v2 = 2;
            break;
        case 4:
            v2 = 3;
            break;
        case 5:
            v2 = 4;
            break;
        default:
            if (v0 == 0) {
                v2 = 4;
            } else {
                v2 = 3;
            }
            break;
        }
        ov00_021E77F4(arg0, v2, v3);
        ov00_021E7A1C(&ov00_021E6994);
        *((s32 *)&((NintendoWFCManager *)_0221A680)->voiceChatActive) = 1;
    }
}

s32 ov00_021E7A30(void);

void ov00_021E6A4C(void) {
    ov00_021E7A30();
    if (_0221A680 != NULL) {
        *((s32 *)&((NintendoWFCManager *)_0221A680)->voiceChatActive) = 0;
        *((s32 *)&((NintendoWFCManager *)_0221A680)->voiceConnectedIDs) = 0;
    }
}
