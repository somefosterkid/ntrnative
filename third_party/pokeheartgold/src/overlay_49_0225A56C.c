#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

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
#include "overlay_45_wifi_plaza_system.h"
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
struct WiFiPlazaProfile;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;
typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;
typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;
typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;
u32 ov45_0222A53C(const WiFiPlazaSystem *param0);
u32 ov45_0222AAC8(const WiFiPlazaProfile *profile);
void ov45_0222AD70(WiFiPlazaSystem *param0, u32 param1);
typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;
void ov49_022653C0(UnkStruct_ov70_02269204 *param0, u32 param1, u32 param2);
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
BOOL ov49_022653F0(const UnkStruct_ov70_02269204 *param0, u32 param1);

void ov49_0225A56C(UnkStruct_ov70_0225DEE8 *param0, u32 param1);
BOOL ov49_0225A5AC(const UnkStruct_ov70_0225DEE8 *param0, u32 param1);

s32 ov49_02258BE0(UnkStruct_ov70_0225CA20 *);
s32 ov49_0225A840(void *);
s32 ov49_0225E3AC(void *);
void ov49_0225A5C8(void *param0);

void ov49_0225A56C(UnkStruct_ov70_0225DEE8 *param0, u32 param1) {
    const WiFiPlazaProfile *v0;
    u32 v1;

    ((param1 < 20) ? (void)0 : GF_AssertFail());

    v0 = ov45_0222A578(param0->unk_34, param1);
    v1 = ov45_0222AAC8(v0);

    ov49_022653C0(param0->unk_438, param1, v1);

    if (param1 == ov45_0222A53C(param0->unk_34)) {
        ov45_0222AD70(param0->unk_34, v1);
    }
}

BOOL ov49_0225A5AC(const UnkStruct_ov70_0225DEE8 *param0, u32 param1) {
    ((param1 < 20) ? (void)0 : GF_AssertFail());
    return ov49_022653F0(param0->unk_438, param1);
}

void ov49_0225A5C8(void *param0) {
    UnkStruct_ov70_0225DEE8 *arg0 = param0;

    ov49_0225E3AC(arg0->unk_448);
    ov49_02258BE0(arg0->unk_444);
    ov49_0225A840(&arg0->unk_3C);
}
