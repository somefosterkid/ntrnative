// Black's code at ov015 0x02179ee0-0x0217a5b4: the link of frames between the two machines of the infrared (pokebw2's
// irc_link.c, the third file of the overlay's four: see net_devirc.c). The SPI bus it sends them over is irc_spi.c.

#include "types.h"
#include "field/irc_link.h"
#include "field/irc_spi.h"
#include "nitro/os.h"

// How long a ping may go unanswered, in milliseconds (pokebw2's irc_link.c)
#define IRC_PING_TIMEOUT 100

// Where the link is on the way to a connection (pokebw2's irc_link.c)
enum {
    IRC_STATE_IDLE,
    IRC_STATE_PING_SENT,
    IRC_STATE_REPLIED,
    IRC_STATE_PARENT_LINKED,
    IRC_STATE_CHILD_LINKED,
};

// The state of the link of frames (pokebw2's IrcLinkData, irc_link.c)
typedef struct {
    u8 unk00;
    u32 errorCount;
    // Called with each frame of data, with the number of bytes, its command and its ID
    void (*recvCallback)(u8 *data, u8 size, u8 cmd, u8 id);
    // Called when the link ends, and when the other side accepts it
    void (*closeCallback)(int a0);
    // Never set. The call keeps the peer's ID in r0 and a zero in r1, so it takes both
    void (*connectCallback)(u8 id, int a1);
    BOOL resend;
    // Whether this machine pinged and got the reply
    BOOL isParent;
    BOOL pinging;
    int state;
    BOOL linked;
    u64 tick;
} IrcLinkData;

// The head of a frame: the command, the ID of the link and the checksum
typedef struct {
    u8 cmd;
    u8 id;
    u8 sumLo;
    u8 sumHi;
} IrcFrameHeader;

// A frame as it goes over the infrared: its head and the data
typedef struct {
    IrcFrameHeader header;
    u8 data[0xb4];
} IrcFrameData;

// The IDs the two machines of the link use for its frames
typedef struct {
    u8 local;
    u8 peer;
} IrcLinkIdData;

static IrcLinkData data_ov015_0217aba8;
void func_ov015_0217a034(u8 *data, s32 size, u8 cmd, u8 id);
static IrcLinkIdData data_ov015_0217aab0 = { 255, 255 };
void func_ov015_0217a594(u8 id);
static IrcFrameData data_ov015_0217abd8 ATTRIBUTE_ALIGN(4);

#pragma thumb off

u16 func_ov015_02179ee0(const u8 *data, u32 size) {
    u32 sum = 0x80;
    u32 i = 0;

    if (i < size) {
        do {
            u8 byte = *data++;

            if (i & 1) {
                sum += byte;
            } else {
                sum += byte << 8;
            }
        } while (++i < size);
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += sum >> 16;
    return sum;
}

void func_ov015_02179f28(void) {
    data_ov015_0217aba8.resend = TRUE;
    data_ov015_0217aba8.errorCount++;
}

void func_ov015_02179f48(void) {
    data_ov015_0217aba8.unk00 = 0;
    data_ov015_0217aba8.state = IRC_STATE_IDLE;
    data_ov015_0217aba8.linked = FALSE;
    data_ov015_0217aba8.resend = FALSE;
    data_ov015_0217aba8.pinging = FALSE;
    data_ov015_0217aba8.isParent = FALSE;
    data_ov015_0217aba8.errorCount = 0;
}

void func_ov015_02179f74(u8 id) {
    intptr_t result;
    s16 val;
    func_ov015_0217a60c();
    func_ov015_0217a7ac((u8 *)&data_ov015_0217abd8);
    func_ov015_02179f48();
    func_ov015_0217a594(id);
    data_ov015_0217aab0.peer = 0xff;
    result = (intptr_t)(data_ov015_0217aba8.connectCallback = NULL);
    data_ov015_0217aba8.recvCallback = NULL;
    /* FAKEMATCH(self-assign): pokebw2's three NULL stores give 8 lines (the zero takes r1 after the peer's strb, Black's r2 before it);
       the three stores chained (casts between the callback types) 8, connectCallback's store before the peer's 12 */
    data_ov015_0217aba8.closeCallback = (void (*)(int))(intptr_t)(val = (val = result));
}

void func_ov015_02179fc4(void) {
    if (!data_ov015_0217aba8.linked && !data_ov015_0217aba8.pinging) {
        u8 ping;
        u8 *buf;

        buf = (u8 *)&ping;
        data_ov015_0217aba8.pinging = TRUE;
        *buf = IRC_CMD_PING ^ IRC_XOR;
        func_ov015_0217a8ec(buf, 1);
        data_ov015_0217aba8.state = IRC_STATE_PING_SENT;
        data_ov015_0217aba8.tick = OS_GetTick();
    }
}



BOOL func_ov015_0217a014(void) {
    return data_ov015_0217aba8.isParent;
}

BOOL func_ov015_0217a024(void) {
    return data_ov015_0217aba8.linked;
}

void func_ov015_0217a034(u8 *data, s32 size, u8 cmd, u8 id) {
    u8 *raw;
    IrcFrameData *frame = &data_ov015_0217abd8;
    u8 *dst;
    int i;
    u16 sum;

    frame->header.cmd = cmd;
    frame->header.id = id;
    frame->header.sumLo = 0;
    frame->header.sumHi = 0;
    dst = data_ov015_0217abd8.data;
    for (i = 0; i < size; i++) {
        *dst++ = *data++;
    }
    size = (u8)(size + sizeof(IrcFrameHeader));
    raw = (u8 *)&data_ov015_0217abd8;
    sum = func_ov015_02179ee0(raw, size);
    frame->header.sumLo = sum;
    frame->header.sumHi = sum >> 8;
    for (i = 0; i < size; i++) {
        raw[i] ^= IRC_XOR;
    }
    func_ov015_0217a8ec((u8 *)&data_ov015_0217abd8, size);
}



#pragma thumb on

#pragma thumb off
void func_ov015_0217a0cc(int a0) {
    if (data_ov015_0217aba8.linked && !data_ov015_0217aba8.isParent && a0 == 0) {
        func_ov015_0217a034(NULL, 0, IRC_CMD_CLOSE, data_ov015_0217aab0.local);
    }
    if (data_ov015_0217aba8.closeCallback) {
        data_ov015_0217aba8.closeCallback(a0);
    }
    func_ov015_02179f48();
}
#pragma thumb on

#pragma thumb off
void func_ov015_0217a134(void) {
    func_ov015_0217a0cc(0);
    func_ov015_0217a648();
}
#pragma thumb on

#pragma thumb off
void func_ov015_0217a148(u8 *data, u8 size, u8 cmd) {
    if (data_ov015_0217aba8.linked) {
        func_ov015_0217a034(data, size, cmd, data_ov015_0217aab0.local);
    }
}
#pragma thumb on

#pragma thumb off
void func_ov015_0217a174(void) {
}

BOOL func_ov015_0217a178(u8 size) {
    u32 i;

    if (size == 1) {
        if (data_ov015_0217aba8.linked) {
            return TRUE;
        }
        if (data_ov015_0217abd8.header.cmd != IRC_CMD_PING) {
            return TRUE;
        }
        return FALSE;
    }
    switch (size) {
    case 2:
    case 3:
        for (i = 0; i < 4; i++) {
        }
        for (i = 0; i < size; i++) {
        }
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov015_0217a1fc(void) {
    if (!data_ov015_0217aba8.linked && !data_ov015_0217aba8.pinging) {
        return FALSE;
    }
    if (OS_TicksToMilliSeconds(OS_GetTick() - data_ov015_0217aba8.tick) < IRC_PING_TIMEOUT) {
        return FALSE;
    }
    func_ov015_0217a0cc(1);
    data_ov015_0217aba8.pinging = FALSE;
    return TRUE;
}

BOOL func_ov015_0217a280(u8 size) {
    IrcFrameHeader *header = &data_ov015_0217abd8.header;
    u8 *raw = (u8 *)&data_ov015_0217abd8;
    u32 i;
    u16 stored;
    u16 sum;

    if (size == 1) {
        if (raw[0] == IRC_CMD_PING) {
            return TRUE;
        }
        return FALSE;
    }
    if (size < sizeof(IrcFrameHeader)) {
        return FALSE;
    }
    stored = header->sumLo + (header->sumHi << 8);
    header->sumLo = 0;
    header->sumHi = 0;
    sum = func_ov015_02179ee0(raw, size);
    header->sumLo = stored;
    header->sumHi = stored >> 8;
    if (stored == sum) {
        data_ov015_0217aba8.unk00 = 0;
        return TRUE;
    }
    if (data_ov015_0217aba8.linked) {
        for (i = 0; i < size; i++) {
        }
        func_ov015_02179f28();
    }
    return FALSE;
}

BOOL func_ov015_0217a334(void) {
    IrcFrameData *frame;
    u8 *raw;
    u8 size = func_ov015_0217a7ac((u8 *)&data_ov015_0217abd8);

    if (func_ov015_0217a178(size)) {
        size = 0;
    }
    if (size == 0) {
        if (func_ov015_0217a1fc()) {
            return TRUE;
        }
        return FALSE;
    }
    if (!func_ov015_0217a280(size)) {
        return FALSE;
    }
    data_ov015_0217aba8.tick = OS_GetTick();
    frame = &data_ov015_0217abd8;
    raw = (u8 *)&data_ov015_0217abd8;
    if (raw[0] < IRC_CMD_MIN) {
        if (!data_ov015_0217aba8.linked) {
            return FALSE;
        }
        if (raw[1] != data_ov015_0217aab0.local) {
            return FALSE;
        }
    }
    switch (frame->header.cmd) {
    case IRC_CMD_PING:
        switch (data_ov015_0217aba8.state) {
        case IRC_STATE_IDLE:
            break;
        case IRC_STATE_PING_SENT:
            data_ov015_0217aba8.state = IRC_STATE_REPLIED;
            func_ov015_0217a034(NULL, 0, IRC_CMD_REPLY, data_ov015_0217aab0.local);
            break;
        case IRC_STATE_REPLIED:
        case IRC_STATE_PARENT_LINKED:
        case IRC_STATE_CHILD_LINKED:
            break;
        }
        if (!data_ov015_0217aba8.pinging) {
            data_ov015_0217aba8.pinging = TRUE;
            data_ov015_0217aba8.state = IRC_STATE_REPLIED;
            data_ov015_0217aba8.isParent = FALSE;
            if (size <= 1 || data_ov015_0217aba8.linked) {
                func_ov015_0217a034(NULL, 0, IRC_CMD_REPLY, data_ov015_0217aab0.local);
                if (data_ov015_0217aba8.linked) {
                    func_ov015_02179f28();
                }
                func_ov015_0217a174();
            }
        }
        break;
    case IRC_CMD_REPLY:
        if (data_ov015_0217aba8.state == IRC_STATE_PING_SENT) {
            data_ov015_0217aba8.state = IRC_STATE_PARENT_LINKED;
            data_ov015_0217aba8.isParent = TRUE;
            data_ov015_0217aab0.peer = frame->header.id;
            if (frame->header.id == data_ov015_0217aab0.local) {
                func_ov015_0217a034(NULL, 0, IRC_CMD_ACCEPT, data_ov015_0217aab0.local);
                data_ov015_0217aba8.linked = TRUE;
                data_ov015_0217aba8.resend = FALSE;
                data_ov015_0217aba8.pinging = FALSE;
            }
        }
        break;
    case IRC_CMD_ACCEPT:
        if (data_ov015_0217aba8.state == IRC_STATE_REPLIED) {
            data_ov015_0217aba8.state = IRC_STATE_CHILD_LINKED;
            data_ov015_0217aab0.peer = frame->header.id;
            data_ov015_0217aba8.linked = TRUE;
            data_ov015_0217aba8.resend = FALSE;
            data_ov015_0217aba8.pinging = FALSE;
            if (data_ov015_0217aba8.connectCallback != NULL) {
                data_ov015_0217aba8.connectCallback(frame->header.id, 0);
            }
        }
        break;
    case IRC_CMD_RESEND:
        func_ov015_02179f28();
        break;
    default:
        if (data_ov015_0217aba8.resend != TRUE && data_ov015_0217aba8.linked) {
            data_ov015_0217aba8.recvCallback(data_ov015_0217abd8.data, size - sizeof(IrcFrameHeader), data_ov015_0217abd8.header.cmd, data_ov015_0217abd8.header.id);
        }
        break;
    }
    return FALSE;
}




#pragma thumb on

#pragma thumb off
void func_ov015_0217a584(void (*callback)(u8 *data, u8 size, u8 cmd, u8 id)) {
    data_ov015_0217aba8.recvCallback = callback;
}
#pragma thumb on

#pragma thumb off
void func_ov015_0217a594(u8 param_1) {
    data_ov015_0217aab0.local = param_1;
}
#pragma thumb on

#pragma thumb off
u8 func_ov015_0217a5a4(void) {
    return data_ov015_0217aab0.peer;
}

#pragma thumb on
