#include "global.h"
struct UnkStruct_02089688;

#include "pm_string.h"
#include "sprite_system.h"
#include "unk_struct_02089688.h"

s32 sub_02085808(void *, s32);

s32 sub_02086D98(s32, s32);

void sub_02085F80(struct UnkStruct_02089688 *arg0);

void sub_02085F80(UnkStruct_02089688 *arg0) {
    String *string;
    s32 v0;
    void *v1;

    string = String_New(0x64U, HEAP_ID_108);
    *((s32 *)&arg0->unk_268[1].unk_00) = 1;
    v0 = 0;
    *((s8 *)&arg0->unk_268[1].unk_14.unk_04) = 0;
    if ((s32) *((s32 *)&arg0->unk_2D0) > 0) {
        v1 = arg0;
        do {
            if (*(s32 *)v1 == 0) {
                *(s32 *)v1 = 1;
                ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)v1 + 0xC), sub_02086D98(1, *(s32 *)((u8 *)v1 + 8)));
            }
            String16_FormatInteger(string, *(s32 *)v1 - 1, 1U, PRINTING_MODE_RIGHT_ALIGN, 1);
            String_Cat(arg0->unk_3B8.unk_1C, string);
            v0 += 1;
            v1 += 0x1C;
        } while (v0 < (s32) *((s32 *)&arg0->unk_2D0));
    }
    String_Delete(string);
    sub_02085808(arg0, 3);
}
