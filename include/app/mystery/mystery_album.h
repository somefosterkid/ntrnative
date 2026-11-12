// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mystery/mystery_album.h: the lines in
// use so far.
#ifndef BWPORT_APP_MYSTERY_MYSTERY_ALBUM_H
#define BWPORT_APP_MYSTERY_MYSTERY_ALBUM_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of mode, unit, giftSave, font, queue, wordSet, msgData and gameData in
// Black
struct MysteryCardViewSetup {
    // 0 to look at the album, 1 to throw a card away when the album is full
    u32 mode;
    u32 unk4;
    ClActUnit *unit;
    MysteryGiftSave *giftSave;
    Font *font;
    PrintQueue *queue;
    WordSet *wordSet;
    MsgData *msgData;
    GameData *gameData;
};

// The card's BGs and palettes, on the main screen when its BG is below 4
struct MysteryCardResSetup {
    // The card's BG, which also gives its Pokémon's BG priority
    u32 bg;
    u32 textBg;
    u32 bgPalette;
    u32 textPalette;
    // The OBJ palettes of the gift's icon and of the gift's Pokémon
    u32 iconPalette;
    u32 pokePalette;
    ClActUnit *unit;
    MysteryGiftSave *giftSave;
    MsgData *msgData;
    Font *font;
    PrintQueue *queue;
    WordSet *wordSet;
};

// pokebw2's MysteryCardView_Create
MysteryCardView *func_ov121_021dac18(const MysteryCardViewSetup *setup, HeapID heapId);
// pokebw2's MysteryCardView_Delete
void func_ov121_021dae1c(MysteryCardView *view);
// pokebw2's MysteryCardView_Main
void func_ov121_021daef0(MysteryCardView *view);
// pokebw2's MysteryCardView_Draw
void func_ov121_021dafcc(MysteryCardView *view);
// pokebw2's MysteryCardView_IsEnd
BOOL func_ov121_021db008(MysteryCardView *view);
// pokebw2's MysteryCardRes_Delete
void func_ov121_021dd1fc(MysteryCardRes *res);
// pokebw2's MysteryAlbum_Delete
void func_ov121_021dd980(MysteryAlbum *album);
// pokebw2's MysteryAlbum_Main
void func_ov121_021dd9cc(MysteryAlbum *album);
// pokebw2's MysteryAlbum_StartOpen
// Brings a Pokémon gift's Pokémon out of the card
void func_ov121_021ddf5c(MysteryAlbum *album);
// pokebw2's MysteryAlbum_IsOpened
BOOL func_ov121_021de014(MysteryAlbum *album);
// pokebw2's MysteryAlbum_SetVisible
// Draws the card at the next VBlank, with its BG's graphics, or only its palette when paletteOnly is set
void func_ov121_021de0ac(MysteryAlbum *album, BOOL paletteOnly);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MysteryCardRes_Create
MysteryCardRes *func_ov121_021dcfc8(const MysteryCardResSetup *setup, HeapID heapId);
// pokebw2's MysteryAlbum_CreateReceived
// A card drawn with the BGs and actors of res: its text, its icon, and the Pokémon of a Pokémon gift
MysteryAlbum *func_ov121_021dd24c(MysteryGift *gift, MysteryCardRes *res, GameData *gameData, HeapID heapId);

#endif // BWPORT_APP_MYSTERY_MYSTERY_ALBUM_H
