#include "global.h"

#include "error_handling.h"
#include "gf_gfx_planes.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_switch.h"

struct BattleAnimSystem;

struct BattleBgSwitch;

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_0221DE04(BattleBgSwitch *bgSwitch, BattleAnimSystem *system, enum GFBgLayer bgLayer, int bgID);

void ov07_0221EBA4(BattleAnimSystem *system, GFBgLayer bgLayer, u16 colorMode);

void ov07_0221EC7C(BattleAnimSystem *system, enum GFBgLayer bgLayer);

void ov07_0221FB30(BattleAnimSystem *system, enum GFBgLayer layer);

int ov07_0221CCF4(BattleAnimSystem *system);

static int hgasset_ov07_0221CCF4_indexTable[6][6] __attribute__((section(BWPORT_DATA_SECTION))) = {

        { 0, 0,    0, 0, 0, 0 },
        { 0,    0, 0, 0, 0, 0 },
        { 0, 0, 0, 0,    0, 0    },
        { 0, 0, 0,    0, 0,    0 },
        { 0, 0, 0, 0,    0, 0    },
        { 0, 0, 0,    0, 0,    0 }
    };

int ov07_0221CCF4(BattleAnimSystem *system) {

    __typeof__(hgasset_ov07_0221CCF4_indexTable) indexTable; __builtin_memcpy((void *)&indexTable, &hgasset_ov07_0221CCF4_indexTable, sizeof(indexTable));

    int attacker = ov07_0221C468(system);
    int defender = ov07_0221C470(system);
    int attackerType = ov07_02231924(system, attacker);
    int defenderType = ov07_02231924(system, defender);
    int index = indexTable[attackerType][defenderType];

    ((index != 0xFF) ? (void)0 : GF_AssertFail());

    return index - 1;
}

int ov07_0221CD4C(BattleAnimSystem *system);

int ov07_0221CD4C(BattleAnimSystem *system) {
    int indexTable[6][6] = {

        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
        { 0xFF, 0xFF, 0xFF, 0xFF, 0,    0xFF },
        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 1    },
        { 0xFF, 0xFF, 1,    0xFF, 0xFF, 0xFF },
        { 0xFF, 0xFF, 0xFF, 0,    0xFF, 0xFF }
    };

    int attacker = ov07_0221C468(system);
    int defender = ov07_0221C470(system);
    int attackerType = ov07_02231924(system, attacker);
    int defenderType = ov07_02231924(system, defender);
    int index = indexTable[attackerType][defenderType];

    ((index != 0xFF) ? (void)0 : GF_AssertFail());

    return index;
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_07_0221C9C0_0221CCF4_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_07_0221C9C0_0221CCF4.c:hgasset_ov07_0221CCF4_indexTable", (void *)&hgasset_ov07_0221CCF4_indexTable, sizeof(hgasset_ov07_0221CCF4_indexTable), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_07_0221C9C0_0221CCF4.c:hgasset_ov07_0221CCF4_indexTable", (void *)&hgasset_ov07_0221CCF4_indexTable, sizeof(hgasset_ov07_0221CCF4_indexTable), "u8");
}
