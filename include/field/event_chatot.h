// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_chatot.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_EVENT_CHATOT_H
#define BWPORT_FIELD_EVENT_CHATOT_H

#include "types.h"
#include "gfl/clact.h"
#include "system/game_event.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of chatter, gsys, gameData, field, player, pkm, msgBGSys, window, unit,
// sprite, chars, palette, cellAnims, animFrame, animOffset, partyIndex and recorded, and its size in Black
struct ChatotEventWork {
    void *chatter;
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    FieldPlayer *player;
    PartyPkm *pkm;
    MsgData *msgData;
    void *msgBGSys;
    void *talkWindow;
    void *yesNo;
    WordSet *wordSet;
    StrBuf *strbuf;
    BmpWin *window;
    WaitIcon *waitIcon;
    ClActUnit *unit;
    ClActor *sprite;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    u8 animFrame;
    s8 animOffset;
    u8 partyIndex;
    u8 unk4F;
    u32 voice;
    // Set by the microphone's callback once the recording is done
    u32 recorded;
};

GameEvent *func_ov021_021d1c24(GameSystem *gsys, Field *field, u8 partyIndex);
void func_ov021_021d1f48(u32 result, u32 *done);
void func_ov021_021d1f50(ChatotEventWork *work);
void func_ov021_021d201c(ChatotEventWork *work);
u32 func_ov021_021d2040(ChatotEventWork *work);
void func_ov021_021d20bc(ChatotEventWork *work);
void func_ov021_021d2124(ChatotEventWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
extern const ClActorSetup data_ov021_021dd60c;
GameEventReturnCode func_ov021_021d1c8c(GameEvent *event, u32 *state, void *data);
void func_ov021_021d1f64(ChatotEventWork *work);
void func_ov021_021d1f78(ChatotEventWork *work);

#endif // BWPORT_FIELD_EVENT_CHATOT_H
