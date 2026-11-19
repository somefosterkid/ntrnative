#include "global.h"

#include "message_format.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "overlay_111.h"

ManagedSprite *ov111_021E6330(SpriteSystem *, SpriteManager *, s16, s16, u8, u8);

void ov111_021E5BE4(BugContestSwapMon *arg0);

void ov111_021E5BE4(BugContestSwapMon *arg0) {
    ManagedSprite *managedSprite;
    MessageFormat *messageFormat;
    Pokemon *pokemon;
    Pokemon *mon;
    BugContestSwapMonArgs *v0;

    messageFormat = ov111_021E6A2C(arg0->text);
    v0 = arg0->args;
    pokemon = v0->newlyCaughtMon;
    mon = v0->currentMon;
    ov111_021E6738(arg0->panels, 0, 0x80, 0x20, pokemon);
    ov111_021E6738(arg0->panels, 1, 0x80, 0x70, mon);
    BufferBoxMonSpeciesName(messageFormat, 0U, Mon_GetBoxMon(pokemon));
    BufferBoxMonSpeciesName(messageFormat, 1U, Mon_GetBoxMon(mon));
    managedSprite = ov111_021E6330(arg0->spriteSystem, arg0->spriteManager, 0x80, 0x48, 1, 0);
    arg0->cursorSprite = managedSprite;
    ManagedSprite_SetPaletteOverride(managedSprite, 1);
}
