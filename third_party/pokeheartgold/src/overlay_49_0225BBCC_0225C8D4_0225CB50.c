#include "global.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "list_menu.h"
#include "overlay_41_sprite_resource_collection.h"
#include "touch_hitbox_controller.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"
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

void ov70_0225E27C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2) {
    UnkStruct_ov70_02260744 *v0 = param2;

    v0->unk_03 = param1;

    if (v0->unk_03 == 0) {
        v0->unk_03 = 2;
    } else {
        if (v0->unk_03 == 3) {
            v0->unk_03 = 1;
        }
    }
}
