#include "global.h"

#include "overlay_75.h"
#include "yes_no_prompt.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

struct UnkStruct_ov75_02248B8C {
    u8 unk_0[0x4];
    void *unk_4;
    s32 unk_8;
    u8 unk_C[0x28];
    void *unk_34;
    u8 unk_38[0xC];
    s32 unk_44;
    u8 unk_48[0x40];
    YesNoPrompt *unk_88;
    u8 unk_8C[0x8];
    u32 unk_94;
};

s32 ov75_02247854(void *, s32, s32);

s32 ov75_02247878(void *);

YesNoPrompt *ov75_02247890(BgConfig *, s32, s32);

s32 ov75_022494CC(void *, void *, s32, s32, u16);

s32 ov75_02249534(s32);

s32 ov75_02248B8C(UnkStruct_ov75_02248B8C *arg0) {
    enum YesNoResponse v1;
    u32 v0;

    if (ov75_02249534(arg0->unk_44) == 1) {
        return 0;
    }
    v0 = arg0->unk_94;
    switch (v0) {
    case 0:
        ov75_02247878(arg0);
        ov75_022494CC(arg0, arg0->unk_34, 0xD, 1, 0xF0F);
        arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        break;
    case 1:
        arg0->unk_88 = ov75_02247890(arg0->unk_4, 0x234, 0);
        arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        break;
    case 2:
        v1 = YesNoPrompt_HandleInput(arg0->unk_88);
        if (v1 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->unk_88);
            arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        } else if (v1 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->unk_88);
            arg0->unk_8 = 0x15;
        }
        break;
    case 3:
        ov75_022494CC(arg0, arg0->unk_34, 0x11, 1, 0xF0F);
        ov75_02247854(arg0, 0x22, 0x14);
        break;
    }
    return 0;
}

s32 ov75_02246BCC(void *, s32, s32);

s32 ov75_02246BE0(void *, s8);

s32 ov75_02248C64(void *arg0) {
    ov75_02246BCC(*(void **)arg0, 2, 1);
    ov75_02246BE0(*(void **)arg0, 0x16);
    *(s32 *)((u8 *)arg0 + 8) = 0x1E;
    return 0;
}

s32 ov75_02248C84(void *arg0) {
    enum YesNoResponse v1;
    s32 v0;

    if (ov75_02249534(*((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_44)) == 1) {
        return 0;
    }
    v0 = *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94);
    switch (v0) {
    case 0:
        ov75_022494CC(arg0, *((void **)&((UnkStruct_ov98_02247704 *)arg0)->unk_34), 0xE, 1, 0xF0F);
        *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) = (s32)(*((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) + 1);
        break;
    case 1:
        *((YesNoPrompt **)&((UnkStruct_ov98_02247704 *)arg0)->unk_88) = ov75_02247890(*(void **)((u8 *)arg0 + 4), 0x234, 0);
        *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) = (s32)(*((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) + 1);
        break;
    case 2:
        v1 = YesNoPrompt_HandleInput(((YesNoPrompt *)((UnkStruct_ov98_02247704 *)arg0)->unk_88));
        if (v1 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(((YesNoPrompt *)((UnkStruct_ov98_02247704 *)arg0)->unk_88));
            ov75_02247854(arg0, 0x1A, 0);
        } else if (v1 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(((YesNoPrompt *)((UnkStruct_ov98_02247704 *)arg0)->unk_88));
            *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_08) = 0x13;
        }
        break;
    }
    return 0;
}
