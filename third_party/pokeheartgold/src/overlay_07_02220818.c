#include "global.h"

#include "library/spl_emitter.h"

void ov07_022207D0(SPLEmitter *emitter, int behaviorMode, int targetMode);

void ov07_02220818(SPLEmitter *emitter);
void ov07_02220824(SPLEmitter *emitter);
void ov07_02220830(SPLEmitter *emitter);
void ov07_0222083C(SPLEmitter *emitter);

void ov07_02220818(SPLEmitter *emitter) {
    ov07_022207D0(emitter, 1, 2);
}

void ov07_02220824(SPLEmitter *emitter) {
    ov07_022207D0(emitter, 1, 3);
}

void ov07_02220830(SPLEmitter *emitter) {
    ov07_022207D0(emitter, 1, 4);
}

void ov07_0222083C(SPLEmitter *emitter) {
    ov07_022207D0(emitter, 1, 5);
}
