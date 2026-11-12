// Black's code at main 0x020090b0-0x02009228. The name is pokebw2's file between mail.c and dream_world_savedata.c (20
// functions there, 19 here; the one at 0x02009228, func_02009930, is dream_world_savedata.c's, which declares it).

#include "types.h"
#include "gfl/std.h"
#include "save/c_gear_savedata.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_data.h"

u8 data_020a7174[0x24] = {
    0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x04, 0x00, 0x04, 0x04, 0x04, 0x04,
    0x81, 0x83, 0x82, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x04, 0x04, 0x00,
    0x00, 0x04, 0x00, 0x00,
};

u32 func_020090b0(void) {
    return sizeof(CGearSettings);
}

void func_020090b4(void *param_1) {
    u32 size;

    size = func_020090b0();
    MI_CpuFill8(param_1, 0, size);
    MI_CpuCopy8(data_020a7174, param_1, sizeof(data_020a7174));
}

void func_020090d8(CGearSettings *settings, u8 value) {
    settings->on = value;
}

BOOL func_020090e0(CGearSettings *settings) {
    return settings->on;
}

void func_020090e8(CGearSettings *settings, u8 skin) {
    settings->skin = skin;
}

u8 func_020090f0(CGearSettings *settings) {
    return settings->skin;
}

void func_020090f8(CGearSettings *settings, u8 value) {
    settings->skinValid = value;
}

u8 func_02009100(CGearSettings *settings) {
    return settings->skinValid;
}

void func_02009108(CGearSettings *settings, u16 crc) {
    settings->skinCrc = crc;
}

u16 func_0200910c(CGearSettings *settings) {
    return settings->skinCrc;
}

u32 func_02009110(u8 *param_1, s32 param_2, s32 param_3) {
    u8 *p;

    if (param_2 >= 9 || param_2 < 0) {
        return 0;
    }
    if (param_3 >= 4 || param_3 < 0) {
        return 0;
    }
    p = param_1 + param_2 * 4;
    return p[param_3] & 0xf;
}

u32 func_02009134(u8 *param_1, s32 param_2, s32 param_3) {
    u8 *p;

    if (param_2 >= 9 || param_2 < 0) {
        return 0;
    }
    if (param_3 >= 4 || param_3 < 0) {
        return 0;
    }
    p = param_1 + param_2 * 4;
    if (p[param_3] & 0x80) {
        return 1;
    }
    return 0;
}

u32 func_02009160(u8 *param_1, s32 param_2, s32 param_3, s32 param_4) {
    u8 *p;
    u32 flag;

    if (param_2 >= 9 || param_2 < 0) {
        return 0;
    }
    if (param_3 >= 4 || param_3 < 0) {
        return 0;
    }
    flag = 0;
    if (param_4 == 1) {
        flag = 0x40;
    } else if (param_4 == 3) {
        flag = 0x20;
    } else if (param_4 == 2) {
        flag = 0x10;
    }
    p = param_1 + param_2 * 4;
    if (p[param_3] & flag) {
        return 1;
    }
    return 0;
}

void func_020091ac(u8 *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6) {
    u8 *p;
    u32 flags;

    if (param_4 >= 5) {
        return;
    }
    if (param_2 >= 9 || param_2 < 0) {
        return;
    }
    if (param_3 >= 4 || param_3 < 0) {
        return;
    }
    flags = param_4;
    if (param_5 == 1 && (u32)(param_4 - 1) <= 2) {
        flags |= 0x80;
    }
    if (param_6 != 0) {
        if (param_4 == 1) {
            flags |= 0x40;
        } else if (param_4 == 3) {
            flags |= 0x20;
        } else if (param_4 == 2) {
            flags |= 0x10;
        }
    }
    p = param_1 + param_2 * 4;
    p[param_3] = flags;
}





void *func_020091fc(void *param_1) {
    return param_1;
}

void func_02009200(CGearSettings *settings, u32 value) {
    settings->wasOn = value;
}

u8 func_02009208(CGearSettings *settings) {
    return settings->wasOn;
}

void *func_02009210(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, 0x20);
}


void *func_0200921c(GameData *gameData) {
    SaveControl *save;

    save = GameData_GetSaveControl(gameData);
    return func_02009210(save);
}
