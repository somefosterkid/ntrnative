#include "global.h"

#include "comm_server_client.h"
#include "constants/heap.h"

#include "heap.h"
#include "mail_message.h"
#include "player_data.h"
#include "pokemon_types_def.h"
#include "save_link_ruleset.h"

u32 sub_020335B4(void);

void sub_02034DF0(s32);

void sub_02034B0C(PlayerProfile *playerProfile, s32 arg1);

void sub_02034B0C(PlayerProfile *playerProfile, s32 arg1) {
    u32 uVar1;
    void *pvVar2;

    if (_021D4134.sCommServerClient == NULL) {
        _021D4134.sCommServerClient = Heap_Alloc(HEAP_ID_15, sizeof(CommServerClient));
        MI_CpuFill8(_021D4134.sCommServerClient, 0, sizeof(CommServerClient));
        uVar1 = sub_020335B4();
        pvVar2 = Heap_Alloc(HEAP_ID_15, uVar1);
        _021D4134.sCommServerClient->unk_D64 = pvVar2;
        uVar1 = sub_020335B4();
        MI_CpuFill8(_021D4134.sCommServerClient->unk_D64, 0, uVar1);
        uVar1 = LinkBattleRuleset_sizeof();
        pvVar2 = Heap_Alloc(HEAP_ID_15, uVar1);
        _021D4134.sCommServerClient->unk_D7C = pvVar2;
        uVar1 = LinkBattleRuleset_sizeof();
        MI_CpuFill8(_021D4134.sCommServerClient->unk_D7C, 0, uVar1);
        pvVar2 = Heap_Alloc(HEAP_ID_15, 0x90);
        _021D4134.sCommServerClient->unk_D84 = pvVar2;
#if defined(SDK_PORT) || defined(NONMATCHING)
        _021D4134.sCommServerClient->unk_D88 = (CommGameInfo *)((u8 *)_021D4134.sCommServerClient->unk_D84 + (0x20 - ((size_t)_021D4134.sCommServerClient->unk_D84 & 0x1f)));
#else
        _021D4134.sCommServerClient->unk_D88 = (CommGameInfo *)((0x20 - ((u32)_021D4134.sCommServerClient->unk_D84 & 0x1f)) + (u32)_021D4134.sCommServerClient->unk_D84);
#endif
        _021D4134.sCommServerClient->unk_D80 = 0x333;
        _021D4134.sCommServerClient->unk_D78 = playerProfile;
        MailMsg_Init(&_021D4134.sCommServerClient->unk_D68);
        sub_02034DF0(arg1);
        _021D4134.tgid = WM_GetNextTgid();
    }
    return;
}
