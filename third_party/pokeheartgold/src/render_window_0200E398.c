#include "global.h"
#include "filesystem_files_def.h"
#include "bg_window.h"

struct BgConfig;
enum StandardWindowType {
    STANDARD_WINDOW_SYSTEM = 0,
    STANDARD_WINDOW_FIELD,
    STANDARD_WINDOW_UNDERGROUND,
};
u32 GfGfxLoader_LoadCharData(enum NarcId narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void sub_0200E398(BgConfig *bgConfig, u8 bgLayer, u16 offset, u8 standardWindowType, u32 heapID);

void sub_0200E398(BgConfig *bgConfig, u8 bgLayer, u16 offset, u8 standardWindowType, u32 heapID) {
    if (standardWindowType == STANDARD_WINDOW_SYSTEM) {
        GfGfxLoader_LoadCharData(NARC_a_0_3_8,
            0,
            bgConfig,
            bgLayer,
            offset,
            0,
            0,
            heapID);
    } else {
        GfGfxLoader_LoadCharData(NARC_a_0_3_8,
            1,
            bgConfig,
            bgLayer,
            offset,
            0,
            0,
            heapID);
    }
}
