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
void ov49_0225B8E0(UnkStruct_ov70_0225F350 *param0);
void ov49_0225B8F8(UnkStruct_ov70_0225F350 *param0);
void ov49_0225B8FC(UnkStruct_ov70_0225F350 *param0);
void ov49_0225B914(UnkStruct_ov70_0225F350 *param0);
void ov49_0225B8EC(UnkStruct_ov70_0225F350 *param0);
BOOL ov49_02268968(const UnkStruct_ov70_0226C6F8 *param0, u32 param1, int param2);
void ov49_0225BA34(UnkStruct_ov70_0225F350 *param0, BOOL param1);
void ov49_0225CB68(UnkStruct_ov70_02260744 *param0);

void ov49_0225A4D0(UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A4E0(UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A4F0(UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A500(UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A510(UnkStruct_ov70_0225DEE8 *param0);
BOOL ov49_0225A520(const UnkStruct_ov70_0225DEE8 *param0, u32 param1);
void ov49_0225A530(UnkStruct_ov70_0225DEE8 *param0);
void ov49_0225A53C(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);
void ov49_0225A54C(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225A4D0(UnkStruct_ov70_0225DEE8 *param0) {
    ov49_0225B8E0(&param0->unk_1E8);
}

void ov49_0225A4E0(UnkStruct_ov70_0225DEE8 *param0) {
    ov49_0225B8F8(&param0->unk_1E8);
}

void ov49_0225A4F0(UnkStruct_ov70_0225DEE8 *param0) {
    ov49_0225B8FC(&param0->unk_1E8);
}

void ov49_0225A500(UnkStruct_ov70_0225DEE8 *param0) {
    ov49_0225B914(&param0->unk_1E8);
}

void ov49_0225A510(UnkStruct_ov70_0225DEE8 *param0) {
    ov49_0225B8EC(&param0->unk_1E8);
}

BOOL ov49_0225A520(const UnkStruct_ov70_0225DEE8 *param0, u32 param1) {
    return ov49_02268968(param0->unk_440, param1, 1);
}

void ov49_0225A530(UnkStruct_ov70_0225DEE8 *param0) {
    param0->unk_1E8.unk_06 = 1;
}

void ov49_0225A53C(UnkStruct_ov70_0225DEE8 *param0, BOOL param1) {
    ov49_0225BA34(&param0->unk_1E8, param1);
}

void ov49_0225A54C(UnkStruct_ov70_0225DEE8 *param0) {
    ov49_0225CB68(&param0->unk_1E8.unk_114);
}
