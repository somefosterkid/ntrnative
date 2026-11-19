#include "global.h"

#include "overlay_manager.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_87_scratch_off_card_app.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct MessagePrinter;
struct MsgData;
struct PaletteAnimator;
struct ScratchOffCardsAppSprite;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct YesNoPrompt;
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};
BOOL ov87_021E5AFC(ScratchOffCardApp *app);
BOOL ov87_021E5B48(ScratchOffCardApp *app);
BOOL ov87_021E5C38(ScratchOffCardApp *app);
BOOL ov87_021E5CEC(ScratchOffCardApp *app);
BOOL ov87_021E5E00(ScratchOffCardApp *app);
BOOL ov87_021E6080(ScratchOffCardApp *app);
BOOL ov87_021E64F8(ScratchOffCardApp *app);
BOOL ov87_021E65FC(ScratchOffCardApp *app);
void ov87_021E6668(ScratchOffCardApp *app);
void ov87_021E66B8(ScratchOffCardApp *app);
void ov87_021E6760(ScratchOffCardApp *app);
void ov87_021E725C(ScratchOffCardApp *app, int *state, int newState);

BOOL ScratchOffCards_Main(OverlayManager *appMan, int *state);

BOOL ScratchOffCards_Main(OverlayManager *appMan, int *state) {
    ScratchOffCardApp *app = OverlayManager_GetData(appMan);

    switch (*state) {
    case 0:
        if (ov87_021E5AFC(app) == 1) {
            ov87_021E725C(app, state, 1);
        }
        break;
    case 1:
        if (ov87_021E5B48(app) == 1) {
            ov87_021E725C(app, state, 2);
        }
        break;
    case 2:
        if (ov87_021E5C38(app) == 1) {
            ov87_021E725C(app, state, 3);
        }
        break;
    case 3:
        if (ov87_021E5CEC(app) == 1) {
            if (app->currentCard >= 3) {
                app->currentCard = 0;

                ov87_021E6760(app);
                ov87_021E6668(app);
                ov87_021E66B8(app);
                ov87_021E725C(app, state, 4);
            } else {
                ov87_021E725C(app, state, 1);
            }
        }
        break;
    case 4:
        if (ov87_021E5E00(app) == 1) {
            ov87_021E725C(app, state, 5);
        }
        break;
    case 5:
        if (ov87_021E6080(app) == 1) {
            if (app->currentCard >= 3) {
                ov87_021E725C(app, state, 7);
            } else {
                ov87_021E725C(app, state, 6);
            }
        }
        break;
    case 6:
        if (ov87_021E64F8(app) == 1) {
            ov87_021E725C(app, state, 4);
        }
        break;
    case 7:
        if (ov87_021E65FC(app) == 1) {
            return 1;
        }
        break;
    }

    SpriteList_RenderAndAnimateSprites(app->spriteMan.spriteList);

    return 0;
}
