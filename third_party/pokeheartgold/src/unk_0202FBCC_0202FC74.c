#include "global.h"

#include "constants/heap.h"

#include "pokemon_types_def.h"

struct BattleRecording;
typedef struct BattleRecording BattleRecording;
extern BattleRecording *_021D2AF8;

void *sub_0202FC74(void);

void *sub_0202FC74(void) {
    u8 *v0;

    ((_021D2AF8) ? (void)0 : GF_AssertFail());

    v0 = (u8 *)_021D2AF8;
    return &v0[sizeof(u32)];
}
