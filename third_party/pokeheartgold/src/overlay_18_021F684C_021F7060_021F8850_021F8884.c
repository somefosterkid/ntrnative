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

void ov18_021F8884(PokedexAppData *pokedexApp, int a1) {
    u32 i;

    MI_CpuClear32(pokedexApp->unk_1030, sizeof(pokedexApp->unk_1030));
    if (a1 == 1) {
        for (i = 0; i < pokedexApp->unk_0878.unk_7B4; i++) {
            u32 index = Pokedex_ConvertToCurrentDexNo(pokedexApp->unk_1858, pokedexApp->unk_0878.unk_000[i][0]) - 1;
            pokedexApp->unk_1030[index].unk_0 = pokedexApp->unk_0878.unk_000[i][0];
            pokedexApp->unk_1030[index].unk_2 = pokedexApp->unk_0878.unk_000[i][1];
        }
    } else {
        for (i = 0; i < pokedexApp->unk_0878.unk_7B4; i++) {
            pokedexApp->unk_1030[i + 1].unk_0 = pokedexApp->unk_0878.unk_000[i][0];
            pokedexApp->unk_1030[i + 1].unk_2 = pokedexApp->unk_0878.unk_000[i][1];
        }
    }
}
