#include "global.h"
#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_49_unk_struct_ov70_02263910.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
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


struct G3DPipelineBuffers;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
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
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;
typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;
typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;
typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
void ov49_0225AFD8(UnkStruct_ov70_0225EC20 *param0);
void ov49_0225B014(UnkStruct_ov70_0225EC20 *param0, const String *param1, u16 *param2);
BOOL ov49_0225B06C(const UnkStruct_ov70_0225EC20 *param0, u32 param1);
void ov49_0225A1D4(UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A1E4(UnkStruct_ov70_0225DEE8 *param0, const String *param1, void *param2);
BOOL ov49_0225A1F4(const UnkStruct_ov70_0225DEE8 *param0, u32 param1);

void ov49_0225AF30(UnkStruct_ov70_0225EC20 *, const ListMenuTemplate *, UnkStruct_ov70_0225E4EC *, u32, u32, u32, u8, u8, u8);

void ov49_0225A1A4(UnkStruct_ov70_0225DEE8 *param_1, const ListMenuTemplate *param_2, u32 param_3, u32 param_4, u8 param_5, u8 param_6, u8 param_7);

void ov49_0225A1A4(UnkStruct_ov70_0225DEE8 *param_1, const ListMenuTemplate *param_2, u32 param_3, u32 param_4, u8 param_5, u8 param_6, u8 param_7) {
    ov49_0225AF30(&param_1->unk_39C, param_2, &param_1->unk_3C, param_3, param_4, 0x77, param_5, param_6, param_7);
    return;
}

void ov49_0225A1D4(UnkStruct_ov70_0225DEE8 *param0) {
    ov49_0225AFD8(&param0->unk_39C);
}

void ov49_0225A1E4(UnkStruct_ov70_0225DEE8 *param0, const String *param1, void *param2) {
    ov49_0225B014(&param0->unk_39C, param1, param2);
}

BOOL ov49_0225A1F4(const UnkStruct_ov70_0225DEE8 *param0, u32 param1) {
    return ov49_0225B06C(&param0->unk_39C, param1);
}
