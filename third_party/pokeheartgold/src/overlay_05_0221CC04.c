#include "global.h"

#include "overlay_05.h"
#include "palette.h"
#include "screen_fade.h"
#include "systask_environment.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_0208805C.h"
#include "vram_transfer_manager.h"
#include "unk_02035900.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

s32 ov05_0221DE6C(void *, u8);

s32 ov05_0221E564(void *, s16);

BOOL ov05_0221CCF4(void);

s32 ov05_0221CC04(UnkStruct_ov10_0221FB28 *arg0) {
    u8 v0;

    if (arg0->unk_B82 == 0) {
        PlaySE(0x719U);
    }
    v0 = arg0->unk_B82;
    if (v0 == 0x10) {
        arg0->unk_B82 = 0U;
        arg0->unk_B81 = 0x40;
        return 1;
    }
    arg0->unk_B82 = (u8)(v0 + 1);
    ov05_0221E564(arg0, (s16)(0x10 - arg0->unk_B82));
    ov05_0221DE6C(arg0, arg0->unk_B82);
    return 0;
}

s32 ov05_0221CC58(UnkStruct_ov10_0221FB28 *arg0) {
    App_StartScreenFade(1U, arg0->trainerIntroData->heapID);
    arg0->unk_B7F = 2;
    return 1;
}

s32 ov05_0221CE0C(void *);

s32 ov05_0221CFF0(void *);

s32 ov05_0221D020(void *);

s32 ov05_0221D054(void *);

s32 ov05_0221D690(UnkStruct_ov10_0221FB28 *);

s32 ov05_0221DB18(UnkStruct_ov10_0221FB28 *);

s32 ov05_0221DC34(void *);

s32 ov05_0221E60C(void *);

s32 ov05_0221CC74(SysTask *task, UnkStruct_ov10_0221FB28 *arg1) {
    if (IsPaletteFadeFinished() == 0) {
        return 0;
    }
    Main_SetVBlankIntrCB(NULL, NULL);
    ov05_0221EA38(arg1);
    if (arg1->trainerIntroData->mode != 0) {
        ov05_0221CFF0(arg1);
        ov05_0221D020(arg1);
        ov05_0221D054(arg1);
        ov05_0221DB18(arg1);
        ov05_0221D690(arg1);
    } else {
        ov05_0221DC34(arg1);
        ov05_0221E60C(arg1);
    }
    ov05_0221CE0C(arg1);
    GF_DestroyVramTransferManager();
    PaletteData_FreeBuffers(arg1->unk_08, PLTTBUF_MAIN_BG);
    PaletteData_Free(arg1->unk_08);
    arg1->trainerIntroData->isDone = 1;
    DestroySysTaskAndEnvironment(task);
    return 1;
}

BOOL ov05_0221CCF4(void) {
    switch (sub_020378AC(sub_0203769C())) {
    case 0:
    case 2:
        return 1;
    case 1:
    case 3:
        return 0;
    }

    return 0;
}
