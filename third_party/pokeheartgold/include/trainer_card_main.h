#ifndef POKEHEARTGOLD_TRAINER_CARD_MAIN_H
#define POKEHEARTGOLD_TRAINER_CARD_MAIN_H

#include "bg_window.h"
#include "g2d_renderer.h"
#include "igt.h"
#include "msgdata.h"
#include "overlay_trainer_card.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"

typedef struct TrainerCardMainCard {
    u8 version;
    u8 language;
    u8 unk_02;
    u8 stars;
    u8 unk_04_0 : 1;
    u8 liveIgt : 1;
    u8 gender : 1;
    u8 pokedexObtained : 1;
    u8 signatureExists : 1;
    u8 unk_04_5 : 3;
    u8 caseVariant;
    u16 badges;
    u16 name[8];
    IGT *igt;
    u32 money;
    u32 pokedexCount;
    u32 score;
    u16 trainerId;
    u16 playHours;
    u16 clearHours;
    u8 playMinutes;
    u8 startYear;
    u8 startMonth;
    u8 startDay;
    u8 clearYear;
    u8 clearMonth;
    u8 clearDay;
    u8 clearMinutes;
    u8 unk_36[2];
    u32 linkCount;
    u32 linkBattleWins;
    u32 linkBattleLosses;
    u32 linkTrades;
    u8 badgeShininess[0x20];
    u8 signature[0x600];
} TrainerCardMainCard;

typedef struct TrainerCardMainSprites {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resourceManagers[2][4];
    SpriteResource *resourceObjects[2][4];
    Sprite *badgeSprites[16];
    u8 unk_1AC[0x44];
    Sprite *buttonSprites[2];
    void *paletteFiles[16];
    NNSG2dPaletteData *palettes[16];
} TrainerCardMainSprites;

typedef struct TrainerCardMain {
    BgConfig *bgConfig;
    Window windows[14];
    TrainerCardAppArgs *args;
    TrainerCardMainCard *card;
    u8 signaturePixels[0x3000];
    void *frontCharFile;
    NNSG2dCharacterData *frontCharData;
    BOOL showingBack;
    u32 unk_30F8;
    fx32 scaleX;
    fx32 scaleY;
    u32 unk_3104;
    int unk_3108;
    BOOL inTouchMode;
    u8 unk_3110[0xC];
    TrainerCardMainSprites sprites;
    int flipSpeed;
    u32 unk_3398;
    u32 unk_339C;
    u8 unk_33A0[4][2];
    u8 unk_33A8[8];
    void *frontScreenFile;
    NNSG2dScreenData *frontScreen;
    void *badgeFrameScreenFile;
    NNSG2dScreenData *badgeFrameScreen;
    YesNoPrompt *yesNoPrompt;
    MsgData *msgData;
    String *unk_33C8;
    String *colonString;
    String *formattedString;
    String *numberString;
    String *buttonStrings[3];
    String *promptStrings[2];
    String *labelStrings[14];
    u8 badgeFlags[16];
    u8 unk_3434;
    u8 unk_3435;
    u8 subState;
    u8 unk_3437;
    u8 unk_3438;
    u8 colonBlinkTimer;
    u8 showKantoBadges : 1;
    u8 isConnected : 1;
    u8 scaleChanged : 1;
    u8 unk_343A_3 : 5;
    u8 unk_343B;
    u8 textPrinterId;
    u8 frame;
    u8 textSpeed;
    u8 scrollOffset;
    u8 touchX;
    u8 touchY;
    u8 unk_3442[2];
} TrainerCardMain;

// sizeof(TrainerCardMain) == 0x3444

void ov51_021E6038(TrainerCardMain *cardMain);
void ov51_021E69EC(TrainerCardMain *cardMain);
void ov51_021E6E60(TrainerCardMain *cardMain);
void ov51_021E6EF0(TrainerCardMain *cardMain);
void ov51_021E6DA8(TrainerCardMain *cardMain, BOOL liveIgt);
void ov51_021E6E10(TrainerCardMain *cardMain, int showButtons);
void ov51_021E76EC(TrainerCardMain *cardMain, int mode);
void ov51_021E77A0(TrainerCardMain *cardMain, int arg1);
void ov51_021E7804(TrainerCardMain *cardMain, int unused);
int ov51_021E786C(TrainerCardMain *cardMain, int arg1);
void ov51_021E78D0(TrainerCardMain *cardMain, int hide);
unsigned int ov51_021E6B44(TrainerCardMain *cardMain);
u32 ov51_021E67A4(TrainerCardMain *cardMain);
u32 ov51_021E6888(TrainerCardMain *cardMain);
void ov51_021E7CA4(TrainerCardMainSprites *sprites);
void ov51_021E7D44(TrainerCardMainSprites *sprites, int buttonIdx, int animSeq, BOOL draw);

#endif // POKEHEARTGOLD_TRAINER_CARD_MAIN_H
