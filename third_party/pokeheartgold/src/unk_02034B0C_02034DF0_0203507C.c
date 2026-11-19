#include "global.h"

#include "comm_server_client.h"
#include "heap.h"
#include "unk_02035900.h"

s32 sub_02033858(void);

s32 sub_0203507C(void);

s32 sub_0203507C(void) {
    if ((_021D4134.sCommServerClient != NULL) && (_021D4134.sCommServerClient->unk_D92 == 0)) {
        _021D4134.sCommServerClient->unk_D92 = 1U;
        sub_02033858();
        return 1;
    }
    return 0;
}

s32 sub_02034DF0(s32);

void sub_020350A8(s32 arg0);

void sub_020350A8(s32 arg0) {
    if (_021D4134.sCommServerClient != NULL) {
        if (arg0 != 0) {
            _021D4134.sCommServerClient->unk_D92 = 2;
            return;
        }
        _021D4134.sCommServerClient->unk_D92 = 0;
        sub_02034DF0(1);
    }
}

void sub_020350D4(void);

void sub_020350D4(void) {
    Heap_Free(_021D4134.sCommServerClient->unk_D7C);
    Heap_Free(_021D4134.sCommServerClient->unk_D64);
    Heap_Free(_021D4134.sCommServerClient->unk_D84);
    Heap_Free(_021D4134.sCommServerClient);
    _021D4134.sCommServerClient = NULL;
}

s32 sub_0203511C(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0203511C(void) {
    s32 v0;
    s32 v1;
    u16 *v2;

    if (sub_02037474() == 0) {
        return 0;
    }
    v0 = 0;
    v2 = _021D4134.sCommServerClient->unk_D44;
    v1 = 0;
    do {
        if (*v2 != 0) {
            v0 += 1;
        }
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    return v0;
}
#else
s32 sub_0203511C(void) {
    s32 v0;
    s32 v1;
    void *v2;

    if (sub_02037474() == 0) {
        return 0;
    }
    v0 = 0;
    v2 = *(void **)((u8 *)(&_021D4134) + 8);
    v1 = 0;
    do {
        if (*(u16 *)((u8 *)v2 + 0xD44) != 0) {
            v0 += 1;
        }
        v1 += 1;
        v2 += 2;
    } while (v1 < 0x10);
    return v0;
}
#endif
