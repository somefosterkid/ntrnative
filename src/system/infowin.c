// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/infowin.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/infowin.h"
#include "gfl/rtc_cache.h"

// The information bar along the top of a screen: the OS_GetTick, the wireless and Wi-Fi icons, the signal strength and the
// battery. Update reads their state and sets a flag for each part that changed, and a VBlank task draws the flagged
// parts into the BG's screen. The names are ours

// The parts to draw, which the VBlank task clears by subtracting them once drawn. 0x2 is set with the rest but
// draws nothing
#define INFOWIN_DRAW_CLOCK 0x1
#define INFOWIN_DRAW_WIRELESS 0x4
#define INFOWIN_DRAW_WIFI 0x8
#define INFOWIN_DRAW_SIGNAL 0x10
#define INFOWIN_DRAW_BATTERY 0x20
#define INFOWIN_DRAW_ALL 0x3f
// The state the icons show
#define INFOWIN_WIRELESS_ON 0x200
#define INFOWIN_SIGNAL_ON 0x400
#define INFOWIN_SIGNAL_BLINK 0x800
#define INFOWIN_BEACON_FOUND 0x1000

// The tiles the bar takes at the end of the BG's characters
#define INFOWIN_CHAR_SIZE 0x1000

typedef struct {
    u32 charBase;
    u16 flags;
    u8 bg;
    u8 palette;
    // The frame of the signal icon's blinking, 5 cycles of 16 frames, the last of them dark
    u8 blinkFrame;
    WifiList *wifiList;
    // Whether the bar shows other players' beacons in the signal icon
    BOOL showBeacons;
    // Whether the colon of the OS_GetTick is lit, which it is every other second
    BOOL colon;
    BOOL pm;
    u8 minute;
    u8 hour;
    u8 batteryTimer;
    u8 batteryLevel;
    // Which of the Wi-Fi icon's tiles and colors it shows
    int wifiIcon;
    TCB *vblankTask;
} InfoWin;

static void func_02024dfc(TCB *tcb, void *data);
static void func_02025154(u16 tile, u8 palette, u8 x, u8 y, u8 width, u8 height);
static void func_020251d8(u16 color, const u16 *colors, u16 count);
static void func_020251fc(u8 bg, u8 palette, HeapID heapId);
static void func_020252a8(void);

// pokebw2's sBatteryColor
static const u16 data_0209f830[] = { 0x7fff, 0x0020 };
// pokebw2's sWirelessTiles
static const u16 data_0209f834[] = { 0x48, 0x48 };
// pokebw2's sWirelessColors
static const u16 data_0209f840[][2] = {
    { 0x18c6, 0x2529 },
    { 0x4e73, 0x7fff },
};
// pokebw2's sSignalColors
static const u16 data_0209f838[][2] = {
    { 0x18c6, 0x2529 },
    { 0x4e73, 0x7fff },
};
// pokebw2's sWifiTiles
static const u16 data_0209f848[] = { 0x28, 0x28, 0x28, 0x28, 0x28 };
// pokebw2's sWifiColors
static const u16 data_0209f852[][2] = {
    { 0x18c6, 0x2529 }, { 0x4e73, 0x7fff }, { 0x0850, 0x109f }, { 0x0170, 0x02df }, { 0x4100, 0x7e00 },
};

// pokebw2's sInfoWin
static InfoWin *data_0214693c;

static inline BOOL InfoWin_IsFlagSet(u16 flag) {
    return (data_0214693c->flags & flag) ? TRUE : FALSE;
}

static inline const u16 *InfoWin_GetSignalColors(u8 on) {
    return data_0209f838[on];
}

// pokebw2's InfoWin_Init
void func_02024a20(u8 bg, u8 palette, GameData *gameData, HeapID heapId) {
    data_0214693c = GFL_HeapAllocate(heapId, sizeof(InfoWin), FALSE, "infowin.c", 259);
    MI_CpuFill8(data_0214693c, 0, sizeof(InfoWin));
    data_0214693c->bg = bg;
    data_0214693c->palette = palette;
    func_020251fc(bg, palette, heapId);
    data_0214693c->vblankTask = GFL_VBlankTCBAdd(func_02024dfc, data_0214693c, 1);
    data_0214693c->wifiIcon = 0;
    data_0214693c->batteryLevel = func_0203568c();
    if (gameData != NULL) {
        data_0214693c->wifiList = func_0201283c(gameData);
        data_0214693c->showBeacons = func_02012bb0(gameData);
    } else {
        data_0214693c->wifiList = NULL;
        data_0214693c->showBeacons = FALSE;
    }
    data_0214693c->blinkFrame = 0;
    func_02024abc();
    data_0214693c->flags = INFOWIN_DRAW_ALL;
}

// pokebw2's InfoWin_Update
void func_02024abc(void) {
    BOOL connected = FALSE;
    u8 connectNum = 0;
    GFLNetInitData *netInit = NULL;
    u32 status;
    int beacon;
    u8 batteryLevel;

    if (data_0214693c == NULL) {
        return;
    }
    func_020252a8();
    data_0214693c->batteryTimer++;
    if (data_0214693c->batteryTimer > 60) {
        batteryLevel = func_0203568c();
        if (data_0214693c->batteryLevel != batteryLevel) {
            data_0214693c->batteryLevel = batteryLevel;
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_BATTERY;
        }
        data_0214693c->batteryTimer = 0;
    }
    if (func_0203cd98() == TRUE) {
        netInit = func_0203d798();
        connectNum = func_0203d1b4();
        if (func_0203d29c() == TRUE) {
            connected = TRUE;
        }
    }

    if (connectNum < 2 && connected == FALSE) {
        status = func_0201058c(data_0214693c->wifiList);
        beacon = 0;
        if (data_0214693c->showBeacons == TRUE && func_02026ca0(&beacon) != 30) {
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_SIGNAL;
            data_0214693c->flags = data_0214693c->flags | INFOWIN_BEACON_FOUND;
        } else {
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_SIGNAL;
            data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_BEACON_FOUND);
        }
        if (!(status & 0x2) && InfoWin_IsFlagSet(INFOWIN_WIRELESS_ON) == TRUE) {
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_WIRELESS;
            data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_WIRELESS_ON);
        }
        if (status & 0x3c) {
            if (status & 0xc) {
                data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_SIGNAL;
                data_0214693c->flags = data_0214693c->flags | INFOWIN_SIGNAL_ON;
                data_0214693c->flags = data_0214693c->flags | INFOWIN_SIGNAL_BLINK;
            } else if (InfoWin_IsFlagSet(INFOWIN_SIGNAL_ON) == FALSE) {
                data_0214693c->blinkFrame = 0;
                data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_SIGNAL_BLINK);
                data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_SIGNAL;
                data_0214693c->flags = data_0214693c->flags | INFOWIN_SIGNAL_ON;
            }
        } else if (InfoWin_IsFlagSet(INFOWIN_SIGNAL_ON) == TRUE) {
            data_0214693c->blinkFrame = 0;
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_SIGNAL;
            data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_SIGNAL_ON);
            data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_SIGNAL_BLINK);
        }
        if (status & 0x3c0) {
            if (data_0214693c->wifiIcon == 0) {
                data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_WIFI;
                data_0214693c->wifiIcon = 1;
            }
        } else if (data_0214693c->wifiIcon != 0) {
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_WIFI;
            data_0214693c->wifiIcon = 0;
        }
    } else if (netInit != NULL) {
        if (netInit->bNetType == 3) {
            if (InfoWin_IsFlagSet(INFOWIN_WIRELESS_ON) == FALSE) {
                data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_WIRELESS;
                data_0214693c->flags = data_0214693c->flags | INFOWIN_WIRELESS_ON;
            }
        } else if (InfoWin_IsFlagSet(INFOWIN_WIRELESS_ON) == TRUE) {
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_WIRELESS;
            data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_WIRELESS_ON);
        }
        if (netInit->bNetType == 0 || netInit->bNetType == 4 || netInit->bNetType == 5) {
            if (InfoWin_IsFlagSet(INFOWIN_SIGNAL_ON) == FALSE) {
                data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_SIGNAL;
                data_0214693c->flags = data_0214693c->flags | INFOWIN_SIGNAL_ON;
                data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_SIGNAL_BLINK);
                data_0214693c->blinkFrame = 0;
            }
            if (netInit->gameCommandBase == 0x2) {
                data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_SIGNAL;
                data_0214693c->flags = data_0214693c->flags | INFOWIN_SIGNAL_ON;
                data_0214693c->flags = data_0214693c->flags | INFOWIN_SIGNAL_BLINK;
            }
        } else if (InfoWin_IsFlagSet(INFOWIN_SIGNAL_ON) == TRUE) {
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_SIGNAL;
            data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_SIGNAL_ON);
            data_0214693c->flags = data_0214693c->flags & (0xffff ^ INFOWIN_SIGNAL_BLINK);
            data_0214693c->blinkFrame = 0;
        }
        if (netInit->bNetType == GFL_NET_TYPE_WIFI || netInit->bNetType == GFL_NET_TYPE_WIFI_LOBBY ||
            netInit->bNetType == GFL_NET_TYPE_WIFI_GTS) {
            if (data_0214693c->wifiIcon == 0) {
                data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_WIFI;
                data_0214693c->wifiIcon = 1;
            }
        } else if (data_0214693c->wifiIcon != 0) {
            data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_WIFI;
            data_0214693c->wifiIcon = 0;
        }
    }
}

// pokebw2's InfoWin_Exit
void func_02024dcc(void) {
    if (data_0214693c != NULL) {
        GFL_TCBRemove(data_0214693c->vblankTask);
        GFL_BGSysFreeCharMemory(data_0214693c->bg, data_0214693c->charBase, INFOWIN_CHAR_SIZE);
        GFL_HeapFree(data_0214693c);
        data_0214693c = NULL;
    }
}

// pokebw2's InfoWin_VBlankTask
static void func_02024dfc(TCB *tcb, void *data) {
    BOOL redraw = FALSE;
    BOOL on;
    u8 lit;
    u8 frame, cycle, brightness, level;

    if (data_0214693c == NULL) {
        return;
    }
    if (data_0214693c->flags & INFOWIN_DRAW_CLOCK) {
        u16 OS_GetTick[10];
        if (data_0214693c->hour < 10) {
            OS_GetTick[0] = data_0214693c->charBase + ((data_0214693c->palette << 12) + 0xf);
            OS_GetTick[5] = data_0214693c->charBase + ((data_0214693c->palette << 12) + 0x1f);
        } else {
            OS_GetTick[0] = data_0214693c->charBase + ((data_0214693c->palette << 12) + (data_0214693c->hour / 10 + 1));
            OS_GetTick[5] = OS_GetTick[0] + 0x10;
        }
        OS_GetTick[1] = data_0214693c->charBase + ((data_0214693c->palette << 12) + (data_0214693c->hour % 10 + 1));
        OS_GetTick[2] = data_0214693c->charBase + ((data_0214693c->palette << 12) + (data_0214693c->colon ? 0xb : 0xf));
        OS_GetTick[3] = data_0214693c->charBase + ((data_0214693c->palette << 12) + (data_0214693c->minute / 10 + 1));
        OS_GetTick[4] = data_0214693c->charBase + ((data_0214693c->palette << 12) + (data_0214693c->minute % 10 + 1));
        OS_GetTick[6] = OS_GetTick[1] + 0x10;
        OS_GetTick[7] = data_0214693c->charBase + ((data_0214693c->palette << 12) + (data_0214693c->colon ? 0x1b : 0x1f));
        OS_GetTick[8] = OS_GetTick[3] + 0x10;
        OS_GetTick[9] = OS_GetTick[4] + 0x10;
        GFL_BGSysLoadScrArea(data_0214693c->bg, 1, 0, 5, 2, OS_GetTick, 0, 0, 5, 2);
        redraw = TRUE;
        data_0214693c->flags -= INFOWIN_DRAW_CLOCK;
    }
    if (data_0214693c->flags & INFOWIN_DRAW_WIRELESS) {
        lit = InfoWin_IsFlagSet(INFOWIN_WIRELESS_ON);
        func_02025154(data_0209f834[lit], data_0214693c->palette, 9, 0, 3, 2);
        func_020251d8(6, data_0209f840[lit], 2);
        redraw = TRUE;
        data_0214693c->flags -= INFOWIN_DRAW_WIRELESS;
    }
    if (data_0214693c->flags & INFOWIN_DRAW_WIFI) {
        func_02025154(data_0209f848[data_0214693c->wifiIcon], data_0214693c->palette, 13, 0, 6, 2);
        func_020251d8(8, data_0209f852[data_0214693c->wifiIcon], 2);
        redraw = TRUE;
        data_0214693c->flags -= INFOWIN_DRAW_WIFI;
    }
    if (data_0214693c->flags & INFOWIN_DRAW_SIGNAL) {
        u16 blinkColors[2];
        func_02025154(0x20, data_0214693c->palette, 19, 0, 7, 2);
        on = FALSE;

        if (data_0214693c->flags & INFOWIN_SIGNAL_BLINK) {
            if (GCTX_HIDGetUpdateRate() == 30) {
                data_0214693c->blinkFrame += 2;
            } else {
                data_0214693c->blinkFrame += 1;
            }
            if (data_0214693c->blinkFrame >= 80) {
                data_0214693c->blinkFrame = 0;
            }
            frame = data_0214693c->blinkFrame % 16;
            cycle = data_0214693c->blinkFrame / 16;
            if (cycle < 4) {
                brightness = 8 - MATH_ABS(frame - 8);
                level = brightness * 13 / 8 + 6;
                blinkColors[0] = GX_RGB(level, level, level);
                level = brightness * 22 / 8 + 9;
                blinkColors[1] = GX_RGB(level, level, level);
                func_020251d8(10, blinkColors, 2);
            } else {
                func_020251d8(10, data_0209f838[0], 2);
            }
        } else {
            if (data_0214693c->flags & (INFOWIN_SIGNAL_ON | INFOWIN_BEACON_FOUND)) {
                on = TRUE;
            }
            func_020251d8(10, InfoWin_GetSignalColors(on), 2);
        }
        redraw = TRUE;
        data_0214693c->flags -= INFOWIN_DRAW_SIGNAL;
    }
    if (data_0214693c->flags & INFOWIN_DRAW_BATTERY) {
        u16 batteryTiles[6] = { 0x40, 0x41, 0x42, 0x43, 0x44, 0x45 };
        u8 dsiBatteryLevels[6] = { 0, 0, 1, 1, 2, 3 };
        u16 battery[4];

        if (OS_IsRunOnTwl()) {
            level = dsiBatteryLevels[data_0214693c->batteryLevel];
        } else if (data_0214693c->batteryLevel == 5) {
            level = 5;
        } else {
            level = 4;
        }
        battery[0] = data_0214693c->charBase + ((data_0214693c->palette << 12) + batteryTiles[level]);
        battery[2] = battery[0] + 0x10;
        battery[1] = battery[0] + 0x400;
        battery[3] = battery[2] + 0x400;
        GFL_BGSysLoadScrArea(data_0214693c->bg, 28, 0, 2, 2, battery, 0, 0, 2, 2);
        func_020251d8(13, data_0209f830, 1);
        redraw = TRUE;
        data_0214693c->flags -= INFOWIN_DRAW_BATTERY;
    }
    if (redraw == TRUE) {
        GFL_BGSysLoadScr(data_0214693c->bg);
    }
}

// pokebw2's InfoWin_LoadScreen
static void func_02025154(u16 tile, u8 palette, u8 x, u8 y, u8 width, u8 height) {
    u16 screen[14];
    u8 i, j;

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            screen[i + j * width] = data_0214693c->charBase + (tile + (palette << 12) + i + j * 16);
        }
    }
    GFL_BGSysLoadScrArea(data_0214693c->bg, x, y, width, height, screen, 0, 0, width, height);
}

// pokebw2's InfoWin_LoadPalette
static void func_020251d8(u16 color, const u16 *colors, u16 count) {
    u16 size = count * sizeof(u16);
    u16 offset = (color + data_0214693c->palette * 16) * sizeof(u16);

    GFL_BGSysUploadStdPalette(data_0214693c->bg, colors, size, offset);
}

// pokebw2's InfoWin_LoadGraphics
static void func_020251fc(u8 bg, u8 palette, HeapID heapId) {
    u32 paletteType = PALTYPE_MAIN_BG;
    ArcTool *arc;

    // BGs 4 to 7 are the sub engine's
    if (bg > 3) {
        paletteType = PALTYPE_SUB_BG;
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_INFOWIN, heapId);
    data_0214693c->charBase = GFL_BGSysAllocChar(bg, INFOWIN_CHAR_SIZE, TRUE);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, paletteType, palette * 32, 32, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 1, bg, data_0214693c->charBase, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysFillScrArea(bg, data_0214693c->charBase + 0xf, 0, 0, 32, 1, palette);
    GFL_BGSysFillScrArea(bg, data_0214693c->charBase + 0x1f, 0, 1, 32, 1, palette);
    GFL_BGSysLoadScr(bg);
}

// pokebw2's InfoWin_UpdateClock
static void func_020252a8(void) {
    RTCTime time;
    BOOL colon;

    RTC_GetCachedTime(&time);
    colon = time.second & 1;
    if (time.hour != data_0214693c->hour) {
        data_0214693c->hour = time.hour;
        if (data_0214693c->hour >= 12) {
            data_0214693c->pm = TRUE;
        } else {
            data_0214693c->pm = FALSE;
        }
        data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_CLOCK;
    }
    if (time.minute != data_0214693c->minute) {
        data_0214693c->minute = time.minute;
        data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_CLOCK;
    }
    if (colon != data_0214693c->colon) {
        data_0214693c->colon = colon;
        data_0214693c->flags = data_0214693c->flags | INFOWIN_DRAW_CLOCK;
    }
}
