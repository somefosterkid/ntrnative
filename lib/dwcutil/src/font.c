// libdwcutil's font.c (Black's ov016 0x021d5824-0x021d5fe8): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;

static s32 data_ov016_021de680;
s32 *func_ov016_021d705c(s32 param_1, s32 param_2, s32 param_3, u32 *param_4);

typedef struct DWCiCanvas {
    NNSG2dCharCanvas chr;
    NNSG2dTextCanvas text;
    union {
        struct {
            void *ovram;
            u16 name;
            u8 width;
            u8 height;
        };
        struct {
            void *buf;
            s32 *task;
        };
    };
} DWCiCanvas;

typedef struct DWCiFontWork {
    NNSG2dFont font[2];
    u8 unk_10[0x600];
    DWCiCanvas bg[2];
    u32 queue;
    u32 arc[2];
    u8 load[2];
} DWCiFontWork;

typedef struct Ov016Mode {
    u8 unk_0[7];
    u8 mode;
} Ov016Mode;

typedef struct Ov016Node {
    struct Ov016Node *unk_0;
    u8 unk_4[4];
    Ov016Mode *mode;
} Ov016Node;

typedef struct Ov016Text {
    u8 unk_0[0x1c];
    Ov016Node *node;
} Ov016Text;

NNSG2dCharCanvas *DWCi_FNTlFormEx(s32 param_1, s32 param_2, s32 param_3, s32 param_4, s32 *param_5, s32 param_6);
void taskLoad_font(u32 param_1, u8 *param_2);
void DWCi_FNTlDrawCharEx(NNSG2dCharCanvas *param_1, s32 param_2, s32 param_3, s32 param_4, u16 param_5, s32 param_6);
void func_ov016_021d5f28(void *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5);


// The prebuilt's names in the comments, defined in the order its string numbers give (FONT_FILE's first), which gives
// Black's layout of MWCC's size sort
static char *data_ov016_021dbfec[2] = { "msg/lc_m.NFTR.l", "msg/lc_s.NFTR.l" };  // FONT_FILE
static char *data_ov016_021dbfe4[2] = { "msg/kc_m.NFTR.l", "msg/lc_s.NFTR.l" };  // KONT_FILE
static const COORD data_ov016_021daa74[2] = { { 0x20, 0x18 }, { 0x20, 0x0c } };  // S_CANV
static const u16 data_ov016_021daa70[2] = { 0, 0x180 };  // START_CHR

void * G2S_GetBG0ScrPtr(void);

void func_ov016_021d5824(void) {
    s32 i;

    data_ov016_021de680 = (s32)DWCi_HEAPlAllocEx(0x680, 4);
    ((DWCiFontWork *)data_ov016_021de680)->queue = DWCi_QUElFormEx(0x20, (s32)((DWCiFontWork *)data_ov016_021de680)->unk_10, 0x30);
    if (DWCi_Language() == 6) {
        for (i = 0; i < 2; i++) {
            ((DWCiFontWork *)data_ov016_021de680)->arc[i] = (u32)DWCi_ARClReadEx((s8 *)data_ov016_021dbfe4[i], NULL, 4);
            NNS_G2dFontInitUTF16(&((DWCiFontWork *)data_ov016_021de680)->font[i], (void *)((DWCiFontWork *)data_ov016_021de680)->arc[i]);
        }
    } else {
        for (i = 0; i < 2; i++) {
            ((DWCiFontWork *)data_ov016_021de680)->arc[i] = (u32)DWCi_ARClReadEx((s8 *)data_ov016_021dbfec[i], NULL, 4);
            NNS_G2dFontInitUTF16(&((DWCiFontWork *)data_ov016_021de680)->font[i], (void *)((DWCiFontWork *)data_ov016_021de680)->arc[i]);
        }
    }
}

void DWCi_FNTlEnd(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        DWCi_ARClRelease(((DWCiFontWork *)data_ov016_021de680)->arc[i]);
    }
    DWCi_QUElDelete(((DWCiFontWork *)data_ov016_021de680)->queue);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de680);
}

NNSG2dCharCanvas *DWCi_FNTlFormEx(s32 lcd, s32 width, s32 height, s32 fromFront, s32 *objCount, s32 font) {
    DWCiCanvas *cv;
    u8 *base;
    u32 name;

    cv = (DWCiCanvas *)DWCi_QUElPopBack((u16 *)((DWCiFontWork *)data_ov016_021de680)->queue);
    cv->width = width;
    cv->height = height;
    cv->ovram = func_ov016_021d705c(lcd, width * height, fromFront, &name);
    cv->name = name;
    *objCount = NNSi_G2dCalcRequiredOBJ(width, height);
    base = (u8 *)(lcd == 1 ? HW_DB_OBJ_VRAM : HW_OBJ_VRAM);
    base += name * 0x80;
    NNS_G2dCharCanvasInitForOBJ1D(&cv->chr, base, width, height, NNS_G2D_CHARA_COLORMODE_16);
    cv->chr.vtable->pClear(&cv->chr, 0);
    NNS_G2dTextCanvasInit(&cv->text, &cv->chr, &((DWCiFontWork *)data_ov016_021de680)->font[font], 1, 1);
    return &cv->chr;
}

void DWCi_FNTlDelete(s32 canvas) {
    DWCi_OVRlFree(*(s32 **)(canvas + 0x28));
    DWCi_QUElPushBack(*(u16 **)(data_ov016_021de680 + 0x670), canvas);
}

NNSG2dCharCanvas *DWCi_FNTlFormBgEx(s32 lcd, s32 font) {
    DWCiCanvas *cv = &((DWCiFontWork *)data_ov016_021de680)->bg[lcd];
    u32 height = data_ov016_021daa74[lcd].y;
    u32 width = data_ov016_021daa74[lcd].x;
    DWCiFontWork *work;
    void *scr;

    cv->buf = DWCi_HEAPlAllocEx(width * height * 0x20, 0x20);
    if (lcd == 1) {
        reg_G2S_DB_BG0CNT = reg_G2S_DB_BG0CNT & 0xffbf;
        reg_G2S_DB_BG0CNT = (reg_G2S_DB_BG0CNT & 0x43) | 0xc00;
    } else {
        reg_G2_BG0CNT = reg_G2_BG0CNT & 0xffbf;
        reg_G2_BG0CNT = (reg_G2_BG0CNT & 0x43) | 0xc00;
    }
    NNS_G2dCharCanvasInitForBG(&cv->chr, cv->buf, width, height, NNS_G2D_CHARA_COLORMODE_16);
    work = (DWCiFontWork *)data_ov016_021de680;
    cv->text.pCanvas = &cv->chr;
    cv->text.pFont = &work->font[font];
    cv->text.hSpace = 1;
    cv->text.vSpace = 1;
    if (lcd == 1) {
        scr = G2S_GetBG0ScrPtr();
    } else {
        scr = G2_GetBG0ScrPtr();
    }
    NNS_G2dMapScrToCharText(scr, width, height, 0, 0, NNS_G2D_TEXT_BG_WIDTH_256, data_ov016_021daa70[lcd], 0xf);
    DWCi_FNTlClear(&cv->chr, 0);
    cv->task = DWCi_TSKlForm(1, (s32)taskLoad_font, &((DWCiFontWork *)data_ov016_021de680)->load[lcd], 200);
    return &cv->chr;
}


void taskLoad_font(u32 param_1, u8 *param_2) {
    if (*param_2 == 0) {
        return;
    }
    if (param_2 == (u8 *)(data_ov016_021de680 + 0x67c)) {
        DC_FlushRange(*(void **)(data_ov016_021de680 + 0x638), 0x6000);
        GX_LoadBG0Char(*(void **)(data_ov016_021de680 + 0x638), 0, 0x6000);
    }
    else {
        DC_FlushRange(*(void **)(data_ov016_021de680 + 0x668), 0x3000);
        GXS_LoadBG0Char(*(void **)(data_ov016_021de680 + 0x668), 0x3000, 0x3000);
    }
    *param_2 = 0;
}

void DWCi_FNTlDeleteBg(s32 canvas) {
    void *dest;
    DWCi_TSKlDeleteEx(1, *(s32 **)(canvas + 0x2c));
    if (canvas == data_ov016_021de680 + 0x610) {
        dest = G2_GetBG0CharPtr();
        MIi_CpuClear16(0, dest, 0x6000);
    } else {
        dest = G2_GetBG0CharPtr();
        MIi_CpuClear16(0, dest, 0x3000);
    }
    DWCi_HEAPlFree_((u32 *)(canvas + 0x28));
}

void DWCi_FNTlDeleteBgLcd(s32 lcd) {
    DWCi_FNTlDeleteBg((0x30 * lcd) + (data_ov016_021de680 + 0x610));
}

void DWCi_FNTlDraw(s32 param_1, s32 param_2, s32 param_3, s32 param_4, u32 param_5, void *param_6) {
    NNS_G2dTextCanvasDrawText((NNSG2dTextCanvas *)(param_1 + 0x18), param_2, param_3, param_4, param_5, param_6);
}

void DWCi_FNTlDrawCharEx(NNSG2dCharCanvas *canvas, s32 x, s32 y, s32 color, u16 chr, s32 font) {
    NNS_G2dCharCanvasDrawChar
                        (canvas, (NNSG2dFont *)(data_ov016_021de680 + font * 8), x, y, color,
                          chr);
}

void DWCi_FNTlDrawStringEx(NNSG2dCharCanvas *canvas, s32 x, s32 y, s32 color, s32 interval, u16 *txt, s32 font) {
    u16 alterCharIndex;
    const NNSG2dCharWidths *pNVar2;
    NNSG2dFont *pFont;

    alterCharIndex = *txt;
    if (alterCharIndex != 0) {
        do {
            pFont = (NNSG2dFont *)(data_ov016_021de680 + font * 8);
            alterCharIndex = NNS_G2dFontFindGlyphIndex(pFont, alterCharIndex);
            if (alterCharIndex == 0xffff) {
                alterCharIndex = pFont->pRes->alterCharIndex;
            }
            pNVar2 = NNS_G2dFontGetCharWidthsFromIndex(pFont, alterCharIndex);
            DWCi_FNTlDrawCharEx(canvas, x + (interval - pNVar2->charWidth >> 1), y, color,
                                                    *txt, font);
            txt = txt + 1;
            alterCharIndex = *txt;
            x = x + interval;
        } while (alterCharIndex != 0);
        return;
    }
}

void DWCi_FNTlDrawRect(NNSG2dCharCanvas *canvas, s32 x, s32 y, s32 w, s32 h, s32 color, u32 flag, void *txt) {
    Ov016Text *text = (Ov016Text *)canvas;
    NNSG2dTextDirection dir;
    s8 *p;

    p = (s8 *)&dir;
    p[0] = 0;
    p[1] = 0;
    switch (text->node->unk_0->mode->mode) {
    case 0:
    case 7:
        dir.x = 1;
        break;
    case 1:
    case 2:
        dir.y = 1;
        break;
    case 3:
    case 4:
        dir.x = -1;
        break;
    case 5:
    case 6:
        dir.y = -1;
        break;
    }
    NNSi_G2dTextCanvasDrawTextRect((NNSG2dTextCanvas *)&text->unk_0[0x18], x, y, w, h, color, flag, txt, dir);
}

void DWCi_FNTlClear(NNSG2dCharCanvas *canvas, u32 color) {
    canvas->vtable->pClear(canvas, (s32) color);
}

void func_ov016_021d5f28(void *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5) {
    GXOamAttr *oam;

    oam = (GXOamAttr *)DWCi_CELLlGetObj((void *)param_4, 0);
    DWCi_CELLlSetEffect((void *)param_4, -1, 0, 0);
    DWCi_CELLlSetPalette((void *)param_4, -1, 0, 0xf);
    DWCi_CELLlSetPriority((void *)param_4, -1, param_5);
    NNS_G2dArrangeOBJ1D(oam, (u32)*(u8 *)((u8 *)param_1 + 0x2e), (u32)*(u8 *)((u8 *)param_1 + 0x2f), param_2,
                                            param_3, 0, (u32)*(u16 *)((u8 *)param_1 + 0x2c), 2);
}

void DWCi_FNTlRenewBg(s32 canvas) {
    if (canvas == data_ov016_021de680 + 0x610) {
        *(u8 *)(data_ov016_021de680 + 0x67c) = 1;
    }
    else {
        *(u8 *)(data_ov016_021de680 + 0x67d) = 1;
    }
}
