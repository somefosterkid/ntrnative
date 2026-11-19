#include "global.h"

#include "field_communication_manager.h"
#include "trainer_case.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"

void sub_020588CC(int netId, int unused1, void *unused2, void *unused3);

void sub_020588CC(int netId, int unused1, void *unused2, void *unused3) {
    _021D41C8->trainerCaseCopied[netId] = 1;
}

u8 *sub_020588DC(int netId, void *unused1, int unused2);

u8 *sub_020588DC(int netId, void *unused1, int unused2) {
    GF_ASSERT(netId < 4);
    return (u8 *)_021D41C8->trainerCase[netId];
}

void sub_020588F8(void);

void sub_020582F4(FieldCommTask task, u16 time);

BOOL sub_02036FD8(int cmd, const void *data, int size);

void sub_02058930(void);

void sub_020588F8(void) {
    int netId = sub_0203769C();

    if (sub_02037B38(95)) {
        sub_02036FD8(88, _021D41C8->trainerCase[netId], sizeof(TrainerCase));
        sub_020582F4(sub_02058930, 0);
    }
}

void sub_020589D8(void);

void sub_02058930(void) {
    int netId;

    for (netId = 0; netId < sub_02037454(); netId++) {
        if (!_021D41C8->trainerCaseCopied[netId]) {
            return;
        }
    }

    sub_02037AC0(97);
    sub_020582F4(sub_020589D8, 0);
}

void sub_0205896C(void);

BOOL sub_02036010(void);

void sub_020589F4(void);

void sub_0205896C(void) {
    if (!sub_02036010()) {
        if (_021D41C8->timer != 0) {
            _021D41C8->timer--;
        }

        if (_021D41C8->timer == 90) {
            sub_02037AC0(5);
        }

        if (sub_02037B38(5)) {
            sub_020582F4(sub_020589F4, 0);
        }
    }
}
