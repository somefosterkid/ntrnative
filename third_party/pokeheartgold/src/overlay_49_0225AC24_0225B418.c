#include "global.h"

#include "message_format.h"

void ov49_0225B418(MessageFormat **arg0, u32 arg1, u32 arg2, u32 arg3);

void ov49_0225B418(MessageFormat **arg0, u32 countryId, u32 cityId, u32 fieldno) {
    BufferCityName(*arg0, fieldno, countryId, cityId);
}
