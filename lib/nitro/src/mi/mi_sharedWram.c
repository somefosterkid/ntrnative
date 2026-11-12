// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/src/mi/mi_sharedWram.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// TwlSDK's shared WRAM manager, the part in ARM9 main: a call into the LTD autoload on a DSi. The file name is a guess;
// the binary can't tell this from mi_wram.c, which follows (neither has data).
#include "nitro/mi.h"
#include "nitro/os.h"
#include "twl/mi.h"

// pokebw2's MI_InitWramManager
void func_02082198(void) {
    if (OS_IsRunOnTwl()) {
        MIi_InitWramManagerCore();
    }
}
