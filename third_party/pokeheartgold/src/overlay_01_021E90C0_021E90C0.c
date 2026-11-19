#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "heap.h"
#include "overlay_01_021E90C0.h"

FieldEnvSubUnk18 *ov01_021E90C0(void);

FieldEnvSubUnk18 *ov01_021E90C0(void) {
    MapPropDoorAnimEnv *env;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(MapPropDoorAnimEnv));
    env->base.state = 0;
    env->unk_1C = 0;
    return &env->base;
}
