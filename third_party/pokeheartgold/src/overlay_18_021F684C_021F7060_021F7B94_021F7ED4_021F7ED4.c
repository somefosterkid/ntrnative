#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "dex_mon_measures.h"
#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_18.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokedex_util.h"
#include "sound_02004A44.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "unk_02092BB8.h"

struct Background;

struct BgConfig;

struct DpadMenuBox;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResHeader;

struct GF_2DGfxResMan;

struct GF_2DGfxResObjList;

struct GridCallbacks;

struct GridInputHandler;

struct MenuInputStateMgr;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct MsgData;

struct NARC;

struct PaletteBuffer;

struct PaletteData;

struct PlayerProfile;

struct Pokedex;

struct PokedexAppData;

struct PokedexAppData_UnkSub0868_State04;

struct PokedexAppData_UnkSub0868_State06;

struct PokedexAppData_UnkSub0868_State09;

struct PokedexAppData_UnkSub0868_State10;

struct PokedexAppData_UnkSub0868_State13;

struct PokedexAppData_UnkSub0868_State16;

struct PokedexAppData_UnkSub0868_State17;

struct PokedexAppData_UnkSub0868_State26;

struct PokedexAppData_UnkSub0868_State38;

struct PokedexAppData_UnkSub0868_State69;

struct PokedexAppData_UnkSub0868_State77;

struct PokedexAppData_UnkSub0868_State77_Sub4;

struct PokedexAppData_UnkSub0878;

struct PokedexAppData_UnkSub1030;

struct PokedexAppData_UnkSub1850;

struct PokedexAppData_UnkSub18CC;

struct PokedexAppData_UnkSub18DC;

struct PokedexAppData_UnkSub18DC_0;

struct PokedexAppData_UnkSub1908;

struct PokedexAppData_UnkSub190C;

struct PokedexArgs;

struct PokedexData;

struct SelectedPaletteData;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceHeaderList;

struct SpriteResourcesHeader;

struct String;

struct UnkStruct_0201956C;

struct UnkStruct_02092BB8;

struct Window;

struct _MAT;

struct _MAT_ENTRY;

void ov18_021F831C(u32 type, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);

void *ov18_021F8168(u32 arg0, u32 *arg1);

void ov18_021F8198(u8 *param_1, s32 *param_2, Pokedex *param_3, u16 *param_4, u32 param_5);

void ov18_021F822C(u32 param_1, void *param_2, s32 *param_3, void *param_4, s32 param_5, void *param_6);

void ov18_021F82CC(s32 arg0, void *arg1, s32 *arg2, void *arg3, s32 arg4, void *arg5);

void ov18_021F8468(u32 param_1, void *param_2, s32 *param_3, void *param_4, s32 param_5, Pokedex *param_6);

void ov18_021F8584(void *arg0, s32 arg1, void *arg2, u32 *arg3, u16 *arg4, u32 arg5, Pokedex *pokedex);

void ov18_021F8640(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);

void ov18_021F86D0(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);

void ov18_021F81D8(u16 *param_1, Pokedex *param_2, u16 *param_3, u32 param_4);

BOOL ov18_021F7ED4(PokedexAppData *pokedexApp, u8 natDexFlag, u32 order, u32 letter, u32 type1, u32 type2, u32 heightMin, u32 heightMax, u32 weightMin, u32 weightMax, u32 areaMask, u32 bodyType) {
    u16 *buf1;
    u16 *buf2;
    u16 *list;
    u32 listCount;
    u32 count1;
    u32 count2;

    MI_CpuClear32(&pokedexApp->unk_0878, sizeof(PokedexAppData_UnkSub0878));

    buf1 = Heap_AllocAtEnd(HEAP_ID_POKEDEX_APP, 493 * sizeof(u16));
    ((buf1 != 0L) ? (void)0 : GF_AssertFail());
    memset(buf1, 0, 493 * sizeof(u16));
    count1 = 0;

    buf2 = Heap_AllocAtEnd(HEAP_ID_POKEDEX_APP, 493 * sizeof(u16));
    ((buf2 != 0L) ? (void)0 : GF_AssertFail());
    memset(buf2, 0, 493 * sizeof(u16));
    count2 = 0;

    if (natDexFlag == 0) {
        list = (unsigned short *)(ov18_021F8168(((NARC_zukan_data_sort_order_dex_order_johto)-NARC_zukan_data_sort_order_dex_order_national), &listCount));
    } else {
        list = (unsigned short *)(ov18_021F8168(((NARC_zukan_data_sort_order_dex_order_national)-NARC_zukan_data_sort_order_dex_order_national), &listCount));
    }

    ov18_021F8198((u8 *)(buf1), &count1, pokedexApp->args->pokedex, list, listCount);

    ov18_021F822C((u32)(order), buf2, &count2, buf1, count1, (u8 *)(pokedexApp->args->pokedex));
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    ov18_021F82CC((s32)(letter), buf2, &count2, buf1, (s32)(count1), (u8 *)(pokedexApp->args->pokedex));
    if (count2 == 0) {
        goto end;
    }
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    ov18_021F831C(type1, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    if (count2 == 0) {
        goto end;
    }
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    ov18_021F831C(type2, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    if (count2 == 0) {
        goto end;
    }
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    ov18_021F8468(bodyType, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    if (count2 == 0) {
        goto end;
    }
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    if (pokedexApp->unk_1860 == 0 && (areaMask & (1 << 2))) {
        ov18_021F8584(pokedexApp, (1 << 1) | (1 << 2), buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
        if (count2 != 0) {
            goto filterHeight;
        }
        goto end;
    } else {
        ov18_021F8584(pokedexApp, areaMask, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
        if (count2 == 0) {
            goto end;
        }
    }

filterHeight:
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    ov18_021F8640(pokedexApp, heightMin, heightMax, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);
    memcpy(buf1, buf2, count2 * sizeof(u16));
    count1 = count2;
    memset(buf2, 0, count2 * sizeof(u16));
    count2 = 0;

    ov18_021F86D0(pokedexApp, weightMin, weightMax, buf2, &count2, buf1, count1, pokedexApp->args->pokedex);

    ov18_021F81D8((u16 *)(&pokedexApp->unk_0878), pokedexApp->args->pokedex, (u16 *)(buf2), count2);

end:
    Heap_Free(list);
    Heap_Free(buf2);
    Heap_Free(buf1);
    return TRUE;
}
