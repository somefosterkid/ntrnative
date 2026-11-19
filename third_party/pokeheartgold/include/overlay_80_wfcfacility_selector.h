#ifndef POKEHEARTGOLD_OVERLAY_80_WFCFACILITY_SELECTOR_H
#define POKEHEARTGOLD_OVERLAY_80_WFCFACILITY_SELECTOR_H

// WFCFacilitySelector: merged by tools/hg/promote.py from 10 m2c local copies (1 variants, base
// src/overlay_80_02235900_02235900.c) and 5 accesses; fields at every accessed offset, pads elsewhere.

struct FieldSystem;
struct PartyMenu;
struct PokemonSummary;
struct SaveData;

typedef struct WFCFacilitySelector {
    struct SaveData *saveData;
    void *unused;
    u16 commBuffer[40];
    u8 partnerGender;
    u8 partnerDroppedOut;
    u16 unused2[8];
    u8 selectedMonSlots[4];
    u8 partnersSelectedFacility;
    u8 msgsReceived;
    u8 unused3;
    u8 partnerNotPlayingAgain;
    u16 partnersLatestStreak;
    u16 partnersStreakDeletionChoice;
    u16 selectedSpecies[4];
    u16 selectedItems[4];
    u16 partnersSelectedSpecies[4];
    u16 partnersSelectedItems[4];
    u8 pad_96[0x2];
    struct FieldSystem *fieldSystem;
    u8 unused4;
    u8 dummy;
    u8 unused5;
    u8 selectedMonSlot;
    u8 selectedFacility;
    u8 partyMenuSelectionOrder[4];
    u8 pad_A5[0x3];
    struct PartyMenu *partyMenu;
    struct PokemonSummary *summaryApp;
    u16 *subAppResult;
} WFCFacilitySelector;

// sizeof(WFCFacilitySelector) == 0xb4

#endif // POKEHEARTGOLD_OVERLAY_80_WFCFACILITY_SELECTOR_H
