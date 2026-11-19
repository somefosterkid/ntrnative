#include "global.h"

extern s32 _020FD4AC;

s32 GetDeltaXByFacingDirection(s32 direction);

s32 GetDeltaXByFacingDirection(s32 direction) {
    return *(s32 *)(&_020FD4AC + direction);
}

extern s32 _020FD49C;

s32 GetDeltaYByFacingDirection(s32 direction);

s32 GetDeltaYByFacingDirection(s32 direction) {
    return *(s32 *)(&_020FD49C + direction);
}
