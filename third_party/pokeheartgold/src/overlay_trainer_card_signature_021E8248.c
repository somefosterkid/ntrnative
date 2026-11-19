#include "global.h"

#include "overlay_manager.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "game_stats.h"
#include "msgdata.h"
#include "yes_no_prompt.h"
#include "signature_app.h"

struct DrawingState;

struct PreviousTouchDataBuffer;

struct SignatureApp;

struct SignatureTouchDataBuffer;

BOOL IsPaletteFadeFinished(void);

void ov52_021E921C(u16 *degrees);

extern int (*ov52_021E96C0[7])(SignatureApp *signatureApp, int state);

enum SignatureAppState {
    SIGNATURE_APP_STATE_INIT = 0,
    SIGNATURE_APP_STATE_FRAME_DELAY,
    SIGNATURE_APP_STATE_WAIT_FOR_FADE = 0,
    SIGNATURE_APP_STATE_MAIN,
    SIGNATURE_APP_STATE_EXIT_AFTER_FADE,
};

BOOL TrainerCardSignature_Main(OverlayManager *appMan, int *state);

BOOL TrainerCardSignature_Main(OverlayManager *appMan, int *state) {
    SignatureApp *signatureApp = OverlayManager_GetData(appMan);

    switch (*state) {
    case SIGNATURE_APP_STATE_WAIT_FOR_FADE:
        if (IsPaletteFadeFinished()) {
            *state = SIGNATURE_APP_STATE_MAIN;
        }

        break;
    case SIGNATURE_APP_STATE_MAIN:
        if (ov52_021E96C0[signatureApp->subState] != NULL) {
            *state = (*ov52_021E96C0[signatureApp->subState])(signatureApp, *state);
        }

        ov52_021E921C(&signatureApp->glowEffectDegreesUnused);
        break;
    case SIGNATURE_APP_STATE_EXIT_AFTER_FADE:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }

        break;
    }

    SpriteList_RenderAndAnimateSprites(signatureApp->spriteList);

    return FALSE;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// asm/overlay_trainer_card_signature.s keeps the DS copy (7 .word entries, the first a literal 0), which the 64-bit port
// emits as 4-byte words (wordmix.py): the PC build uses this C definition instead.
int (*ov52_021E96C0[7])(SignatureApp *signatureApp, int state) = {
    NULL,
    ov52_021E8CF4,
    ov52_021E8D8C,
    ov52_021E8DC4,
    ov52_021E8EDC,
    ov52_021E8EFC,
    ov52_021E8E64,
};
#endif
