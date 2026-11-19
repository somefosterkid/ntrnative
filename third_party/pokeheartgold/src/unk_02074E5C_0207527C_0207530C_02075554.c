#include "global.h"
#include "player_data.h"
#include "save_palpad.h"

typedef u16 charcode_t;

const charcode_t *PlayerProfile_GetNamePtr(const PlayerProfile *info);





charcode_t *CopyU16StringArray(charcode_t *dst, const charcode_t *src);

void sub_02075554(PlayerProfile *trainerInfo, SavePalPad *source, SavePalPad *destination);

void sub_02075554(PlayerProfile *trainerInfo, SavePalPad *source, SavePalPad *destination) {
    CopyU16StringArray(destination->name, PlayerProfile_GetNamePtr(trainerInfo));

    destination->otid = PlayerProfile_GetTrainerID(trainerInfo);
    destination->filler_14[0] = PlayerProfile_GetLanguage(trainerInfo);
    destination->filler_14[1] = PlayerProfile_GetVersion(trainerInfo);
    destination->filler_14[2] = PlayerProfile_GetTrainerGender(trainerInfo);

    for (int i = 0; i < 16; i++) {
        destination->unk_18[i] = source[i].otid;
        destination->unk_58[i] = source[i].filler_14[1];
        destination->unk_68[i] = source[i].filler_14[0];
        destination->filler_78[i] = source[i].filler_14[2];
    }
}
