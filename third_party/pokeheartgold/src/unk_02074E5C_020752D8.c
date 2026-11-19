#include "global.h"

#include "unk_020379A0.h"
#include "bg_window.h"
#include "palette.h"
#include "overlay_12_field_battle_dto.h"
#include "save_palpad.h"

s32 sub_02036FD8(s32, void *, s32);

s32 sub_02037190(void);

s32 sub_020752D8(u8 *arg0);

struct FieldBattleDTO;
struct SavePalPad;
typedef struct LinkBattleCommState {
    FieldBattleDTO *dto;
    BgConfig *bgConfig;
    Window *window;
    PaletteData *paletteData;
    SavePalPad *palPad[4];
    u8 sendBuffer[0x1000];
    u8 recvCount;
    u8 handshakeStep;
    u16 syncTimer;
    void *waitDial;
} LinkBattleCommState;
void sub_0207530C(int param0, int param1, void *param2, void *commState);

s32 sub_020752D8(u8 *arg0) {
    if (sub_02037190() != 0x108) {
        return 0;
    }
    if (sub_02037B38(0x38U) == 0) {
        return 0;
    }
    return sub_02036FD8(0x21, arg0 + 0x20, 0x3E8);
}

void sub_0207530C(int param0, int param1, void *param2, void *commState) {
    LinkBattleCommState *linkBattleCommState = (LinkBattleCommState *)commState;
    linkBattleCommState->recvCount++;
}
