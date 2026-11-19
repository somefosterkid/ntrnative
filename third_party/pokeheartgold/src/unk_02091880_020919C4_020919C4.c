#include "global.h"
#include "constants/heap.h"
#include "pokemon_types_def.h"
#include "easy_chat.h"
#include "menu_input_state.h"
#include "pokedex.h"
#include "mail_misc.h"

struct ECMenu;

struct EasyChatManager;

struct SaveEasyChat;

u16 GetECWordIndexByPair(u32 category, u32 msgNo);

BOOL Save_EasyChat_GetTrendySayingFlag(SaveEasyChat *saveEasyChat, u32 flag);

SaveEasyChat *sub_02090D80(EasyChatArgs *args);

typedef struct ECMenu ECMenu;

struct ECMenu {
    EasyChatArgs *args;
    EasyChatManager *manager;
    u32 categoryCounts[12];
    u32 categoryOffsets[12];
    u16 categoryWords[1496];
    u32 initialCounts[27];
    u32 initialOffsets[27];
    u16 initialWords[1495];
    u8 knownWords[0xBB];
};

ECMenu *ECMenu_New(enum HeapID heapID, EasyChatArgs *args);

u32 ECMenuBuild_TrendySayings(ECMenu *menu, const u16 *words, u32 count, u16 *dest);

u32 ECMenuBuild_TrendySayings(ECMenu *menu, const u16 *words, u32 count, u16 *dest) {
    u32 i;
    u32 written;
    SaveEasyChat *saveEasyChat = sub_02090D80(menu->args);
    u32 base = GetECWordIndexByPair(0x122, 0);

    i = 0;
    written = i;
    for (; i < count; i++) {
        if (Save_EasyChat_GetTrendySayingFlag(saveEasyChat, words[i] - base)) {
            u32 word = words[i];
            menu->knownWords[word >> 3] |= 1 << (word & 7);
            written++;
            *dest = words[i];
            dest++;
        }
    }
    return written;
}
