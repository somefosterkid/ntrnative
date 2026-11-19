#include "global.h"

#include "unk_02037C94.h"
#include "overlay_00_nintendo_wfcmanager.h"

s32 sub_02034084(int param0);

extern void *_0221A680;

void ov00_021E644C(s32 arg0);

void ov00_021E644C(s32 arg0) {
    if (sub_02034084(sub_0203993C()) == 0) {
        if (0x100 & arg0) {
            *((s32 *)&((NintendoWFCManager *)_0221A680)->otherPlayersEnabledVoiceChat) = 1;
            return;
        }
        *((s32 *)&((NintendoWFCManager *)_0221A680)->otherPlayersEnabledVoiceChat) = 0;
    }
}
