// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_sound.h, the lines in use so far.
#ifndef BWPORT_FIELD_EVENT_SOUND_H
#define BWPORT_FIELD_EVENT_SOUND_H

// Events that send requests to the field's sound system. Names from swan (https://github.com/ds-pokemon-hacking/swan,
// GPL-3.0)

#include "types.h"
#include "struct_decls.h"

GameEvent *EventBattleBGMPlay_Create(GameSystem *gsys, u32 bgm);
GameEvent *EventBGMFadePop_Create(GameSystem *gsys);
// Waits until no pop is left
GameEvent *EventBGMFadeWait_Create(GameSystem *gsys);

// Pops the BGM and waits until the sound system is idle
GameEvent *EventPushBGMFinish_Create(GameSystem *gsys, u32 fadeOutFrames, u32 fadeInFrames);
GameEvent *EventBGMPlayEx_Create(GameSystem *gsys, u32 bgm, u32 fadeOutFrames);

// Fades out, waits, then releases the field's sound
GameEvent *EventBGMFadeStop_Create(GameSystem *gsys, u16 fadeOutFrames);

GameEvent *EventMEPlay_Create(GameSystem *gsys, u32 meId);

GameEvent *EventBGMPlay_Create(GameSystem *gsys, u32 bgm);

// pokebw2's EventWaitFieldSound_Create
// Waits until the BGM has stopped changing
GameEvent *func_02029c64(GameSystem *gsys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Pushes the BGM and waits until the sound system is idle
GameEvent *EventBGMPushWait_Create(GameSystem *gsys, u32 fadeOutFrames);
GameEvent *EventBGMPop_CreateEx(GameSystem *gsys, u32 fadeOutFrames, u32 fadeInFrames);
// Pops every pushed BGM, fading in the last
GameEvent *EventBGMPopAll_Create(GameSystem *gsys, u32 fadeInFrames);
GameEvent *EventBGMChange_Create(GameSystem *gsys, u32 bgm, u32 fadeOutFrames, u32 fadeInFrames);
GameEvent *EventBGMPlayPush_Create(GameSystem *gsys, u32 bgm);

// Declared until now by their callers only; Black's take the frame counts as u16 (pokebw2's: u32)
GameEvent *EventBGMFadeIn_Create(GameSystem *gsys, u16 fadeInFrames);
GameEvent *EventBGMFadeOut_Create(GameSystem *gsys, u16 fadeOutFrames);
// pokebw2's EventBGMPlayPushEx_Create: pushes the BGM, then changes to bgm
GameEvent *func_020297ac(GameSystem *gsys, u32 bgm, u16 fadeOutFrames, u16 fadeInFrames);
// pokebw2's CreateBGMFadeOutEvent: fades out to the field's silence
GameEvent *func_02029a74(GameSystem *gsys, u16 fadeOutFrames);

#endif // BWPORT_FIELD_EVENT_SOUND_H
