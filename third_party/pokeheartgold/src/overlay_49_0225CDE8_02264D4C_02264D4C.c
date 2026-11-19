#include "global.h"

#include "overlay_49_unk_struct_ov49_02264cf8.h"
#include "pm_string.h"
#include "bg_window.h"
#include "list_menu.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_45_wifi_plaza_system.h"

struct ColoredArrow;

struct ListMenu;

struct ListMenuTemplate;

struct String;

struct ListMenuItem;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225DEE8_t;

struct WiFiPlazaProfile;

struct Window;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

u8 ov45_0222AA10(const WiFiPlazaProfile *profile);

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

void ov49_0225A08C(UnkStruct_ov70_0225DEE8 *param0, const String *param1);

String *ov49_0225A30C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);

void ov49_02264D9C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2, const WiFiPlazaProfile *param3);

void ov49_02264D4C(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, u32 param3, const WiFiPlazaProfile *param4, const WiFiPlazaProfile *param5);

static u16 hgasset_ov49_02264D4C_v3[8][8] __attribute__((section(BWPORT_DATA_SECTION))) = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0,  0,  0,  0,  0,  0,  0,  0  },
        { 0, 0,  0, 0, 0, 0, 0, 0 },
        { 0, 0,  0, 0, 0, 0, 0, 0 },
        { 0, 0,  0, 0, 0, 0, 0, 0 },
        { 0, 0,  0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
    };

void ov49_02264D4C(UnkStruct_ov49_02264CF8 *param0, WiFiPlazaSystem *param1, UnkStruct_ov70_0225DEE8 *param2, u32 param3, const WiFiPlazaProfile *param4, const WiFiPlazaProfile *param5) {
    u32 v0;
    u32 v1, v2;
    

    ov49_02264D9C(param2, 0, param3, param4);
    ov49_02264D9C(param2, 1, param3, param5);

    v1 = ov45_0222AA10(param4);
    v2 = ov45_0222AA10(param5);
    v0 = hgasset_ov49_02264D4C_v3[v1][v2];

    ov49_0225A08C(param2, ov49_0225A30C(param2, 3, v0));
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_49_0225CDE8_02264D4C_02264D4C_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_49_0225CDE8_02264D4C_02264D4C.c:hgasset_ov49_02264D4C_v3", (void *)&hgasset_ov49_02264D4C_v3, sizeof(hgasset_ov49_02264D4C_v3), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_49_0225CDE8_02264D4C_02264D4C.c:hgasset_ov49_02264D4C_v3", (void *)&hgasset_ov49_02264D4C_v3, sizeof(hgasset_ov49_02264D4C_v3), "u8");
}
