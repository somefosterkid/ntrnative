#include "global.h"
#include "bg_window.h"
#include "filesystem.h"
#include "math_util.h"
#include "options.h"
#include "overlay_manager.h"
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

void ov87_021E725C(ScratchOffCardApp *app, int *state, int newState);

struct OverlayManager;
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
void ov87_021E7264(ScratchOffCardApp *app);
void ov87_021E7294(ScratchOffCardApp *app);

void ov87_021E725C(ScratchOffCardApp *app, int *state, int newState) {
    app->subState = 0;
    *state = newState;
}

void ov87_021E7264(ScratchOffCardApp *app) {
    for (int i = 0; i < 2; i++) {
        while (1) {
            u8 index = LCRandom() % 9;

            if (app->scratchCellValues[index] == 176) {
                app->scratchCellValues[index] = 4;
                break;
            }
        }
    }
}

void ov87_021E7294(ScratchOffCardApp *app) {
    u8 cellValue = LCRandom() % 4;
    int retries = 0;

    for (int i = 0; i < 9; i++) {
        while (1) {
            int cellIdx = LCRandom() % 9;

            if (app->scratchCellValues[cellIdx] == 176) {
                retries = 0;
                app->scratchCellValues[cellIdx] = cellValue;

                if ((i == 2) || (i == 4) || (i == 6)) {
                    cellValue++;

                    if (cellValue == 4) {
                        cellValue = 0;
                    }
                }
                break;
            }

            retries++;

            if (retries >= 30) {
                retries = 0;

                for (int j = 0; j < 9; j++) {
                    if (app->scratchCellValues[j] == 176) {
                        app->scratchCellValues[j] = cellValue;

                        if (i == 2 || i == 4 || i == 6) {
                            cellValue++;

                            if (cellValue == 4) {
                                cellValue = 0;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}
