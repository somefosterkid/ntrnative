#include "global.h"
struct UnkStruct_ov114_0225D084;
struct UnkStruct_ov70_0225CA20_t;

#include "message_format.h"
#include "overlay_90.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"

void ov90_02259220(struct UnkStruct_ov70_0225CA20_t *arg0, u32 arg1, u32 arg2);

void ov90_02259220(UnkStruct_ov70_0225CA20_t *arg0, u32 countryId, u32 cityId) {
    BufferCityName(*((MessageFormat * *)&arg0->unk_04), 0U, countryId, cityId);
}

void ov90_02259234(struct UnkStruct_ov114_0225D084 *arg0, s32 arg1, u32 arg2);

void ov90_02259234(UnkStruct_ov114_0225D084 *arg0, s32 num, u32 numDigits) {
    BufferIntegerAsString(*((MessageFormat * *)&arg0->unk_04), 0U, num, numDigits, PRINTING_MODE_RIGHT_ALIGN, 1);
}

void ov90_02259250(UnkStruct_ov114_0225D084 *arg0, s32 num) {
    BufferIntegerAsString(arg0->unk_04, 0U, num, 2U, PRINTING_MODE_RIGHT_ALIGN, 1);
}
