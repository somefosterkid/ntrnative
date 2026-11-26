#ifndef POKEHEARTGOLD_LINK_PARTY_SELECT_H
#define POKEHEARTGOLD_LINK_PARTY_SELECT_H

// Task environment of the link party-select task (sub_02059538, DS size 0x8C).
// Other translation units still carry layout-identical local copies named UnkStruct_0205A0D8.

#include "bg_window.h"

struct FieldSystem;
struct ListMenuCursor;
struct MessageFormat;
struct MsgData;
struct Party;
struct PartyMenuArgs;
struct PlayerProfile;
struct PokemonSummary;
struct String;

typedef struct LinkPartySelect {
    struct PokemonSummary *summary;                 // 0x00
    struct PartyMenuArgs *partyMenu;                // 0x04
    void (*callback)(s32, struct Party *);         // 0x08
    struct String *unk_0C;                          // 0x0C
    struct String *unk_10;                          // 0x10
    Window unk_14;                                  // 0x14
    struct FieldSystem *fieldSystem;                // 0x24
    struct MessageFormat *msgFormat;                // 0x28
    struct MsgData *msgData;                        // 0x2C
    int unk_30;                                     // 0x30
    int state;                                      // 0x34
    int unk_38;                                     // 0x38
    u8 unk_3C;                                      // 0x3C
    u8 unk_3D[6];                                   // 0x3D
    u8 unk_43;                                      // 0x43
    u8 unk_44;                                      // 0x44
    u8 *unk_48;                                     // 0x48
    u8 *unk_4C;                                     // 0x4C
    struct Party *party;                            // 0x50
    Window unk_54;                                  // 0x54
    Window unk_64;                                  // 0x64
    struct PlayerProfile *unk_74;                   // 0x74
    struct ListMenuCursor *unk_78;                  // 0x78
    Window *unk_7C;                                 // 0x7C
    u8 unk_80;
    s8 unk_81;
    u8 unk_82;
    u8 unk_83;
    u8 unk_84;
    u8 unk_85;
    u16 unk_86;                                     // 0x86
    u8 unk_88;                                      // 0x88
    u8 unk_89;                                      // 0x89
} LinkPartySelect;

#endif // POKEHEARTGOLD_LINK_PARTY_SELECT_H
