#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"

struct ParticleSystem;

typedef struct ParticleSystem ParticleSystem;

void sub_020153D8(ParticleSystem *particleSystem);

#include "particle_system_globals.h"

int sub_0201543C(void);

void sub_02015414(ParticleSystem *particleSystem);
int sub_02015460(void);

int sub_0201543C(void) {
    int count = 0;
    for (int i = 0; i < 16; i++) {
        if (_021D10A8[i] != 0L) {
            sub_020153D8(_021D10A8[i]);
            count++;
        }
    }

    return count;
}

int sub_02015460(void) {
    int count = 0;
    for (int i = 0; i < 16; i++) {
        if (_021D10A8[i] != 0L) {
            sub_02015414(_021D10A8[i]);
            count++;
        }
    }

    return count;
}
