#include "global.h"

#include "comm_cmd_manager.h"
#include "heap.h"
#include "unk_02033AE0.h"

void sub_02034154(void) {
    if (_021D412C != NULL) {
        Heap_Free(_021D412C);
        _021D412C = NULL;
    }
}
