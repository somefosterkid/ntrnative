#include "global.h"

#include "sys_task.h"
#include "field_types_def.h"

void PlayerAvatar_SetTransitionFlagsBits(PlayerAvatar *avatar, u32 flags);

void Field_PlayerAvatar_OrrTransitionFlags(PlayerAvatar *avatar, u32 flags);

void Field_PlayerAvatar_OrrTransitionFlags(PlayerAvatar *avatar, u32 flags) {
    PlayerAvatar_SetTransitionFlagsBits(avatar, flags);
}
