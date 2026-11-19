#include "global.h"

#include "field_system.h"

s32 sub_02054654(const FieldSystem *fieldSystem, s32, s32, s32, s32, u8 *);

const fx32 sub_02054774(const FieldSystem *fieldSystem, fx32 objectHeight, fx32 objectX, fx32 objectZ, u8 *newObjectHeightSourceOut);

const fx32 sub_02054774(const FieldSystem *fieldSystem, fx32 objectHeight, fx32 objectX, fx32 objectZ, u8 *newObjectHeightSourceOut) {
    return sub_02054654(fieldSystem, 0, objectHeight, objectX, objectZ, newObjectHeightSourceOut);
}
