#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"

struct UnkStruct_0205C95C_t;
typedef struct UnkStruct_0205C95C_t UnkStruct_0205C95C;
typedef struct {
    String *unk_00;
    String *unk_04;
    String *unk_08;
    u32 unk_0C;
    int unk_10;
    MailMessage unk_14;
} UnkStruct_0205C924;
struct UnkStruct_0205C95C_t {
    UnkStruct_0205C924 unk_00[30];
    int unk_348;
    int unk_34C;
};
void sub_0205B338(UnkStruct_0205C924 *param0);

void sub_0205B35C(UnkStruct_0205C95C *param0);
void sub_0205B380(UnkStruct_0205C924 *param0);

void sub_0205B35C(UnkStruct_0205C95C *param0) {
    int v0;

    for (v0 = 0; v0 < 30; v0++) {
        sub_0205B338(&param0->unk_00[v0]);
    }

    param0->unk_348 = 0;
    param0->unk_34C = 0;
}

void sub_0205B380(UnkStruct_0205C924 *param0) {
    Heap_Free(param0->unk_00);

    if (param0->unk_04 != 0L) {
        String_Delete(param0->unk_04);
    }

    if (param0->unk_08 != 0L) {
        String_Delete(param0->unk_08);
    }
}
