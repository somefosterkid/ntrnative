#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "render_window.h"
#include "sprite.h"
#include "sys_task.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "heap.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;

typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;

typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;

void ov48_0225A288(void *arg0);

void ov48_0225AAAC(UnkStruct_ov69_0225EB60 *param0, UnkStruct_ov69_0225D35C *param1, UnkStruct_ov69_0225E478 *param2, u32 param3);

void ov48_0225A5C4(UnkStruct_ov69_0225E478 *);

void ov48_0225A6DC(UnkStruct_ov69_0225E084 *, UnkStruct_ov69_0225D35C *, enum HeapID);

void ov48_0225A894(UnkStruct_ov69_0225E7DC *, UnkStruct_ov69_0225D35C *, u8 *, u16, enum HeapID);

String *ov48_0225B0C4(UnkStruct_ov69_0225EF54 *, u32);

extern u32 _0225B164;

extern u32 ov48_0225B268;

void ov48_0225A00C(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, UnkStruct_ov69_0225EF54 *param2, enum HeapID heapID);

void ov48_0225A294(u8 *);
void ov48_0225A634(u8 *);
void ov48_0225A768(u8 *);
void ov48_0225A928(u8 *);
void ov48_0225AC34(u8 *, void *);
void ov48_0225A108(u8 *param_1, void *param_2);

void ov48_0225A00C(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, UnkStruct_ov69_0225EF54 *param2, enum HeapID heapID) {
    String *text;
    void *pvVar1;
    u8 *puVar2;
    UnkStruct_ov69_0225E7DC *pUVar3;
    u8 *puVar4;
    s32 iVar5;

    param0->unk_00 = 0;
    ov48_0225A288(param0);
    puVar2 = (unsigned char *)(&_0225B164);
    puVar4 = (unsigned char *)(&ov48_0225B268);
    iVar5 = 0;
    pUVar3 = param0->unk_A0;
    do {
        ov48_0225A894(pUVar3, param1, puVar4, *puVar2, heapID);
        puVar2 = puVar2 + 1;
        iVar5 = iVar5 + 1;
        puVar4 = puVar4 + 0xc;
        pUVar3 = pUVar3 + 1;
    } while (iVar5 < 5);
    AddWindowParameterized(param1->unk_00, &param0->unk_1A8, 4, 2, 0x13, 0x1b, 4, 0xb, 0x28);
    FillWindowPixelBuffer(&param0->unk_1A8, 0xf);
    text = (String *)ov48_0225B0C4(param2, 0);
    AddTextPrinterParameterized(&param0->unk_1A8, FONT_MESSAGE, text, 0, 0, 0xff, (PrinterCallback_t)0x0);
    DrawFrameAndWindow2(&param0->unk_1A8, 0, 10, 0xa);
    ov48_0225A6DC(param0, param1, heapID);
    pvVar1 = GfGfxLoader_GetScrnDataFromOpenNarc(param1->unk_1A8, 0x13, 0, &param0->unk_98, heapID);
    param0->unk_94 = pvVar1;
    ov48_0225AAAC(&param0->unk_1B8, param1, &param0->unk_2E4, heapID);
    ov48_0225A5C4(&param0->unk_2E4);
    return;
}

void ov48_0225A108(u8 *param_1, void *param_2) {
    s32 iVar1;
    u8 *iVar2;

    ov48_0225A634(param_1 + 0x2e4);
    ov48_0225AC34(param_1 + 0x1b8, param_2);
    Heap_Free(*(void **)(param_1 + 0x94));
    ov48_0225A768(param_1);
    RemoveWindow((Window *)(param_1 + 0x1a8));
    ov48_0225A294(param_1);
    iVar1 = 0;
    iVar2 = param_1 + 0xa0;
    do {
        ov48_0225A928(iVar2);
        iVar1 = iVar1 + 1;
        iVar2 = iVar2 + 0x28;
    } while (iVar1 < 5);
    return;
}
