#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "overlay_96_ov96hurdle_work.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "overlay_96_ov96_mon_icon_work.h"

void *ov96_021EB594(void *);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021EB630(struct Ov96Obj *, s32);
#else
s32 ov96_021EB630(s32, s32);
#endif

s32 ov96_02207BD8(s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_0220764C(Ov96HurdleWork *arg0);

void ov96_0220764C(Ov96HurdleWork *arg0) {
    s32 v0;
    s32 v1;
    s32 v3;
    s32 v5;
    Ov96HurdleCellPair *v2;
    Ov96HurdleCell *v4;

    v4 = &arg0->cells[0];
    v5 = 0;
    v2 = &arg0->pairs[0];
    do {
        ov96_021EB630(v2->sprites[1], 3);
        v0 = *(s32 *)((u8 *)(ov96_021EB594(v2->sprites[0])) + 4);
        ov96_021EB630(v2->sprites[0], ov96_02207BD8((s32)(v0 + ((u32)(v0 >> 0xB) >> 0x14)) >> 0xC) + 7);
        v5 += 1;
        v2++;
    } while (v5 < 0xA);
    v3 = 0;
    do {
        v1 = *(s32 *)((u8 *)(ov96_021EB594(v4->sprite)) + 4);
        ov96_021EB630(v4->sprite, ov96_02207BD8((s32)(v1 + ((u32)(v1 >> 0xB) >> 0x14)) >> 0xC) + 1);
        v3 += 1;
        v4++;
    } while (v3 < 0x14);
}
#else
void ov96_0220764C(void *arg0);

void ov96_0220764C(void *arg0) {
    s32 v0;
    s32 v1;
    s32 v3;
    s32 v5;
    void *v2;
    void *v4;

    v4 = arg0;
    v5 = 0;
    v2 = v4;
    do {
        ov96_021EB630(*(s32 *)((u8 *)v2 + 0x474), 3);
        v0 = *(s32 *)((u8 *)(ov96_021EB594(*(void **)((u8 *)v2 + 0x470))) + 4);
        ov96_021EB630(*(s32 *)((u8 *)v2 + 0x470), ov96_02207BD8((s32)(v0 + ((u32)(v0 >> 0xB) >> 0x14)) >> 0xC) + 7);
        v5 += 1;
        v2 += 0x10;
    } while (v5 < 0xA);
    v3 = 0;
    do {
        v1 = *(s32 *)((u8 *)(ov96_021EB594(*(void **)((u8 *)v4 + 0x380))) + 4);
        ov96_021EB630(*(s32 *)((u8 *)v4 + 0x380), ov96_02207BD8((s32)(v1 + ((u32)(v1 >> 0xB) >> 0x14)) >> 0xC) + 1);
        v3 += 1;
        v4 += 0xC;
    } while (v3 < 0x14);
}
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_022076C0(Ov96HurdleCellPair *param_1, s32 param_2);

void ov96_022076C0(Ov96HurdleCellPair *param_1, s32 param_2) {
    s32 iVar1;

    iVar1 = ov96_02207BD8(param_2);
    ov96_021EB630(param_1->sprites[0], iVar1 + 9);
    ov96_021EB630(param_1->sprites[1], iVar1 + 8);
    return;
}
#else
void ov96_022076C0(u8 *param_1, s32 param_2);

void ov96_022076C0(u8 *param_1, s32 param_2) {
    s32 iVar1;

    iVar1 = ov96_02207BD8(param_2);
    ov96_021EB630(*(s32 *)(param_1 + 4), iVar1 + 9);
    ov96_021EB630(*(s32 *)(param_1 + 8), iVar1 + 8);
    return;
}
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_022076E4(Ov96HurdleWork *arg0);

void ov96_022076E4(Ov96HurdleWork *arg0) {
    s32 v0;
    s32 v1;
    void **v2;

    v2 = &arg0->unk_35C[0];
    v1 = 0;
    do {
        v0 = *(s32 *)((u8 *)(ov96_021EB594(*v2)) + 4);
        ov96_021EB630(*v2, ov96_02207BD8((s32)(v0 + ((u32)(v0 >> 0xB) >> 0x14)) >> 0xC) + 2);
        v1 += 1;
        v2++;
    } while (v1 < 4);
}
#else
void ov96_022076E4(void *arg0);

void ov96_022076E4(void *arg0) {
    s32 v0;
    s32 v1;
    void *v2;

    v2 = arg0;
    v1 = 0;
    do {
        v0 = *(s32 *)((u8 *)(ov96_021EB594(*(void **)((u8 *)v2 + 0x35C))) + 4);
        ov96_021EB630(*(s32 *)((u8 *)v2 + 0x35C), ov96_02207BD8((s32)(v0 + ((u32)(v0 >> 0xB) >> 0x14)) >> 0xC) + 2);
        v1 += 1;
        v2 += 4;
    } while (v1 < 4);
}
#endif

void ov96_021EABA8(s32 *param_1, u32 param_2);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_02207718(Ov96HurdleRunner *param_1, s32 param_2);

void ov96_02207718(Ov96HurdleRunner *param_1, s32 param_2) {
    s32 iVar1;
    s32 iVar2;
    void **p;

    iVar1 = ov96_02207BD8(param_2);
    ov96_021EB630(param_1->unk_48, iVar1 + 3);
    iVar2 = 0;
    p = &param_1->unk_00[0];
    do {
        ov96_021EABA8((s32 *)*p, iVar1 + 4);
        iVar2 = iVar2 + 1;
        p++;
    } while (iVar2 < 3);
    return;
}
#else
void ov96_02207718(u32 *param_1, s32 param_2);

void ov96_02207718(u32 *param_1, s32 param_2) {
    s32 iVar1;
    s32 iVar2;

    iVar1 = ov96_02207BD8(param_2);
    ov96_021EB630(param_1[0x12], iVar1 + 3);
    iVar2 = 0;
    do {
        ov96_021EABA8((s32 *)*param_1, iVar1 + 4);
        iVar2 = iVar2 + 1;
        param_1 = param_1 + 1;
    } while (iVar2 < 3);
    return;
}
#endif

extern WindowTemplate ov96_0221CAC0;

void ov96_02207740(Ov96HurdleWork *arg0);

void ov96_02207740(Ov96HurdleWork *arg0) {
    AddWindow(arg0->bgConfig, &arg0->window, &ov96_0221CAC0);
    BG_FillCharDataRange(arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, arg0->heapId);
}

void ov96_02207774(Ov96HurdleWork *param_1, s32 param_2);

void ov96_02207774(Ov96HurdleWork *param_1, s32 param_2) {
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;

    FillWindowPixelBuffer(&param_1->window, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, param_1->heapId);
    messageFormat = MessageFormat_New(param_1->heapId);
    BufferIntegerAsString(messageFormat, 0, param_2, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0x9d, param_1->heapId);
    AddTextPrinterParameterizedWithColor(&param_1->window, FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&param_1->window);
    return;
}

void ov96_022077F4(Ov96HurdleWork *param_1);

void ov96_022077F4(Ov96HurdleWork *param_1) {
    param_1->unk_618 = 0;
    return;
}

void ov96_02207800(u8 *param_1);

void ov96_02207800(u8 *param_1) {
    u16 remainder;

    remainder = (s32)(*(u16 *)(param_1 + 0xb4) + 1) % (s32)(0x14);
    *(u16 *)(param_1 + 0xb4) = remainder;
    return;
}
