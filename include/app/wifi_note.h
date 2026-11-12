// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_note.h: the lines in use so far.
#ifndef BWPORT_APP_WIFI_NOTE_H
#define BWPORT_APP_WIFI_NOTE_H

#include "types.h"
#include "dwc/dwc.h"
#include "struct_decls.h"

// The screen's heap, a child of HEAPID_USER
#define HEAPID_WIFI_NOTE 0x34

// How the Pal Pad was left
enum {
    // With the return button, back to the bag
    WIFI_NOTE_RESULT_RETURN,
    // With the X button, back to the field
    WIFI_NOTE_RESULT_EXIT,
};

struct WifiNoteParam {
    GameData *gameData;
    // WIFI_NOTE_RESULT_*
    u32 result;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The friend key of a friend's data, 0 if it holds none (DWC, not declared in dwc/dwc.h)
u64 DWC_GetFriendKey(const DWCFriendData *friendData);

// The pal pad's proc, from the field menu
extern const GameProcFunctions data_ov128_021ca658;

#endif // BWPORT_APP_WIFI_NOTE_H
