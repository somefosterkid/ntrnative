#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_74_mystery_gift_animation.h"

struct AffineSpriteListTemplate;

struct OverlayManager;

struct OverlayManagerTemplate;

struct G2dRenderer;

struct MainMenuSpriteManager;

struct MysteryGiftAnimationManager;

struct MysteryGiftParticle;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct SysTask;

struct SysTaskManager;

struct Window;

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct AffineSpriteListTemplate {
    SpriteList *list;
    const SpriteResourcesHeader *resourceData;
    VecFx32 position;
    VecFx32 affineScale;
    u16 affineZRotation;
    u32 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    enum HeapID heapID;
} AffineSpriteListTemplate;

Sprite *Sprite_CreateAffine(const AffineSpriteListTemplate *template, int);

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);

void Sprite_SetAnimCtrlSeq(Sprite *sprite, u32 animID);

void Sprite_SetExplicitPriority(Sprite *sprite, u8 priority);

typedef struct GF_2DGfxRawResObj {
    int id;
    void *data;
} GF_2DGfxRawResObj;

typedef struct GF_2DGfxRawResMan {
    GF_2DGfxRawResObj *resources;
    int capacity;
    int count;
} GF_2DGfxRawResMan;

typedef struct SpriteResource {
    GF_2DGfxRawResObj *rawResource;
    GfGfxResType type;
    void *data;
} SpriteResource;

typedef struct SpriteResourceCollection {
    GF_2DGfxRawResMan *collection;
    SpriteResource *resources;
    void *capacity;
    void *count;
    GfGfxResType type;
} SpriteResourceCollection;

typedef struct SysTaskManager SysTaskManager;

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

enum MysteryGiftAppState {
    MG_APP_STATE_SETUP = 0,
    MG_APP_STATE_INIT_GRAPHICS,
    MG_APP_STATE_WAIT_SCREEN_TRANSITION,
    MG_APP_STATE_WAIT_MAIN_MENU_INPUT,
    MG_APP_STATE_WAIT_RECEPTION_METHOD_CHOICE,
    MG_APP_STATE_WAIT_CONFIRM_WIRELESS_COMMS,
    MG_APP_STATE_UNUSED_6,

    MG_APP_STATE_SEARCH_FOR_FRIEND_GIFT,
    MG_APP_STATE_WAIT_CONFIRM_RECEIVE_FRIEND_GIFT,
    MG_APP_STATE_CONNECT_WITH_FRIEND,
    MG_APP_STATE_RECEIVING_FROM_FRIEND,
    MG_APP_STATE_SAVE_GIFT_FROM_FRIEND,
    MG_APP_STATE_CHECK_FRIEND_CONNECTION_STILL_VALID,
    MG_APP_STATE_WAIT_FRIEND_READY_TO_DISCONNECT,
    MG_APP_STATE_PERSON_SHARING_DISCONNECTED,
    MG_APP_STATE_RECEIVE_FROM_FRIEND_SUCCESS,
    MG_APP_STATE_FRIEND_LEFT_BEFORE_SENDING,

    MG_APP_STATE_NO_GIFT_FOUND,
    MG_APP_STATE_RETURN_TO_MENU,
    MG_APP_STATE_FOUND_WIFI_OR_GBA_GIFT,
    MG_APP_STATE_WAIT_ACCEPT_WIFI_OR_GBA_GIFT,
    MG_APP_STATE_GOTO_SHOW_RECEIVING_GIFT_MSG,
    MG_APP_STATE_SHOW_RECEIVING_GIFT_MSG,
    MG_APP_STATE_SAVE_RECEIVED_MYSTERY_GIFT,
    MG_APP_STATE_WAIT_MYSTERY_GIFT_SAVED,
    MG_APP_STATE_WAIT_BEFORE_EXIT_AFTER_RECEIVING_GIFT,
    MG_APP_STATE_UNUSED_26,
    MG_APP_STATE_SEARCH_FOR_WIFI_EVENT,

    MG_APP_STATE_UNUSED_28,
    MG_APP_STATE_UNUSED_29,
    MG_APP_STATE_UNUSED_30,

    MG_APP_STATE_WAIT_CONFIRM_RECEIVE_LOCAL_WIRELESS_GIFT,
    MG_APP_STATE_SEARCH_FOR_LOCAL_WIRELESS_EVENT,
    MG_APP_STATE_RECEIVE_FROM_LOCAL_WIRELESS,
    MG_APP_STATE_WAIT_LOCAL_WIRELESS_GIFT_SAVED,
    MG_APP_STATE_WAIT_FOR_ANIMATION_FINISHED,
    MG_APP_STATE_DISCONNECT_FROM_LOCAL_WIRELESS,

    MG_APP_STATE_UNUSED_37,
    MG_APP_STATE_UNUSED_38,
    MG_APP_STATE_UNUSED_39,
    MG_APP_STATE_UNUSED_40,
    MG_APP_STATE_UNUSED_41,
    MG_APP_STATE_UNUSED_42,
    MG_APP_STATE_UNUSED_43,
    MG_APP_STATE_UNUSED_44,
    MG_APP_STATE_UNUSED_45,
    MG_APP_STATE_UNUSED_46,
    MG_APP_STATE_UNUSED_47,
    MG_APP_STATE_UNUSED_48,

    MG_APP_STATE_FROM_FRIEND_OR_CANT_RECEIVE,
    MG_APP_STATE_WAIT_MESSAGE_BOX_FOR_TRANSITION,
    MG_APP_STATE_WAIT_INPUT_RETURN_TO_MENU,
    MG_APP_STATE_ASK_CONFIRM_RECEIVE_SHARED_GIFT,
    MG_APP_STATE_EXIT_TO_TITLE_SCREEN,
    MG_APP_STATE_EXIT_TO_WONDER_CARDS_APP,
    MG_APP_STATE_EXIT_AFTER_RECEIVING_GIFT,
    MG_APP_STATE_SHOW_RECEIVED_WONDERCARD,
    MG_APP_STATE_RESET_SYSTEM,

    MG_APP_KEEP_PREVIOUS_STATE = -1,
};

enum MysteryGiftAppState AskConfirmWireless_FriendOrGBA(OverlayManager *appMan);

enum MysteryGiftAppState AskConfirmConnectToWFC(OverlayManager *appMan);

enum MysteryGiftAppState AskConfirmWireless_WirelessDistribution(OverlayManager *appMan);

enum MysteryGiftAppState ShowMysteryGiftReceptionMethodsMenu(OverlayManager *appMan);

int ShowMessageBoxIntoStateTransition(OverlayManager *appMan, Window *window, int textEntryId, enum MysteryGiftAppState nextState);

Sprite *ov74_0222B0C8(MysteryGiftAnimationManager *animMan, int vramType);

void Sprite_SetPriority(Sprite *sprite, u8 priority);

Sprite *ov74_0222B0C8(MysteryGiftAnimationManager *animMan, int vramType) {
    Sprite *sprite;

    {
        AffineSpriteListTemplate template;

        enum GFScreen screen = (enum GFScreen)((vramType == NNS_G2D_VRAM_TYPE_2DMAIN) ? 0 : 1);

        template.list = animMan->spriteMan->spriteList;
        template.resourceData = &animMan->spriteResourcesHeaders[screen];
        template.position.z = 0;
        template.affineScale.x = ((fx32)0x0000000000001000L);
        template.affineScale.y = ((fx32)0x0000000000001000L);
        template.affineScale.z = ((fx32)0x0000000000001000L);
        template.affineZRotation = 0;
        template.position.x = ((fx32)(((256 / 2) > 0) ? ((256 / 2) * (1 << 12) + 0.5f) : ((256 / 2) * (1 << 12) - 0.5f)));
        template.position.y = ((fx32)(((32) > 0) ? ((32) * (1 << 12) + 0.5f) : ((32) * (1 << 12) - 0.5f)));
        template.priority = 10;
        template.vramType = (enum NNS_G2D_VRAM_TYPE)(vramType);
        template.heapID = HEAP_ID_84;

        if (template.vramType == NNS_G2D_VRAM_TYPE_2DSUB) {
            template.position.y += ((fx32)(((256) > 0) ? ((256) * (1 << 12) + 0.5f) : ((256) * (1 << 12) - 0.5f)));
        }

        sprite = Sprite_CreateAffine(&template, vramType);
    }

    if (sprite) {
        Sprite_SetAnimActiveFlag(sprite, 1);
        Sprite_SetPriority(sprite, 0);
        Sprite_SetAnimCtrlSeq(sprite, 0);
        Sprite_SetDrawFlag(sprite, 1);
    }

    return sprite;
}
