#include "global.h"

#include "overlay_27.h"
#include "overlay_27_ov27task.h"
#include "pm_string.h"

void ov27_0225BC34(Ov27Task *arg0) {
    String *string;
    s32 v1;
    s32 v3;

    String_Delete(arg0->unk_4B0[6]);
    String_Delete(arg0->unk_4B0[5]);
    v3 = 0;
    do {
        String_Delete(arg0->unk_4B0[v3]);
        v3 += 1;
    } while (v3 < 5);
    v1 = 0;
    do {
        string = arg0->unk_470[v1].unk_4;
        if (string != NULL) {
            String_Delete(string);
        }
        v1 += 1;
    } while (v1 < 7);
}
