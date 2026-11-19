#include "global.h"

#include "bg_window.h"
#include "party.h"
#include "pokemon.h"

struct BgConfig;

struct BoxPokemon;

struct CAPSULE;

struct Mail;

struct MailMessage;

struct Party;

struct PartyPokemon;

struct SEAL;

struct UnkStruct_ov65_0221CADC;

struct UnkStruct_ov65_0221CADC_Mon;

struct Window;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct UnkStruct_ov65_0221CADC_Mon {
    u8 filler_00[4];
    u8 pokeBall;
    u8 filler_05[0xB];
} UnkStruct_ov65_0221CADC_Mon;

typedef struct UnkStruct_ov65_0221CADC {
    u8 filler_000[0x444];
    Window windows[24];
    u8 filler_5C4[0xD8];
    UnkStruct_ov65_0221CADC_Mon monDisplayData[13];
    u8 filler_76C[0x90];
    u8 previewPixelBuf[2][0xC80];
    u8 filler_20FC[0x2224 - 0x20FC];
    Party *playerParty;
    Party *partnerParty;
} UnkStruct_ov65_0221CADC;

int ov65_0221D57C(int side, Pokemon *mon, void *pixelBuf, void *tmpl);

void ov65_0221D674(Window *windows, int a1, Party *party, int a3, UnkStruct_ov65_0221CADC *tr);

void ov65_0221CA64(UnkStruct_ov65_0221CADC *tr, int a1, int a2);

void ov65_0221CADC(UnkStruct_ov65_0221CADC *tr, int slot);

void ov65_0221CADC(UnkStruct_ov65_0221CADC *tr, int slot) {
    int rem = slot % 6;
    Pokemon *mon = Party_GetMonByIndex(tr->partnerParty, rem);
    int quot = slot / 6;
    *(int *)((u8 *)tr + 0x211C) = ov65_0221D57C(1, mon, tr->previewPixelBuf[quot], (u8 *)tr + 0x20FC + quot * 0x10);
    ov65_0221D674(tr->windows, 1, tr->partnerParty, rem, tr);
    ov65_0221CA64(tr, 3, tr->monDisplayData[slot].pokeBall);
}
