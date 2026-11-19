#include "global.h"

#include "field_communication_manager.h"
#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "script_manager.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"

u8 sub_02037B5C(s32);

void sub_0205857C(void);

void sub_0205857C(void) {
    s32 sp0;
    FieldSystem *temp_r1;
    s32 i;
    void *temp_r0;

    sp0 = 0;
    if (sub_02037454() > 0) {
        do {
            if ((sp0 != sub_0203769C()) && (sub_02037B5C(sp0) == 0x5E)) {
                temp_r1 = _021D41C8->fieldSystem;
                if ((temp_r1->taskman == NULL) && (temp_r1->unkD2_0 == 0)) {
                    i = 0;
                    do {
                        temp_r0 = _021D41C8->trainerCase[i];
                        if (temp_r0 != NULL) {
                            Heap_Free(temp_r0);
                            _021D41C8->trainerCase[i] = NULL;
                        }
                        i += 1;
                    } while (i < FIELD_COMM_MAX_PLAYERS);
                    StartMapSceneScript(_021D41C8->fieldSystem, 0x238EU, NULL);
                }
            }
            sp0 += 1;
        } while (sp0 < sub_02037454());
    }
    sub_020399FC(4, _021D41C8->fieldSystem->bgConfig);
}
