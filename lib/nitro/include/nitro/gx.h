// NitroSDK's graphics registers and inline functions. From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// lib/nitro/include/nitro/gx.h, trimmed to what Black's C uses so far.
#ifndef BWPORT_NITRO_GX_H
#define BWPORT_NITRO_GX_H

#include "types.h"
#include "nitro/mi.h"
#include "nitro/misc.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/misc.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"

typedef u16 GXRgb;

#define GX_RGB(r, g, b) ((GXRgb)((r) | ((g) << 5) | ((b) << 10)))

// OAM attributes, cut short: pokebw2's has the bit fields too
typedef union {
    struct {
        u32 attr01;
        u32 attr23;
    };
    struct {
        u16 attr0;
        u16 attr1;
        u16 attr2;
        u16 affineParam;
    };
    struct {
        u32 y : 8;
        u32 rsMode : 2;
        u32 mode : 2;
        u32 mosaic : 1;
        u32 colorMode : 1;
        u32 shape : 2;
        u32 x : 9;
        u32 rsParam : 5;
        u32 size : 2;
        u32 charName : 10;
        u32 priority : 2;
        u32 cParam : 4;
        u32 : 16;
    };
    // The flips, the top bits of an rsParam that isn't affine
    struct {
        u32 : 28;
        u32 flipH : 1;
        u32 flipV : 1;
        u32 : 2;
        u32 : 32;
    };
} GXOamAttr;

// The affine parameters of an OAM, stored in the OAM buffer's four halfwords of attributes 0 to 3, one per attribute
typedef struct {
    u16 _0;
    u16 _1;
    u16 _2;
    s16 PA;
    u16 _4;
    u16 _5;
    u16 _6;
    s16 PB;
    u16 _8;
    u16 _9;
    u16 _10;
    s16 PC;
    u16 _12;
    u16 _13;
    u16 _14;
    s16 PD;
} GXOamAffine;


#define GX_OAM_ATTR01_Y_SHIFT 0
#define GX_OAM_ATTR01_MODE_SHIFT 10
#define GX_OAM_ATTR01_MOSAIC_SHIFT 12
#define GX_OAM_ATTR01_CM_SHIFT 13
#define GX_OAM_ATTR01_X_SHIFT 16
#define GX_OAM_ATTR01_RS_SHIFT 25
#define GX_OAM_ATTR2_NAME_SHIFT 0
#define GX_OAM_ATTR2_PRIORITY_SHIFT 10
#define GX_OAM_ATTR2_PRIORITY_MASK 0x0c00
#define GX_OAM_ATTR2_CPARAM_SHIFT 12

#define GX_OAM_SHAPE_64x64 0xc0000000

static inline void G2_SetOBJAttr(GXOamAttr *oam, int x, int y, int priority, int mode, BOOL mosaic, int effect,
                                 u32 shape, int color, int charName, int cParam, int rsParam) {
    oam->attr01 = (u32)(shape | ((y & 0xff) << GX_OAM_ATTR01_Y_SHIFT) | (mode << GX_OAM_ATTR01_MODE_SHIFT) |
                        (mosaic << GX_OAM_ATTR01_MOSAIC_SHIFT) | effect | (color << GX_OAM_ATTR01_CM_SHIFT) |
                        ((x & 0x1ff) << GX_OAM_ATTR01_X_SHIFT) | (rsParam << GX_OAM_ATTR01_RS_SHIFT));
    oam->attr2 = (u16)((charName << GX_OAM_ATTR2_NAME_SHIFT) | (priority << GX_OAM_ATTR2_PRIORITY_SHIFT) |
                       (cParam << GX_OAM_ATTR2_CPARAM_SHIFT));
}

#define reg_GX_DISPCNT (*(vu32 *)0x04000000)
#define reg_GX_DISPSTAT (*(vu16 *)0x04000004)
#define reg_G2_WIN0H (*(vu16 *)0x04000040)
#define reg_G2_WIN1H (*(vu16 *)0x04000042)
#define reg_G2_WIN0V (*(vu16 *)0x04000044)
#define reg_G2_WIN1V (*(vu16 *)0x04000046)
#define reg_G2_WININ (*(vu16 *)0x04000048)
#define reg_G2_WINOUT (*(vu16 *)0x0400004a)
#define reg_G2_BLDCNT (*(vu16 *)0x04000050)
#define reg_GX_DISPCAPCNT (*(vu32 *)0x04000064)
#define reg_G3_MTX_MODE (*(vu32 *)0x04000440)
#define reg_G3_MTX_PUSH (*(vu32 *)0x04000444)
#define reg_G3_MTX_POP (*(vu32 *)0x04000448)
#define reg_G3_MTX_IDENTITY (*(vu32 *)0x04000454)
#define reg_G3_MTX_SCALE (*(vu32 *)0x0400046c)
#define reg_G3_MTX_TRANS (*(vu32 *)0x04000470)
#define reg_G3_COLOR (*(vu32 *)0x04000480)
#define reg_G3_NORMAL (*(vu32 *)0x04000484)
#define reg_G3_TEXCOORD (*(vu32 *)0x04000488)
#define reg_G3_VTX_16 (*(vu32 *)0x0400048c)
#define reg_G3_POLYGON_ATTR (*(vu32 *)0x040004a4)
#define reg_G3_TEXIMAGE_PARAM (*(vu32 *)0x040004a8)
#define reg_G3_TEXPLTT_BASE (*(vu32 *)0x040004ac)
#define reg_G3_DIF_AMB (*(vu32 *)0x040004c0)
#define reg_G3_SPE_EMI (*(vu32 *)0x040004c4)
#define reg_G3_LIGHT_VECTOR (*(vu32 *)0x040004c8)
#define reg_G3_LIGHT_COLOR (*(vu32 *)0x040004cc)
#define reg_G3_BEGIN_VTXS (*(vu32 *)0x04000500)
#define reg_G3_END_VTXS (*(vu32 *)0x04000504)
#define reg_GXS_DB_DISPCNT (*(vu32 *)0x04001000)
#define reg_G2S_DB_WIN0H (*(vu16 *)0x04001040)
#define reg_G2S_DB_WIN1H (*(vu16 *)0x04001042)
#define reg_G2S_DB_WIN0V (*(vu16 *)0x04001044)
#define reg_G2S_DB_WIN1V (*(vu16 *)0x04001046)
#define reg_G2S_DB_WININ (*(vu16 *)0x04001048)
#define reg_G2S_DB_WINOUT (*(vu16 *)0x0400104a)
// The vertices in the 3D engine's vertex RAM, of the last frame
#define reg_G3X_VTXRAM_COUNT (*(vu16 *)0x04000606)

#define REG_GX_DISPSTAT_VBLK_MASK 0x0001

#define REG_GX_DISPCNT_W0_SHIFT 13
#define REG_GX_DISPCNT_W0_MASK 0x00002000
#define REG_GX_DISPCNT_W1_MASK 0x00004000
#define REG_GX_DISPCNT_OW_MASK 0x00008000

#define GX_WNDMASK_NONE 0x00
#define GX_WNDMASK_W1 0x02

#define GX_PLANEMASK_BG0 0x01
#define GX_PLANEMASK_BG1 0x02
#define GX_PLANEMASK_BG2 0x04
#define GX_PLANEMASK_BG3 0x08
#define GX_PLANEMASK_OBJ 0x10
#define GX_PLANEMASK_ALL 0x1f

// The planes that blending takes, which include the backdrop
#define GX_BLEND_PLANEMASK_BG0 0x01
#define GX_BLEND_PLANEMASK_BG2 0x04
#define GX_BLEND_PLANEMASK_BG3 0x08

static inline void GX_SetVisibleWnd(int window) {
    reg_GX_DISPCNT = (reg_GX_DISPCNT & ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK)) |
                     (window << REG_GX_DISPCNT_W0_SHIFT);
}

static inline int GX_GetVisibleWnd(void) {
    return (reg_GX_DISPCNT & (REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK)) >>
           REG_GX_DISPCNT_W0_SHIFT;
}

static inline void GXS_SetVisibleWnd(int window) {
    reg_GXS_DB_DISPCNT =
        (reg_GXS_DB_DISPCNT & ~(REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK)) |
        (window << REG_GX_DISPCNT_W0_SHIFT);
}

// NitroSDK's GXDispCnt and GXSDispCnt, the main and the sub screen's display control register by field, and its
// readers GX_GetDispCnt, GXS_GetDispCnt and GXS_GetVisibleWnd
typedef union {
    u32 raw;
    struct {
        u32 bgMode : 3;
        u32 bg0_2d3d : 1;
        u32 objMapChar : 1;
        u32 objMapBmp : 2;
        u32 blankScr : 1;
        u32 visiblePlane : 5;
        u32 visibleWnd : 3;
        u32 dispMode : 4;
        u32 extObjMapChar : 2;
        u32 extObjMapBmp : 1;
        u32 hBlankObjProc : 1;
        u32 bgCharOffset : 3;
        u32 bgScrOffset : 3;
        u32 bgExtPltt : 1;
        u32 objExtPltt : 1;
    };
} GXDispCnt;
typedef union {
    u32 raw;
    struct {
        u32 bgMode : 3;
        u32 _reserve1 : 1;
        u32 objMapChar : 1;
        u32 objMapBmp : 2;
        u32 blankScr : 1;
        u32 visiblePlane : 5;
        u32 visibleWnd : 3;
        u32 dispMode : 1;
        u32 _reserve2 : 3;
        u32 extObjMapChar : 2;
        u32 _reserve3 : 1;
        u32 hBlankObjProc : 1;
        u32 _reserve4 : 6;
        u32 bgExtPltt : 1;
        u32 objExtPltt : 1;
    };
} GXSDispCnt;

static inline GXDispCnt GX_GetDispCnt(void) {
    return *(volatile GXDispCnt *)&reg_GX_DISPCNT;
}

static inline GXSDispCnt GXS_GetDispCnt(void) {
    return *(volatile GXSDispCnt *)&reg_GXS_DB_DISPCNT;
}

static inline int GXS_GetVisibleWnd(void) {
    return (reg_GXS_DB_DISPCNT & (REG_GX_DISPCNT_W0_MASK | REG_GX_DISPCNT_W1_MASK | REG_GX_DISPCNT_OW_MASK)) >>
           REG_GX_DISPCNT_W0_SHIFT;
}

// The planes inside a window or outside all of them, and whether color effects apply there. NitroSDK's
// G2_SetWnd0InsidePlane, G2_SetWnd1InsidePlane and G2_SetWndOutsidePlane, and their G2S_ forms for the sub screen
#define REG_G2_WININ_WIN0IN_MASK 0x003f
#define REG_G2_WININ_WIN1IN_SHIFT 8
#define REG_G2_WININ_WIN1IN_MASK 0x3f00
#define REG_G2_WINOUT_WINOUT_MASK 0x003f
#define G2_WND_EFFECT 0x20

// The planes inside a window or outside all of them, and whether color effects apply there, as one byte of the window
// registers: WININ holds window 0's then window 1's, WINOUT the outside's then the OBJ window's
typedef struct {
    u8 planeMask : 5;
    u8 effect : 1;
    u8 : 2;
} GXWndPlane;

static inline GXWndPlane G2_GetWnd0InsidePlane(void) {
    // Volatile: the window register, which the game sets at any time
    return *(volatile GXWndPlane *)&reg_G2_WININ;
}

static inline GXWndPlane G2_GetWnd1InsidePlane(void) {
    // Volatile: the window register, which the game sets at any time
    return *((volatile GXWndPlane *)&reg_G2_WININ + 1);
}

static inline GXWndPlane G2_GetWndOutsidePlane(void) {
    return *(volatile GXWndPlane *)&reg_G2_WINOUT;
}

// The OBJ window's planes: WINOUT's second byte
static inline GXWndPlane G2_GetWndOBJInsidePlane(void) {
    return *((volatile GXWndPlane *)&reg_G2_WINOUT + 1);
}

static inline GXWndPlane G2S_GetWnd0InsidePlane(void) {
    // Volatile: the window register, which the game sets at any time
    return *(volatile GXWndPlane *)&reg_G2S_DB_WININ;
}

static inline GXWndPlane G2S_GetWnd1InsidePlane(void) {
    // Volatile: the window register, which the game sets at any time
    return *((volatile GXWndPlane *)&reg_G2S_DB_WININ + 1);
}

static inline GXWndPlane G2S_GetWndOutsidePlane(void) {
    // Volatile: the window register, which the game sets at any time
    return *(volatile GXWndPlane *)&reg_G2S_DB_WINOUT;
}

static inline GXWndPlane G2S_GetWndOBJInsidePlane(void) {
    return *((volatile GXWndPlane *)&reg_G2S_DB_WINOUT + 1);
}

static inline void G2_SetWnd0InsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2_WININ & ~REG_G2_WININ_WIN0IN_MASK) | wnd;

    if (effect) {
        tmp |= G2_WND_EFFECT;
    }
    reg_G2_WININ = (u16)tmp;
}

static inline void G2_SetWnd1InsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2_WININ & ~REG_G2_WININ_WIN1IN_MASK) | (wnd << REG_G2_WININ_WIN1IN_SHIFT);

    if (effect) {
        tmp |= G2_WND_EFFECT << REG_G2_WININ_WIN1IN_SHIFT;
    }
    reg_G2_WININ = (u16)tmp;
}

static inline void G2_SetWndOutsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2_WINOUT & ~REG_G2_WINOUT_WINOUT_MASK) | wnd;

    if (effect) {
        tmp |= G2_WND_EFFECT;
    }
    reg_G2_WINOUT = (u16)tmp;
}

static inline void G2S_SetWnd0InsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2S_DB_WININ & ~REG_G2_WININ_WIN0IN_MASK) | wnd;

    if (effect) {
        tmp |= G2_WND_EFFECT;
    }
    reg_G2S_DB_WININ = (u16)tmp;
}

static inline void G2S_SetWnd1InsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2S_DB_WININ & ~REG_G2_WININ_WIN1IN_MASK) | (wnd << REG_G2_WININ_WIN1IN_SHIFT);

    if (effect) {
        tmp |= G2_WND_EFFECT << REG_G2_WININ_WIN1IN_SHIFT;
    }
    reg_G2S_DB_WININ = (u16)tmp;
}

static inline void G2S_SetWndOutsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2S_DB_WINOUT & ~REG_G2_WINOUT_WINOUT_MASK) | wnd;

    if (effect) {
        tmp |= G2_WND_EFFECT;
    }
    reg_G2S_DB_WINOUT = (u16)tmp;
}

// A window's rectangle, from (x1, y1) to before (x2, y2). NitroSDK's G2_SetWnd0Position, G2_SetWnd1Position and their
// G2S_ forms
static inline void G2_SetWnd0Position(int x1, int y1, int x2, int y2) {
    reg_G2_WIN0H = (u16)(((x1 << 8) & 0xff00) | ((x2 << 0) & 0xff));
    reg_G2_WIN0V = (u16)(((y1 << 8) & 0xff00) | ((y2 << 0) & 0xff));
}

static inline void G2_SetWnd1Position(int x1, int y1, int x2, int y2) {
    reg_G2_WIN1H = (u16)(((x1 << 8) & 0xff00) | ((x2 << 0) & 0xff));
    reg_G2_WIN1V = (u16)(((y1 << 8) & 0xff00) | ((y2 << 0) & 0xff));
}

static inline void G2S_SetWnd0Position(int x1, int y1, int x2, int y2) {
    reg_G2S_DB_WIN0H = (u16)(((x1 << 8) & 0xff00) | (u8)x2);
    reg_G2S_DB_WIN0V = (u16)(((y1 << 8) & 0xff00) | (u8)y2);
}

static inline void G2S_SetWnd1Position(int x1, int y1, int x2, int y2) {
    reg_G2S_DB_WIN1H = (u16)(((x1 << 8) & 0xff00) | (u8)x2);
    reg_G2S_DB_WIN1V = (u16)(((y1 << 8) & 0xff00) | (u8)y2);
}



typedef enum {
    GX_POLYGONMODE_MODULATE,
    GX_POLYGONMODE_DECAL,
    GX_POLYGONMODE_TOON,
    GX_POLYGONMODE_SHADOW,
} GXPolygonMode;

typedef enum {
    GX_LIGHTMASK_NONE = 0,
    GX_LIGHTMASK_0 = 1,
    GX_LIGHTMASK_1 = 2,
    GX_LIGHTMASK_2 = 4,
    GX_LIGHTMASK_3 = 8,
} GXLightMask;

#define GX_POLYGON_ATTR_MISC_FOG 0x8000

typedef enum {
    GX_LIGHTID_0,
    GX_LIGHTID_1,
    GX_LIGHTID_2,
    GX_LIGHTID_3,
} GXLightId;

typedef enum {
    GX_CULL_ALL,
    GX_CULL_FRONT,
    GX_CULL_BACK,
    GX_CULL_NONE,
} GXCull;

// The texture parameters as the SDK's enums, which G3_TexImageParam takes: SPL's texture setup only matches with them
typedef enum {
    GX_TEXFMT_NONE,
    GX_TEXFMT_A3I5,
    GX_TEXFMT_PLTT4,
    GX_TEXFMT_PLTT16,
    GX_TEXFMT_PLTT256,
    GX_TEXFMT_COMP4x4,
    GX_TEXFMT_A5I3,
    GX_TEXFMT_DIRECT,
} GXTexFmt;

typedef enum {
    GX_TEXGEN_NONE,
    GX_TEXGEN_TEXCOORD,
    GX_TEXGEN_NORMAL,
    GX_TEXGEN_VERTEX,
} GXTexGen;

typedef enum {
    GX_TEXSIZE_S8,
    GX_TEXSIZE_S16,
    GX_TEXSIZE_S32,
    GX_TEXSIZE_S64,
    GX_TEXSIZE_S128,
    GX_TEXSIZE_S256,
    GX_TEXSIZE_S512,
    GX_TEXSIZE_S1024,
} GXTexSizeS;

typedef enum {
    GX_TEXSIZE_T8,
    GX_TEXSIZE_T16,
    GX_TEXSIZE_T32,
    GX_TEXSIZE_T64,
    GX_TEXSIZE_T128,
    GX_TEXSIZE_T256,
    GX_TEXSIZE_T512,
    GX_TEXSIZE_T1024,
} GXTexSizeT;

typedef enum {
    GX_TEXREPEAT_NONE,
    GX_TEXREPEAT_S,
    GX_TEXREPEAT_T,
    GX_TEXREPEAT_ST,
} GXTexRepeat;

typedef enum {
    GX_TEXFLIP_NONE,
    GX_TEXFLIP_S,
    GX_TEXFLIP_T,
    GX_TEXFLIP_ST,
} GXTexFlip;

typedef enum {
    GX_TEXPLTTCOLOR0_USE,
    GX_TEXPLTTCOLOR0_TRNS,
} GXTexPlttColor0;

#define REG_G3_TEXIMAGE_PARAM_RS_SHIFT 16
#define REG_G3_TEXIMAGE_PARAM_FS_SHIFT 18
#define REG_G3_TEXIMAGE_PARAM_V_SIZE_SHIFT 20
#define REG_G3_TEXIMAGE_PARAM_T_SIZE_SHIFT 23
#define REG_G3_TEXIMAGE_PARAM_TEXFMT_SHIFT 26
#define REG_G3_TEXIMAGE_PARAM_TR_SHIFT 29
#define REG_G3_TEXIMAGE_PARAM_TGEN_SHIFT 30
#define REG_G3_TEXIMAGE_PARAM_TGEN_MASK 0xc0000000

#define GX_PACK_TEXIMAGE_PARAM(texFmt, texGen, s, t, repeat, flip, pltt0, addr)                                        \
    ((u32)(((addr) >> 3) | ((texFmt) << REG_G3_TEXIMAGE_PARAM_TEXFMT_SHIFT) |                                          \
           ((texGen) << REG_G3_TEXIMAGE_PARAM_TGEN_SHIFT) | ((s) << REG_G3_TEXIMAGE_PARAM_V_SIZE_SHIFT) |              \
           ((t) << REG_G3_TEXIMAGE_PARAM_T_SIZE_SHIFT) | ((repeat) << REG_G3_TEXIMAGE_PARAM_RS_SHIFT) |                \
           ((flip) << REG_G3_TEXIMAGE_PARAM_FS_SHIFT) | ((pltt0) << REG_G3_TEXIMAGE_PARAM_TR_SHIFT)))

#define GX_PACK_TEXPLTTBASE_PARAM(addr, texFmt) ((u32)((addr) >> (4 - ((texFmt) == GX_TEXFMT_PLTT4))))

#define REG_G3_POLYGON_ATTR_LE_SHIFT 0
#define REG_G3_POLYGON_ATTR_PM_SHIFT 4
#define REG_G3_POLYGON_ATTR_BK_SHIFT 6
#define REG_G3_POLYGON_ATTR_ALPHA_SHIFT 16
#define REG_G3_POLYGON_ATTR_ALPHA_MASK 0x001f0000
#define REG_G3_POLYGON_ATTR_ID_SHIFT 24
#define REG_G3_POLYGON_ATTR_LE_MASK 0x0000000f
#define REG_G3_POLYGON_ATTR_PM_MASK 0x00000030
#define REG_G3_POLYGON_ATTR_BK_MASK 0x00000040
#define REG_G3_POLYGON_ATTR_FR_MASK 0x00000080
#define REG_G3_POLYGON_ATTR_ID_MASK 0x3f000000

#define GX_PACK_POLYGONATTR_PARAM(light, polyMode, cullMode, polygonID, alpha, misc)                                   \
    ((u32)(((light) << REG_G3_POLYGON_ATTR_LE_SHIFT) | ((polyMode) << REG_G3_POLYGON_ATTR_PM_SHIFT) |                  \
           ((cullMode) << REG_G3_POLYGON_ATTR_BK_SHIFT) | (misc) | ((polygonID) << REG_G3_POLYGON_ATTR_ID_SHIFT) |     \
           ((alpha) << REG_G3_POLYGON_ATTR_ALPHA_SHIFT)))

#define REG_G3_DIF_AMB_AMBIENT_RED_SHIFT 16
#define REG_G3_DIF_AMB_C_SHIFT 15
#define REG_G3_DIF_AMB_C_MASK 0x00008000
#define REG_G3_SPE_EMI_S_MASK 0x00008000
#define REG_G3_SPE_EMI_EMISSION_RED_SHIFT 16
#define REG_G3_SPE_EMI_S_SHIFT 15

#define GX_PACK_DIFFAMB_PARAM(diffuse, ambient, IsSetVtxColor)                                                         \
    ((u32)((diffuse) | ((ambient) << REG_G3_DIF_AMB_AMBIENT_RED_SHIFT) |                                               \
           (((IsSetVtxColor) != FALSE) << REG_G3_DIF_AMB_C_SHIFT)))

#define GX_PACK_SPECEMI_PARAM(specular, emission, IsShininess)                                                         \
    ((u32)((specular) | ((emission) << REG_G3_SPE_EMI_EMISSION_RED_SHIFT) |                                            \
           (((IsShininess) != FALSE) << REG_G3_SPE_EMI_S_SHIFT)))

// A normal's components, 10 bits each, and a texture coordinate's, fixed point with 4 fractional bits
#define GX_FX16_FX10(x) ((fx16)((x) >> 3))
#define GX_VECFX10(x, y, z) ((u32)(((x) & 0x3ff) | (((y) & 0x3ff) << 10) | (((z) & 0x3ff) << 20)))
#define REG_G3_LIGHT_VECTOR_LNUM_SHIFT 30
#define GX_PACK_LIGHTVECTOR_PARAM(lightID, x, y, z)                                                                    \
    ((u32)(((lightID) << REG_G3_LIGHT_VECTOR_LNUM_SHIFT) |                                                             \
           GX_VECFX10(GX_FX16_FX10(x), GX_FX16_FX10(y), GX_FX16_FX10(z))))
#define GX_PACK_LIGHTCOLOR_PARAM(lightID, rgb) ((u32)(((lightID) << REG_G3_LIGHT_VECTOR_LNUM_SHIFT) | (rgb)))
#define GX_ST(s, t) ((u32)(u16)(fx16)((s) >> 8) | ((u32)(u16)(fx16)((t) >> 8) << 16))

static inline void G3_MtxMode(int mode) {
    reg_G3_MTX_MODE = mode;
}

static inline void G3_PushMtx(void) {
    reg_G3_MTX_PUSH = 0;
}

static inline void G3_PopMtx(int num) {
    reg_G3_MTX_POP = num;
}

static inline void G3_Identity(void) {
    reg_G3_MTX_IDENTITY = 0;
}

static inline void G3_Translate(fx32 x, fx32 y, fx32 z) {
    reg_G3_MTX_TRANS = (u32)x;
    reg_G3_MTX_TRANS = (u32)y;
    reg_G3_MTX_TRANS = (u32)z;
}

static inline void G3_Scale(fx32 x, fx32 y, fx32 z) {
    reg_G3_MTX_SCALE = (u32)x;
    reg_G3_MTX_SCALE = (u32)y;
    reg_G3_MTX_SCALE = (u32)z;
}

static inline void G3_TexImageParam(GXTexFmt texFmt, GXTexGen texGen, GXTexSizeS s, GXTexSizeT t, GXTexRepeat repeat,
                                    GXTexFlip flip, GXTexPlttColor0 pltt0, u32 addr) {
    reg_G3_TEXIMAGE_PARAM = GX_PACK_TEXIMAGE_PARAM(texFmt, texGen, s, t, repeat, flip, pltt0, addr);
}

static inline void G3_TexPlttBase(u32 addr, GXTexFmt texFmt) {
    reg_G3_TEXPLTT_BASE = GX_PACK_TEXPLTTBASE_PARAM(addr, texFmt);
}

// The lights as an int, as NitroSDK declares them: a GXLightMask parameter changes the order MWCC computes the fields in
static inline void G3_PolygonAttr(int light, GXPolygonMode polyMode, GXCull cullMode, int polygonID, int alpha, int misc) {
    reg_G3_POLYGON_ATTR = GX_PACK_POLYGONATTR_PARAM(light, polyMode, cullMode, polygonID, alpha, misc);
}

static inline void G3_MaterialColorDiffAmb(GXRgb diffuse, GXRgb ambient, BOOL IsSetVtxColor) {
    reg_G3_DIF_AMB = GX_PACK_DIFFAMB_PARAM(diffuse, ambient, IsSetVtxColor);
}

static inline void G3_MaterialColorSpecEmi(GXRgb specular, GXRgb emission, BOOL IsShininess) {
    reg_G3_SPE_EMI = GX_PACK_SPECEMI_PARAM(specular, emission, IsShininess);
}

static inline void G3_Begin(int primitive) {
    reg_G3_BEGIN_VTXS = (u32)primitive;
}

static inline void G3_End(void) {
    reg_G3_END_VTXS = 0;
}

static inline void G3_Normal(fx16 x, fx16 y, fx16 z) {
    reg_G3_NORMAL = GX_VECFX10(GX_FX16_FX10(x), GX_FX16_FX10(y), GX_FX16_FX10(z));
}

static inline void G3_TexCoord(fx32 s, fx32 t) {
    reg_G3_TEXCOORD = GX_ST(s, t);
}

static inline void G3_Color(GXRgb rgb) {
    reg_G3_COLOR = rgb;
}

static inline void G3_LightVector(GXLightId lightID, fx16 x, fx16 y, fx16 z) {
    reg_G3_LIGHT_VECTOR = GX_PACK_LIGHTVECTOR_PARAM(lightID, x, y, z);
}

static inline void G3_LightColor(GXLightId lightID, GXRgb rgb) {
    reg_G3_LIGHT_COLOR = GX_PACK_LIGHTCOLOR_PARAM(lightID, rgb);
}

static inline void G3_Vtx(fx16 x, fx16 y, fx16 z) {
    reg_G3_VTX_16 = (u32)((u16)x | ((u16)y << 16));
    reg_G3_VTX_16 = (u32)(u16)z;
}

// NitroSDK's G3X_Reset, G3i_LookAt_ (which loads the camera matrix into the geometry engine when isLoad is set),
// G3_RotZ and G3_MultTransMtx33
void G3X_Reset(void);

// Read the geometry engine's clip matrix / vector matrix back; nonzero while the engine is busy: NitroSDK's G3X_GetClipMtx, G3X_GetVectorMtx
int G3X_GetClipMtx(MtxFx44 *m);
int G3X_GetVectorMtx(MtxFx33 *m);
void G3i_LookAt_(const VecFx32 *camPos, const VecFx32 *camUp, const VecFx32 *target, BOOL isLoad, MtxFx43 *mtx);
void G3_RotZ(fx32 sin, fx32 cos);
void G3_MultTransMtx33(const MtxFx33 *mtx, const VecFx32 *trans);

// VRAM banks, as bits of a mask
#define GX_VRAM_A 0x001
#define GX_VRAM_B 0x002
#define GX_VRAM_C 0x004
#define GX_VRAM_D 0x008
#define GX_VRAM_E 0x010
#define GX_VRAM_F 0x020
#define GX_VRAM_G 0x040
#define GX_VRAM_H 0x080
#define GX_VRAM_I 0x100

#define REG_GX_DISPCAPCNT_E_MASK 0x80000000
#define REG_GX_DISPCAPCNT_MOD_SHIFT 29
#define REG_GX_DISPCAPCNT_SRCB_SHIFT 25
#define REG_GX_DISPCAPCNT_SRCA_SHIFT 24
#define REG_GX_DISPCAPCNT_WSIZE_SHIFT 20
#define REG_GX_DISPCAPCNT_WB_SHIFT 16
#define REG_GX_DISPCAPCNT_EVB_SHIFT 8
#define REG_GX_DISPCAPCNT_EVA_SHIFT 0

// The display capture, which writes the main engine's output to VRAM, blended with an image in VRAM with AB
// NitroSDK's enums: the parameters' enum types make MWCC load the arguments of GX_SetCapture before it uses them
typedef enum {
    GX_CAPTURE_SIZE_128x128 = 0,
    GX_CAPTURE_SIZE_256x64 = 1,
    GX_CAPTURE_SIZE_256x128 = 2,
    GX_CAPTURE_SIZE_256x192 = 3
} GXCaptureSize;

typedef enum {
    GX_CAPTURE_MODE_A = 0,
    GX_CAPTURE_MODE_B = 1,
    GX_CAPTURE_MODE_AB = 2
} GXCaptureMode;

typedef enum {
    GX_CAPTURE_SRCA_2D3D = 0,
    GX_CAPTURE_SRCA_3D = 1
} GXCaptureSrcA;

typedef enum {
    GX_CAPTURE_SRCB_VRAM_0x00000 = 0,
    GX_CAPTURE_SRCB_MRAM = 1
} GXCaptureSrcB;

typedef enum {
    GX_CAPTURE_DEST_VRAM_A_0x00000 = 0,
    GX_CAPTURE_DEST_VRAM_B_0x00000 = 1,
    GX_CAPTURE_DEST_VRAM_C_0x00000 = 2,
    GX_CAPTURE_DEST_VRAM_D_0x00000 = 3
} GXCaptureDest;

// The fields go from the highest bit to the lowest, as NitroSDK's register field macros write them
static inline void GX_SetCapture(GXCaptureSize sz, GXCaptureMode mode, GXCaptureSrcA a, GXCaptureSrcB b,
                                 GXCaptureDest dest, int eva, int evb) {
    reg_GX_DISPCAPCNT = REG_GX_DISPCAPCNT_E_MASK | (mode << REG_GX_DISPCAPCNT_MOD_SHIFT) |
                        (b << REG_GX_DISPCAPCNT_SRCB_SHIFT) | (a << REG_GX_DISPCAPCNT_SRCA_SHIFT) |
                        (sz << REG_GX_DISPCAPCNT_WSIZE_SHIFT) | (dest << REG_GX_DISPCAPCNT_WB_SHIFT) |
                        (evb << REG_GX_DISPCAPCNT_EVB_SHIFT) | (eva << REG_GX_DISPCAPCNT_EVA_SHIFT);
}

// VRAM bank assignments. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// NitroSDK's enums of the banks each part of the VRAM can take, ints, as the functions below take and return them
typedef int GXVRamLCDC;
typedef int GXVRamBG;
typedef int GXVRamBGExtPltt;
typedef int GXVRamOBJ;
typedef int GXVRamOBJExtPltt;
typedef int GXVRamSubBG;
typedef int GXVRamSubBGExtPltt;
typedef int GXVRamSubOBJ;
typedef int GXVRamSubOBJExtPltt;
typedef int GXVRamTex;
typedef int GXVRamTexPltt;
typedef int GXVRamClearImage;

// NitroSDK's GX_ResetBankFor* and GX_SetBankFor*, which take VRAM banks as masks
// Return the banks the BG had, as GX_ResetBankForTex does
GXVRamBG GX_ResetBankForBG(void);
GXVRamBGExtPltt GX_ResetBankForBGExtPltt(void);
GXVRamSubBG GX_ResetBankForSubBG(void);
GXVRamSubBGExtPltt GX_ResetBankForSubBGExtPltt(void);
GXVRamOBJ GX_ResetBankForOBJ(void);
GXVRamOBJExtPltt GX_ResetBankForOBJExtPltt(void);
GXVRamSubOBJ GX_ResetBankForSubOBJ(void);
GXVRamSubOBJExtPltt GX_ResetBankForSubOBJExtPltt(void);
// Returns the banks the textures had, as GX_ResetBankForTex does
GXVRamTex GX_ResetBankForTex(void);
GXVRamTexPltt GX_ResetBankForTexPltt(void);
void GX_SetBankForBG(GXVRamBG banks);
void GX_SetBankForBGExtPltt(GXVRamBGExtPltt banks);
void GX_SetBankForSubBG(GXVRamSubBG banks);
void GX_SetBankForSubBGExtPltt(GXVRamSubBGExtPltt banks);
void GX_SetBankForOBJ(GXVRamOBJ banks);
void GX_SetBankForOBJExtPltt(GXVRamOBJExtPltt banks);
void GX_SetBankForSubOBJ(GXVRamSubOBJ banks);
void GX_SetBankForSubOBJExtPltt(GXVRamSubOBJExtPltt banks);
void GX_SetBankForTex(GXVRamTex banks);
GXVRamClearImage GX_ResetBankForClearImage(void);
void GX_SetBankForClearImage(GXVRamClearImage banks);
// NitroSDK's GX_GetBankForTex
GXVRamTex GX_GetBankForTex(void);
void GX_SetBankForTexPltt(GXVRamTexPltt banks);
// NitroSDK's GX_DisableBankForSubBG and GX_DisableBankForSubOBJ
GXVRamSubBG GX_DisableBankForSubBG(void);
GXVRamSubOBJ GX_DisableBankForSubOBJ(void);
// NitroSDK's GX_DisableBankForLCDC
GXVRamLCDC GX_DisableBankForLCDC(void);
// NitroSDK's GX_SetBankForLCDC and GX_GetBankForLCDC
void GX_SetBankForLCDC(GXVRamLCDC banks);
GXVRamLCDC GX_GetBankForLCDC(void);

// NitroSDK's GX_DispOn
void GX_DispOn(void);

#define GX_VRAM_LCDC_NONE 0x000
#define GX_VRAM_LCDC_ALL 0x1ff

// VRAM as the CPU sees it with every bank given to it, and OAM
#define HW_LCDC_VRAM 0x06800000
#define HW_LCDC_VRAM_SIZE 0xa4000

// The banks in LCDC mode, where the CPU reads and writes them
#define HW_LCDC_VRAM_A 0x06800000
#define HW_LCDC_VRAM_B 0x06820000
#define HW_LCDC_VRAM_C 0x06840000
#define HW_LCDC_VRAM_D 0x06860000
#define HW_LCDC_VRAM_E 0x06880000
#define HW_LCDC_VRAM_F 0x06890000
#define HW_LCDC_VRAM_G 0x06894000
#define HW_LCDC_VRAM_H 0x06898000
#define HW_LCDC_VRAM_I 0x068a0000

#define GX_DISPMODE_GRAPHICS 1
#define GX_BGMODE_0 0
#define GX_BGMODE_5 5
#define GX_BG0_AS_2D 0
#define GX_BG0_AS_3D 1

#define GX_VRAM_BG_128_A GX_VRAM_A
#define GX_VRAM_BGEXTPLTT_NONE 0x000
#define GX_VRAM_SUB_BG_NONE 0x000
#define GX_VRAM_SUB_BG_128_C GX_VRAM_C
#define GX_VRAM_SUB_BGEXTPLTT_NONE 0x000
#define GX_VRAM_OBJ_64_E GX_VRAM_E
#define GX_VRAM_OBJEXTPLTT_NONE 0x000
#define GX_VRAM_SUB_OBJ_16_I GX_VRAM_I
#define GX_VRAM_SUB_OBJ_128_D GX_VRAM_D
#define GX_VRAM_SUB_OBJEXTPLTT_NONE 0x000
#define GX_VRAM_TEX_0_B GX_VRAM_B
#define GX_VRAM_TEXPLTT_0_F GX_VRAM_F

typedef enum {
    GX_OBJVRAMMODE_CHAR_2D = 0x00000000,
    GX_OBJVRAMMODE_CHAR_1D_32K = 0x00000010,
    GX_OBJVRAMMODE_CHAR_1D_64K = 0x00100010,
    GX_OBJVRAMMODE_CHAR_1D_128K = 0x00200010,
    GX_OBJVRAMMODE_CHAR_1D_256K = 0x00300010,
} GXOBJVRamModeChar;

typedef enum {
    GX_OBJVRAMMODE_BMP_2D_W128 = 0x00000000,
    GX_OBJVRAMMODE_BMP_2D_W256 = 0x00000020,
    GX_OBJVRAMMODE_BMP_1D_128K = 0x00000040,
} GXOBJVRamModeBmp;

// NitroSDK passes the fields of the BG control registers as enums, which MWCC schedules differently from ints
typedef enum {
    GX_BG_COLORMODE_16 = 0,
    GX_BG_COLORMODE_256 = 1,
} GXBGColorMode;

typedef enum {
    GX_BG_EXTPLTT_01 = 0,
    GX_BG_EXTPLTT_23 = 1,
} GXBGExtPltt;

typedef enum {
    GX_BG_AREAOVER_XLU = 0,
    GX_BG_AREAOVER_REPEAT = 1,
} GXBGAreaOver;

// Direct color bitmap BGs, of 16 bit colors
typedef enum {
    GX_BG_SCRSIZE_DCBMP_128x128 = 0,
    GX_BG_SCRSIZE_DCBMP_256x256 = 1,
    GX_BG_SCRSIZE_DCBMP_512x256 = 2,
    GX_BG_SCRSIZE_DCBMP_512x512 = 3,
} GXBGScrSizeDcBmp;

typedef enum {
    GX_BG_BMPSCRBASE_0x00000 = 0,
    GX_BG_BMPSCRBASE_0x10000 = 4,
    GX_BG_BMPSCRBASE_0x14000 = 5,
    GX_BG_BMPSCRBASE_0x20000 = 8,
    GX_BG_BMPSCRBASE_0x28000 = 10,
} GXBGBmpScrBase;

// Which engine draws to the top screen
#define GX_DISP_SELECT_SUB_MAIN 0
#define GX_DISP_SELECT_MAIN_SUB 1
#define REG_GX_POWCNT_DSEL_SHIFT 15

static inline void GX_SetDispSelect(int select) {
    reg_GX_POWCNT = (u16)((reg_GX_POWCNT & ~REG_GX_POWCNT_DSEL_MASK) | (select << REG_GX_POWCNT_DSEL_SHIFT));
}

static inline int GX_GetDispSelect(void) {
    return (reg_GX_POWCNT & REG_GX_POWCNT_DSEL_MASK) >> REG_GX_POWCNT_DSEL_SHIFT;
}

// The BG and OBJ VRAM of each engine, at their largest
#define HW_BG_VRAM 0x06000000
#define HW_BG_VRAM_SIZE 0x80000
#define HW_DB_BG_VRAM 0x06200000
#define HW_DB_BG_VRAM_SIZE 0x20000
#define HW_OBJ_VRAM_SIZE 0x40000
// The sub engine's OBJ characters
#define HW_DB_OBJ_VRAM 0x06600000
#define HW_DB_OBJ_VRAM_SIZE 0x20000
#define HW_OAM 0x07000000
#define HW_DB_OAM 0x07000400
#define HW_OAM_SIZE 0x400
#define HW_PLTT_SIZE 0x400

#define REG_GX_DISPCNT_DISPLAY_SHIFT 8
#define REG_GX_DISPCNT_DISPLAY_MASK 0x00001f00

#define REG_GX_DISPCNT_OBJMAP_CH_MASK 0x00000010
#define REG_GX_DISPCNT_EXOBJ_CH_MASK 0x00300000
#define REG_GXS_DB_DISPCNT_MODE_MASK 0x00010000

static inline void GX_SetVisiblePlane(int plane) {
    reg_GX_DISPCNT = (u32)((reg_GX_DISPCNT & ~REG_GX_DISPCNT_DISPLAY_MASK) | (plane << REG_GX_DISPCNT_DISPLAY_SHIFT));
}

static inline void GXS_SetVisiblePlane(int plane) {
    reg_GXS_DB_DISPCNT =
        (u32)((reg_GXS_DB_DISPCNT & ~REG_GX_DISPCNT_DISPLAY_MASK) | (plane << REG_GX_DISPCNT_DISPLAY_SHIFT));
}

static inline void GX_SetOBJVRamModeChar(GXOBJVRamModeChar mode) {
    reg_GX_DISPCNT = (u32)((reg_GX_DISPCNT & ~(REG_GX_DISPCNT_EXOBJ_CH_MASK | REG_GX_DISPCNT_OBJMAP_CH_MASK)) | mode);
}

static inline void GXS_SetOBJVRamModeChar(GXOBJVRamModeChar mode) {
    reg_GXS_DB_DISPCNT =
        (u32)((reg_GXS_DB_DISPCNT & ~(REG_GX_DISPCNT_EXOBJ_CH_MASK | REG_GX_DISPCNT_OBJMAP_CH_MASK)) | mode);
}

static inline GXOBJVRamModeChar GX_GetOBJVRamModeChar(void) {
    return (GXOBJVRamModeChar)(reg_GX_DISPCNT & (REG_GX_DISPCNT_EXOBJ_CH_MASK | REG_GX_DISPCNT_OBJMAP_CH_MASK));
}

static inline GXOBJVRamModeChar GXS_GetOBJVRamModeChar(void) {
    return (GXOBJVRamModeChar)(reg_GXS_DB_DISPCNT & (REG_GX_DISPCNT_EXOBJ_CH_MASK | REG_GX_DISPCNT_OBJMAP_CH_MASK));
}

#define REG_GX_DISPCNT_OBJMAP_BM_MASK 0x00000060

static inline void GXS_SetOBJVRamModeBmp(GXOBJVRamModeBmp mode) {
    reg_GXS_DB_DISPCNT = (u32)((reg_GXS_DB_DISPCNT & ~REG_GX_DISPCNT_OBJMAP_BM_MASK) | mode);
}

// NitroSDK's GX_LoadOAM and GXS_LoadOAM
void GX_LoadOAM(const void *src, u32 offset, u32 size);
void GXS_LoadOAM(const void *src, u32 offset, u32 size);

// NitroSDK's GX_GetBankForOBJ and GX_GetBankForSubOBJ
GXVRamOBJ GX_GetBankForOBJ(void);
GXVRamSubOBJ GX_GetBankForSubOBJ(void);

typedef enum {
    GX_BG_SCRBASE_0x0000 = 0,
    GX_BG_SCRBASE_0x0800 = 1,
    GX_BG_SCRBASE_0x1000 = 2,
    GX_BG_SCRBASE_0x1800 = 3,
    GX_BG_SCRBASE_0x2000 = 4,
    GX_BG_SCRBASE_0x2800 = 5,
    GX_BG_SCRBASE_0x3000 = 6,
    GX_BG_SCRBASE_0x3800 = 7,
    GX_BG_SCRBASE_0x4000 = 8,
    GX_BG_SCRBASE_0x4800 = 9,
    GX_BG_SCRBASE_0x5000 = 10,
    GX_BG_SCRBASE_0x5800 = 11,
    GX_BG_SCRBASE_0x6000 = 12,
    GX_BG_SCRBASE_0x6800 = 13,
    GX_BG_SCRBASE_0x7000 = 14,
    GX_BG_SCRBASE_0x7800 = 15,
    GX_BG_SCRBASE_0x8000 = 16,
    GX_BG_SCRBASE_0x8800 = 17,
    GX_BG_SCRBASE_0x9000 = 18,
    GX_BG_SCRBASE_0x9800 = 19,
    GX_BG_SCRBASE_0xa000 = 20,
    GX_BG_SCRBASE_0xa800 = 21,
    GX_BG_SCRBASE_0xb000 = 22,
    GX_BG_SCRBASE_0xb800 = 23,
    GX_BG_SCRBASE_0xc000 = 24,
    GX_BG_SCRBASE_0xc800 = 25,
    GX_BG_SCRBASE_0xd000 = 26,
    GX_BG_SCRBASE_0xd800 = 27,
    GX_BG_SCRBASE_0xe000 = 28,
    GX_BG_SCRBASE_0xe800 = 29,
    GX_BG_SCRBASE_0xf000 = 30,
    GX_BG_SCRBASE_0xf800 = 31,
} GXBGScrBase;

typedef enum {
    GX_BG_CHARBASE_0x00000 = 0,
    GX_BG_CHARBASE_0x04000 = 1,
    GX_BG_CHARBASE_0x08000 = 2,
    GX_BG_CHARBASE_0x0c000 = 3,
    GX_BG_CHARBASE_0x10000 = 4,
    GX_BG_CHARBASE_0x14000 = 5,
    GX_BG_CHARBASE_0x18000 = 6,
    GX_BG_CHARBASE_0x1c000 = 7,
    GX_BG_CHARBASE_0x20000 = 8,
    GX_BG_CHARBASE_0x24000 = 9,
    GX_BG_CHARBASE_0x28000 = 10,
    GX_BG_CHARBASE_0x2c000 = 11,
    GX_BG_CHARBASE_0x30000 = 12,
    GX_BG_CHARBASE_0x34000 = 13,
    GX_BG_CHARBASE_0x38000 = 14,
    GX_BG_CHARBASE_0x3c000 = 15,
} GXBGCharBase;

// The BG registers of both engines. The sub engine's are at the main engine's address plus 0x1000
#define reg_G2_BG0CNT (*(vu16 *)0x04000008)
#define reg_G2_BG1CNT (*(vu16 *)0x0400000a)
#define reg_G2_BG2CNT (*(vu16 *)0x0400000c)
#define reg_G2_BG3CNT (*(vu16 *)0x0400000e)
#define reg_G2_BG0OFS (*(vu32 *)0x04000010)
#define reg_G2_BG1OFS (*(vu32 *)0x04000014)
#define reg_G2_BG2OFS (*(vu32 *)0x04000018)
#define reg_G2_BG3OFS (*(vu32 *)0x0400001c)
#define reg_G2_BG2PA (*(vu16 *)0x04000020)
#define reg_G2_BG3PA (*(vu16 *)0x04000030)
#define reg_G2S_DB_BG0CNT (*(vu16 *)0x04001008)
#define reg_G2S_DB_BG1CNT (*(vu16 *)0x0400100a)
#define reg_G2S_DB_BG2CNT (*(vu16 *)0x0400100c)
#define reg_G2S_DB_BG3CNT (*(vu16 *)0x0400100e)
#define reg_G2S_DB_BG0OFS (*(vu32 *)0x04001010)
#define reg_G2S_DB_BG1OFS (*(vu32 *)0x04001014)
#define reg_G2S_DB_BG2OFS (*(vu32 *)0x04001018)
#define reg_G2S_DB_BG3OFS (*(vu32 *)0x0400101c)
#define reg_G2S_DB_BG2PA (*(vu16 *)0x04001020)
#define reg_G2S_DB_BG3PA (*(vu16 *)0x04001030)

// The fields of a BG control register. Bit 13 is the extended palette slot of BG 0 and 1, and the area overflow
// mode of BG 2 and 3
#define REG_G2_BG0CNT_PRIORITY_SHIFT 0
#define REG_G2_BG0CNT_PRIORITY_MASK 0x0003
#define REG_G2_BGCNT_CHARBASE_SHIFT 2
#define REG_G2_BGCNT_MOSAIC_MASK 0x0040
#define REG_G2_BGCNT_COLORMODE_SHIFT 7
#define REG_G2_BGCNT_SCREENBASE_SHIFT 8
#define REG_G2_BGCNT_BGPLTTSLOT_SHIFT 13
#define REG_G2_BGCNT_AREAOVER_SHIFT 13
#define REG_G2_BGCNT_SCREENSIZE_SHIFT 14

#define REG_G2_BG0OFS_HOFFSET_MASK 0x000001ff
#define REG_G2_BGOFS_VOFFSET_SHIFT 16
#define REG_G2_BGOFS_VOFFSET_MASK 0x01ff0000

#define REG_GX_DISPCNT_BGCHAROFFSET_SHIFT 24
#define REG_GX_DISPCNT_BGCHAROFFSET_MASK 0x07000000
#define REG_GX_DISPCNT_BGSCREENOFFSET_SHIFT 27
#define REG_GX_DISPCNT_BGSCREENOFFSET_MASK 0x38000000

#define GX_BGSCROFFSET_0x00000 0
#define GX_BGCHAROFFSET_0x00000 0

// The screen sizes of each kind of BG, as the control registers take them
typedef enum {
    GX_BG_SCRSIZE_TEXT_256x256 = 0,
    GX_BG_SCRSIZE_TEXT_512x256 = 1,
    GX_BG_SCRSIZE_TEXT_256x512 = 2,
    GX_BG_SCRSIZE_TEXT_512x512 = 3,
} GXBGScrSizeText;

typedef enum {
    GX_BG_SCRSIZE_AFFINE_128x128 = 0,
    GX_BG_SCRSIZE_AFFINE_256x256 = 1,
    GX_BG_SCRSIZE_AFFINE_512x512 = 2,
    GX_BG_SCRSIZE_AFFINE_1024x1024 = 3,
} GXBGScrSizeAffine;

typedef enum {
    GX_BG_SCRSIZE_256x16PLTT_128x128 = 0,
    GX_BG_SCRSIZE_256x16PLTT_256x256 = 1,
    GX_BG_SCRSIZE_256x16PLTT_512x512 = 2,
    GX_BG_SCRSIZE_256x16PLTT_1024x1024 = 3,
} GXBGScrSize256x16Pltt;

#define GX_BG_EXTMODE_256x16PLTT 0
#define GX_BG_EXTMODE_DCBMP 0x84

typedef union {
    u16 raw;
    struct {
        u16 priority : 2;
        u16 charBase : 4;
        u16 mosaic : 1;
        u16 colorMode : 1;
        u16 screenBase : 5;
        u16 bgExtPltt : 1;
        u16 screenSize : 2;
    };
} GXBg01Control;

typedef union {
    u16 raw;
    struct {
        u16 priority : 2;
        u16 charBase : 4;
        u16 mosaic : 1;
        u16 colorMode : 1;
        u16 screenBase : 5;
        u16 _reserve : 1;
        u16 screenSize : 2;
    };
} GXBg23ControlText;

typedef union {
    u16 raw;
    struct {
        u16 priority : 2;
        u16 charBase : 4;
        u16 mosaic : 1;
        u16 _reserve : 1;
        u16 screenBase : 5;
        u16 areaOver : 1;
        u16 screenSize : 2;
    };
} GXBg23ControlAffine;

typedef union {
    u16 raw;
    struct {
        u16 priority : 2;
        u16 _reserve1 : 1;
        u16 charBase : 3;
        u16 mosaic : 1;
        u16 _reserve2 : 1;
        u16 screenBase : 5;
        u16 areaOver : 1;
        u16 screenSize : 2;
    };
} GXBg23Control256x16Pltt;

static inline void G2_BlendNone(void) {
    reg_G2_BLDCNT = 0;
}

static inline void GX_SetBGScrOffset(int offset) {
    reg_GX_DISPCNT =
        (u32)((reg_GX_DISPCNT & ~REG_GX_DISPCNT_BGSCREENOFFSET_MASK) | (offset << REG_GX_DISPCNT_BGSCREENOFFSET_SHIFT));
}

static inline void GX_SetBGCharOffset(int offset) {
    reg_GX_DISPCNT =
        (u32)((reg_GX_DISPCNT & ~REG_GX_DISPCNT_BGCHAROFFSET_MASK) | (offset << REG_GX_DISPCNT_BGCHAROFFSET_SHIFT));
}

// NitroSDK has an inline function of each of these for each BG of each engine, such as G2_SetBG0Control and
// G2S_SetBG0Control. They are generated here from the register

#define GX_DEFINE_BG01_CONTROL(name, reg)                                                                              \
    static inline void name(GXBGScrSizeText screenSize, GXBGColorMode colorMode, GXBGScrBase screenBase,               \
                            GXBGCharBase charBase, GXBGExtPltt bgExtPltt) {                                            \
        reg = (u16)((reg & (REG_G2_BG0CNT_PRIORITY_MASK | REG_G2_BGCNT_MOSAIC_MASK)) |                                 \
                    (screenSize << REG_G2_BGCNT_SCREENSIZE_SHIFT) | (colorMode << REG_G2_BGCNT_COLORMODE_SHIFT) |      \
                    (screenBase << REG_G2_BGCNT_SCREENBASE_SHIFT) | (charBase << REG_G2_BGCNT_CHARBASE_SHIFT) |        \
                    (bgExtPltt << REG_G2_BGCNT_BGPLTTSLOT_SHIFT));                                                     \
    }

#define GX_DEFINE_BG23_CONTROL_TEXT(name, reg)                                                                         \
    static inline void name(GXBGScrSizeText screenSize, GXBGColorMode colorMode, GXBGScrBase screenBase,               \
                            GXBGCharBase charBase) {                                                                   \
        reg = (u16)((reg & (REG_G2_BG0CNT_PRIORITY_MASK | REG_G2_BGCNT_MOSAIC_MASK)) |                                 \
                    (screenSize << REG_G2_BGCNT_SCREENSIZE_SHIFT) | (colorMode << REG_G2_BGCNT_COLORMODE_SHIFT) |      \
                    (screenBase << REG_G2_BGCNT_SCREENBASE_SHIFT) | (charBase << REG_G2_BGCNT_CHARBASE_SHIFT));        \
    }

#define GX_DEFINE_BG23_CONTROL_AFFINE(name, reg)                                                                       \
    static inline void name(GXBGScrSizeAffine screenSize, GXBGAreaOver areaOver, GXBGScrBase screenBase,               \
                            GXBGCharBase charBase) {                                                                   \
        reg = (u16)((reg & (REG_G2_BG0CNT_PRIORITY_MASK | REG_G2_BGCNT_MOSAIC_MASK)) |                                 \
                    (screenSize << REG_G2_BGCNT_SCREENSIZE_SHIFT) | (screenBase << REG_G2_BGCNT_SCREENBASE_SHIFT) |    \
                    (charBase << REG_G2_BGCNT_CHARBASE_SHIFT) | (areaOver << REG_G2_BGCNT_AREAOVER_SHIFT));            \
    }

#define GX_DEFINE_BG23_CONTROL_256x16PLTT(name, reg)                                                                   \
    static inline void name(GXBGScrSize256x16Pltt screenSize, GXBGAreaOver areaOver, GXBGScrBase screenBase,           \
                            GXBGCharBase charBase) {                                                                   \
        reg = (u16)((reg & (REG_G2_BG0CNT_PRIORITY_MASK | REG_G2_BGCNT_MOSAIC_MASK)) |                                 \
                    (screenSize << REG_G2_BGCNT_SCREENSIZE_SHIFT) | (charBase << REG_G2_BGCNT_CHARBASE_SHIFT) |        \
                    GX_BG_EXTMODE_256x16PLTT | (screenBase << REG_G2_BGCNT_SCREENBASE_SHIFT) |                         \
                    (areaOver << REG_G2_BGCNT_AREAOVER_SHIFT));                                                        \
    }

#define GX_DEFINE_BG23_CONTROL_DCBMP(name, reg)                                                                        \
    static inline void name(GXBGScrSizeDcBmp screenSize, GXBGAreaOver areaOver, GXBGBmpScrBase screenBase) {           \
        reg = (u16)((reg & (REG_G2_BG0CNT_PRIORITY_MASK | REG_G2_BGCNT_MOSAIC_MASK)) |                                 \
                    (screenSize << REG_G2_BGCNT_SCREENSIZE_SHIFT) | GX_BG_EXTMODE_DCBMP |                              \
                    (screenBase << REG_G2_BGCNT_SCREENBASE_SHIFT) | (areaOver << REG_G2_BGCNT_AREAOVER_SHIFT));        \
    }

#define GX_DEFINE_BG_PRIORITY(name, reg)                                                                               \
    static inline void name(int priority) {                                                                            \
        reg = (u16)((reg & ~REG_G2_BG0CNT_PRIORITY_MASK) | (priority << REG_G2_BG0CNT_PRIORITY_SHIFT));                \
    }

#define GX_DEFINE_BG_MOSAIC(name, reg)                                                                                 \
    static inline void name(BOOL enable) {                                                                             \
        if (enable) {                                                                                                  \
            reg |= REG_G2_BGCNT_MOSAIC_MASK;                                                                           \
        } else {                                                                                                       \
            reg &= ~REG_G2_BGCNT_MOSAIC_MASK;                                                                          \
        }                                                                                                              \
    }

#define GX_DEFINE_BG_OFFSET(name, reg)                                                                                 \
    static inline void name(int hOffset, int vOffset) {                                                                \
        reg = (u32)((hOffset & REG_G2_BG0OFS_HOFFSET_MASK) |                                                            \
                    ((vOffset << REG_G2_BGOFS_VOFFSET_SHIFT) & REG_G2_BGOFS_VOFFSET_MASK));                            \
    }

#define GX_DEFINE_BG_GET_CONTROL(name, type, reg)                                                                      \
    static inline type name(void) {                                                                                    \
        return *(volatile type *)&reg;                                                                                 \
    }

// clang-format off
GX_DEFINE_BG01_CONTROL(G2_SetBG0Control, reg_G2_BG0CNT)
GX_DEFINE_BG01_CONTROL(G2_SetBG1Control, reg_G2_BG1CNT)
GX_DEFINE_BG01_CONTROL(G2S_SetBG0Control, reg_G2S_DB_BG0CNT)
GX_DEFINE_BG01_CONTROL(G2S_SetBG1Control, reg_G2S_DB_BG1CNT)

GX_DEFINE_BG23_CONTROL_TEXT(G2_SetBG2ControlText, reg_G2_BG2CNT)
GX_DEFINE_BG23_CONTROL_TEXT(G2_SetBG3ControlText, reg_G2_BG3CNT)
GX_DEFINE_BG23_CONTROL_TEXT(G2S_SetBG2ControlText, reg_G2S_DB_BG2CNT)
GX_DEFINE_BG23_CONTROL_TEXT(G2S_SetBG3ControlText, reg_G2S_DB_BG3CNT)

GX_DEFINE_BG23_CONTROL_AFFINE(G2_SetBG2ControlAffine, reg_G2_BG2CNT)
GX_DEFINE_BG23_CONTROL_AFFINE(G2_SetBG3ControlAffine, reg_G2_BG3CNT)
GX_DEFINE_BG23_CONTROL_AFFINE(G2S_SetBG2ControlAffine, reg_G2S_DB_BG2CNT)
GX_DEFINE_BG23_CONTROL_AFFINE(G2S_SetBG3ControlAffine, reg_G2S_DB_BG3CNT)

GX_DEFINE_BG23_CONTROL_256x16PLTT(G2_SetBG2Control256x16Pltt, reg_G2_BG2CNT)
GX_DEFINE_BG23_CONTROL_256x16PLTT(G2_SetBG3Control256x16Pltt, reg_G2_BG3CNT)
GX_DEFINE_BG23_CONTROL_256x16PLTT(G2S_SetBG2Control256x16Pltt, reg_G2S_DB_BG2CNT)
GX_DEFINE_BG23_CONTROL_256x16PLTT(G2S_SetBG3Control256x16Pltt, reg_G2S_DB_BG3CNT)

GX_DEFINE_BG23_CONTROL_DCBMP(G2S_SetBG2ControlDCBmp, reg_G2S_DB_BG2CNT)

GX_DEFINE_BG_PRIORITY(G2_SetBG0Priority, reg_G2_BG0CNT)
GX_DEFINE_BG_PRIORITY(G2_SetBG1Priority, reg_G2_BG1CNT)
GX_DEFINE_BG_PRIORITY(G2_SetBG2Priority, reg_G2_BG2CNT)
GX_DEFINE_BG_PRIORITY(G2_SetBG3Priority, reg_G2_BG3CNT)
GX_DEFINE_BG_PRIORITY(G2S_SetBG0Priority, reg_G2S_DB_BG0CNT)
GX_DEFINE_BG_PRIORITY(G2S_SetBG1Priority, reg_G2S_DB_BG1CNT)
GX_DEFINE_BG_PRIORITY(G2S_SetBG2Priority, reg_G2S_DB_BG2CNT)
GX_DEFINE_BG_PRIORITY(G2S_SetBG3Priority, reg_G2S_DB_BG3CNT)

GX_DEFINE_BG_MOSAIC(G2_BG0Mosaic, reg_G2_BG0CNT)
GX_DEFINE_BG_MOSAIC(G2_BG1Mosaic, reg_G2_BG1CNT)
GX_DEFINE_BG_MOSAIC(G2_BG2Mosaic, reg_G2_BG2CNT)
GX_DEFINE_BG_MOSAIC(G2_BG3Mosaic, reg_G2_BG3CNT)
GX_DEFINE_BG_MOSAIC(G2S_BG0Mosaic, reg_G2S_DB_BG0CNT)
GX_DEFINE_BG_MOSAIC(G2S_BG1Mosaic, reg_G2S_DB_BG1CNT)
GX_DEFINE_BG_MOSAIC(G2S_BG2Mosaic, reg_G2S_DB_BG2CNT)
GX_DEFINE_BG_MOSAIC(G2S_BG3Mosaic, reg_G2S_DB_BG3CNT)

GX_DEFINE_BG_OFFSET(G2_SetBG0Offset, reg_G2_BG0OFS)
GX_DEFINE_BG_OFFSET(G2_SetBG1Offset, reg_G2_BG1OFS)
GX_DEFINE_BG_OFFSET(G2_SetBG2Offset, reg_G2_BG2OFS)
GX_DEFINE_BG_OFFSET(G2_SetBG3Offset, reg_G2_BG3OFS)
GX_DEFINE_BG_OFFSET(G2S_SetBG0Offset, reg_G2S_DB_BG0OFS)
GX_DEFINE_BG_OFFSET(G2S_SetBG1Offset, reg_G2S_DB_BG1OFS)
GX_DEFINE_BG_OFFSET(G2S_SetBG2Offset, reg_G2S_DB_BG2OFS)
GX_DEFINE_BG_OFFSET(G2S_SetBG3Offset, reg_G2S_DB_BG3OFS)

GX_DEFINE_BG_GET_CONTROL(G2_GetBG0Control, GXBg01Control, reg_G2_BG0CNT)
GX_DEFINE_BG_GET_CONTROL(G2_GetBG1Control, GXBg01Control, reg_G2_BG1CNT)
GX_DEFINE_BG_GET_CONTROL(G2_GetBG2ControlText, GXBg23ControlText, reg_G2_BG2CNT)
GX_DEFINE_BG_GET_CONTROL(G2_GetBG2ControlAffine, GXBg23ControlAffine, reg_G2_BG2CNT)
GX_DEFINE_BG_GET_CONTROL(G2_GetBG2Control256x16Pltt, GXBg23Control256x16Pltt, reg_G2_BG2CNT)
GX_DEFINE_BG_GET_CONTROL(G2_GetBG3ControlText, GXBg23ControlText, reg_G2_BG3CNT)
GX_DEFINE_BG_GET_CONTROL(G2_GetBG3ControlAffine, GXBg23ControlAffine, reg_G2_BG3CNT)
GX_DEFINE_BG_GET_CONTROL(G2_GetBG3Control256x16Pltt, GXBg23Control256x16Pltt, reg_G2_BG3CNT)
GX_DEFINE_BG_GET_CONTROL(G2S_GetBG0Control, GXBg01Control, reg_G2S_DB_BG0CNT)
GX_DEFINE_BG_GET_CONTROL(G2S_GetBG1Control, GXBg01Control, reg_G2S_DB_BG1CNT)
GX_DEFINE_BG_GET_CONTROL(G2S_GetBG2ControlText, GXBg23ControlText, reg_G2S_DB_BG2CNT)
GX_DEFINE_BG_GET_CONTROL(G2S_GetBG2ControlAffine, GXBg23ControlAffine, reg_G2S_DB_BG2CNT)
GX_DEFINE_BG_GET_CONTROL(G2S_GetBG2Control256x16Pltt, GXBg23Control256x16Pltt, reg_G2S_DB_BG2CNT)
GX_DEFINE_BG_GET_CONTROL(G2S_GetBG3ControlText, GXBg23ControlText, reg_G2S_DB_BG3CNT)
GX_DEFINE_BG_GET_CONTROL(G2S_GetBG3ControlAffine, GXBg23ControlAffine, reg_G2S_DB_BG3CNT)
GX_DEFINE_BG_GET_CONTROL(G2S_GetBG3Control256x16Pltt, GXBg23Control256x16Pltt, reg_G2S_DB_BG3CNT)
// clang-format on

// NitroSDK's blending and master brightness functions, which its inlines call with a register's address
void G3X_SetEdgeColorTable(const GXRgb *table);
void G3X_SetFogTable(const u32 *table);
void G2x_SetBlendAlpha_(u32 addr, int plane1, int plane2, int ev1, int ev2);
void G2x_SetBlendBrightness_(u32 addr, int plane, int brightness);
void G2x_SetBlendBrightnessExt_(u32 addr, int plane1, int plane2, int ev1, int ev2, int brightness);
void G2x_ChangeBlendBrightness_(u32 addr, int brightness);
void GXx_SetMasterBrightness_(vu16 *reg, int brightness);
int GXx_GetMasterBrightness_(vu16 *reg);

static inline void GX_SetMasterBrightness(int brightness) {
    GXx_SetMasterBrightness_(REG_MASTER_BRIGHT_ADDR, brightness);
}

static inline void GXS_SetMasterBrightness(int brightness) {
    GXx_SetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR, brightness);
}

// Sets a BG's affine matrix, its center and its offset. NitroSDK's G2x_SetBGyAffine_
void G2x_SetBGyAffine_(u32 addr, const MtxFx22 *mtx, int centerX, int centerY, int x, int y);

static inline void G2_SetBG2Affine(const MtxFx22 *mtx, int centerX, int centerY, int x, int y) {
    G2x_SetBGyAffine_((u32)&reg_G2_BG2PA, mtx, centerX, centerY, x, y);
}

static inline void G2_SetBG3Affine(const MtxFx22 *mtx, int centerX, int centerY, int x, int y) {
    G2x_SetBGyAffine_((u32)&reg_G2_BG3PA, mtx, centerX, centerY, x, y);
}

static inline void G2S_SetBG2Affine(const MtxFx22 *mtx, int centerX, int centerY, int x, int y) {
    G2x_SetBGyAffine_((u32)&reg_G2S_DB_BG2PA, mtx, centerX, centerY, x, y);
}

static inline void G2S_SetBG3Affine(const MtxFx22 *mtx, int centerX, int centerY, int x, int y) {
    G2x_SetBGyAffine_((u32)&reg_G2S_DB_BG3PA, mtx, centerX, centerY, x, y);
}

// NitroSDK's GX_SetGraphicsMode and GXS_SetGraphicsMode
void GX_SetGraphicsMode(int dispMode, int bgMode, int bg0As3D);
void GXS_SetGraphicsMode(int bgMode);
// NitroSDK's GX_LoadBG0Scr to GXS_LoadBG3Scr, GX_LoadBG0Char to GXS_LoadBG3Char, and GX_LoadBGPltt and GXS_LoadBGPltt
void GX_LoadBG0Scr(const void *src, u32 offset, u32 size);
void GX_LoadBG1Scr(const void *src, u32 offset, u32 size);
void GX_LoadBG2Scr(const void *src, u32 offset, u32 size);
void GX_LoadBG3Scr(const void *src, u32 offset, u32 size);
void GXS_LoadBG0Scr(const void *src, u32 offset, u32 size);
void GXS_LoadBG1Scr(const void *src, u32 offset, u32 size);
void GXS_LoadBG2Scr(const void *src, u32 offset, u32 size);
void GXS_LoadBG3Scr(const void *src, u32 offset, u32 size);
void GX_LoadBG0Char(const void *src, u32 offset, u32 size);
void GX_LoadBG1Char(const void *src, u32 offset, u32 size);
void GX_LoadBG2Char(const void *src, u32 offset, u32 size);
void GX_LoadBG3Char(const void *src, u32 offset, u32 size);
void GXS_LoadBG0Char(const void *src, u32 offset, u32 size);
void GXS_LoadBG1Char(const void *src, u32 offset, u32 size);
void GXS_LoadBG2Char(const void *src, u32 offset, u32 size);
void GXS_LoadBG3Char(const void *src, u32 offset, u32 size);
void GX_LoadBGPltt(const void *src, u32 offset, u32 size);
// NitroSDK's G2_GetBG0ScrPtr
void *G2_GetBG0ScrPtr(void);
void GXS_LoadBGPltt(const void *src, u32 offset, u32 size);
// NitroSDK's G2_GetBG0CharPtr to G2S_GetBG3CharPtr
void *G2_GetBG0CharPtr(void);
void *G2_GetBG1CharPtr(void);
void *G2_GetBG2CharPtr(void);
void *G2_GetBG3CharPtr(void);
void *G2S_GetBG0CharPtr(void);
void *G2S_GetBG1CharPtr(void);
void *G2S_GetBG2CharPtr(void);
void *G2S_GetBG3CharPtr(void);

static inline void GXS_DispOn(void) {
    reg_GXS_DB_DISPCNT |= REG_GXS_DB_DISPCNT_MODE_MASK;
}

#define GX_RGB_R_SHIFT 0
#define GX_RGB_R_MASK 0x001f
#define GX_RGB_G_SHIFT 5
#define GX_RGB_G_MASK 0x03e0
#define GX_RGB_B_SHIFT 10
#define GX_RGB_B_MASK 0x7c00
#define reg_G2_BLDALPHA (*(vu16 *)0x04000052)
#define reg_G3X_DISP3DCNT (*(vu16 *)0x04000060)
#define reg_G3X_GXFIFO (*(vu32 *)0x04000400)
#define reg_G3_MTX_STORE (*(vu32 *)0x0400044c)
#define reg_G3_MTX_RESTORE (*(vu32 *)0x04000450)
#define reg_G3_VTX_10 (*(vu32 *)0x04000490)
#define reg_G3_SWAP_BUFFERS (*(vu32 *)0x04000540)
#define reg_G3_VIEWPORT (*(vu32 *)0x04000580)

// The counts of polygons in polygon list RAM and of vertices in vertex RAM
#define reg_G3X_LISTRAM_COUNT (*(vu16 *)0x04000604)

#define reg_G2S_DB_BLDCNT (*(vu16 *)0x04001050)
#define reg_G2S_DB_BLDALPHA (*(vu16 *)0x04001052)
#define REG_G3X_DISP3DCNT_THS_SHIFT 1
#define REG_G3X_DISP3DCNT_THS_MASK 0x0002
#define REG_G3X_DISP3DCNT_ATE_MASK 0x0004
#define REG_G3X_DISP3DCNT_ABE_MASK 0x0008
#define REG_G3X_DISP3DCNT_AAE_MASK 0x0010
#define REG_G3X_DISP3DCNT_EME_MASK 0x0020
#define REG_G3X_DISP3DCNT_RO_MASK 0x1000
#define REG_G3X_DISP3DCNT_GO_MASK 0x2000
#define GX_WNDMASK_W0 0x01
#define GX_WNDMASK_OW 0x04
#define GX_WND_PLANEMASK_BG0 0x01
#define GX_WND_PLANEMASK_BG1 0x02
#define GX_WND_PLANEMASK_BG2 0x04
#define GX_WND_PLANEMASK_BG3 0x08
#define GX_WND_PLANEMASK_OBJ 0x10
#define REG_G2S_DB_WININ_WIN0IN_SHIFT 0
#define REG_G2S_DB_WININ_WIN0IN_MASK 0x003f
#define REG_G2S_DB_WININ_WIN1IN_SHIFT 8
#define REG_G2S_DB_WININ_WIN1IN_MASK 0x3f00
#define REG_G2S_DB_WINOUT_WINOUT_SHIFT 0
#define REG_G2S_DB_WINOUT_WINOUT_MASK 0x003f

// A window's effect enable, above its planes
#define GX_WND_EFFECT_SHIFT 5

#define GX_OAM_ATTR01_CM_MASK 0x00002000

// The shape bits of the first two attributes (the size bits are GX_OAM_ATTR01_SIZE_MASK); a GXOamShape has both
#define GX_OAM_ATTR01_SHAPE_MASK 0x0000c000

// NitroSDK's GXOamShape is an enum of these bits, which MWCC makes an int: its shape bits shift as signed
typedef int GXOamShape;

#define GX_OAM_ATTR2_NAME_MASK 0x03ff
#define GX_OAM_SHAPE_8x8 0x00000000
#define GX_OAM_SHAPE_16x16 0x40000000
#define GX_OAM_SHAPE_32x32 0x80000000
#define GX_OAM_SHAPE_16x8 0x00004000
#define GX_OAM_SHAPE_32x8 0x40004000
#define GX_OAM_SHAPE_32x16 0x80004000
#define GX_OAM_SHAPE_64x32 0xc0004000
#define GX_OAM_SHAPE_8x16 0x00008000
#define GX_OAM_SHAPE_8x32 0x40008000
#define GX_OAM_SHAPE_16x32 0x80008000
#define GX_OAM_SHAPE_32x64 0xc0008000

// The affine, flip and position fields of the first two attributes, and the effects that G2_GetOBJEffect and
// G2_SetOBJEffect read and write
#define GX_OAM_ATTR01_Y_MASK 0x000000ff

#define GX_OAM_ATTR01_RSENABLE_SHIFT 8
#define GX_OAM_ATTR01_RSENABLE_MASK 0x00000300
#define GX_OAM_ATTR01_SHAPE_SHIFT 14
#define GX_OAM_ATTR01_X_MASK 0x01ff0000
#define GX_OAM_ATTR01_RS_MASK 0x3e000000
#define GX_OAM_ATTR01_HF_SHIFT 28
#define GX_OAM_ATTR01_HF_MASK 0x10000000
#define GX_OAM_ATTR01_VF_SHIFT 29
#define GX_OAM_ATTR01_VF_MASK 0x20000000
#define GX_OAM_ATTR01_SIZE_SHIFT 30
#define GX_OAM_ATTR01_SIZE_MASK 0xc0000000

static inline void G2_SetOBJPosition(GXOamAttr *oam, int x, int y) {
    oam->attr01 = ((oam->attr01 & ~(GX_OAM_ATTR01_Y_MASK | GX_OAM_ATTR01_X_MASK)) | (y & GX_OAM_ATTR01_Y_MASK) |
                   ((x & (GX_OAM_ATTR01_X_MASK >> GX_OAM_ATTR01_X_SHIFT)) << GX_OAM_ATTR01_X_SHIFT));
}

static inline void G2_SetOBJShape(GXOamAttr *oam, GXOamShape shape) {
    oam->attr01 = ((oam->attr01 & ~(GX_OAM_ATTR01_SHAPE_MASK | GX_OAM_ATTR01_SIZE_MASK)) | shape);
}

static inline void G2_SetOBJCharName(GXOamAttr *oam, int name) {
    oam->attr2 = (u16)((oam->attr2 & ~GX_OAM_ATTR2_NAME_MASK) | name);
}

static inline void G2_SetOBJPriority(GXOamAttr *oam, int priority) {
    oam->attr2 = (u16)((oam->attr2 & ~GX_OAM_ATTR2_PRIORITY_MASK) | (priority << GX_OAM_ATTR2_PRIORITY_SHIFT));
}

static inline void G2_SetOBJColorMode(GXOamAttr *oam, int color) {
    oam->attr01 = ((oam->attr01 & ~GX_OAM_ATTR01_CM_MASK) | (color << GX_OAM_ATTR01_CM_SHIFT));
}
#define GX_OAM_ATTR2_CPARAM_MASK 0xf000
#define GX_OAM_ATTR01_MODE_MASK 0x00000c00
#define GX_OAM_ATTR01_MOSAIC_MASK 0x00001000

static inline u32 G2_GetOBJColorParam(const GXOamAttr *oam) {
    return (GX_OAM_ATTR2_CPARAM_MASK & oam->attr2) >> GX_OAM_ATTR2_CPARAM_SHIFT;
}

static inline void G2_SetOBJMode(GXOamAttr *oam, int mode, int cParam) {
    oam->attr01 = (mode << GX_OAM_ATTR01_MODE_SHIFT) | (oam->attr01 & ~GX_OAM_ATTR01_MODE_MASK);
    oam->attr2 = (oam->attr2 & ~GX_OAM_ATTR2_CPARAM_MASK) | (cParam << GX_OAM_ATTR2_CPARAM_SHIFT);
}

static inline void G2_OBJMosaic(GXOamAttr *oam, BOOL on) {
    if (on) {
        oam->attr01 |= GX_OAM_ATTR01_MOSAIC_MASK;
    } else {
        oam->attr01 &= ~GX_OAM_ATTR01_MOSAIC_MASK;
    }
}

// The planes that blending takes, which include the backdrop
#define GX_BLEND_PLANEMASK_NONE 0x00

#define GX_BLEND_PLANEMASK_BG1 0x02
#define GX_BLEND_PLANEMASK_OBJ 0x10
#define GX_BLEND_PLANEMASK_BD 0x20

// Shows VRAM D, where the display capture can write, instead of the main engine's output
#define GX_DISPMODE_VRAM_D 0xe

#define GX_BGMODE_3 3
#define GX_SHADING_TOON 0
#define GX_SHADING_HIGHLIGHT 1

typedef enum {
    GX_SORTMODE_AUTO,
    GX_SORTMODE_MANUAL,
} GXSortMode;

typedef enum {
    GX_BUFFERMODE_Z,
    GX_BUFFERMODE_W,
} GXBufferMode;

#define GX_VRAM_BG_NONE 0x000
#define GX_VRAM_BG_16_F GX_VRAM_F
#define GX_VRAM_BG_32_FG (GX_VRAM_F | GX_VRAM_G)
#define GX_VRAM_BG_64_E GX_VRAM_E
#define GX_VRAM_BG_128_B GX_VRAM_B
#define GX_VRAM_BG_128_C GX_VRAM_C
#define GX_VRAM_BG_128_D GX_VRAM_D
#define GX_VRAM_BG_256_AB (GX_VRAM_A | GX_VRAM_B)
#define GX_VRAM_BGEXTPLTT_23_G GX_VRAM_G
#define GX_VRAM_BGEXTPLTT_01_F GX_VRAM_F
#define GX_VRAM_BGEXTPLTT_0123_FG (GX_VRAM_F | GX_VRAM_G)
#define GX_VRAM_OBJ_NONE 0x000
#define GX_VRAM_OBJ_16_F GX_VRAM_F
#define GX_VRAM_OBJ_16_G GX_VRAM_G
#define GX_VRAM_OBJ_32_FG (GX_VRAM_F | GX_VRAM_G)
#define GX_VRAM_OBJ_128_B GX_VRAM_B
#define GX_VRAM_OBJ_256_AB (GX_VRAM_A | GX_VRAM_B)
#define GX_VRAM_OBJEXTPLTT_0_F GX_VRAM_F
#define GX_VRAM_OBJEXTPLTT_0_G GX_VRAM_G
#define GX_VRAM_SUB_OBJ_NONE 0x000
#define GX_VRAM_TEX_NONE 0x000
#define GX_VRAM_TEX_0_D GX_VRAM_D
#define GX_VRAM_TEXPLTT_NONE 0x000
#define GX_VRAM_TEX_0_A GX_VRAM_A
#define GX_VRAM_TEX_01_AB (GX_VRAM_A | GX_VRAM_B)
#define GX_VRAM_TEX_01_BD (GX_VRAM_B | GX_VRAM_D)
#define GX_VRAM_TEX_01_CD (GX_VRAM_C | GX_VRAM_D)
#define GX_VRAM_TEX_012_ABC (GX_VRAM_A | GX_VRAM_B | GX_VRAM_C)
#define GX_VRAM_TEX_0123_ABCD (GX_VRAM_A | GX_VRAM_B | GX_VRAM_C | GX_VRAM_D)
#define GX_VRAM_TEXPLTT_0_G GX_VRAM_G
#define GX_VRAM_TEXPLTT_01_FG (GX_VRAM_F | GX_VRAM_G)
#define GX_VRAM_TEXPLTT_0123_E GX_VRAM_E
#define GX_PACK_VIEWPORT_PARAM(x1, y1, x2, y2) ((u32)(x1) | ((u32)(y1) << 8) | ((u32)(x2) << 16) | ((u32)(y2) << 24))
#define GX_POLYGON_ATTR_MISC_FAR_CLIPPING 0x1000
#define GX_POLYGON_ATTR_MISC_DISP_1DOT 0x2000

// A box for the geometry engine's box test, in model coordinates
typedef struct {
    fx16 x;
    fx16 y;
    fx16 z;
    fx16 width;
    fx16 height;
    fx16 depth;
} GXBoxTestParam;

// Geometry commands, as NitroSystem buffers them
#define G3OP_MTX_STORE 0x13
#define G3OP_MTX_RESTORE 0x14
#define G3OP_TEXCOORD 0x22
#define G3OP_MTX_PUSH 0x11

#define G3OP_MTX_POP 0x12
#define G3OP_MTX_IDENTITY 0x15
#define G3OP_MTX_LOAD_4x3 0x17
#define G3OP_MTX_MULT_3x3 0x1a
#define G3OP_MTX_MULT_4x3 0x19
#define G3OP_MTX_SCALE 0x1b
#define G3OP_MTX_TRANS 0x1c
#define GX_PACK_OP(op0, op1, op2, op3) ((u32)((op0) | ((op1) << 8) | ((op2) << 16) | ((op3) << 24)))
#define G3OP_NOP 0x00
#define G3OP_MTX_MODE 0x10
#define G3OP_MTX_LOAD_4x4 0x16
#define G3OP_MTX_MULT_4x4 0x18
#define G3OP_TEXIMAGE_PARAM 0x2a
#define G3OP_DIF_AMB 0x30
#define G3OP_SPE_EMI 0x31
#define G3OP_LIGHT_VECTOR 0x32
#define G3OP_LIGHT_COLOR 0x33
#define G3OP_VIEWPORT 0x60
#define GX_POLYGON_ATTR_MISC_NONE 0
#define G3OP_POLYGON_ATTR 0x29
#define G3OP_BEGIN 0x40
#define G3OP_END 0x41
#define G3OP_BOX_TEST 0x70
#define REG_G2_WINOUT_OBJWININ_SHIFT 8
#define REG_G2_WINOUT_OBJWININ_MASK 0x3f00

// The OBJ character boundary of 1D mapping, as a shift of 32 bytes
#define REG_GX_DISPCNT_EXOBJ_SHIFT 20

#define REG_GX_DISPCNT_EXOBJ_MASK 0x00700000
#define REG_G2_MOSAIC_BG_V_SHIFT 4
#define GX_DMA_NOT_USE 0xffffffff
// The DMA channel the geometry commands use, set by GX_InitEx: GXi_DmaId (gxcommon.h)
extern u32 GXi_DmaId;
#define GX_DMAID GXi_DmaId
#define MI_DMA_MAX_NUM 3

// NitroSDK's GX_SetGraphicsMode and GXS_SetGraphicsMode
// NitroSDK's GX_BeginLoadTex, GX_LoadTex and GX_EndLoadTex, and the same for palettes
void GX_BeginLoadTex(void);
void GX_LoadTex(const void *src, u32 dest, u32 size);
void GX_EndLoadTex(void);
void GX_BeginLoadTexPltt(void);
void GX_LoadTexPltt(const void *src, u32 dest, u32 size);
void GX_EndLoadTexPltt(void);
// NitroSDK's G3_LoadMtx43 and G3_MultMtx43
void G3_LoadMtx43(const MtxFx43 *mtx);
void G3_MultMtx43(const MtxFx43 *mtx);
// NitroSDK's GX_LoadOBJPltt, GXS_LoadOBJPltt, GX_LoadOBJ and GXS_LoadOBJ
void GX_LoadOBJPltt(const void *src, u32 offset, u32 size);
void GXS_LoadOBJPltt(const void *src, u32 offset, u32 size);
void GX_LoadOBJ(const void *src, u32 offset, u32 size);
void GXS_LoadOBJ(const void *src, u32 offset, u32 size);
// NitroSDK's loads to extended palette VRAM: GX_BeginLoadBGExtPltt, GX_LoadBGExtPltt and GX_EndLoadBGExtPltt, and the
// same for OBJ and for the sub engine
void GX_BeginLoadBGExtPltt(void);
void GX_LoadBGExtPltt(const void *src, u32 offset, u32 size);
void GX_EndLoadBGExtPltt(void);
void GX_BeginLoadOBJExtPltt(void);
void GX_LoadOBJExtPltt(const void *src, u32 offset, u32 size);
void GX_EndLoadOBJExtPltt(void);
void GXS_BeginLoadBGExtPltt(void);
void GXS_LoadBGExtPltt(const void *src, u32 offset, u32 size);
void GXS_EndLoadBGExtPltt(void);
void GXS_BeginLoadOBJExtPltt(void);
void GXS_LoadOBJExtPltt(const void *src, u32 offset, u32 size);
void GXS_EndLoadOBJExtPltt(void);
// The 3D clear-image loads: NitroSDK's GX_BeginLoadClearImage, GX_LoadClearImageColor, GX_LoadClearImageDepth and GX_EndLoadClearImage
void GX_BeginLoadClearImage(void);
void GX_LoadClearImageColor(const void *pSrc, u32 szByte);
void GX_LoadClearImageDepth(const void *pSrc, u32 szByte);
void GX_EndLoadClearImage(void);
void *G2_GetBG1ScrPtr(void);
// NitroSDK's G2_GetBG2ScrPtr and G2_GetBG3ScrPtr
void *G2_GetBG2ScrPtr(void);
void *G2_GetBG3ScrPtr(void);
// NitroSDK's G3X_Init, under swan's name
void G3X_Init(void);
typedef enum {
    GX_FIFOINTR_COND_DISABLE = 0,
    GX_FIFOINTR_COND_UNDERHALF = 1,
    GX_FIFOINTR_COND_EMPTY = 2
} GXFifoIntrCond;
#define REG_G3X_GXSTAT_FI_MASK 0xc0000000
#define REG_G3X_GXSTAT_FI_SHIFT 30
// GXSTAT's command FIFO flags: full, less than half, empty
#define REG_G3X_GXSTAT_F_SHIFT 24
#define REG_G3X_GXSTAT_F_MASK 0x01000000
#define REG_G3X_GXSTAT_H_MASK 0x02000000
#define REG_G3X_GXSTAT_E_MASK 0x04000000
static inline void G3X_SetFifoIntrCond(GXFifoIntrCond cond)
{
    reg_G3X_GXSTAT = ((reg_G3X_GXSTAT & ~REG_G3X_GXSTAT_FI_MASK) | (cond << REG_G3X_GXSTAT_FI_SHIFT));
}
// NitroSDK's GXFifoStat and G3X_GetCommandFifoStatus: how full the geometry command FIFO is
typedef enum {
    GX_FIFOSTAT_EMPTY = 6,
    GX_FIFOSTAT_UNDERHALF = 2,
    GX_FIFOSTAT_OVERHALF = 0,
    GX_FIFOSTAT_FULL = 1
} GXFifoStat;
static inline GXFifoStat G3X_GetCommandFifoStatus(void)
{
    return (GXFifoStat)((reg_G3X_GXSTAT & (REG_G3X_GXSTAT_F_MASK | REG_G3X_GXSTAT_H_MASK | REG_G3X_GXSTAT_E_MASK)) >>
                        REG_G3X_GXSTAT_F_SHIFT);
}
// NitroSDK's G3i_PerspectiveW_, which loads the projection matrix into the geometry engine when isLoad is set
void G3i_PerspectiveW_(fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, BOOL isLoad, MtxFx44 *mtx);
// NitroSDK's G3i_OrthoW_, which loads the projection matrix into the geometry engine when isLoad is set
void G3i_OrthoW_(fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, BOOL isLoad, MtxFx44 *mtx);
void G3_RotX(fx32 sin, fx32 cos);
void G3_RotY(fx32 sin, fx32 cos);
void G3X_InitMtxStack(void);
int G3X_GetBoxTestResult(s32 *in);
// NitroSDK's G3X_SetFog (fogMode a GXFogBlend, fogSlope a GXFogSlope) and G3X_SetClearColor
void G3X_SetFog(BOOL enable, int fogMode, int fogSlope, int fogOffset);
// Sets the color the 3D engine clears its rendered image to
void G3X_SetClearColor(GXRgb rgb, int alpha, int depth, int polygonID, BOOL fog);
void G3X_SetDisp1DotDepth(fx32 depth);

#define GX_BGMODE_1 1

static inline void G2S_BlendNone(void) {
    reg_G2S_DB_BLDCNT = 0;
}

#define GX_VRAM_BGEXTPLTT_0123_E GX_VRAM_E

static inline void G3X_SetShading(int shading) {
    reg_G3X_DISP3DCNT = (u16)((reg_G3X_DISP3DCNT &
                               ~(REG_G3X_DISP3DCNT_THS_MASK | REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK)) |
                              (shading << REG_G3X_DISP3DCNT_THS_SHIFT));
}

static inline void G3X_AntiAlias(BOOL enable) {
    if (enable) {
        reg_G3X_DISP3DCNT = (u16)((reg_G3X_DISP3DCNT & ~(REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK)) |
                                  REG_G3X_DISP3DCNT_AAE_MASK);
    } else {
        reg_G3X_DISP3DCNT &= ~(REG_G3X_DISP3DCNT_AAE_MASK | REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK);
    }
}

static inline void G3X_AlphaTest(BOOL enable, int ref) {
    if (enable) {
        reg_G3X_DISP3DCNT = (u16)((reg_G3X_DISP3DCNT & ~(REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK)) |
                                  REG_G3X_DISP3DCNT_ATE_MASK);
        *(vu16 *)0x04000340 = (u16)ref;
    } else {
        reg_G3X_DISP3DCNT &= ~(REG_G3X_DISP3DCNT_ATE_MASK | REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK);
    }
}

static inline void G3X_AlphaBlend(BOOL enable) {
    if (enable) {
        reg_G3X_DISP3DCNT = (u16)((reg_G3X_DISP3DCNT & ~(REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK)) |
                                  REG_G3X_DISP3DCNT_ABE_MASK);
    } else {
        reg_G3X_DISP3DCNT &= ~(REG_G3X_DISP3DCNT_ABE_MASK | REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK);
    }
}

static inline void G3X_EdgeMarking(BOOL enable) {
    if (enable) {
        reg_G3X_DISP3DCNT = (u16)((reg_G3X_DISP3DCNT & ~(REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK)) |
                                  REG_G3X_DISP3DCNT_EME_MASK);
    } else {
        reg_G3X_DISP3DCNT &= ~(REG_G3X_DISP3DCNT_EME_MASK | REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK);
    }
}

static inline void G3_ViewPort(int x1, int y1, int x2, int y2) {
    reg_G3_VIEWPORT = GX_PACK_VIEWPORT_PARAM(x1, y1, x2, y2);
}

// A vertex with 10-bit coordinates, of 6 fractional bits
static inline void G3_Vtx10(fx16 x, fx16 y, fx16 z) {
    reg_G3_VTX_10 = (u32)((x >> 6) & 0x3ff) | ((u32)((y >> 6) & 0x3ff) << 10) | ((u32)((z >> 6) & 0x3ff) << 20);
}

// NitroSDK's G3_MultMtx44 and G3_MultMtx33 (g3imm.c)
void G3_MultMtx44(const MtxFx44 *mtx);
void G3_MultMtx33(const MtxFx33 *mtx);

// The line the display is drawing
static inline s32 GX_GetVCount(void) {
    return reg_GX_VCOUNT;
}

static inline void G3_StoreMtx(int num) {
    reg_G3_MTX_STORE = (u32)num;
}

static inline void G3_RestoreMtx(int num) {
    reg_G3_MTX_RESTORE = (u32)num;
}

// NitroSDK's GX_HBlankIntr and GX_VBlankIntr, which return whether the interrupt was enabled
s32 GX_HBlankIntr(BOOL enable);
s32 GX_VBlankIntr(BOOL enable);

static inline void G2_SetWndOBJInsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2_WINOUT & ~REG_G2_WINOUT_OBJWININ_MASK) | (wnd << REG_G2_WINOUT_OBJWININ_SHIFT);

    if (effect) {
        tmp |= G2_WND_EFFECT << REG_G2_WINOUT_OBJWININ_SHIFT;
    }
    reg_G2_WINOUT = (u16)tmp;
}

static inline void G2S_SetWndOBJInsidePlane(int wnd, BOOL effect) {
    u32 tmp = (reg_G2S_DB_WINOUT & ~REG_G2_WINOUT_OBJWININ_MASK) | (wnd << REG_G2_WINOUT_OBJWININ_SHIFT);

    if (effect) {
        tmp |= G2_WND_EFFECT << REG_G2_WINOUT_OBJWININ_SHIFT;
    }
    reg_G2S_DB_WINOUT = (u16)tmp;
}

#define GX_VRAM_SUB_BGEXTPLTT_0123_H GX_VRAM_H

static inline void G2_ChangeBlendAlpha(int eva, int evb) {
    reg_G2_BLDALPHA = (u16)(eva | (evb << 8));
}

static inline void G2S_ChangeBlendAlpha(int eva, int evb) {
    reg_G2S_DB_BLDALPHA = (u16)(eva | (evb << 8));
}

GX_DEFINE_BG23_CONTROL_DCBMP(G2_SetBG2ControlDCBmp, reg_G2_BG2CNT)
GX_DEFINE_BG23_CONTROL_DCBMP(G2_SetBG3ControlDCBmp, reg_G2_BG3CNT)

// Whether the display is in a line's H-blank
static inline BOOL GX_IsHBlank(void) {
    return reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK;
}

#define GX_VRAM_SUB_BG_32_H GX_VRAM_H

GX_DEFINE_BG23_CONTROL_DCBMP(G2S_SetBG3ControlDCBmp, reg_G2S_DB_BG3CNT)

// The main screen's OBJ characters
static inline void *G2_GetOBJCharPtr(void) {
    return (void *)HW_OBJ_VRAM;
}

// The sub screen's OBJ characters
static inline void *G2S_GetOBJCharPtr(void) {
    return (void *)HW_DB_OBJ_VRAM;
}

static inline void G2_SetBGMosaicSize(int hSize, int vSize) {
    *(vu8 *)REG_MOSAIC_ADDR = (u8)(hSize | (vSize << REG_G2_MOSAIC_BG_V_SHIFT));
}

GXVRamOBJExtPltt GX_GetBankForOBJExtPltt(void);
GXVRamSubOBJExtPltt GX_GetBankForSubOBJExtPltt(void);

// Swaps the geometry engine's buffers at the next vertical blank, with how it sorts translucent polygons and whether
// it depth tests with Z or W
static inline void G3_SwapBuffers(GXSortMode am, GXBufferMode zw) {
    reg_G3_SWAP_BUFFERS = am | (zw << 1);
}

static inline void G2S_SetBGMosaicSize(int hSize, int vSize) {
    *(vu8 *)REG_DB_MOSAIC_ADDR = (u8)(hSize | (vSize << REG_G2_MOSAIC_BG_V_SHIFT));
}

static inline void G3X_SetFogColor(GXRgb rgb, int alpha) {
    *(vu32 *)0x04000358 = (u32)(rgb | (alpha << 16));
}

// Sets the graphics engines up (gx.c)
void GX_Init(void);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/gx/g2.h, include/nitro/gx/g2_oam.h, include/nitro/gx/g3_util.h, include/nitro/gx/g3x.h, include/nitro/gx/gx.h, include/nitro/gx/gx_capture.h, include/nitro/gx/gx_load.h
static inline void G2_SetBlendAlpha (int plane1, int plane2, int ev1, int ev2)
{
	G2x_SetBlendAlpha_((u32) & reg_G2_BLDCNT, plane1, plane2, ev1, ev2);
}
static inline void G2_SetBlendBrightness (int plane, int brightness)
{
	G2x_SetBlendBrightness_((u32) & reg_G2_BLDCNT, plane, brightness);
}
static inline void G2_SetBlendBrightnessExt (int plane1, int plane2, int ev1, int ev2, int brightness)
{
	G2x_SetBlendBrightnessExt_((u32) & reg_G2_BLDCNT, plane1, plane2, ev1, ev2, brightness);
}
static inline void G2_ChangeBlendBrightness (int brightness)
{
	G2x_ChangeBlendBrightness_((u32) & reg_G2_BLDCNT, brightness);
}
static inline void G2S_SetBlendAlpha (int plane1, int plane2, int ev1, int ev2)
{
	G2x_SetBlendAlpha_((u32) & reg_G2S_DB_BLDCNT, plane1, plane2, ev1, ev2);
}
static inline void G2S_SetBlendBrightness (int plane, int brightness)
{
	G2x_SetBlendBrightness_((u32) & reg_G2S_DB_BLDCNT, plane, brightness);
}
static inline void G2S_SetBlendBrightnessExt (int plane1,
                                              int plane2, int ev1, int ev2, int brightness)
{
	G2x_SetBlendBrightnessExt_((u32) & reg_G2S_DB_BLDCNT, plane1, plane2, ev1, ev2, brightness);
}
static inline void G2S_ChangeBlendBrightness (int brightness)
{
	G2x_ChangeBlendBrightness_((u32) & reg_G2S_DB_BLDCNT, brightness);
}
static inline void G2_GetOBJPosition (const GXOamAttr * oam, u32 * x, u32 * y)
{
    SDK_NULL_ASSERT(oam);
    SDK_NULL_ASSERT(x);
    SDK_NULL_ASSERT(y);
    *x = (oam->attr01 & GX_OAM_ATTR01_X_MASK) >> GX_OAM_ATTR01_X_SHIFT;
    *y = oam->attr01 & GX_OAM_ATTR01_Y_MASK;
}
static inline GXOamShape G2_GetOBJShape (const GXOamAttr * oam)
{
    SDK_NULL_ASSERT(oam);
    return (GXOamShape)(oam->attr01 & (GX_OAM_ATTR01_SHAPE_MASK | GX_OAM_ATTR01_SIZE_MASK));
}
static inline int G2_GetOBJCharName (const GXOamAttr * oam)
{
    SDK_NULL_ASSERT(oam);
    return oam->attr2 & GX_OAM_ATTR2_NAME_MASK;
}
static inline void G2_SetOBJAffine (GXOamAffine * oam, const MtxFx22 * mtx)
{
    SDK_MINMAX_ASSERT(mtx->_00, -128 * FX32_ONE, 128 * FX32_ONE - 1);
    SDK_MINMAX_ASSERT(mtx->_01, -128 * FX32_ONE, 128 * FX32_ONE - 1);
    SDK_MINMAX_ASSERT(mtx->_10, -128 * FX32_ONE, 128 * FX32_ONE - 1);
    SDK_MINMAX_ASSERT(mtx->_11, -128 * FX32_ONE, 128 * FX32_ONE - 1);
    oam->PA = (s16)(mtx->_00 >> 4);
    oam->PB = (s16)(mtx->_01 >> 4);
    oam->PC = (s16)(mtx->_10 >> 4);
    oam->PD = (s16)(mtx->_11 >> 4);
}
static inline void G2_GetOBJAffine (const GXOamAffine * oam, MtxFx22 * mtx)
{
    mtx->_00 = (fx32)(oam->PA << 4);
    mtx->_01 = (fx32)(oam->PB << 4);
    mtx->_10 = (fx32)(oam->PC << 4);
    mtx->_11 = (fx32)(oam->PD << 4);
}
static inline void G3_Perspective (fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, MtxFx44 * mtx)
{
	G3i_PerspectiveW_(fovySin, fovyCos, aspect, n, f, FX32_ONE, TRUE, mtx);
}
static inline void G3_Ortho (fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, MtxFx44 * mtx)
{
	G3i_OrthoW_(t, b, l, r, n, f, FX32_ONE, TRUE, mtx);
}
static inline void G3_PerspectiveW (fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, MtxFx44 * mtx)
{
	G3i_PerspectiveW_(fovySin, fovyCos, aspect, n, f, scaleW, TRUE, mtx);
}
static inline void G3_OrthoW (fx32 t, fx32 b, fx32 l, fx32 r, fx32 n, fx32 f, fx32 scaleW, MtxFx44 * mtx)
{
	G3i_OrthoW_(t, b, l, r, n, f, scaleW, TRUE, mtx);
}
static inline void G3_LookAt (const VecFx32 * camPos, const VecFx32 * camUp, const VecFx32 * target, MtxFx43 * mtx)
{
	G3i_LookAt_(camPos, camUp, target, TRUE, mtx);
}
static inline s32 G3X_IsLineBufferUnderflow (void)
{
	return (reg_G3X_DISP3DCNT & REG_G3X_DISP3DCNT_RO_MASK);
}
static inline s32 G3X_IsListRamOverflow (void)
{
	return (reg_G3X_DISP3DCNT & REG_G3X_DISP3DCNT_GO_MASK);
}
static inline void G3X_ResetLineBufferUnderflow (void)
{
	reg_G3X_DISP3DCNT |= REG_G3X_DISP3DCNT_RO_MASK;
}
static inline void G3X_ResetListRamOverflow (void)
{
	reg_G3X_DISP3DCNT |= REG_G3X_DISP3DCNT_GO_MASK;
}
static inline s32 G3X_GetVtxListRamCount (void)
{
	return reg_G3X_VTXRAM_COUNT;
}
static inline int GX_GetVisiblePlane (void)
{
	return (int)((reg_GX_DISPCNT & REG_GX_DISPCNT_DISPLAY_MASK) >> REG_GX_DISPCNT_DISPLAY_SHIFT);
}
static inline int GXS_GetVisiblePlane (void)
{
	return (int)((reg_GXS_DB_DISPCNT & REG_GX_DISPCNT_DISPLAY_MASK) >> REG_GX_DISPCNT_DISPLAY_SHIFT);
}
static inline void GXS_DispOff (void)
{
	reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_MODE_MASK;
}
static inline s32 GX_IsVBlank (void)
{
	return (reg_GX_DISPSTAT & REG_GX_DISPSTAT_VBLK_MASK);
}
static inline s32 GX_GetVCountEqVal (void)
{
	u16 val = reg_GX_DISPSTAT;
	return ((val >> 8) & 0x00ff) | ((val << 1) & 0x0100);
}
static inline u32 GX_GetDefaultDMA (void)
{
	return GXi_DmaId;
}
    static inline void GX_ResetCapture (void)
    {
        reg_GX_DISPCAPCNT &= ~REG_GX_DISPCAPCNT_E_MASK;
    }
static inline void GX_LoadBG2Bmp (const void * pSrc, u32 offset, u32 szByte)
{
	GX_LoadBG2Scr(pSrc, offset, szByte);
}
static inline void GX_LoadBG3Bmp (const void * pSrc, u32 offset, u32 szByte)
{
	GX_LoadBG3Scr(pSrc, offset, szByte);
}
static inline void GXS_LoadBG2Bmp (const void * pSrc, u32 offset, u32 szByte)
{
	GXS_LoadBG2Scr(pSrc, offset, szByte);
}
static inline void GXS_LoadBG3Bmp (const void * pSrc, u32 offset, u32 szByte)
{
	GXS_LoadBG3Scr(pSrc, offset, szByte);
}

#define reg_GX_VRAMCNT_E (*(vu8 *)0x04000244)
#define reg_GX_VRAMCNT_F (*(vu8 *)0x04000245)
#define reg_GX_VRAMCNT_G (*(vu8 *)0x04000246)

// A to D together, and H and I together
#define reg_GX_VRAMCNT (*(vu32 *)0x04000240)

#define reg_GX_VRAM_HI_CNT (*(vu16 *)0x04000248)

// The state of the VRAM banks that NitroSDK keeps in gGXState (gx_state.c): the banks that each use has taken, and the
// free ones, which are in LCDC mode. A bank is in only one of the fields
typedef struct GXState {
    u16 lcdc;
    u16 bg;
    u16 obj;
    u16 arm7;
    u16 tex;
    u16 texPltt;
    u16 clrImg;
    u16 bgExtPltt;
    u16 objExtPltt;
    u16 subBg;
    u16 subObj;
    u16 subBgExtPltt;
    u16 subObjExtPltt;
} GXState;

// NitroSDK's GX_InitGXState (gx_state.c)
void GX_InitGXState(void); // swan's gfxInitVRAM

// NitroSDK's GX_DispOff (gx.c)
void GX_DispOff(void);

typedef enum {
    GX_OAM_MODE_NORMAL = 0,
    GX_OAM_MODE_XLU = 1,
    GX_OAM_MODE_OBJWND = 2,
    GX_OAM_MODE_BITMAPOBJ = 3,
} GXOamMode;

#define GX_OAM_ATTR0_SHAPE_MASK 0x0000c000

// The geometry engine's matrix stacks, primitives, polygon attributes and textures
typedef enum {
    GX_MTXMODE_PROJECTION = 0,
    GX_MTXMODE_POSITION = 1,
    GX_MTXMODE_POSITION_VECTOR = 2,
    GX_MTXMODE_TEXTURE = 3,
} GXMtxMode;

typedef enum {
    GX_OAM_COLORMODE_16 = 0,
    GX_OAM_COLORMODE_256 = 1,
} GXOamColorMode;

typedef enum {
    GX_OAM_EFFECT_NONE = 0,
    GX_OAM_EFFECT_FLIP_H = 1 << GX_OAM_ATTR01_HF_SHIFT,
    GX_OAM_EFFECT_FLIP_V = 1 << GX_OAM_ATTR01_VF_SHIFT,
    GX_OAM_EFFECT_FLIP_HV = (1 << GX_OAM_ATTR01_HF_SHIFT) | (1 << GX_OAM_ATTR01_VF_SHIFT),
    GX_OAM_EFFECT_AFFINE = 1 << GX_OAM_ATTR01_RSENABLE_SHIFT,
    GX_OAM_EFFECT_NODISPLAY = 2 << GX_OAM_ATTR01_RSENABLE_SHIFT,
    GX_OAM_EFFECT_AFFINE_DOUBLE = 3 << GX_OAM_ATTR01_RSENABLE_SHIFT,
} GXOamEffect;

typedef enum {
    GX_BEGIN_TRIANGLES = 0,
    GX_BEGIN_QUADS = 1,
    GX_BEGIN_TRIANGLE_STRIP = 2,
    GX_BEGIN_QUAD_STRIP = 3,
} GXBegin;

static inline void GXi_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size) {
    if (dmaNo > MI_DMA_MAX_NUM) {
        dmaNo = GX_DMA_NOT_USE;
    }
    if (dmaNo != GX_DMA_NOT_USE) {
        MIi_DmaFill32(dmaNo, dest, data, size, TRUE);
    } else {
        MIi_CpuClear32(data, dest, size);
    }
}



static inline void G2_SetOBJEffect(GXOamAttr *oam, GXOamEffect effect, int rsParam) {
    // The plain store comes first: with constant arguments, MWCC folds the other order into one store
    if (effect != GX_OAM_EFFECT_AFFINE && effect != GX_OAM_EFFECT_AFFINE_DOUBLE) {
        oam->attr01 = (oam->attr01 & ~(GX_OAM_ATTR01_RSENABLE_MASK | GX_OAM_ATTR01_HF_MASK | GX_OAM_ATTR01_VF_MASK |
                                       GX_OAM_ATTR01_RS_MASK)) |
                      effect;
    } else {
        oam->attr01 = (oam->attr01 & ~(GX_OAM_ATTR01_RSENABLE_MASK | GX_OAM_ATTR01_HF_MASK | GX_OAM_ATTR01_VF_MASK |
                                       GX_OAM_ATTR01_RS_MASK)) |
                      effect | (rsParam << GX_OAM_ATTR01_RS_SHIFT);
    }
}

static inline GXOamEffect G2_GetOBJEffect(const GXOamAttr *oam) {
    u32 rs = oam->attr01 & GX_OAM_ATTR01_RSENABLE_MASK;

    if (rs == GX_OAM_EFFECT_AFFINE || rs == GX_OAM_EFFECT_AFFINE_DOUBLE) {
        return rs;
    }
    return rs | (oam->attr01 & (GX_OAM_ATTR01_HF_MASK | GX_OAM_ATTR01_VF_MASK));
}


#endif // BWPORT_NITRO_GX_H
