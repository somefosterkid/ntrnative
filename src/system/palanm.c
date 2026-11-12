// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/palanm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/memorymap.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "system/palanm.h"

// Palette fades, and blends and filters of colors

// A color's channels
// Black 2's; twinned code confirms the offsets of r, g and b in Black
typedef struct {
    u16 r : 5;
    u16 g : 5;
    u16 b : 5;
    u16 unused : 1;
} PaletteColor;

// How a buffer fades: the palettes it fades, and its steps from cur to end of 16 toward color
typedef struct {
    u16 paletteMask;
    u16 delay : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 color : 15;
    u16 direction : 1;
    u16 step : 5;
    u16 delayCounter : 6;
} FadeControl;

typedef struct {
    u16 *unfaded;
    u16 *faded;
    u32 size;
    FadeControl fade;
} PaletteBuffer;

// A channel blended fraction of 16 of the way from one value to another
#define BLEND_CHANNEL(from, to, fraction) ((from) + ((((to) - (from)) * (fraction)) >> 4))

// The fade task's states
#define PALFADE_STATE_IDLE 0
#define PALFADE_STATE_ACTIVE 1

struct PaletteFade {
    PaletteBuffer buffers[PALFADE_BUFFER_COUNT];
    u16 state : 2;
    // The buffers still fading
    u16 activeMask : 14;
    // The buffers func_02021d50 copies
    u16 transferMask : 14;
    u16 taskActive : 1;
    u16 transferAll : 1;
    u8 stopRequested;
};

static void func_02021548(PaletteFade *fade, u32 buffer, u16 *unfaded, u16 *faded, u32 size);
static u8 func_02021930(u16 mask, u16 bit);
static void func_02021944(PaletteFade *fade, u8 buffer);
static void func_02021984(int buffer, PaletteBuffer *paletteBuffer, u16 *paletteMask);
static void func_020219c4(FadeControl *control, u16 paletteMask, s8 delay, u8 start, u8 end, u16 color);
static void func_02021a84(TCB *tcb, void *data);
static void func_02021b2c(PaletteFade *fade);
static void func_02021b4c(PaletteFade *fade);
static void func_02021b6c(PaletteFade *fade, u8 buffer, u16 paletteSize);
static void func_02021bc8(PaletteFade *fade, u16 buffer, u16 paletteSize);
static void func_02021c20(const u16 *src, u16 *dst, FadeControl *control, u32 count);
static void func_02021ca8(PaletteFade *fade, u8 buffer, FadeControl *control);

// The weights ColorFilter_ApplyVintage gives each channel of a gray, out of 256
static u32 g_FieldColorPostFXWeightR = 155;
// pokebw2's g_FieldColorPostFXWeightG
static u32 data_020a73bc = 135;
static u32 g_FieldColorPostFXWeightB = 85;

// pokebw2's PaletteFade_Create
PaletteFade *func_02021518(u32 heapId) {
    PaletteFade *fade = GFL_HeapAllocate(heapId, sizeof(PaletteFade), FALSE, "palanm.c", 88);

    MI_CpuFill8(fade, 0, sizeof(PaletteFade));
    return fade;
}

// pokebw2's PaletteFade_Free
void func_02021540(PaletteFade *fade) {
    GFL_HeapFree(fade);
}

// pokebw2's PaletteFade_SetBuffer
static void func_02021548(PaletteFade *fade, u32 buffer, u16 *unfaded, u16 *faded, u32 size) {
    fade->buffers[buffer].unfaded = unfaded;
    fade->buffers[buffer].faded = faded;
    fade->buffers[buffer].size = size;
}

// pokebw2's PaletteFade_AllocBuffer
void func_0202155c(PaletteFade *fade, u32 buffer, u32 size, HeapID heapId) {
    u16 *unfaded = GFL_HeapAllocate(heapId, size, FALSE, "palanm.c", 142);
    u16 *faded = GFL_HeapAllocate(heapId, size, FALSE, "palanm.c", 143);

    func_02021548(fade, buffer, unfaded, faded, size);
}

// pokebw2's PaletteFade_FreeBuffer
void func_020215a0(PaletteFade *fade, u32 buffer) {
    GFL_HeapFree(fade->buffers[buffer].unfaded);
    GFL_HeapFree(fade->buffers[buffer].faded);
}

// pokebw2's PaletteFade_LoadData
void func_020215bc(PaletteFade *fade, const void *src, u32 buffer, u16 offset, u16 size) {
    MIi_CpuCopy16(src, fade->buffers[buffer].unfaded + offset, size);
    MIi_CpuCopy16(src, fade->buffers[buffer].faded + offset, size);
}

void func_020215ec(PaletteFade *fade, u32 arcId, u32 fileId, HeapID heapId, u32 buffer, u32 size, u16 offset,
                            u16 srcOffset) {
    NNSG2dPaletteData *palette;
    void *file = GFL_G2DIOReadNCLR(arcId, fileId, &palette, HEAPID_TAIL(heapId));

    if (size == 0) {
        size = palette->szByte;
    }
    func_020215bc(fade, (u16 *)palette->pRawData + srcOffset, buffer, offset, size);
    GFL_HeapFree(file);
}

// pokebw2's PaletteFade_LoadNCLR
void func_02021640(PaletteFade *fade, u32 arcId, u32 fileId, HeapID heapId, u32 buffer, u32 size, u16 offset) {
    func_020215ec(fade, arcId, fileId, heapId, buffer, size, offset, 0);
}

void func_02021660(PaletteFade *fade, ArcTool *arc, u32 fileId, HeapID heapId, u32 buffer, u32 size,
                               u16 offset, u16 srcOffset) {
    NNSG2dPaletteData *palette;
    void *file = GFL_G2DIOReadNCLRArc(arc, fileId, &palette, HEAPID_TAIL(heapId));

    if (size == 0) {
        size = palette->szByte;
    }
    func_020215bc(fade, (u16 *)palette->pRawData + srcOffset, buffer, offset, size);
    GFL_HeapFree(file);
}

void func_020216b4(PaletteFade *fade, ArcTool *arc, u32 fileId, HeapID heapId, u32 buffer, u32 size,
                             u16 offset) {
    func_02021660(fade, arc, fileId, heapId, buffer, size, offset, 0);
}

// pokebw2's PaletteFade_LoadFromVRAM
void func_020216d4(PaletteFade *fade, u32 vram, u16 offset, u32 size) {
    u16 *src;

    switch (vram) {
    case PALFADE_VRAM_MAIN_BG:
        src = (u16 *)HW_BG_PLTT;
        break;
    case PALFADE_VRAM_SUB_BG:
        src = (u16 *)HW_DB_BG_PLTT;
        break;
    case PALFADE_VRAM_MAIN_OBJ:
        src = (u16 *)HW_OBJ_PLTT;
        break;
    case PALFADE_VRAM_SUB_OBJ:
        src = (u16 *)HW_DB_OBJ_PLTT;
        break;
    default:
        return;
    }
    func_020215bc(fade, src + offset, vram, offset, size);
}

// pokebw2's PaletteFade_GetUnfadedBuffer
u16 *func_02021728(PaletteFade *fade, u16 buffer) {
    return fade->buffers[buffer].unfaded;
}

// pokebw2's PaletteFade_GetFadedBuffer
u16 *func_02021730(PaletteFade *fade, u32 buffer) {
    return fade->buffers[buffer].faded;
}

u8 func_0202173c(PaletteFade *fade, u16 bufferMask, u16 paletteMask, s8 delay, u8 start, u8 end, u16 color,
                         TCBManager *tcbManager) {
    u16 allPalettes = paletteMask;
    u8 started = FALSE;
    u8 i;

    for (i = 0; i < PALFADE_BUFFER_COUNT; i++) {
        if (func_02021930(bufferMask, i) == TRUE && func_02021930(fade->activeMask, i) == FALSE) {
            func_02021984(i, &fade->buffers[i], &paletteMask);
            func_020219c4(&fade->buffers[i].fade, paletteMask, delay, start, end, color);
            func_02021944(fade, i);
            if (i >= PALFADE_BUFFERS_STD) {
                func_02021bc8(fade, i, 256);
            } else {
                func_02021bc8(fade, i, 16);
            }
            paletteMask = allPalettes;
            started = TRUE;
        }
    }
    if (started == TRUE) {
        fade->activeMask |= bufferMask;
        if (fade->taskActive == FALSE) {
            fade->taskActive = TRUE;
            fade->state = PALFADE_STATE_ACTIVE;
            fade->stopRequested = FALSE;
            GFL_TCBMgrAddTask(tcbManager, func_02021a84, fade, -2);
        }
    }
    return started;
}

u8 func_02021848(PaletteFade *fade, u16 bufferMask, u16 paletteMask, s8 delay, u8 start, u8 end, u16 color,
                           TCBManager *tcbManager) {
    u16 allPalettes = paletteMask;
    u8 started = FALSE;
    u8 i;

    for (i = 0; i < PALFADE_BUFFER_COUNT; i++) {
        if (func_02021930(bufferMask, i) == TRUE) {
            func_02021984(i, &fade->buffers[i], &paletteMask);
            func_020219c4(&fade->buffers[i].fade, paletteMask, delay, start, end, color);
            func_02021944(fade, i);
            if (i >= PALFADE_BUFFERS_STD) {
                func_02021bc8(fade, i, 256);
            } else {
                func_02021bc8(fade, i, 16);
            }
            paletteMask = allPalettes;
            started = TRUE;
        }
    }
    if (started == TRUE) {
        fade->activeMask = bufferMask;
        if (fade->taskActive == FALSE) {
            fade->taskActive = TRUE;
            fade->state = PALFADE_STATE_ACTIVE;
            fade->stopRequested = FALSE;
            GFL_TCBMgrAddTask(tcbManager, func_02021a84, fade, -2);
        }
    }
    return started;
}

// pokebw2's IsBitSet
static u8 func_02021930(u16 mask, u16 bit) {
    BOOL result = TRUE;

    if ((mask & (1 << bit)) == 0) {
        result = FALSE;
    }
    return result;
}

// pokebw2's PaletteFade_SetTransfer
static void func_02021944(PaletteFade *fade, u8 buffer) {
    if (func_02021930(fade->transferMask, buffer) != TRUE) {
        fade->transferMask |= (u16)(1 << buffer);
    }
}

// Keeps the palettes of paletteMask that the buffer has
// pokebw2's MaskPalettes
static void func_02021984(int buffer, PaletteBuffer *paletteBuffer, u16 *paletteMask) {
    u8 count;
    u16 mask;
    u8 i;

    if (buffer < PALFADE_BUFFERS_STD) {
        count = paletteBuffer->size / 32;
    } else {
        count = paletteBuffer->size / 512;
    }
    mask = 0;
    for (i = 0; i < count; i++) {
        mask += (u16)(1 << i);
    }
    *paletteMask &= mask;
}

// pokebw2's FadeControl_Init
static void func_020219c4(FadeControl *control, u16 paletteMask, s8 delay, u8 start, u8 end, u16 color) {
    if (delay < 0) {
        u8 step = MATH_ABS(delay) + 2;

        if (step > 16) {
            step = 16;
        }
        control->step = step;
        control->delay = 0;
    } else {
        control->step = 2;
        control->delay = delay;
    }
    control->paletteMask = paletteMask;
    control->cur = start;
    control->end = end;
    control->color = color;
    control->delayCounter = control->delay;
    if (start < end) {
        control->direction = 0;
    } else {
        control->direction = 1;
    }
}

// pokebw2's PaletteFade_Task
static void func_02021a84(TCB *tcb, void *data) {
    PaletteFade *fade = data;

    if (fade->stopRequested == TRUE) {
        fade->stopRequested = FALSE;
        fade->transferMask = 0;
        fade->activeMask = 0;
        fade->taskActive = FALSE;
        GFL_TCBRemove(tcb);
        return;
    }
    if (fade->state == PALFADE_STATE_ACTIVE) {
        fade->transferMask = fade->activeMask;
        func_02021b2c(fade);
        func_02021b4c(fade);
        if (fade->activeMask == 0) {
            fade->taskActive = FALSE;
            GFL_TCBRemove(tcb);
        }
    }
}

// pokebw2's PaletteFade_RequestStop
void func_02021b18(PaletteFade *fade) {
    if (fade->activeMask != 0) {
        fade->stopRequested = TRUE;
    }
}

// pokebw2's PaletteFade_StepStd
static void func_02021b2c(PaletteFade *fade) {
    u8 i;

    for (i = 0; i < PALFADE_BUFFERS_STD; i++) {
        func_02021b6c(fade, i, 16);
    }
}

// pokebw2's PaletteFade_StepExt
static void func_02021b4c(PaletteFade *fade) {
    u8 i;

    for (i = PALFADE_BUFFERS_STD; i < PALFADE_BUFFER_COUNT; i++) {
        func_02021b6c(fade, i, 256);
    }
}

// pokebw2's PaletteFade_StepBuffer
static void func_02021b6c(PaletteFade *fade, u8 buffer, u16 paletteSize) {
    if (func_02021930(fade->activeMask, buffer)) {
        PaletteBuffer *paletteBuffer = &fade->buffers[buffer];

        if (paletteBuffer->fade.delayCounter < paletteBuffer->fade.delay) {
            paletteBuffer->fade.delayCounter++;
        } else {
            paletteBuffer->fade.delayCounter = 0;
            func_02021bc8(fade, buffer, paletteSize);
        }
    }
}

// pokebw2's PaletteFade_ApplyBuffer
static void func_02021bc8(PaletteFade *fade, u16 buffer, u16 paletteSize) {
    u32 i;
    PaletteBuffer *paletteBuffer = &fade->buffers[buffer];

    for (i = 0; i < 16; i++) {
        if (func_02021930(paletteBuffer->fade.paletteMask, i)) {
            func_02021c20(paletteBuffer->unfaded + i * paletteSize, paletteBuffer->faded + i * paletteSize,
                            &paletteBuffer->fade, paletteSize);
        }
    }
    func_02021ca8(fade, buffer, &paletteBuffer->fade);
}

// pokebw2's BlendFadeColors
static void func_02021c20(const u16 *src, u16 *dst, FadeControl *control, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        u8 r = BLEND_CHANNEL(src[i] & 0x1f, control->color & 0x1f, control->cur);
        u8 b = BLEND_CHANNEL((src[i] >> 10) & 0x1f, (control->color >> 10) & 0x1f, control->cur);
        u8 g = BLEND_CHANNEL((src[i] >> 5) & 0x1f, (control->color >> 5) & 0x1f, control->cur);

        dst[i] = (b << 10) | (g << 5) | r;
    }
}

// pokebw2's FadeControl_Advance
static void func_02021ca8(PaletteFade *fade, u8 buffer, FadeControl *control) {
    s16 cur;

    if (control->cur == control->end) {
        if (fade->activeMask & (1 << buffer)) {
            fade->activeMask ^= (u16)(1 << buffer);
        }
    } else if (control->direction == 0) {
        cur = control->cur;
        cur += control->step;
        if (cur > control->end) {
            cur = control->end;
        }
        control->cur = cur;
    } else {
        cur = control->cur;
        cur -= control->step;
        if (cur < control->end) {
            cur = control->end;
        }
        control->cur = cur;
    }
}

// pokebw2's PaletteFade_Transfer
void func_02021d50(PaletteFade *fade) {
    int i;

    if (fade->transferAll == FALSE && fade->state != PALFADE_STATE_ACTIVE) {
        return;
    }
    for (i = 0; i < PALFADE_BUFFER_COUNT; i++) {
        if (fade->transferAll == FALSE &&
            (fade->buffers[i].faded == NULL || func_02021930(fade->transferMask, i) == FALSE)) {
            continue;
        }
        DC_FlushRange(fade->buffers[i].faded, fade->buffers[i].size);
        switch (i) {
        case PALFADE_BUFFER_MAIN_BG:
            GX_LoadBGPltt(fade->buffers[i].faded, 0, fade->buffers[i].size);
            break;
        case PALFADE_BUFFER_SUB_BG:
            GXS_LoadBGPltt(fade->buffers[i].faded, 0, fade->buffers[i].size);
            break;
        case PALFADE_BUFFER_MAIN_OBJ:
            GX_LoadOBJPltt(fade->buffers[i].faded, 0, fade->buffers[i].size);
            break;
        case PALFADE_BUFFER_SUB_OBJ:
            GXS_LoadOBJPltt(fade->buffers[i].faded, 0, fade->buffers[i].size);
            break;
        case PALFADE_BUFFER_MAIN_BG_EX0:
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(fade->buffers[i].faded, 0, fade->buffers[i].size);
            GX_EndLoadBGExtPltt();
            break;
        case PALFADE_BUFFER_MAIN_BG_EX1:
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(fade->buffers[i].faded, 0x2000, fade->buffers[i].size);
            GX_EndLoadBGExtPltt();
            break;
        case PALFADE_BUFFER_MAIN_BG_EX2:
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(fade->buffers[i].faded, 0x4000, fade->buffers[i].size);
            GX_EndLoadBGExtPltt();
            break;
        case PALFADE_BUFFER_MAIN_BG_EX3:
            GX_BeginLoadBGExtPltt();
            GX_LoadBGExtPltt(fade->buffers[i].faded, 0x6000, fade->buffers[i].size);
            GX_EndLoadBGExtPltt();
            break;
        case PALFADE_BUFFER_SUB_BG_EX0:
            GXS_BeginLoadBGExtPltt();
            GXS_LoadBGExtPltt(fade->buffers[i].faded, 0, fade->buffers[i].size);
            GXS_EndLoadBGExtPltt();
            break;
        case PALFADE_BUFFER_SUB_BG_EX1:
            GXS_BeginLoadBGExtPltt();
            GXS_LoadBGExtPltt(fade->buffers[i].faded, 0x2000, fade->buffers[i].size);
            GXS_EndLoadBGExtPltt();
            break;
        case PALFADE_BUFFER_SUB_BG_EX2:
            GXS_BeginLoadBGExtPltt();
            GXS_LoadBGExtPltt(fade->buffers[i].faded, 0x4000, fade->buffers[i].size);
            GXS_EndLoadBGExtPltt();
            break;
        case PALFADE_BUFFER_SUB_BG_EX3:
            GXS_BeginLoadBGExtPltt();
            GXS_LoadBGExtPltt(fade->buffers[i].faded, 0x6000, fade->buffers[i].size);
            GXS_EndLoadBGExtPltt();
            break;
        case PALFADE_BUFFER_MAIN_OBJ_EX:
            GX_BeginLoadOBJExtPltt();
            GX_LoadOBJExtPltt(fade->buffers[i].faded, 0, fade->buffers[i].size);
            GX_EndLoadOBJExtPltt();
            break;
        case PALFADE_BUFFER_SUB_OBJ_EX:
            GXS_BeginLoadOBJExtPltt();
            GXS_LoadOBJExtPltt(fade->buffers[i].faded, 0, fade->buffers[i].size);
            GXS_EndLoadOBJExtPltt();
            break;
        }
    }
    fade->transferMask = fade->activeMask;
    if (fade->transferMask == 0) {
        fade->state = PALFADE_STATE_IDLE;
    }
}

// pokebw2's PaletteFade_GetActiveMask
u16 func_02021ed8(PaletteFade *fade) {
    return fade->activeMask;
}

// pokebw2's PaletteFade_SetTransferAll
void func_02021ee4(PaletteFade *fade, u32 transferAll) {
    fade->transferAll = transferAll;
}

// pokebw2's PaletteFade_SetAllActive
void func_02021f08(PaletteFade *fade, u32 active) {
    fade->state = active & 1;
    fade->activeMask = 0x3fff;
}

// pokebw2's PaletteFade_GetColor
u16 func_02021f3c(PaletteFade *fade, u32 buffer, u32 which, u16 pos) {
    if (which == PALFADE_UNFADED) {
        return fade->buffers[buffer].unfaded[pos];
    }
    if (which == PALFADE_FADED) {
        return fade->buffers[buffer].faded[pos];
    }
    return 0;
}

// pokebw2's BlendColors
void func_02021f64(const u16 *src, u16 *dst, u16 count, u8 fraction, u16 color) {
    // Read through the RGB555 bitfields: shifts and masks don't give the original's code
    const PaletteColor *target = (const PaletteColor *)&color;
    int r = target->r;
    int g = target->g;
    int b = target->b;
    u16 i;

    for (i = 0; i < count; i++) {
        const PaletteColor *rgb = (const PaletteColor *)&src[i];
        int cr = rgb->r;
        int cg = rgb->g;
        int cb = rgb->b;

        dst[i] = GX_RGB(BLEND_CHANNEL(cr, r, fraction), BLEND_CHANNEL(cg, g, fraction), BLEND_CHANNEL(cb, b, fraction));
    }
}

// pokebw2's PaletteFade_BlendBuffer
void func_02021fd8(PaletteFade *fade, u32 buffer, u16 offset, u16 count, u8 fraction, u16 color) {
    func_02021f64(fade->buffers[buffer].unfaded + offset, fade->buffers[buffer].faded + offset, count, fraction, color);
}

// pokebw2's PaletteFade_BlendPalettes
void func_02022004(PaletteFade *fade, u32 buffer, u16 paletteMask, u8 fraction, u16 color) {
    u32 offset = 0;

    while (paletteMask != 0) {
        if (paletteMask & 1) {
            func_02021fd8(fade, buffer, offset, 16, fraction, color);
        }
        paletteMask >>= 1;
        offset += 16;
    }
}

void ColorFilter_ApplyLUT(u16 *colors, int count, const u8 *lut) {
    int i;

    for (i = 0; i < count; i++) {
        int red = *colors & 0x1f;
        int green = (*colors >> 5) & 0x1f;
        int blue = (*colors >> 10) & 0x1f;
        u8 gray = lut[(red * 76 + green * 151 + blue * 29) >> 8];

        *colors = (gray << 10) | (gray << 5) | gray;
        colors++;
    }
}

void ColorFilter_ApplyVintage(u16 *colors, int count);

