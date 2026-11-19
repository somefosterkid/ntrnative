#include "global.h"

#include "unk_0202C034.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern void *_0221A680;

void ov00_021E6274(s32 arg0);

void ov00_021E6274(s32 arg0) {
    if (arg0 == 0) {
        *((s32 *)&((NintendoWFCManager *)_0221A680)->state) = 4;
        return;
    }
    *((s32 *)&((NintendoWFCManager *)_0221A680)->state) = 0xC;
}

void ov00_021E6298(int arg0, u8 arg1, const char *arg2, void *arg3);

void ov00_021E6298(int arg0, u8 arg1, const char *arg2, void *arg3) {
}

s32 sub_0202C5E4(WiFiList *, s32, s32);

s32 sub_02031214(FrontierSave *, s32, s32);

void ov00_021E629C(int arg0, int arg1, void *arg2);

void ov00_021E629C(int arg0, int arg1, void *arg2) {
    sub_0202C5E4(sub_0202C6F4(((NintendoWFCManager *)_0221A680)->saveData), arg0, arg1);
    sub_02031214(Save_Frontier_GetStatic(((NintendoWFCManager *)_0221A680)->saveData), arg0, arg1);
}

void ov00_021E62D4(int arg0, void *arg1);

void ov00_021E62D4(int arg0, void *arg1) {
}
