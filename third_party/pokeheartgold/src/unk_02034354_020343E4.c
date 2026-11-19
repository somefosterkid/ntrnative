#include "global.h"

#include "heap.h"
#include "unk_02034354.h"
#include "unk_02034354_internal.h"

void sub_020343E4(void) {
    int netId;

    if (_021D4130 != NULL) {
        for (netId = 0; netId < 8; netId++) {
            _021D4130->trainerInfo[netId] = NULL;
        }
        if (_021D4130 != NULL) {
            Heap_Free(_021D4130);
        }
        _021D4130 = NULL;
    }
}
