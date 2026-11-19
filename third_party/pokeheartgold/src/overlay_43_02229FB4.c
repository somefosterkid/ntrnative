#include "global.h"
#include "sprite_resources_header.h"

#include "brightness.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "menu_input_state.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "render_text.h"
#include "render_window.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "vram_transfer_manager.h"
#include "message_format.h"
#include "msgdata.h"
#include "sprite.h"
#include "list_menu_items.h"
#include "yes_no_prompt.h"
#include "dex_mon_measures.h"
#include "overlay_43.h"
#include "naming_screen.h"
#include "touchscreen_list_menu.h"
#include "overlay_43_palpad.h"
#include "code_entry_args.h"

struct BgConfig;

struct CharSprite;

struct CharSpriteManager;

struct CodeEntryArgs;

struct G2dRenderer;

struct GF_2DGfxResMan;

struct ListMenuItem;

struct MessageFormat;

struct MsgData;

struct NARC;

struct NamingScreenArgs;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PalPadArgs;

struct PalPadDetails;

struct PalPadDetailsScreen;

struct PalPadDetailsSlot;

struct PalPadFriendCode;

struct PalPadFriendList;

struct PalPadGfx;

struct PalPadMenu;

struct PalPadNaming;

struct PalPadPanelLayout;

struct PalPadRoster;

struct PalPadRosterPage;

struct PalPadScreens;

struct PalPadShared;

struct PalPadWork;

struct PokedexData;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourcesHeader;

struct String;

struct TouchscreenListMenu;

struct TouchscreenListMenuHeader;

struct TouchscreenListMenuSpawner;

struct TouchscreenListMenuTemplate;

struct Window;

struct YesNoPrompt;

PokedexData *PokedexData_Create(enum HeapID heapID);

void PokedexData_Delete(PokedexData *pokedexData);

void PokedexData_LoadAll(PokedexData *pokedexData, int playerGender, enum HeapID heapID);

void PokedexData_UnloadAll(PokedexData *pokedexData);

u32 PokedexData_GetHeight(PokedexData *pokedexData, int species);

u32 PokedexData_GetWeight(PokedexData *pokedexData, int species);

typedef struct CharSprite CharSprite;

typedef struct CharSpriteManager CharSpriteManager;

BOOL ov43_0222AE64(PalPadMenu *menu, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

BOOL ov43_0222B5D0(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

BOOL ov43_0222CBB4(PalPadNaming *naming, PalPadWork *work, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

BOOL ov43_0222CD98(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);

BOOL ov43_0222D15C(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

enum {
    PALPAD_STATE_RUN_SCREEN,
    PALPAD_STATE_CHANGE_SCREEN,
};

void ov43_0222A500(PalPadGfx *gfx);

void PalPad_SetupScreens(PalPadWork *work, enum HeapID heapId);

void PalPadShared_Init(PalPadShared *shared, PalPadArgs *args, enum HeapID heapId);

void PalPadGfx_Init(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapId);

void PalPadGfx_InitBgs(PalPadGfx *gfx, enum HeapID heapId);

void PalPadGfx_InitSpriteSystem(PalPadGfx *gfx, enum HeapID heapId);

void PalPadGfx_InitMsg(PalPadGfx *gfx, enum HeapID heapId);

void PalPadGfx_InitWindows(PalPadGfx *gfx, PalPadShared *shared, enum HeapID heapId);

void PalPadGfx_LoadScreens(PalPadGfx *gfx, enum HeapID heapId);

void PalPadGfx_CreateSprites(PalPadGfx *gfx, enum HeapID heapId);

BOOL PalPad_Main(OverlayManager *man, int *state);

BOOL PalPad_Main(OverlayManager *man, int *state) {
    PalPadWork *work = OverlayManager_GetData(man);
    BOOL result;

    switch (*state) {
    case PALPAD_STATE_RUN_SCREEN:
        switch (work->screen) {
        case PALPAD_SCREEN_MENU:
            result = ov43_0222AE64(&work->menu, &work->shared, &work->gfx, HEAP_ID_51);
            break;
        case PALPAD_SCREEN_ROSTER:
            result = ov43_0222B5D0(&work->roster, &work->shared, &work->gfx, HEAP_ID_51);
            break;
        case PALPAD_SCREEN_NAMING:
            result = ov43_0222CBB4(&work->naming, work, &work->shared, &work->gfx, HEAP_ID_51);
            break;
        case PALPAD_SCREEN_FRIEND_CODE:
            result = ov43_0222CD98(&work->friendCode, &work->shared, &work->gfx);
            break;
        case PALPAD_SCREEN_DETAILS:
            result = ov43_0222D15C(&work->details, &work->shared, &work->gfx, HEAP_ID_51);
            break;
        case PALPAD_SCREEN_EXIT:
            return TRUE;
        }
        if (result == 1) {
            work->screen = work->shared.nextScreen;
            work->shared.subState = work->shared.nextSubState;
            work->screenChangeDelay = 4;
            *state = PALPAD_STATE_CHANGE_SCREEN;
        }
        break;
    case PALPAD_STATE_CHANGE_SCREEN:
        work->screenChangeDelay--;
        if (work->screenChangeDelay <= 0) {
            *state = PALPAD_STATE_RUN_SCREEN;
        }
        break;
    }
    ov43_0222A500(&work->gfx);
    return FALSE;
}
