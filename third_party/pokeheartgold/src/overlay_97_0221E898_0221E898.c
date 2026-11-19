#include "global.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"

s32 ov97_0221E898(PCStorage *arg0, u32 arg1, u32 arg2, void *arg3);

s32 ov97_0221E898(PCStorage *arg0, u32 arg1, u32 arg2, void *arg3) {
    BoxPokemon *temp_r0;

    temp_r0 = PCStorage_GetMonByIndexPair(arg0, arg1, arg2);
    if (GetBoxMonData(temp_r0, 0xAC, NULL) != 0) {
        *(u32 *)arg3 = GetBoxMonData(temp_r0, 5, NULL);
        *(u32 *)((u8 *)arg3 + 4) = GetBoxMonData(temp_r0, 0, NULL);
        *(s16 *)((u8 *)arg3 + 8) = (s16) GetBoxMonData(temp_r0, 0x4C, NULL);
        *(s16 *)((u8 *)arg3 + 0xA) = (s16) GetBoxMonData(temp_r0, 0x70, NULL);
        *(s16 *)((u8 *)arg3 + 0xC) = 0;
        *(s16 *)((u8 *)arg3 + 0xE) = 0;
        GetBoxMonData(temp_r0, 0x75, arg3 + 0x18);
        *(s16 *)((u8 *)arg3 + 0x10) = (s16) BoxMonIsShiny(temp_r0);
        *(s16 *)((u8 *)arg3 + 0x12) = (s16) GetBoxMonData(temp_r0, 0x6F, NULL);
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

s32 ov97_0221E91C(PCStorage *arg0, u32 arg1, u32 arg2, void *arg3);

s32 ov97_0221E91C(PCStorage *arg0, u32 arg1, u32 arg2, void *arg3) {
    BoxPokemon *temp_r0;

    temp_r0 = PCStorage_GetMonByIndexPair(arg0, arg1, arg2);
    if (GetBoxMonData(temp_r0, 0xAC, NULL) != 0) {
        *(u32 *)arg3 = GetBoxMonData(temp_r0, 5, NULL);
        *(s32 *)((u8 *)arg3 + 4) = 0;
        *(s16 *)((u8 *)arg3 + 8) = (s16) GetBoxMonData(temp_r0, 0x4C, NULL);
        *(s16 *)((u8 *)arg3 + 0xA) = (s16) GetBoxMonData(temp_r0, 0x70, NULL);
        *(s16 *)((u8 *)arg3 + 0xC) = 0;
        *(s16 *)((u8 *)arg3 + 0xE) = 0;
        *(s16 *)((u8 *)arg3 + 0x10) = 0;
        *(s16 *)((u8 *)arg3 + 0x12) = 0;
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
    return 0;
}
