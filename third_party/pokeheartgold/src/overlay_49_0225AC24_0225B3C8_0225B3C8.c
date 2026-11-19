#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "player_data.h"

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

typedef struct MsgData MsgData;

typedef struct MessageFormat MessageFormat;

void BufferPlayersName(MessageFormat *template, u32 idx, const PlayerProfile *playerInfo);

void ov49_0225B3C8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2);

void ov49_0225B3C8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2) {
    BufferPlayersName(param0->unk_00, param2, param1);
}

void BufferWiFiPlazaActivityName(MessageFormat *template, u32 idx, const PlayerProfile *playerInfo);

void ov49_0225B3D8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2);

void ov49_0225B3D8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2) {
    BufferWiFiPlazaActivityName(param0->unk_00, param2, param1);
}

void BufferWiFiPlazaEventName(MessageFormat *template, u32 idx, const PlayerProfile *playerInfo);

void ov49_0225B3E8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2);

void ov49_0225B3E8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2) {
    BufferWiFiPlazaEventName(param0->unk_00, param2, param1);
}

void BufferWiFiPlazaInstrumentName(MessageFormat *template, u32 idx, const PlayerProfile *playerInfo);

void ov49_0225B3F8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2);

void ov49_0225B3F8(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2) {
    BufferWiFiPlazaInstrumentName(param0->unk_00, param2, param1);
}

void BufferCountryName(MessageFormat *template, u32 idx, const PlayerProfile *playerInfo);

void ov49_0225B408(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2);

void ov49_0225B408(UnkStruct_ov70_0225F208 *param0, const PlayerProfile *param1, u32 param2) {
    BufferCountryName(param0->unk_00, param2, param1);
}
