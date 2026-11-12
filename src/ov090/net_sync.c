// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov164/net_sync.c, ported by tools/port_file.py.
#include "types.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_sync.h"

const NetSyncCommand data_ov090_021b6228 = {func_ov090_021b61c4, NULL};

void func_ov090_021b6100(PokeListParam *param) {
    param->unk62[0] = 0;
}

void func_ov090_021b6108(PokeListParam *param) {
    func_0203a5e4(0x2d00);
}

void func_ov090_021b6114(PokeListParam *param) {
    switch (param->unk62[0]) {
    case 0:
        func_02039c68(func_020399a8(), 1, 0x2d);
        param->unk62[0] = 1;
        break;
    case 1:
        if (func_02039cb4(func_020399a8(), 1, 0x2d) == 1) {
            func_0203a574(0x2d00, &data_ov090_021b6228, 1, param);
            param->unk62[0] = 2;
        }
        break;
    case 2:
        break;
    }
    if (func_0200fb24() && param->unk62[1] == 0) {
        param->unk62[1] = 1;
    }
}

u32 func_ov090_021b6184(PokeListParam *param, u8 kind, u16 value) {
    NetHandle *handle = func_020399a8();
    NetSyncPacket packet;
    packet.kind = kind;
    packet.value = value;
    if (param->unk62[0] != 2) {
        return 0;
    }
    return func_0203d3f8(handle, 0xff, 0x2d00, 4, &packet, 1, 0, 0);
}

void func_ov090_021b61c4(u32 a0, u32 a1, const NetSyncPacket *packet, PokeListParam *param) {
    func_0203d1a4(func_020399a8());
    if (packet->kind == 0) {
        param->battleMsg++;
    }
}

void func_ov090_021b61e8(PokeListParam *ignored, u32 a1) {
    func_02039c68(func_020399a8(), a1, 0x2d);
}

BOOL func_ov090_021b61fc(PokeListParam *param, u32 a1) {
    if (func_02039cb4(func_020399a8(), a1, 0x2d) == 1) {
        return TRUE;
    }
    if (param->unk62[1] == 1) {
        return TRUE;
    }
    return FALSE;
}
