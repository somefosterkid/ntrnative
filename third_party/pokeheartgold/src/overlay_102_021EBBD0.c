#include "global.h"
#include "simple3dmodel.h"
struct EasyChatCategoriesContext;

#include "bg_window.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "overlay_85_ov109_021D0F70.h"

u8 ov102_021E8F7C(void *);

void ov102_021EBBD0(struct EasyChatCategoriesContext *arg0);

void ov102_021EC2EC(fx32 *param0, fx32 param1);
void ov102_021EBC28(UnkStruct_ov109_021D2AE4 *param0, fx32 param1);

void ov102_021EBBD0(EasyChatCategoriesContext *arg0) {
    s32 val;

    if (ov102_021E8F7C(*((void **)&arg0->easyChatApp)) == 0) {
        val = -4;
    } else {
        val = 0xFC;
    }
    BgSetPosTextAndCommit(arg0->bgConfig, 1U, BG_POS_OP_SET_ROT, val);
}

s32 ov102_021EC298(void *, BgConfig *, s32, s32, s32, s32);

void ov102_021EBBF4(void *arg0);

void ov102_021EBBF4(void *arg0) {
    s32 v0;

    if (ov102_021E8F7C(*(void **)((u8 *)arg0 + 4)) == 0) {
        v0 = -0x100;
    } else {
        v0 = 0x100;
    }
    ov102_021EC298(arg0 + 0x3C, *((void **)&((EasyChatCategoriesContext *)arg0)->bgConfig), 1, 0, v0, 0x10);
}

void ov102_021EBC28(UnkStruct_ov109_021D2AE4 *param0, fx32 param1) {
    ov102_021EC2EC(&param0->unk_3C, param1);
}
