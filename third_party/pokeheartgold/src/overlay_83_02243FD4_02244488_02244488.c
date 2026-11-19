#include "global.h"

#include "battle_castle_app_sprite_manager.h"
#include "bg_window.h"
#include "palette.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_83_battle_castle_self_app.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "list_menu.h"
#include "overlay_manager.h"
#include "battle_castle_save.h"
#include "save_frontier.h"
#include "message_printer.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

struct BgConfig;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct PaletteData;

struct ListMenuItem;

struct Window;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D ListMenu2D;

void ov83_02244488(void *data);

void ov83_02244488(void *data) {
    u8 *app = data;

    if (*(PaletteData **)(app + 0x2b0) != NULL) {
        PaletteData_PushTransparentBuffers(*(PaletteData **)(app + 0x2b0));
    }

    DoScheduledBgGpuUpdates(*(BgConfig **)(app + 0x4c));
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
