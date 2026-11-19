#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"

s32 ov41_02247D1C(void *, s32);

void ov41_02247AB4(void *arg0);

SysTask *ov41_02247D3C(void *);
void ov41_02247B5C(void *arg0);

void ov41_02247AB4(void *arg0) {
    YesNoPromptTemplate dest;

    BgClearTilemapBufferAndCommit(*(BgConfig **)((u8 *)arg0 + 0x40), 3U);
    BgSetPosTextAndCommit(*(BgConfig **)((u8 *)arg0 + 0x40), 3U, BG_POS_OP_SET_ROT, 0);
    BgSetPosTextAndCommit(*(BgConfig **)((u8 *)arg0 + 0x40), 3U, BG_POS_OP_SET_XSCALE, 0);
    MI_CpuFill8(&dest, 0U, sizeof(YesNoPromptTemplate));
    dest.bgConfig = *(BgConfig **)((u8 *)arg0 + 0x40);
    dest.bgId = 3;
    dest.tileStart = 0;
    dest.plttSlot = 5;
    dest.x = 0x19;
    dest.y = 4;
    *(u8 *)((u8 *)(&dest) + 0x12) = (u8)((*(u8 *)((u8 *)(&dest) + 0x12) & ~0xF) | (0xF & (u8) * (s32 *)((u8 *)arg0 + 0x6EC)));
    YesNoPrompt_InitFromTemplate(*(YesNoPrompt **)((u8 *)arg0 + 0x6B8), (YesNoPromptTemplate *)&dest);
    ov41_02247D1C(arg0, 1);
    reg_G2_BG0CNT = (u16)(2 | (reg_G2_BG0CNT & ~3));
    reg_G2_BG1CNT = (u16)(1 | (reg_G2_BG1CNT & ~3));
    reg_G2_BG2CNT = (u16)(3 | (reg_G2_BG2CNT & ~3));
    reg_G2_BG3CNT = (u16)(reg_G2_BG3CNT & ~3);
}

void ov41_02247B5C(void *arg0) {
    YesNoPrompt_Reset(*(YesNoPrompt **)((u8 *)arg0 + 0x6B8));
    ov41_02247D3C(arg0);
    BgClearTilemapBufferAndCommit(*(BgConfig **)((u8 *)arg0 + 0x40), 3U);
}
