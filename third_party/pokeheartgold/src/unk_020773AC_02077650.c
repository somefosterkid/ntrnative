#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "particle_system.h"

struct ParticleSystem;
typedef struct {
    enum HeapID heapID;
    int unk_04;
} UnkStruct_0207C894;
s32 sub_020154B0(ParticleSystem *particleSystem);
typedef struct {
    UnkStruct_0207C894 unk_00;
    SPLEmitter *unk_08;
    ParticleSystem *unk_0C;
} UnkStruct_0207C8C4;

BOOL sub_02077650(UnkStruct_0207C8C4 *param0);

BOOL sub_02077650(UnkStruct_0207C8C4 *param0) {
    UnkStruct_0207C8C4 *v0 = param0;

    if (sub_020154B0(v0->unk_0C) == 0) {
        return 0;
    }

    return 1;
}
