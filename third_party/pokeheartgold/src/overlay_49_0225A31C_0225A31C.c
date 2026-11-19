#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_49_unk_struct_ov70_02263910.h"
#include "bg_window.h"
#include "pm_string.h"
#include "filesystem.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "touch_hitbox_controller.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225f098.h"
#include "overlay_49_unk_struct_ov70_0225f114.h"
#include "overlay_49_unk_struct_ov70_0225efd4.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_02260744.h"
#include "overlay_49_unk_struct_ov70_0225fa84.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0226_c6f8.h"

struct BgConfig;

struct ColoredArrow;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ListMenu;

struct ListMenuTemplate;

struct MsgData;

struct NARC;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct TouchHitboxController;

struct PlayerProfile;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02269204_t;

struct UnkStruct_ov70_0226C6F8_t;

struct Window;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov49_0225B3A8(UnkStruct_ov70_0225F208 *param0, u32 param1, u32 param2, u32 param3, int param4);

void ov49_0225A37C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov49_0225A31C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2, u32 param3, int param4);

void ov49_0225A31C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2, u32 param3, int param4) {
    ov49_0225B3A8(&param0->unk_340, param1, param2, param3, param4);
}
