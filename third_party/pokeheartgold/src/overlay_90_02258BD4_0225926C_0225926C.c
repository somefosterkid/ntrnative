#include "global.h"

#include "pm_string.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "player_data.h"

struct MsgData;

struct MessageFormat;

typedef struct MsgData MsgData;

typedef struct MessageFormat MessageFormat;

void BufferWiFiPlazaInstrumentName(MessageFormat *template, u32 idx, u32 item);

void ov90_0225926C(UnkStruct_ov114_0225D084 *param0, u32 param1);

void ov90_0225926C(UnkStruct_ov114_0225D084 *param0, u32 param1) {
    BufferWiFiPlazaInstrumentName(param0->unk_04, 1, param1);
}

struct PlayerProfile;

void BufferWiFiPlazaActivityName(MessageFormat *template, u32 idx, const PlayerProfile *playerInfo);

void ov90_0225927C(UnkStruct_ov114_0225D084 *param0, const PlayerProfile *param1);

void ov90_0225927C(UnkStruct_ov114_0225D084 *param0, const PlayerProfile *param1) {
    BufferWiFiPlazaActivityName(param0->unk_04, 0, param1);
}
