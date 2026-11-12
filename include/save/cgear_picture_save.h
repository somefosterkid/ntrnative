// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/cgear_picture_save.h: the lines in use
// so far.
#ifndef BWPORT_SAVE_CGEAR_PICTURE_SAVE_H
#define BWPORT_SAVE_CGEAR_PICTURE_SAVE_H

#include "types.h"

// Black 2's; twinned code confirms the offsets of chars in Black
typedef struct {
    u8 chars[0x2000];
    u8 unk2000[0x600];
} CGearPictureSave; // size: 0x2600

// Whether the skin's last tile has any pixel, which an empty block doesn't
BOOL func_0200c6f0(CGearPictureSave *picture);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_0200c6ec(CGearPictureSave *picture);

#endif // BWPORT_SAVE_CGEAR_PICTURE_SAVE_H
