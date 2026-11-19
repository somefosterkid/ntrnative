/* PORTOK hk42 */
/* PORTOK hk39 */
u32 CARD_SpiWaitReadRange(u8 *buf);
s32 ov112_021E5938();
s32 ov112_021E59B4(s32, s32, s32, u8, s32);
s32 ov112_021E5A80();
s32 ov112_021E5A84(u32);
s32 ov112_021E5AD0();
s32 ov112_021E5B28(u32);
extern s32 _021FF500;
extern s32 _021FF9E0;
extern s32 ov112_021FFA18;
extern s32 ov112_021FFA20;

// _021FF9E0: the IR link state (raw offsets as in the sibling TUs): +0x04 u32 session id, +0x08, +0x0C void (*)(void),
// +0x18, +0x1C callback(payload, len, cmd, arg), +0x20 u32 key, +0x24, +0x28 u32 link state, +0x2C connected, +0x30 u64 tick.
// ov112_021FFA18: the received packet: +0 u8 cmd, +1 u8 arg, +4 u32 id, +8 (ov112_021FFA20) payload.
#define IR_STATE(off, T) (*(T *)((u8 *)&_021FF9E0 + (off)))
typedef void (*IrConnectFunc)(void);
typedef void (*IrRecvFunc)(s32 *payload, u8 len, u8 cmd, u8 arg);
#define IR_PACKET ((u8 *)&ov112_021FFA18)

s32 ov112_021E5B98(void);

s32 ov112_021E5B98(void) {
    u32 size;
    OSTick tick;

    size = CARD_SpiWaitReadRange(IR_PACKET);
    if (ov112_021E5A84(size) != 0) {
        size = 0;
    }
    if (size == 0) {
        if (ov112_021E5AD0() != 0) {
            return 1;
        }
        return 0;
    }
    if (ov112_021E5B28(size) == 0) {
        return 0;
    }
    tick = OS_GetTick();
    IR_STATE(0x30, u32) = (u32)tick;
    IR_STATE(0x34, u32) = (u32)(tick >> 32);
    if (IR_PACKET[0] < 0xF0) {
        if (IR_STATE(0x2C, s32) == 0) {
            return 0;
        }
        if (*(u32 *)(IR_PACKET + 4) != IR_STATE(0x04, u32)) {
            return 0;
        }
    }
    switch (IR_PACKET[0]) {
    case 0xFC:
        switch (IR_STATE(0x28, u32)) {
        case 1:
            IR_STATE(0x28, u32) = 2;
            ov112_021E59B4(0, 0, 0xFA, *(u8 *)&_021FF500, IR_STATE(0x20, s32));
            break;
        }
        if (IR_STATE(0x24, s32) == 0) {
            IR_STATE(0x24, s32) = 1;
            IR_STATE(0x28, u32) = 2;
            IR_STATE(0x08, s32) = 0;
            if (size <= 1 || IR_STATE(0x2C, s32) != 0) {
                ov112_021E59B4(0, 0, 0xFA, *(u8 *)&_021FF500, IR_STATE(0x20, s32));
                if (IR_STATE(0x2C, s32) != 0) {
                    ov112_021E5938();
                }
                ov112_021E5A80();
            }
        }
        break;
    case 0xFA:
        if (IR_STATE(0x28, u32) == 1) {
            IR_STATE(0x28, u32) = 3;
            IR_STATE(0x08, s32) = 1;
            ((u8 *)&_021FF500)[1] = IR_PACKET[1];
            if (IR_PACKET[1] == 1) {
                IR_STATE(0x04, u32) = *(u32 *)(IR_PACKET + 4) ^ IR_STATE(0x20, u32);
                ov112_021E59B4(0, 0, 0xF8, *(u8 *)&_021FF500, IR_STATE(0x20, s32));
                IR_STATE(0x2C, s32) = 1;
                IR_STATE(0x18, s32) = 0;
                IR_STATE(0x24, s32) = 0;
            }
        }
        break;
    case 0xF8:
        if (IR_STATE(0x28, u32) == 2) {
            IR_STATE(0x28, u32) = 4;
            ((u8 *)&_021FF500)[1] = IR_PACKET[1];
            IR_STATE(0x04, u32) = IR_STATE(0x20, u32) ^ *(u32 *)(IR_PACKET + 4);
            IR_STATE(0x2C, s32) = 1;
            IR_STATE(0x18, s32) = 0;
            IR_STATE(0x24, s32) = 0;
            if (IR_STATE(0x0C, IrConnectFunc) != NULL) {
                IR_STATE(0x0C, IrConnectFunc)();
            }
        }
        break;
    case 0xF6:
        ov112_021E5938();
        break;
    default:
        if (IR_STATE(0x18, s32) != 1 && IR_STATE(0x2C, s32) != 0) {
            IR_STATE(0x1C, IrRecvFunc)(&ov112_021FFA20, size - 8, IR_PACKET[0], IR_PACKET[1]);
        }
        break;
    }
    return 0;
}
