#include "global.h"
#include "comm_server_client.h"
#include "pokemon_types_def.h"
#include "unk_02032844.h"
#include "unk_02034B0C.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: host-layout copy of the asm's .bss object (asm/unk_02034B0C.s: 0xC bytes on the DS, 4-aligned: the
// CommServerClient pointer sat misaligned). The asm label stays for the DS build.
CommServerClientStatics _021D4134;
#endif

BOOL sub_020355DC(u16 netId);

int sub_02035610(void);

BOOL sub_02033920(void);
BOOL sub_0203567C(void);
BOOL sub_0203569C(void);

int sub_02035610(void) {
    int v0 = 0, i;

    for (i = 0; i < (7 + 1); i++) {
        if (sub_020355DC(i)) {
            v0++;
        }
    }

    return v0;
}

BOOL sub_02035630(void) {
    if (_021D4134.sCommServerClient && (_021D4134.sCommServerClient->unk_D92 == 3)) {
        return TRUE;
    }

    return FALSE;
}

BOOL sub_02035650(void) {
    if (_021D4134.sCommServerClient) {
        return TRUE;
    }

    return FALSE;
}

BOOL sub_02035664(void) {
    if (_021D4134.sCommServerClient) {
        return sub_02033920();
    }

    return TRUE;
}

BOOL sub_0203567C(void) {
    if (_021D4134.sCommServerClient) {
        return sub_02033250() & 0xFFFE;
    }

    return FALSE;
}

BOOL sub_0203569C(void) {
    if (_021D4134.sCommServerClient && _021D4134.sCommServerClient->unk_D95_0) {
        return TRUE;
    }

    return FALSE;
}
