#include "global.h"

#include "encounter.h"
#include "field_communication_manager.h"
#include "heap.h"
#include "unk_02037C94.h"
#include "unk_02058034.h"

void sub_020586EC(void);

void sub_02058608(void);

BOOL sub_02058740(void) {
    if ((_021D41C8 != 0) && (_021D41C8->task == sub_020586EC || (_021D41C8->task == sub_02058608))) {
        return 1;
    }
    return 0;
}

s32 sub_02058098(void);

void sub_0205876C(void);

void sub_0205876C(void) {
    long long v0;
    FieldSystem *fieldSystem;
    Party *party;
    s32 v2;
    s32 battleType;
    u16 v1;
    v1 = _021D41C8->timer;
    if (v1 != 0) {
        _021D41C8->timer = (u16)(v1 - 1);
        return;
    }
    battleType = 5;
    v2 = (s32)sub_0203993C();
    switch (v2) {
    case 4:

    case 5:
        battleType = 0xF;
        break;

    case 38:

    case 2:
        battleType = 7;
        break;
    }

    sub_02039980(&v0);
    party = _021D41C8->party;
    fieldSystem = _021D41C8->fieldSystem;
    if (party == 0L) {
        sub_02051598(fieldSystem, &v0, battleType);
    } else {
        sub_020515FC(fieldSystem, party, battleType);
        Heap_Free(_021D41C8->party);
        _021D41C8->party = 0L;
    }
    sub_02058098();
}
