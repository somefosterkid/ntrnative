#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "dex_mon_measures.h"
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

struct BgConfig;

struct GridInputHandler;

struct MessageFormat;

struct MsgData;

struct NARC;

struct PaletteData;

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

struct SpriteManager;

struct String;

struct UnkStruct_0201956C;

struct Window;

void ov18_021F8640(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);

void ov18_021F8640(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex) {
    u32 i;

    if (min == 0 && max == 152) {
        memcpy(dst, src, srcCount * sizeof(u16));
        *count = srcCount;
        return;
    }

    for (i = 0; i < srcCount; i++) {
        s32 height = ((s32 *)pokedexApp->heights)[src[i]];
        if (height >= pokedexApp->unk_1850[min].unk_0 && height <= pokedexApp->unk_1850[max].unk_0 && Pokedex_CheckMonCaughtFlag(pokedex, src[i])) {
            dst[*count] = src[i];
            (*count)++;
        }
    }
}

void ov18_021F86D0(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex);

void ov18_021F86D0(PokedexAppData *pokedexApp, u16 min, u16 max, u16 *dst, u32 *count, u16 *src, u32 srcCount, Pokedex *pokedex) {
    u32 i;

    if (min == 0 && max == 152) {
        memcpy(dst, src, srcCount * sizeof(u16));
        *count = srcCount;
        return;
    }

    for (i = 0; i < srcCount; i++) {
        s32 weight = ((s32 *)pokedexApp->weights)[src[i]];
        if (weight >= pokedexApp->unk_1850[min].unk_2 && weight <= pokedexApp->unk_1850[max].unk_2 && Pokedex_CheckMonCaughtFlag(pokedex, src[i])) {
            dst[*count] = src[i];
            (*count)++;
        }
    }
}
