#include "global.h"

#include "comm_server_client.h"

s32 sub_02034C94(void);

void sub_0203528C(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0203528C(void) {
    s32 var_r4;
    u16 temp_r6;

    sub_02034C94();
    var_r4 = 0;
    do {
        temp_r6 = (s16)_021D4134.sCommServerClient->unk_D44[var_r4];
        if ((temp_r6 != 0) && (temp_r6 != 0)) {
            _021D4134.sCommServerClient->unk_D44[var_r4] = (u16)(_021D4134.sCommServerClient->unk_D44[var_r4] - 1);
            if (_021D4134.sCommServerClient->unk_D44[var_r4] == 0) {
                _021D4134.sCommServerClient->unk_D74 = 1;
            }
        }
        var_r4 += 1;
    } while (var_r4 < 0x10);
}
#else
void sub_0203528C(void) {
    s32 var_r4;
    s32 var_r5;
    u16 temp_r6;
    void *temp_r3;

    sub_02034C94();
    var_r4 = 0;
    var_r5 = 0;
    do {
        temp_r3 = *(void **)((u8 *)(&_021D4134) + 8) + var_r5;
        temp_r6 = *(s16 *)((u8 *)temp_r3 + 0xD44);
        if ((temp_r6 != 0) && (temp_r6 != 0)) {
            *(u16 *)((u8 *)temp_r3 + 0xD44) = (u16)(*(u16 *)((u8 *)temp_r3 + 0xD44) - 1);
            if (*(u16 *)((u8 *)(*(void **)((u8 *)(&_021D4134) + 8) + var_r5) + 0xD44) == 0) {
                *(s8 *)((u8 *)(*(void **)((u8 *)(&_021D4134) + 8)) + 0xD74) = 1;
            }
        }
        var_r4 += 1;
        var_r5 += 2;
    } while (var_r4 < 0x10);
}
#endif
