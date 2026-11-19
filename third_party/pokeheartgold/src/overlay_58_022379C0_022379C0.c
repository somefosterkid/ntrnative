#include "global.h"

#include "party.h"
#include "pokemon_mood.h"

s32 sub_02031DA0(void *, void *);

void ov58_022379C0(void *arg0, Party *party, s32 slot);

void ov58_022379C0(void *arg0, Party *party, s32 slot) {
    u16 data[7];
    s8 mods[5];
    PartyExtraSub cur;
    u32 val1;
    u32 val2;
    u32 t;
    u8 idx1;
    u8 idx2;
    u8 friendship;
    u8 sum;
    u8 sub;
    s32 i;
    s32 v;
    s8 *p;
    s8 *src;
    u8 *dst;

    MI_CpuFill8(mods, 0, 5);
    sub_02031DA0(arg0, data);
    friendship = ((u8 *)arg0)[2];
    t = (u32)(data[0] << 28) >> 28;
    if (t >= 5) {
        idx1 = ((u8 *)data)[6];
    } else {
        idx1 = t;
    }
    t = (u32)(data[1] << 28) >> 28;
    if (t >= 5) {
        idx2 = ((u8 *)data)[7];
    } else {
        idx2 = t;
    }
    Party_GetMonAprijuiceModifiers(party, &cur, slot);
    val1 = (u32)(data[0] << 16) >> 24;
    p = &mods[idx1];
    mods[idx1] = (u32)(1.5f * val1);
    if (val1 != 0) {
        *p += 10;
    }
    val2 = (u32)(data[1] << 16) >> 24;
    mods[idx2] = (u32)(1.5f * val2);
    sum = val1 + val2;
    if (friendship == 0xFF) {
        sub = (u32)(0.1f * sum);
    } else if (friendship >= 200) {
        sub = (u32)(0.2f * sum);
    } else {
        sub = (u32)(sum * (1.0f - 0.1f * (u8)(friendship / 25)));
    }
    src = mods;
    src[((u8 *)data)[10]] -= sub;
    dst = cur.unk_00;
    for (i = 0; i < 5; i++) {
        v = *src;
        if (v < -127) {
            v = -127;
        } else if (v > 127) {
            v = 127;
        }
        *dst = v;
        src++;
        dst++;
    }
    Party_SetMonAprijuiceModifiers(party, &cur, slot);
    ApplyMonMoodModifier(Party_GetMonByIndex(party, slot), 7);
}
