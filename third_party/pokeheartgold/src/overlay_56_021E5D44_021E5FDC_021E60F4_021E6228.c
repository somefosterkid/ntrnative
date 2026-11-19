#include "global.h"
#include "overlay_56_mail_viewer_app.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "msgdata.h"

struct BgConfig;

struct ManagedSprite;

struct Menu;

struct MsgData;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct Window;

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

enum DSScreen {
    DS_SCREEN_MAIN = 0,
    DS_SCREEN_SUB,
    DS_SCREEN_MAX,
};

void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void GfGfx_DisableEngineBPlanes(void);

typedef struct Menu Menu;

typedef void (*Callback)(void *);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void ResetVisibleHardwareWindows(enum DSScreen screen);

void SetMasterBrightnessNeutral(enum DSScreen screen);

void sub_0200FBF4(enum DSScreen screen, u16 color);

enum MailViewerState {
    MAIL_VIEWER_STATE_INIT = 0,
    MAIL_VIEWER_STATE_INIT_GRAPHICS,
    MAIL_VIEWER_STATE_FADE_IN,
    MAIL_VIEWER_STATE_MAIN,
    MAIL_VIEWER_STATE_FADE_OUT,
    MAIL_VIEWER_STATE_EXIT,
};

typedef BOOL (*MailViewerModeFunc)(MailViewerApp *mailViewerApp);

BOOL ov56_021E647C(MailViewerApp *mailViewerApp);

void ov56_021E63C0(void *data);

BOOL MailViewer_HandleInput_Read(MailViewerApp *unused);
BOOL ov56_021E5DA4(MailViewerApp *unused);

BOOL ov56_021E5DDC(MailViewerApp *mailViewerApp);
BOOL ov56_021E5FB4(MailViewerApp *mailViewerApp);

BOOL ov56_021E5FDC(MailViewerApp *mailViewerApp);

BOOL ov56_021E614C(MailViewerApp *mailViewerApp);

s32 ov56_021E64C8(void *arg0);

BOOL ov56_021E6228(MailViewerApp *mailViewerApp);

BOOL ov56_021E6228(MailViewerApp *mailViewerApp) {
    switch (mailViewerApp->state) {
    case MAIL_VIEWER_STATE_INIT:
        Main_SetVBlankIntrCB(0L, 0L);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();

        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);

        sub_0200FBF4(DS_SCREEN_MAIN, (((0) << 10) | ((0) << 5) | (0)));
        sub_0200FBF4(DS_SCREEN_SUB, (((0) << 10) | ((0) << 5) | (0)));
        ResetVisibleHardwareWindows(DS_SCREEN_MAIN);
        ResetVisibleHardwareWindows(DS_SCREEN_SUB);
        break;
    case MAIL_VIEWER_STATE_INIT_GRAPHICS:
        if (!ov56_021E647C(mailViewerApp)) {
            return 0;
        }

        Main_SetVBlankIntrCB(ov56_021E63C0, mailViewerApp);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
        SetMasterBrightnessNeutral(DS_SCREEN_MAIN);

        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3, 28, 4);

        PaletteData_BeginPaletteFade(mailViewerApp->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_MAIN_OBJ), 0xFFFF, -1, 16, 0, 0x0);
        break;
    case MAIL_VIEWER_STATE_FADE_IN:
        if (mailViewerApp->spriteMan != 0L) {
            SpriteSystem_DrawSprites(mailViewerApp->spriteMan);
        }

        if (PaletteData_GetSelectedBuffersBitmask(mailViewerApp->paletteData)) {
            return 0;
        }
        break;
    case MAIL_VIEWER_STATE_MAIN:
        static const MailViewerModeFunc funcs[] = {
            [MAIL_VIEWER_MODE_READ] = ov56_021E5DA4,
            [MAIL_VIEWER_MODE_WRITE] = ov56_021E5FB4,
            [MAIL_VIEWER_MODE_CONFIRM_EMPTY] = ov56_021E5FDC,
            [MAIL_VIEWER_MODE_CANCEL] = ov56_021E614C,
        };

        if (mailViewerApp->spriteMan != 0L) {
            SpriteSystem_DrawSprites(mailViewerApp->spriteMan);
        }

        if (!funcs[mailViewerApp->mode](mailViewerApp)) {
            return 0;
        }

        PaletteData_BeginPaletteFade(mailViewerApp->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_MAIN_OBJ), 0xFFFF, -1, 0, 16, 0x0);
        break;
    case MAIL_VIEWER_STATE_FADE_OUT:
        if (PaletteData_GetSelectedBuffersBitmask(mailViewerApp->paletteData)) {
            if (mailViewerApp->spriteMan != 0L) {
                SpriteSystem_DrawSprites(mailViewerApp->spriteMan);
            }

            return 0;
        }

        sub_0200FBF4(DS_SCREEN_MAIN, (((0) << 10) | ((0) << 5) | (0)));
        sub_0200FBF4(DS_SCREEN_SUB, (((0) << 10) | ((0) << 5) | (0)));
        Main_SetVBlankIntrCB(0L, 0L);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();

        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        break;
    case MAIL_VIEWER_STATE_EXIT:
        if (ov56_021E64C8(mailViewerApp)) {
            return 1;
        }

        return 0;
    }

    mailViewerApp->state++;
    return 0;
}
