#include "global.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_88.h"
#include "overlay_88_wifi_plaza_news.h"

void ov45_0222AB38(WiFiPlazaSystem *plazaSys, UnkStruct_ov66_02232068 *dst);

BOOL ov45_0222DD44(const struct UnkStruct_ov66_0223177C_t *playerList);

u8 ov45_0222DD5C(struct UnkStruct_ov66_0223177C_t *playerList);

BOOL ov45_0222DD78(const struct UnkStruct_ov66_0223177C_t *playerList, const void *a1, int a2, String *a3, enum HeapID heapID);

void ov45_0222DCE8(struct UnkStruct_ov66_0223177C_t *playerList);

void ov88_02259280(WiFiPlazaNewsNamePalette *namePalette, const struct UnkStruct_ov66_0223177C_t *playerList, int page, const WiFiPlazaSystem *plazaSys);

// the three words after the window rows of _02259904 (the asm only labels the last one, ov88_02259910)
static const s32 sNameHoldDurations[3] = { 0x150, 0x120, 0xF0 };

BOOL ov88_02259404(WiFiPlazaNewsNameList *nameList, struct UnkStruct_ov66_0223177C_t *playerList, const WiFiPlazaSystem *plazaSys, enum HeapID heapID);

BOOL ov88_02259404(WiFiPlazaNewsNameList *nameList, struct UnkStruct_ov66_0223177C_t *playerList, const WiFiPlazaSystem *plazaSys, enum HeapID heapID) {
    UnkStruct_ov66_02232068 unk;
    WiFiPlazaNewsNamePalette namePalette;
    int i;
    int playerIndex;

    ov45_0222AB38((WiFiPlazaSystem *)plazaSys, &unk);

    for (i = 2; i >= 0; i--) {
        if (nameList->entries[i].active != 0 && ov88_022595E4(&nameList->entries[i]) == 1) {
            if (i + 1 < 3) {
                ov88_02259560(&nameList->entries[i + 1], nameList->entries[i].name, sNameHoldDurations[(i) + 1], &nameList->entries[i].namePalette, nameList->paletteData);
            }
            ov88_022595DC(&nameList->entries[i]);
        }
    }

    if (nameList->entries[0].active == 0 && ov45_0222DD44(playerList) == 1) {
        do {
            playerIndex = ov45_0222DD5C(playerList);

            if (ov45_0222DD78(playerList, &unk, playerIndex, nameList->pendingName, heapID) == 1) {
                ov88_02259280(&namePalette, playerList, playerIndex, plazaSys);
                ov88_02259560(&nameList->entries[0], nameList->pendingName, 0x150, &namePalette, nameList->paletteData);
                ov45_0222DCE8(playerList);
                return 1;
            }
            ov45_0222DCE8(playerList);
        } while (ov45_0222DD44(playerList) == 1);
    }
    return 0;
}
#else
s32 ov45_0222AB38(void *, s32 *, void *, s32);

s32 ov45_0222DCE8(u8 *);

s32 ov45_0222DD44(void *);

s32 ov45_0222DD5C(void *);

s32 ov45_0222DD78(void *, s32 *, s32, void *, s32);

s32 ov88_02259280(s32 *, void *, s32, void *);

s32 ov88_02259560(void *, void *, s32, s32 *, void *);

s32 ov88_022595DC(void *);

s32 ov88_022595E4(void *);

extern s32 ov88_02259910;

s32 ov88_02259404(void *arg0, void *arg1, void *arg2, s32 arg3);

s32 ov88_02259404(void *arg0, void *arg1, void *arg2, s32 arg3) {
    s32 sp14[5];
    s32 sp10;
    s32 spC;
    s32 sp8;
    void *sp4;
    s32 *var_r7;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r1;
    void *var_r4;
    sp4 = arg2;
    sp8 = arg3;
    ov45_0222AB38(arg2, sp14, arg2, arg3);
    var_r7 = &ov88_02259910;
    spC = 2;
    var_r4 = arg0 + 0x40;
    do {
        if (((*((u8 *)var_r4)) != 0) && (ov88_022595E4(var_r4) == 1)) {
            temp_r1 = spC + 1;
            if (temp_r1 < 3) {
                ov88_02259560(arg0 + (temp_r1 << 5), *((void **)(((u8 *)var_r4) + 8)), *((s32 *)(((u8 *)var_r7) + 4)), var_r4 + 0x1C, *((void **)(((u8 *)arg0) + 0x98)));
            }
            ov88_022595DC(var_r4);
        }
        var_r4 -= 0x20;
        var_r7 -= 1;
        temp_r0 = spC - 1;
        spC = temp_r0;
    } while (temp_r0 >= 0);
    if (((*((u8 *)arg0)) == 0) && (ov45_0222DD44(arg1) == 1)) {
        do {
            temp_r0_2 = ov45_0222DD5C(arg1);

            if (ov45_0222DD78(arg1, sp14, temp_r0_2, *((void **)(((u8 *)arg0) + 0x90)), sp8) == 1) {
                ov88_02259280(&sp10, arg1, temp_r0_2, sp4);
                ov88_02259560(arg0, *((void **)(((u8 *)arg0) + 0x90)), 0x150, &sp10, *((void **)(((u8 *)arg0) + 0x98)));
                ov45_0222DCE8(arg1);
                return 1;
            }
            ov45_0222DCE8(arg1);
        } while (ov45_0222DD44(arg1) == 1);
    }
    return 0;
}
#endif
