#include "global.h"
struct PokeathlonMedals;

#include "message_format.h"
#include "overlay_99_pokeathlon_medals.h"
#include "overlay_98_ov98text_sys.h"

void ov99_021E7758(struct PokeathlonMedals *arg0, u16 arg1);

void ov99_021E7758(PokeathlonMedals *arg0, u16 species) {
    MessageFormat *messageFormat;

    messageFormat = ov98_0221EEFC(arg0->textContext);
    if (species != 0) {
        BufferSpeciesName(messageFormat, 0U, species);
        ov98_0221EBD8(arg0->textContext, 5, 0x7C, 0);
        return;
    }
    ov98_0221EE84(arg0->textContext, 5);
}
