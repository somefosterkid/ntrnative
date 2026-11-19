#include "global.h"
#include "string_util.h"
#include "unk_02031B0C.h"

s32 sub_02032504(s32, u32, u16);

void sub_02032588(u16 *arg0, void *arg1, u16 arg2);

void sub_020325CC(ApricornRecord *arg0);

void sub_02032588(u16 *arg0, void *arg1, u16 arg2) {
    *arg0 = ((*arg0) & (~0xF)) | (0xF & (*((u8 *)arg1)));
    *arg0 = ((*arg0) & 0xFFFF00FF) | ((u16)((*((u8 *)(((u8 *)arg1) + 1))) << 8));
    *arg0 = ((u8)(sub_02032504((u8)arg2, *((u8 *)(((u8 *)arg1) + 1)), arg2) * 0x10)) | ((*arg0) & (~0xF0));
}

void sub_020325CC(ApricornRecord *dest) {
    MI_CpuFill8(dest, 0U, sizeof(ApricornRecord));
    sub_02031B5C(&dest->data);
    StringFillEOS(dest->name, 8U);
}
