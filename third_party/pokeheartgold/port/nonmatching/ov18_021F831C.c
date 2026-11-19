#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#include "dex_mon_measures.h"
#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokedex.h"
#include "pokedex_util.h"
#include "sound_02004A44.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
struct Pokedex;
void *ov18_021F8168(u32 fileIdx, u32 *countOut);
void ov18_021F8764(u16 *dst, u32 *count, u16 *lookup, u32 lookupCount, u16 *src, u32 srcCount, BOOL skipCaughtCheck, Pokedex *pokedex);
u32 ov18_021F8970(u32 type);

void ov18_021F831C(u32 type, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);
void ov18_021F831C(u32 type, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex) {
    u16 *fileList;
    u32 fileCount;

    switch (ov18_021F8970(type)) {
    case (((NARC_zukan_data_sort_order_types_dark - NARC_zukan_data_sort_order_types_normal)) + 1) :
        memcpy(dst, src, srcCount * sizeof(u16));
        *count = srcCount;
        return;
    case (NARC_zukan_data_sort_order_types_normal - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_normal) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_fighting - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_fighting) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_flying - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_flying) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_poison - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_poison) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_ground - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_ground) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_rock - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_rock) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_bug - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_bug) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_ghost - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_ghost) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_steel - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_steel) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_fire - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_fire) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_water - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_water) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_grass - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_grass) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_electric - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_electric) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_psychic - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_psychic) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_ice - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_ice) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_dragon - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_dragon) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    case (NARC_zukan_data_sort_order_types_dark - NARC_zukan_data_sort_order_types_normal) :
        fileList = ov18_021F8168(((NARC_zukan_data_sort_order_types_dark) - NARC_zukan_data_sort_order_dex_order_national) , &fileCount);
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail()) ;
        break;
    }

    ov18_021F8764(dst, count, fileList, fileCount, src, srcCount, 0 , pokedex);
    Heap_Free(fileList);
}
