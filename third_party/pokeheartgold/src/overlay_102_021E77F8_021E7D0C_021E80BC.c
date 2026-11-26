#include "global.h"

#include "unk_02005D10.h"
#include "overlay_102_easy_chat_app.h"

void ov102_021E7968(void *, MailMessage *);

void ov102_021E79A0(void *, MailMessage *);

u32 ov102_021E7D30(void *param_1, u32 param_2);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov102_021E7F6C(void *param_1, s32);
#else
u8 ov102_021E7F6C(void *param_1, s32);
#endif

void ov102_021E940C(void *, u32);

u32 ov102_021E80BC(EasyChatApp *param_1, s32 arg1);

u32 ov102_021E80BC(EasyChatApp *param_1, s32 arg1) {
    u8 iVar1;
    u32 uVar2;

    iVar1 = ov102_021E7F6C(param_1, arg1);
    switch (iVar1) {
    case 0:
    case 1:
        PlaySE(0x5dc);
        *((s16 *)&param_1->unk_4C) = (s16)iVar1 + 1;
        ov102_021E940C(((void *)param_1->unk_14), 0xf);
        uVar2 = ov102_021E7D30(param_1, (u32) param_1->unk_4C);
        return uVar2;
    case 2:
    case 3:
        param_1->unk_4C = 0;
        ov102_021E940C(((void *)param_1->unk_14), 0xf);
        if (iVar1 == 2) {
            ov102_021E79A0(&param_1->unk_64, &param_1->sentence);
        } else {
            ov102_021E7968(&param_1->unk_64, &param_1->sentence);
        }
        uVar2 = ov102_021E7D30(param_1, 4);
        return uVar2;
    case 4:
    case 5:
        param_1->unk_4C = 0;
        *((s16 *)&param_1->currentWordSlot) = (s16)iVar1 + -4;
        ov102_021E940C(((void *)param_1->unk_14), 0xf);
        uVar2 = ov102_021E7D30(param_1, 0);
        return uVar2;
    case 6:
        param_1->unk_4C = 0;
        ov102_021E940C(((void *)param_1->unk_14), 0xf);
    }
    return 1;
}
