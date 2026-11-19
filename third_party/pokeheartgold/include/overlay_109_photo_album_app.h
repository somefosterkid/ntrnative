#ifndef POKEHEARTGOLD_OVERLAY_109_PHOTO_ALBUM_APP_H
#define POKEHEARTGOLD_OVERLAY_109_PHOTO_ALBUM_APP_H

#include "constants/heap.h"

#include "bg_window.h"
#include "list_menu_items.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_109.h"
#include "photo_album.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "touchscreen_list_menu.h"
#include "yes_no_prompt.h"

typedef struct PhotoAlbumEntry {
    u8 index;
    Photo *photo;
} PhotoAlbumEntry;

// The photo album application's work (OverlayManager data of PhotoAlbum_Init).
typedef struct PhotoAlbumApp {
    enum HeapID heapID;                  // 0x00
    s32 state;                           // 0x04
    u16 subState;                        // 0x08
    u16 timer;                           // 0x0A
    MenuInputState menuInputState;       // 0x0C
    PhotoAlbumArgs *args;                // 0x10
    BgConfig *bgConfig;                  // 0x14
    u8 busy : 1;                         // 0x18
    u8 affineUpdate : 1;
    u8 bgTogglePending : 1;
    u8 bgToggleValue : 1;
    u8 scrollStep : 4;
    u8 page;                             // 0x19
    u8 numPages;                         // 0x1A
    u8 cursorX;                          // 0x1B
    u8 cursorY;                          // 0x1C
    u8 heldCursorX;                      // 0x1D
    u8 heldCursorY;                      // 0x1E
    u8 heldIndex;                        // 0x1F
    u8 mode;                             // 0x20
    u8 textPrinterId;                    // 0x21
    u8 textFrameDelay;                   // 0x22
    u8 frame;                            // 0x23
    MsgData *msgData;                    // 0x24
    MessageFormat *msgFmt;               // 0x28
    String *tmpString;                   // 0x2C
    String *nameString;                  // 0x30
    String *msgStrings[5];               // 0x34
    String *photoStrings[2];             // 0x48
    Window windows[3];                   // 0x50
    TouchscreenListMenuSpawner *listMenuSpawner; // 0x80
    TouchscreenListMenu *listMenu;       // 0x84
    ListMenuItem *listMenuItems;         // 0x88
    YesNoPrompt *yesNoPrompt;            // 0x8C
    SpriteSystem *spriteSystem;          // 0x90
    SpriteManager *spriteManager;        // 0x94
    Sprite *sprites[5];                  // 0x98
    struct {
        NNSG2dScreenData *data;
        void *buf;
    } scrn[3];                           // 0xAC
    u8 numPhotos;                        // 0xC4
    u8 numEntries;                       // 0xC5
    u8 unk_C6[0x2];                      // 0xC6
    Photo *photos;                       // 0xC8
    PhotoAlbumEntry entries[PHOTO_ALBUM_MAX]; // 0xCC
    PhotoAlbum *photoAlbum;              // 0x1EC
    fx32 affineScale;                    // 0x1F0
} PhotoAlbumApp;

// sizeof(PhotoAlbumApp) == 0x1F4

// Task work of ov109_021E77D4 (Heap_AllocAtEnd 8 bytes).
typedef struct PhotoAlbumTaskWork {
    PhotoAlbumApp *app;
    u32 unk_04;
} PhotoAlbumTaskWork;

void ov109_021E5A20(void);
void ov109_021E5A70(PhotoAlbumApp *app);
void ov109_021E5B40(PhotoAlbumApp *app);
s32 ov109_021E5B60(PhotoAlbumApp *app);
s32 ov109_021E5B98(PhotoAlbumApp *app);
s32 ov109_021E5BAC(PhotoAlbumApp *app);
s32 ov109_021E5BF4(PhotoAlbumApp *app);
s32 ov109_021E5C40(PhotoAlbumApp *app);
s32 ov109_021E5C48(PhotoAlbumApp *app);
s32 ov109_021E5C68(PhotoAlbumApp *app);
s32 ov109_021E5C88(PhotoAlbumApp *app);
short ov109_021E5C90(PhotoAlbumApp *app);
s32 ov109_021E5C98(PhotoAlbumApp *app);
s32 ov109_021E5CAC(PhotoAlbumApp *app);
void ov109_021E5CB4(PhotoAlbumApp *app);
void ov109_021E5CE4(PhotoAlbumApp *app);
void ov109_021E5D08(PhotoAlbumApp *app);
void ov109_021E5D5C(PhotoAlbumApp *app);
void ov109_021E5DB8(PhotoAlbumApp *app, s32 a, s32 b);
void ov109_021E5DEC(PhotoAlbumApp *app, u32 idx);
void ov109_021E5EC8(PhotoAlbumApp *app);
s32 ov109_021E5F10(PhotoAlbumApp *app);
s32 ov109_021E5F40(PhotoAlbumApp *app, s32 button);
s32 ov109_021E5FFC(PhotoAlbumApp *app, s32 *touched);
s32 ov109_021E6058(PhotoAlbumApp *app);
short ov109_021E61BC(PhotoAlbumApp *app);
s32 ov109_021E61DC(PhotoAlbumApp *app);
s32 ov109_021E628C(PhotoAlbumApp *app, s32 button);
s32 ov109_021E638C(PhotoAlbumApp *app, s32 *touched);
s32 ov109_021E63E8(PhotoAlbumApp *app);
s32 ov109_021E656C(PhotoAlbumApp *app);
s32 ov109_021E66C4(PhotoAlbumApp *app);
s32 ov109_021E681C(PhotoAlbumApp *app);
s32 ov109_021E682C(PhotoAlbumApp *app);
void ov109_021E685C(void *app);
void ov109_021E6898(PhotoAlbumApp *app);
void ov109_021E68B8(PhotoAlbumApp *app, u32 value);
void ov109_021E68D4(PhotoAlbumApp *app);
void ov109_021E691C(PhotoAlbumApp *app, s32 scale);
void ov109_021E692C(void);
void ov109_021E694C(PhotoAlbumApp *app);
void ov109_021E6AFC(PhotoAlbumApp *app);
void ov109_021E6B5C(PhotoAlbumApp *app);
void ov109_021E6C7C(PhotoAlbumApp *app);
void ov109_021E6C9C(PhotoAlbumApp *app);
void ov109_021E6D2C(PhotoAlbumApp *app);
void ov109_021E6D84(PhotoAlbumApp *app);
void ov109_021E6DBC(PhotoAlbumApp *app);
void ov109_021E6DE4(PhotoAlbumApp *app);
void ov109_021E6E64(PhotoAlbumApp *app);
void ov109_021E6E9C(PhotoAlbumApp *app);
void ov109_021E6EC0(PhotoAlbumApp *app);
void ov109_021E6EE4(PhotoAlbumApp *app);
void ov109_021E6F60(PhotoAlbumApp *app);
void ov109_021E6F7C(PhotoAlbumApp *app, u32 a, s32 b);
void ov109_021E6FD4(PhotoAlbumApp *app, u16 bgId, s32 side, s32 page);
void ov109_021E7030(PhotoAlbumApp *app, u32 bgId, u32 page, u8 col);
void ov109_021E70C4(PhotoAlbumApp *app, u8 bgId, u32 sides, u8 page);
void ov109_021E7114(PhotoAlbumApp *app, u8 icon, u8 bgId, u8 x, u8 y);
void ov109_021E7178(PhotoAlbumApp *app, Photo *photo, u16 bgId, s32 x, u32 y);
void ov109_021E71BC(PhotoAlbumApp *app, s32 a, s32 b);
void ov109_021E7248(PhotoAlbumApp *app, Photo *photo);
void ov109_021E7388(PhotoAlbumApp *app, s32 anim);
void ov109_021E73F8(PhotoAlbumApp *app, s32 msgIdx, s32 frame, u32 textSpeed);
void ov109_021E7474(PhotoAlbumApp *app, u32 x, s32 y, s32 visible);
void ov109_021E74D4(PhotoAlbumApp *app, s32 visible);
void ov109_021E7524(PhotoAlbumApp *app);
void ov109_021E7584(PhotoAlbumApp *app, s32 idx);
s32 ov109_021E75B4(PhotoAlbumApp *app);
void ov109_021E75C8(PhotoAlbumApp *app);
s32 ov109_021E7640(PhotoAlbumApp *app);
void ov109_021E76F0(PhotoAlbumApp *app);
s32 ov109_021E7748(PhotoAlbumApp *app);
void ov109_021E77D4(PhotoAlbumApp *app);
void ov109_021E7810(SysTask *task, void *param);
u8 ov109_021E7850(Photo *photo);

#endif // POKEHEARTGOLD_OVERLAY_109_PHOTO_ALBUM_APP_H
