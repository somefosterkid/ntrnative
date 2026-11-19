#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"
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
void ov49_0225A04C(UnkStruct_ov70_0225DEE8 *param0, u8 param1, BOOL param2);

void ov49_0225A04C(UnkStruct_ov70_0225DEE8 *param0, u8 param1, BOOL param2) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());
    param0->unk_0C.unk_00[param1] = param2;
}
