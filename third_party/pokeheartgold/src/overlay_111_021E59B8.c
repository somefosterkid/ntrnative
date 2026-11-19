#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "yes_no_prompt.h"
#include "overlay_111.h"

struct BgConfig;

struct BoxPokemon;

struct BugContestSwapMonArgs;

struct CAPSULE;

struct Mail;

struct MailMessage;

struct ManagedSprite;

struct MessageFormat;

struct MsgData;

struct NARC;

struct Options;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PartyPokemon;

struct SEAL;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct Window;

struct YesNoPrompt;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef BOOL (*BugContestSwapMonStateFunc)(BugContestSwapMon *swapMon);

BOOL BugContestSwapMon_ShowCaughtMon(BugContestSwapMon *swapMon);

BOOL BugContestSwapMon_ChooseMon(BugContestSwapMon *swapMon);

extern BugContestSwapMonStateFunc _021E6D40[2];

BOOL BugContestSwapMon_Main(OverlayManager *man, int *state);

BOOL BugContestSwapMon_Main(OverlayManager *man, int *state) {
    BugContestSwapMon *swapMon = OverlayManager_GetData(man);
    GF_ASSERT(swapMon != NULL);
    if (_021E6D40[swapMon->state](swapMon)) {
        return TRUE;
    }
    return FALSE;
}
