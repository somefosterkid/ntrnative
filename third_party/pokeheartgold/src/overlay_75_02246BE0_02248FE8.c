#include "global.h"

#include "message_format.h"
#include "player_data.h"
#include "save.h"
#include "unk_02005D10.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

s32 ov75_0224785C(void *);

s32 ov75_02247878(void *);

s32 ov75_022494CC(void *, void *, s32, s32, u16);

s32 ov75_02249534(s32);

s32 ov75_02248FE8(void *arg0);

s32 ov75_02248FE8(void *arg0) {
    u32 v0;

    v0 = *((u32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94);
    switch (v0) {
    case 0:
        ov75_022494CC(arg0, *((void **)&((UnkStruct_ov98_02247704 *)arg0)->unk_34), 0x20, 0, 0xF0F);
        ov75_0224785C(arg0);
        *((u32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) = (u32)(*((u32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) + 1);
        break;
    case 1:
        if (SaveGameNormal(*(SaveData **)((u8 *)(*(void **)arg0) + 4)) == 2) {
            BufferPlayersName(((MessageFormat *)((UnkStruct_ov98_02247704 *)arg0)->unk_20), 0U, Save_PlayerData_GetProfile(*(SaveData **)((u8 *)(*(void **)arg0) + 4)));
            ov75_022494CC(arg0, *((void **)&((UnkStruct_ov98_02247704 *)arg0)->unk_34), 0x21, 0, 0xF0F);
            PlaySE(0x61AU);
            Save_ClearStatusFlags(*(SaveData **)((u8 *)(*(void **)arg0) + 4));
        } else {
            ov75_022494CC(arg0, *((void **)&((UnkStruct_ov98_02247704 *)arg0)->unk_34), 0x22, 0, 0xF0F);
        }
        ov75_02247878(arg0);
        *((u32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) = (u32)(*((u32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) + 1);
        break;
    case 2:
        if (ov75_02249534(*((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_44)) == 0) {
            *((u32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) = (u32)(*((u32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_94) + 1);
        }
        break;
    case 3:
        *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_98) = (s32)(*((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_98) + 1);
        if ((s32) *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_98) > 0x3C) {
            *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_98) = 0;
            *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_08) = (s32) *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_0C);
        }
        break;
    }
    return 0;
}
