#include "global.h"

#include "library/spl_emitter.h"

struct GenericEmitterCallbackData;

typedef struct GenericEmitterCallbackData GenericEmitterCallbackData;

extern BOOL (*const ov07_022363A0[9])(SPLEmitter *, GenericEmitterCallbackData *);

void ov07_02221C74(int funcID, SPLEmitter *emitter, GenericEmitterCallbackData *data);

void ov07_02221C74(int funcID, SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    ov07_022363A0[funcID](emitter, data);
}
