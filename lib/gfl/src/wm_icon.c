// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/gfl/wm_icon.c.
#include "types.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/wm_icon.h"
#include "nitro/gx.h"

#define WM_ICON_PLTT_OFFSET (14 * 0x20)

// The icon's OBJ, and the one behind it as an OBJ window
#define WM_ICON_ATTR01 0x40000000
#define WM_ICON_ATTR01_WINDOW 0x40000800
#define WM_ICON_ATTR01_HIDDEN 0x40000200
#define WM_ICON_ATTR2 0xe000

typedef struct {
    int level;
    u16 x;
    u16 y;
    u8 unk8[4];
    u8 type;
    u8 screen;
    // Whether the icon may be in the sub engine's OAM
    u8 subOam;
    u8 visible;
    GXOamAttr *oam;
} WmIcon;

typedef struct {
    u16 x;
    u16 y : 14;
    u16 screen : 2;
    WmIcon *icon;
} WmIconSys;

static WmIcon *loadWifiStrengthIcons(u32 unused, u32 heapId, u16 x, u16 y, u32 type, WmIcon *icon, u32 screen,
    u32 subOam, int level);
static void func_02036ca8(WmIcon *icon);
static u32 func_02036d64(WmIcon *icon);
static u32 func_02036dc8(u32 engine, u32 charNo);
static void func_02036e74(WmIcon *icon, int level);
static void func_02036e80(WmIcon *icon);
static void func_02036ec8(WmIcon *icon, BOOL top, HeapID heapId);
static u32 getObjBank(u32 engine);
static void func_02036f24(u32 engine, u32 type, HeapID heapId);
static void copyEmptyOAM(void);
static void func_020371d8(u32 screen);

// Two OBJ off the bottom of the screen
static const u32 data_020a0810[8] = { 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 };

static WmIconSys data_021469c8;

static inline void HideOam(GXOamAttr *oam) {
    oam->attr01 = WM_ICON_ATTR01_HIDDEN;
    oam->attr2 = 0;
}

static WmIcon *loadWifiStrengthIcons(u32 unused, u32 heapId, u16 x, u16 y, u32 type, WmIcon *icon, u32 screen,
    u32 subOam, int level) {
    if (icon == NULL) {
        icon = GFL_HeapAllocate(HEAPID_TAIL((HeapID)heapId), sizeof(WmIcon), FALSE, "wm_icon.c", 100);
    }
    icon->x = x;
    icon->y = y;
    if (data_021469c8.x != 0) {
        icon->x = data_021469c8.x;
        icon->y = data_021469c8.y;
    }
    icon->level = level;
    icon->type = type;
    icon->subOam = subOam;
    icon->screen = screen;
    icon->visible = FALSE;
    icon->oam = (GXOamAttr *)HW_OAM;
    func_02036f24(func_02036d64(icon), icon->type, heapId);
    return icon;
}

static void func_02036ca8(WmIcon *icon) {
    u32 engine = func_02036d64(icon);
    u32 charNo = func_02036dc8(engine, icon->level);
    GXOamAttr *oam;

    if (engine == 1) {
        oam = (GXOamAttr *)HW_OAM;
    } else {
        oam = (GXOamAttr *)HW_DB_OAM;
    }
    if (icon->visible == TRUE) {
        oam[0].attr01 = ((icon->x & 0x1ff) << 16) | (WM_ICON_ATTR01_WINDOW | (icon->y & 0xff));
        oam[0].attr2 = WM_ICON_ATTR2 | charNo;
    } else {
        oam[0].attr01 = WM_ICON_ATTR01_HIDDEN;
        oam[0].attr2 = 0;
    }
    oam[1].attr01 = ((icon->x & 0x1ff) << 16) | (WM_ICON_ATTR01 | (icon->y & 0xff));
    oam[1].attr2 = charNo | WM_ICON_ATTR2;
    if (oam != icon->oam) {
        HideOam(icon->oam);
        HideOam(icon->oam + 1);
        icon->oam = oam;
    }
}

static u32 func_02036d64(WmIcon *icon) {
    switch (icon->screen) {
    case WM_ICON_SCREEN_TOP:
        if (GX_GetDispSelect() == GX_DISP_SELECT_MAIN_SUB) {
            return 1;
        }
        return 2;
    case WM_ICON_SCREEN_BOTTOM:
        if (GX_GetDispSelect() == GX_DISP_SELECT_MAIN_SUB) {
            return 2;
        }
        return 1;
    }
    return 1;
}

// The character name of character charNo in the OBJ VRAM's mapping
static u32 func_02036dc8(u32 engine, u32 charNo) {
    GXOBJVRamModeChar mode;

    if (engine == 1) {
        GX_GetBankForOBJ();
        mode = GX_GetOBJVRamModeChar();
    } else {
        GX_GetBankForSubOBJ();
        mode = GXS_GetOBJVRamModeChar();
    }
    switch (mode) {
    case GX_OBJVRAMMODE_CHAR_1D_32K:
        return charNo << 2;
    case GX_OBJVRAMMODE_CHAR_1D_64K:
        return charNo << 1;
    case GX_OBJVRAMMODE_CHAR_1D_128K:
        return charNo;
    case GX_OBJVRAMMODE_CHAR_1D_256K:
    default:
        // GFL_ASSERT_AT on the unsupported OBJ mapping mode; its text is Shift-JIS
        GFL_DebugAssertFail(__FILE__, 251,
            "0 && \"\x94\xf1\x91\xce\x89\x9e\x82\xccOBJ"
            "\x83\x7d\x83\x62\x83\x73\x83\x93\x83\x4f\x83\x82\x81\x5b\x83\x68\"");
        return charNo;
    }
}

static void func_02036e74(WmIcon *icon, int level) {
    if (level < 4) {
        icon->level = level;
    }
}

static void func_02036e80(WmIcon *icon) {
    GXOamAttr *oam = (GXOamAttr *)HW_OAM;

    oam[0].attr01 = WM_ICON_ATTR01_HIDDEN;
    oam[0].attr2 = 0;
    oam[1].attr01 = WM_ICON_ATTR01_HIDDEN;
    oam[1].attr2 = 0;
    if (icon->subOam) {
        oam = (GXOamAttr *)HW_DB_OAM;
        oam[0].attr01 = WM_ICON_ATTR01_HIDDEN;
        oam[0].attr2 = 0;
        oam[1].attr01 = WM_ICON_ATTR01_HIDDEN;
        oam[1].attr2 = 0;
    }
    GFL_HeapFree(icon);
}

static void func_02036ec8(WmIcon *icon, BOOL top, HeapID heapId) {
    icon->screen = top ? WM_ICON_SCREEN_TOP : WM_ICON_SCREEN_BOTTOM;
    icon->subOam = TRUE;
    func_02036f24(func_02036d64(icon), icon->type, heapId);
}

static u32 getObjBank(u32 engine) {
    if (engine == 1) {
        GX_GetBankForOBJ();
    } else {
        GX_GetBankForSubOBJ();
    }
    return 0;
}

static void func_02036f24(u32 engine, u32 type, HeapID heapId) {
    u32 obj;
    ArcTool *arc;
    WmIconFiles files;
    u32 pltt;

    arc = GFL_ArcSysCreateFileHandle(getWifiIconNarcIdxNum(), HEAPID_TAIL(heapId));
    func_0200f16c(&files);
    if (engine == 1) {
        pltt = 1;
        obj = 0;
    } else {
        pltt = 5;
        obj = 1;
    }
    GFL_G2DIOLoadArcNCLRDefault(arc, files.pltt, pltt, WM_ICON_PLTT_OFFSET, 0x20, heapId);
    loadOBJCharToVram(arc, files.chars[type == 0 ? 1 : 0], obj, getObjBank(engine), 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
}

static void copyEmptyOAM(void) {
    GX_LoadOAM(data_020a0810, 0, sizeof(data_020a0810));
    GXS_LoadOAM(data_020a0810, 0, sizeof(data_020a0810));
}

void func_02037010(u16 x, u16 y, u32 type, u32 heapId) {
    copyEmptyOAM();
    if (data_021469c8.icon != NULL) {
        u32 screen;
        int level;
        u32 subOam;

        subOam = data_021469c8.icon->subOam;
        screen = data_021469c8.icon->screen;
        level = data_021469c8.icon->level;
        func_020370c8();
        data_021469c8.icon = loadWifiStrengthIcons(0, heapId, x, y, type, data_021469c8.icon, screen, subOam, level);
    } else {
        data_021469c8.icon =
            loadWifiStrengthIcons(0, heapId, x, y, type, data_021469c8.icon, data_021469c8.screen, FALSE, 3);
    }
}

void func_020370c8(void) {
    copyEmptyOAM();
    if (data_021469c8.icon != NULL) {
        func_02036e80(data_021469c8.icon);
        data_021469c8.icon = NULL;
    }
}

void func_020370f4(int level) {
    if (data_021469c8.icon != NULL) {
        func_02036e74(data_021469c8.icon, level);
    }
}

void func_02037118(BOOL top, HeapID heapId) {
    func_020371d8(top ? WM_ICON_SCREEN_TOP : WM_ICON_SCREEN_BOTTOM);
    if (data_021469c8.icon != NULL) {
        func_02036ec8(data_021469c8.icon, top, heapId);
    }
}

void func_02037154(void) {
    if (data_021469c8.icon != NULL) {
        func_02036ca8(data_021469c8.icon);
    }
}

void func_02037174(void) {
    if (data_021469c8.icon != NULL) {
        data_021469c8.icon->visible = TRUE;
    }
}

void func_02037190(void) {
    if (data_021469c8.icon != NULL) {
        data_021469c8.icon->visible = FALSE;
    }
}

void func_020371ac(u16 x, u16 y) {
    data_021469c8.x = x;
    data_021469c8.y = y;
}

static void func_020371d8(u32 screen) {
    data_021469c8.screen = screen;
}

u32 func_02037200(void) {
    if (data_021469c8.icon != NULL) {
        return func_02036d64(data_021469c8.icon);
    }
    return 1;
}
