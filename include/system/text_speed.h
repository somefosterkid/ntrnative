// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/text_speed.h, the lines in use so
// far.
#ifndef BWPORT_SYSTEM_TEXT_SPEED_H
#define BWPORT_SYSTEM_TEXT_SPEED_H

#include "types.h"
#include "struct_decls.h"

// The wait between characters of the text speed in the options (pokebw2's func_02017bcc)
s32 func_02012ffc(void);

// The waits of the two faster speeds
s32 func_02013020(void);
s32 func_02013050(void);
s32 func_02013080(u32 speed);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Sets the save control whose options give the text speed
void saveMainSaveAreaAddress(SaveControl *save);

#endif // BWPORT_SYSTEM_TEXT_SPEED_H
