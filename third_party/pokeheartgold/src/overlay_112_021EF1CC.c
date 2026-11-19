#include "global.h"

#include "unk_0200B150.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"

void ov112_021EF1CC(void);

s32 ov112_021EF1F0(PCStorage *arg0, u32 arg1, u32 arg2, void *arg3);

s32 ov112_021EF298(PCStorage *arg0, u32 arg1, u32 arg2, void *arg3);

void ov112_021EF1CC(void) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_154);
}

s32 ov112_021EF1F0(PCStorage *storage, u32 boxno, u32 slotno, void *arg3) {
    BoxPokemon *boxMon;

    boxMon = PCStorage_GetMonByIndexPair(storage, boxno, slotno);
    if (GetBoxMonData(boxMon, 0xAC, NULL) != 0) {
        *(u32 *)arg3 = GetBoxMonData(boxMon, 5, NULL);
        *(u32 *)((u8 *)arg3 + 4) = GetBoxMonData(boxMon, 0, NULL);
        *(s16 *)((u8 *)arg3 + 8) = (s16)GetBoxMonData(boxMon, 0x4C, NULL);
        *(s16 *)((u8 *)arg3 + 0xA) = (s16)GetBoxMonData(boxMon, 0x70, NULL);
        *(s16 *)((u8 *)arg3 + 0xC) = (s16)GetBoxMonData(boxMon, 6, NULL);
        *(s16 *)((u8 *)arg3 + 0xE) = 0;
        GetBoxMonData(boxMon, 0x75, arg3 + 0x18);
        *(s16 *)((u8 *)arg3 + 0x10) = (s16)BoxMonIsShiny(boxMon);
        *(s16 *)((u8 *)arg3 + 0x12) = (s16)GetBoxMonData(boxMon, 0x6F, NULL);
        *(s16 *)((u8 *)arg3 + 0x14) = (s16)GetBoxMonData(boxMon, 0xA1, NULL);
        *(s16 *)((u8 *)arg3 + 0x16) = (s16)GetBoxMonData(boxMon, 0xB, NULL);
        return 1;
    }
    *(u32 *)arg3 = 0U;
    *(u32 *)((u8 *)arg3 + 4) = 0U;
    *(s16 *)((u8 *)arg3 + 8) = 0;
    *(s16 *)((u8 *)arg3 + 0xA) = 0;
    *(s16 *)((u8 *)arg3 + 0xC) = 0;
    *(s16 *)((u8 *)arg3 + 0xE) = 0;
    *(s16 *)((u8 *)arg3 + 0x10) = 0;
    *(s16 *)((u8 *)arg3 + 0x12) = 0;
    return 0;
}

s32 ov112_021EF298(PCStorage *storage, u32 boxno, u32 slotno, void *arg3) {
    BoxPokemon *boxMon;

    boxMon = PCStorage_GetMonByIndexPair(storage, boxno, slotno);
    if (GetBoxMonData(boxMon, 0xAC, NULL) != 0) {
        *(u32 *)arg3 = GetBoxMonData(boxMon, 5, NULL);
        *(s32 *)((u8 *)arg3 + 4) = 0;
        *(s16 *)((u8 *)arg3 + 8) = (s16)GetBoxMonData(boxMon, 0x4C, NULL);
        *(s16 *)((u8 *)arg3 + 0xA) = (s16)GetBoxMonData(boxMon, 0x70, NULL);
        *(s16 *)((u8 *)arg3 + 0xC) = 0;
        *(s16 *)((u8 *)arg3 + 0xE) = 0;
        *(s16 *)((u8 *)arg3 + 0x10) = 0;
        *(s16 *)((u8 *)arg3 + 0x12) = 0;
        *(s16 *)((u8 *)arg3 + 0x14) = 0;
        *(s16 *)((u8 *)arg3 + 0x16) = 0;
        return 1;
    }
    *(u32 *)arg3 = 0U;
    *(s32 *)((u8 *)arg3 + 4) = 0;
    *(s16 *)((u8 *)arg3 + 8) = 0;
    *(s16 *)((u8 *)arg3 + 0xA) = 0;
    *(s16 *)((u8 *)arg3 + 0xC) = 0;
    *(s16 *)((u8 *)arg3 + 0xE) = 0;
    *(s16 *)((u8 *)arg3 + 0x10) = 0;
    *(s16 *)((u8 *)arg3 + 0x12) = 0;
    *(s16 *)((u8 *)arg3 + 0x14) = 0;
    *(s16 *)((u8 *)arg3 + 0x16) = 0;
    return 0;
}
