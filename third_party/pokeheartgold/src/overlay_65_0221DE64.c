#include "global.h"

#include "pokemon_types_def.h"
#include "save.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_65_trade_room.h"
#include "player_data.h"
#include "save_palpad.h"
#include "sav_chatot.h"

struct SOUND_CHATOT;
struct PlayerProfile;
enum TradeRoomCommCmd {
    TRADE_CMD_SEND_PARTY = 22,
    TRADE_CMD_SYNC_CURSOR = 23,
    TRADE_CMD_SYNC_STATUS = 24,
    TRADE_CMD_PARTY_RECEIVED_ACK = 27,
    TRADE_CMD_SEND_PALPAD = 28,
    TRADE_CMD_SEND_CHATOT_CRY = 29,
    TRADE_CMD_STAGGER_DELAY = 31,
    TRADE_CMD_SEND_RIBBONS = 32,
};
BOOL sub_02036FD8(int cmd, const void *data, int size);
BOOL sub_020373B4(u16 netId);
void *ov65_0221DE58(Party *party, int index);
BOOL sub_02037030(int cmd, const void *data, int size);
u8 *Save_SpecialRibbons_Get(SaveData *saveData);
typedef u16 charcode_t;
charcode_t *CopyU16StringArray(charcode_t *dst, const charcode_t *src);
const charcode_t *PlayerProfile_GetNamePtr(const PlayerProfile *info);

void ov65_0221DE64(int netId, Party *party, int index);
void ov65_0221DE8C(SaveData *saveData);
void ov65_0221DEA0(PlayerProfile *trainerInfo, SavePalPad *source, SavePalPad *destination);
void ov65_0221DF0C(SOUND_CHATOT *chatotCryData);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov65_0221CB5C(void *);
#else
s32 ov65_0221CB5C(void *);
#endif
void ov65_0221DBF4(u32 *direction);
s32 ov65_0221DD34(s32, void *, s32);
extern s32 ov65_0221E144;
extern s32 ov65_0221E9A8;
extern s32 ov65_0221F3F4;
s32 ov65_0221DF1C(TradeRoom *arg0);

void ov65_0221DE64(int netId, Party *party, int index) {
    if (sub_020373B4(netId)) {
        sub_02036FD8(TRADE_CMD_SEND_PARTY, ov65_0221DE58(party, index), 236 * 6 + 4 * 2);
    }
}

void ov65_0221DE8C(SaveData *saveData) {
    u8 *ribbonData = Save_SpecialRibbons_Get(saveData);

    sub_02037030(TRADE_CMD_SEND_RIBBONS, ribbonData, 14);
}

void ov65_0221DEA0(PlayerProfile *trainerInfo, SavePalPad *source, SavePalPad *destination) {
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

    sub_02036FD8(TRADE_CMD_SEND_PALPAD, destination, sizeof(SavePalPad));
}

void ov65_0221DF0C(SOUND_CHATOT *chatotCryData) {
    sub_02036FD8(TRADE_CMD_SEND_CHATOT_CRY, chatotCryData, 1000);
}

s32 ov65_0221DF1C(TradeRoom *arg0) {
    s32 v0;

    if (2 & gSystem.newKeys) {
        PlaySE(0x5DCU);
        arg0->selectedSlot[0] = 0xC;
        ov65_0221CB5C(arg0);
        ov65_0221DD34(arg0->selectedSlot[0], arg0->cursorSprites[0], 0);
    } else {
        ov65_0221DBF4(arg0->pendingDirection);
        if ((1 & gSystem.newKeys) && (*((s32 *)&arg0->pendingDirection[0]) == 0)) {
            v0 = arg0->selectedSlot[0];
            if (v0 == 0xC) {
                arg0->subStepCallback = &ov65_0221E144;
            } else if (v0 < 6) {
                arg0->subStepCallback = &ov65_0221E9A8;
            } else if ((v0 >= 6) && (v0 < 0xC)) {
                arg0->subStepCallback = &ov65_0221F3F4;
            }
        }
    }
    return 0;
}
