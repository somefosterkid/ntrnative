// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/photo_sign_client.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "nitro/math.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/pxi.h"
#include "photo_sign_private.h"
#include "gfl/std.h"
#include "photo_sign.h"

// The ARM9's side of the ARM7's photo signature commands: it sets up the FIFO tag, sends the commands, and takes the
// ARM7's results, which a thread waits for or a callback gets. ARM code in the DSi-only LTD autoload. The ROM names
// none of it; the file's name and those of its functions and data are descriptive guesses. Its .bss is reached from
// its own base, so it was a file of its own, built with -ipa file like the library's other files.

// The MAC length the ARM7 is given, (PHOTO_SIGN_MAC_SIZE - 2) / 2
#define MAC_LENGTH_CODE 7

// The MAC command's parameters, which are sent to the ARM7 as they are
// Black 2's; twinned code confirms the offsets of nonce, data, dataSize, payloadSize, macLength and mac in Black
typedef struct {
    u8 nonce[PHOTO_SIGN_NONCE_SIZE];
    u32 unk_0c;
    const void *data;
    u32 dataSize;
    u32 payloadSize;
    u32 macLength;
    u8 *mac;
} MacParam;

static void func_02707310(int fifoTag, uPtr data, BOOL err);
static int func_0270738c(void);

static OSMessage data_02735a9c[1];
static OSMessageQueue data_02735aac;
static void *sCallbackArg;
static PhotoSignCallback sCallback;
static BOOL sInitialized;
u8 data_02735acc[0x14];

// Takes the words the ARM7 sends: a MAC's result goes to the callback and ends the command, another result to the
// thread that waits for it
// pokebw2's PhotoSign_FifoCallback
static void func_02707310(int fifoTag, uPtr data, BOOL err) {
    u32 command;

    if (err) {
        return;
    }
    command = (data & PHOTO_SIGN_FIFO_COMMAND_MASK) >> PHOTO_SIGN_FIFO_COMMAND_SHIFT;
    data &= PHOTO_SIGN_FIFO_DATA_MASK;
    switch (command) {
    case PHOTO_SIGN_COMMAND_MAC_RESULT:
        if (sCallback != NULL) {
            sCallback(data, sCallbackArg);
        }
        func_02707bdc();
        break;
    case PHOTO_SIGN_COMMAND_RESULT:
        OS_SendMessage(&data_02735aac, (OSMessage)data, OS_MESSAGE_NOBLOCK);
        break;
    default:
        func_02707acc(PHOTO_SIGN_COMMAND_RESULT, 6);
        break;
    }
}

// Waits for the ARM7's result of a command
// pokebw2's PhotoSign_WaitResult
static int func_0270738c(void) {
    OSMessage result;

    OS_ReceiveMessage(&data_02735aac, &result, OS_MESSAGE_BLOCK);
    return (int)result;
}

// pokebw2's PhotoSign_Init
void func_027073ac(void) {
    if (sInitialized) {
        return;
    }
    OS_InitMessageQueue(&data_02735aac, data_02735a9c, NELEMS(data_02735a9c));
    func_02707aec();
    PXI_Init();
    PXI_SetFifoRecvCallback(PHOTO_SIGN_FIFO_TAG, func_02707310);
    while (!PXI_IsCallbackReady(PHOTO_SIGN_FIFO_TAG, MI_PROCESSOR_ARM7)) {
    }
    sInitialized = TRUE;
}

// pokebw2's PhotoSign_SeedRandom
int func_0270741c(const void *seed) {
    int result;

    if (!func_02707b0c()) {
        return PHOTO_SIGN_RESULT_BUSY;
    }
    func_02707a94(PHOTO_SIGN_COMMAND_SEED, seed, MATH_SHA1_DIGEST_SIZE);
    result = func_0270738c();
    func_02707bdc();
    return result;
}

// The command carries the buffer's address, which the ARM7 writes to
// pokebw2's PhotoSign_GetRandomBlock
int func_02707458(void *buffer) {
    int result;

    if (!func_02707b0c()) {
        return PHOTO_SIGN_RESULT_BUSY;
    }
    func_02707c48(buffer, PHOTO_SIGN_RANDOM_BLOCK_SIZE);
    func_02707a94(PHOTO_SIGN_COMMAND_RANDOM, &buffer, sizeof(buffer));
    result = func_0270738c();
    func_02707bdc();
    return result;
}

// The lock taken here is released when the ARM7's result comes
// pokebw2's PhotoSign_StartMac
int func_027074b0(const u8 *nonce, const void *data, u32 size, u8 *mac, PhotoSignCallback callback, void *arg) {
    MacParam param;

    if (!func_02707b0c()) {
        return PHOTO_SIGN_RESULT_BUSY;
    }
    sCallback = callback;
    sCallbackArg = arg;
    MI_CpuCopy8(nonce, param.nonce, PHOTO_SIGN_NONCE_SIZE);
    param.data = data;
    param.dataSize = size;
    param.payloadSize = 0;
    param.macLength = MAC_LENGTH_CODE;
    param.mac = mac;
    func_02707c2c(data, size);
    func_02707c48(mac, PHOTO_SIGN_MAC_SIZE);
    func_02707a94(PHOTO_SIGN_COMMAND_MAC, &param, sizeof(param));
    return PHOTO_SIGN_RESULT_SUCCESS;
}
