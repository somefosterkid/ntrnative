#include "global.h"
struct UnkStruct_ov112_021EA230;

#include "constants/heap.h"

#include "heap.h"
#include "main.h"
#include "overlay_manager.h"
#include "render_text.h"
#include "system.h"
#include "title_screen.h"
#include "bag.h"
#include "player_data.h"
#include "pokedex.h"
#include "pokewalker.h"
#include "bg_window.h"
#include "list_menu.h"
#include "options.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"
#include "overlay_112_unk_struct_ov112_021_ea230.h"
#include "overlay_112_pokewalker_app.h"
#include "pokemon_summary_app.h"

s32 ov112_021E77E4(void *);

s32 ov112_021E7910(OverlayManager *arg0);

void ov112_021E795C(Ov112PokewalkerApp *arg0);

struct OverlayManager;
struct SOUND_CHATOT;
struct Menu;
struct MsgData;
struct MoveReminderData;
struct ListMenuItem;
struct MessageFormat;
typedef u16 charcode_t;
typedef struct Menu Menu;
typedef struct MoveReminderData MoveReminderData;
enum {
    MOVE_REMINDER_WIN_LABEL_BATTLE_MOVES = 0,
    MOVE_REMINDER_WIN_LABEL_CONTEST_MOVES,
    MOVE_REMINDER_WIN_LABEL_CATEGORY,
    MOVE_REMINDER_WIN_LABEL_POWER,
    MOVE_REMINDER_WIN_LABEL_ACCURACY,
    MOVE_REMINDER_WIN_LABEL_PP,
    MOVE_REMINDER_WIN_LABEL_APPEAL_PTS,
    MOVE_REMINDER_WIN_MOVE_POWER,
    MOVE_REMINDER_WIN_MOVE_ACCURACY,
    MOVE_REMINDER_WIN_MOVE_PP,
    MOVE_REMINDER_WIN_MOVE_BATTLE_DESCRIPTION,
    MOVE_REMINDER_WIN_MOVE_CONTEST_DESCRIPTION,
    MOVE_REMINDER_WIN_MESSAGE_BOX,
    MOVE_REMINDER_WIN_MOVES_NAMES,
    MOVE_REMINDER_WIN_YES_NO_MENU,
    MAX_MOVE_REMINDER_WIN
};
enum {
    MOVE_REMINDER_SPRITE_SIDE_ARROW_LEFT = 0,
    MOVE_REMINDER_SPRITE_SIDE_ARROW_RIGHT,
    MOVE_REMINDER_SPRITE_SCROLL_ARROW_UP,
    MOVE_REMINDER_SPRITE_SCROLL_ARROW_DOWN,
    MOVE_REMINDER_SPRITE_MOVE_SELECTOR,
    MOVE_REMINDER_SPRITE_CATEGORY,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_0,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_1,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_2,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_3,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_4,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_5,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_6,
    MAX_MOVE_REMINDER_SPRITE
};
typedef struct MoveReminderController {
    MoveReminderData *data;
    struct BgConfig *bgConfig;
    Window windows[15];
    struct MsgData *messageLoader;
    struct MessageFormat *stringTemplate;
    struct String *string;
    struct ListMenu *listMenu;
    struct ListMenuItem *stringList;
    MsgData *yesNoMenu;
    MessageFormat *spriteSystem;
    String *spriteManager;
    struct ManagedSprite *managedSprites[13];
    struct { void *monData; struct Options *options; u16 *OTName; u32 OTID; u8 OTGender; u8 dataType; u8 mode; u8 monMax; u8 monIndex; u8 pageFlags; u8 selectedMoveSlot; u8 returnMode; u16 move; u8 padding_1A[2]; u32 dexMode; void *specialRibbons; void *poffin; struct SOUND_CHATOT *chatotCry; BOOL showContest; } summary;
    struct OverlayManager *appMan;
    int nextState;
    u8 numMoves;
    u8 textPrinterID;
    u8 yesNoCallback;
    u8 pad_187[1];
    u16 unk_188;
    u8 pad_18A[14];
    u32 unk_198;
    u32 unk_19C;
    u32 unk_1A0;
    u8 pad_1A4[8];
    u32 unk_1AC;
    u8 pad_1B0[9];
    u8 unk_1B9;
    u8 pad_1BA[6];
    u32 unk_1C0;
} MoveReminderController;
void ov112_021E79A8(void *data);

s32 ov112_021E7910(OverlayManager *man) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    ov112_021E77E4(v0);
    OverlayManager_FreeData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_154);
    sub_0201A738(0x10);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    return 1;
}

void ov112_021E795C(Ov112PokewalkerApp *arg0) {
    arg0->bag = Save_Bag_Get(arg0->saveData);
    arg0->profile = Save_PlayerData_GetProfile(arg0->saveData);
    arg0->options = Save_PlayerData_GetOptionsAddr(arg0->saveData);
    arg0->pokewalker = Save_Pokewalker_Get(arg0->saveData);
    arg0->pokedex = Save_Pokedex_Get(arg0->saveData);
}

void ov112_021E79A8(void *data) {
    DoScheduledBgGpuUpdates(((MoveReminderController *)data)->windows[1].bgConfig);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
