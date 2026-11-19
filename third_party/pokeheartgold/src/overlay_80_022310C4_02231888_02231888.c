#include "global.h"

#include "frontier/frontier_system.h"
#include "frontier/overlay_80_02229EE0.h"

#include "heap.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "save.h"
#include "save_frontier.h"
#include "save_vars_flags.h"
#include "sys_vars.h"
#include "unk_0205BFF0.h"

extern const u8 ov80_0223BDFC[];

extern const u8 ov80_0223BE10[];

u16 ov80_02231888(void *data_);

u16 ov80_02231888(void *data_) {
    u8 *data = data_;
    u8 mode = data[4];
    u16 bracket = *(u16 *)(data + 0xa);
    u16 result;

    if (mode <= 1) {
        if (bracket >= 0x12) {
            result = 0xc;
        } else {
            result = ov80_0223BDFC[bracket];
        }
    } else {
        if (bracket >= 0x12) {
            result = 0x17;
        } else {
            result = ov80_0223BE10[bracket];
        }
    }
    if (mode == 0) {
        if (*(u16 *)(data + 8) == 0x32 || *(u16 *)(data + 8) == 0xaa) {
            result = 0x14;
        }
    } else if (mode == 2) {
        result = 0xc;
    }
    return result;
}
