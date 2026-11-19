#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_49_unk_struct_ov70_02263910.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "overlay_45_wifi_plaza_system.h"
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

struct ColoredArrow;

struct G3DPipelineBuffers;

struct ListMenu;

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

typedef struct ColoredArrow ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

PlayerProfile *PlayerProfile_New(enum HeapID heapID);

u32 ov45_0222A53C(const WiFiPlazaSystem *param0);

const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);

WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0);

void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *param1, enum HeapID heapID);

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov49_0225B3C8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2);

void ov49_0225A334(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);

String *ov49_0225B3D8(UnkStruct_ov70_0225F208 *param0, int param1, u32 param2);
String *ov49_0225B3E8(UnkStruct_ov70_0225F208 *param0, int param1, u32 param2);
String *ov49_0225B3F8(UnkStruct_ov70_0225F208 *param0, int param1, u32 param2);
void BufferJPGreeting(MessageFormat *template, u32 idx, u32 greeting);
void BufferENGreeting(MessageFormat *template, u32 idx, u32 greeting);
void BufferFRGreeting(MessageFormat *template, u32 idx, u32 greeting);
void BufferITGreeting(MessageFormat *template, u32 idx, u32 greeting);
void BufferDEGreeting(MessageFormat *template, u32 idx, u32 greeting);
void BufferSPGreeting(MessageFormat *template, u32 idx, u32 greeting);
u32 ov45_0222D7C0(int param0);
void BufferTypeName(MessageFormat *template, u32 idx, u32 type);
String *ov49_0225A37C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);
String *ov49_0225A38C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);
String *ov49_0225A39C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2);
void ov49_0225A3AC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);
void ov49_0225A3BC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);
void ov49_0225A3CC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);
void ov49_0225A3DC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);
void ov49_0225A3EC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);
void ov49_0225A3FC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2);
void ov49_0225A40C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, int param2);

void ov49_0225A334(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2) {
    PlayerProfile *v0;
    const WiFiPlazaProfile *v1;

    v0 = PlayerProfile_New(HEAP_ID_119);

    if (param1 == ov45_0222A53C(param0->unk_34)) {
        v1 = ov45_0222A5C0(param0->unk_34);
    } else {
        v1 = ov45_0222A578(param0->unk_34, param1);
    }

    ov45_0222A844(v1, v0, HEAP_ID_119);
    ov49_0225B3C8(&param0->unk_340, v0, param2);
    Heap_Free(v0);
}

String *ov49_0225A37C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2) {
    return ov49_0225B3D8(&param0->unk_340, param1, param2);
}

String *ov49_0225A38C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2) {
    return ov49_0225B3E8(&param0->unk_340, param1, param2);
}

String *ov49_0225A39C(UnkStruct_ov70_0225DEE8 *param0, int param1, u32 param2) {
    return ov49_0225B3F8(&param0->unk_340, param1, param2);
}

void ov49_0225A3AC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2) {
    BufferJPGreeting(param0->unk_340.unk_00, param1, param2);
}

void ov49_0225A3BC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2) {
    BufferENGreeting(param0->unk_340.unk_00, param1, param2);
}

void ov49_0225A3CC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2) {
    BufferFRGreeting(param0->unk_340.unk_00, param1, param2);
}

void ov49_0225A3DC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2) {
    BufferITGreeting(param0->unk_340.unk_00, param1, param2);
}

void ov49_0225A3EC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2) {
    BufferDEGreeting(param0->unk_340.unk_00, param1, param2);
}

void ov49_0225A3FC(UnkStruct_ov70_0225DEE8 *param0, u32 param1, u32 param2) {
    BufferSPGreeting(param0->unk_340.unk_00, param1, param2);
}

void ov49_0225A40C(UnkStruct_ov70_0225DEE8 *param0, u32 param1, int param2) {
    param2 = ov45_0222D7C0(param2);
    BufferTypeName(param0->unk_340.unk_00, param1, param2);
}
