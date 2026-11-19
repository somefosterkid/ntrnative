#ifndef POKEHEARTGOLD_OVERLAY_59_JUICE_FEED_APP_H
#define POKEHEARTGOLD_OVERLAY_59_JUICE_FEED_APP_H

#include "bg_window.h"
#include "filesystem.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_58.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02031B0C.h"
#include "unk_struct_ov59_0223c5b0.h"
#include "yes_no_prompt.h"

// one party member of the Apricorn juice feeding screen (DS size 0x34)
typedef struct Ov59JuiceMon {
    Pokemon *mon;                               // 0x00
    u16 species;                                // 0x04
    u8 level;                                   // 0x06
    u8 gender;                                  // 0x07
    u8 slot;                                    // 0x08
    u8 unk_09;                                  // 0x09
    u8 pad_0A[2];
    String *name;                               // 0x0C
    PartyExtraSub modifiers;                    // 0x10
    u8 pad_15;
    struct PokeathlonPerformanceStars stars;    // 0x16
    struct PokeathlonTodayPerformance today;    // 0x1E
    u8 pad_32[2];
} Ov59JuiceMon;

// the 0x2f4 heap object of ov59_0223A8E0 (feed the Apricorn juice to a party member)
typedef struct Ov59JuiceFeedApp {
    enum HeapID heapId;                         // 0x00
    Ov58ApricornBoxWork *parent;                // 0x04
    SaveData *saveData;                         // 0x08
    u8 unk_0C[4];
    Party *party;                               // 0x10
    SaveApricornBox *apricornBox;               // 0x14
    ApricornJuice juice;                        // 0x18
    ApricornJuiceRanks ranks;                   // 0x20
    struct PokeathlonPerformanceStars stars;    // 0x2E
    u8 statIndex;                               // 0x36
    s8 deltaAbs;                                // 0x37
    u8 deltaStep;                               // 0x38
    s8 deltas[PERFORMANCE_MAX];                 // 0x39
    u8 deltaCount;                              // 0x3E
    u8 pad_3F;
    u16 state;                                  // 0x40
    u16 subState;                               // 0x42
    enum MenuInputState menuInputState;         // 0x44
    u8 mode;                                    // 0x48
    u8 monCount;                                // 0x49
    u8 selected;                                // 0x4A
    u8 textFrameDelay;                          // 0x4B
    u8 frame;                                   // 0x4C
    u8 printerId;                               // 0x4D
    u8 pad_4E;
    u8 taskCount;                               // 0x4F
    u8 pad_50;
    u8 timer;                                   // 0x51
    u8 pad_52[2];
    BgConfig *bgConfig;                         // 0x54
    u8 pad_58[4];
    MsgData *msgData;                           // 0x5C
    MessageFormat *msgFormat;                   // 0x60
    String *unk_64;                             // 0x64
    String *unk_68;                             // 0x68
    String *unk_6C;                             // 0x6C
    String *genderStrings[2];                   // 0x70
    UnkStruct_ov59_0223C5B0 *monSprite;         // 0x78
    Ov59JuiceMon mons[PARTY_SIZE];              // 0x7C
    Window windows[10];                         // 0x1B4
    YesNoPrompt *yesNoPrompt;                   // 0x254
    SpriteSystem *spriteSystem;                 // 0x258
    SpriteManager *spriteManager;               // 0x25C
    Sprite *sprites[0x22];                      // 0x260
    NARC *narc;                                 // 0x2E8
    u8 pad_2EC[8];
} Ov59JuiceFeedApp;

void ov59_0223A9E4(Ov59JuiceFeedApp *app);
s32 ov59_0223AC48(Ov59JuiceFeedApp *app);
void ov59_0223AD84(Ov59JuiceFeedApp *app);
void ov59_0223AE20(Ov59JuiceFeedApp *app);
void ov59_0223AEB0(Ov59JuiceFeedApp *app, s32 mode);
s32 ov59_0223AF9C(Ov59JuiceFeedApp *app);
s32 ov59_0223B164(Ov59JuiceFeedApp *app);
void ov59_0223BA64(Ov59JuiceMon *mon, Party *party, Pokemon *pokemon, u8 slot, enum HeapID heapId);
void ov59_0223BADC(Ov59JuiceMon *mon);
void ov59_0223BAE8(Ov59JuiceFeedApp *app);
void ov59_0223BBD4(Ov59JuiceFeedApp *app, s32 msgMode);
void ov59_0223BC88(Ov59JuiceFeedApp *app, s32 index);
void ov59_0223BD4C(Ov59JuiceFeedApp *app, Ov59JuiceMon *mon, s32 stat);
void ov59_0223BE18(Ov59JuiceFeedApp *app, Ov59JuiceMon *mon);
void ov59_0223BE44(Ov59JuiceFeedApp *app, s32 index, s32 animSeq, BOOL draw);
void ov59_0223BE70(Ov59JuiceFeedApp *app, s32 stat, s8 isOut);
void ov59_0223BBB0(Ov59JuiceFeedApp *app);
void ov59_0223C054(Ov59JuiceFeedApp *app);
void ov59_0223C138(Ov59JuiceFeedApp *app);

#endif // POKEHEARTGOLD_OVERLAY_59_JUICE_FEED_APP_H
