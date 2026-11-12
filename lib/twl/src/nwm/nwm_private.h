#ifndef TWL_NWM_PRIVATE_H
#define TWL_NWM_PRIVATE_H

// What TwlSDK's NWM files share and twl/nwm.h doesn't export (pokebw2's nwm_private.h): the commands and states, the
// system area and the blocks that the ARM7 shares with the ARM9, and what nwm_setup.c and nwm_events.c give the
// commands of nwm.c

#include "types.h"
#include "nitro/os.h"
#include "nitro/spi/ARM9/pm.h"
#include "twl/nwm.h"

// The commands, by TwlSDK's API IDs (twl/nwm/common/nwm.h's NWMApiid, in libntr): the first halfword of the buffer
// that goes to the ARM7 and of the event that comes back. They also index the callbacks and the ARM7's flags of the
// commands it is running
typedef enum NWMApiid {
    NWM_APIID_INIT = 0,
    NWM_APIID_RESET,
    NWM_APIID_END,
    NWM_APIID_LOAD_DEVICE,
    NWM_APIID_UNLOAD_DEVICE,
    NWM_APIID_OPEN,
    NWM_APIID_CLOSE,
    NWM_APIID_START_SCAN,
    NWM_APIID_CONNECT,
    NWM_APIID_DISCONNECT,
    NWM_APIID_SET_RECEIVING_BUF,
    NWM_APIID_SEND_FRAME,
    NWM_APIID_UNSET_RECEIVING_BUF,
    NWM_APIID_SET_WEPKEY,
    NWM_APIID_SET_PS_MODE,
    NWM_APIID_SET_WPA_KEY,
    NWM_APIID_SET_WPA_PARAMS,
    NWM_APIID_CREATE_QOS,
    NWM_APIID_SET_WPA_PSK,
    NWM_APIID_INSTALL_FIRMWARE,
    NWM_APIID_ASYNC_KIND_MAX,
    NWM_APIID_INDICATION = 128,
    NWM_APIID_UNKNOWN = 255
} NWMApiid;

// The results of an event that the library tells apart: a sent frame that the ARM7 gave up, and a receive buffer
// that is full (the callback of the command that sets it gives it back)
#define NWM_RESULT_12 12
#define NWM_RESULT_BUFFER_FULL 11

// What the ARM7 reports as the library's state, which the commands check. The numbers 0 to 4 are in the order that
// the commands take them in; 5 is only known from the command that unloads the device in any state
enum { NWM_STATE_NONE, NWM_STATE_INITIALIZED, NWM_STATE_LOADED, NWM_STATE_READY, NWM_STATE_CONNECTED, NWM_STATE_5 };

// The first halfword of a command buffer while the ARM7 is done with the buffer, and the number of buffers
#define NWM_CMD_BUF_FREE 0x8000
#define NWM_CMD_BUF_COUNT 32
#define NWM_INDICATE_COUNT 3

// The priority of the library's callbacks in the lists of those that run when the console sleeps and exits
#define NWM_PM_PRIORITY 0xffff

// What the exit callback waits for the ARM7: a first spin of 400000 cycles, spins of 10000 until the commands it
// runs are done, and at most about a second (the ARM9's clock, in cycles) for the device to unload
#define NWM_EXIT_SPIN_FIRST 400000
#define NWM_EXIT_SPIN_STEP 10000
#define NWM_EXIT_TIMEOUT 67028000

// A command buffer: the command and its words
// Black 2's; twinned code confirms the offsets of cmd and args, and its size in Black
typedef struct NWMCmdBuf {
    u16 cmd;
    u16 unk_02;
    u32 args[63];
} NWMCmdBuf;

// The flags that the ARM7 sets in the shared memory, of which the library clears the first
// Black 2's; twinned code confirms the offsets of flags in Black
typedef struct NWMInfo {
    u8 flags;
    u8 unk_01[0x1f];
} NWMInfo;

#define NWM_INFO_PENDING 0x01

// The ARM7's status block, which it writes in the shared memory
// Black 2's; twinned code confirms the offsets of state, macAddr and unk_9e, and its size in Black
typedef struct NWMStatus {
    u8 pending[0x24]; // By command: 1 while the ARM7 runs it
    u16 state;
    u8 unk_26[0x1a];
    u8 macAddr[6];
    u8 unk_46[0x58];
    u16 unk_9e;
    u8 unk_a0[0x20];
} NWMStatus;

// What the ARM7 sends: the number of the command it answers, its result, and the command's data
// Black 2's; twinned code confirms the offsets of cmd, result, u04, u0c and data, and its size in Black
typedef struct NWMEvent {
    u16 cmd;
    u16 result;
    union {
        NWMCallback sendCallback; // The event of a frame sent: the callback that the frame was given
        u16 index;                // The event of frames received: the index of the callback
    } u04;
    u32 unk_08;
    union {
        u32 size;     // The event of a scan: the size of the data
        void *buffer; // The event of frames received: the buffer that is full
    } u0c;
    void *data;
    u8 unk_14[0xec];
} NWMEvent;

// The system area that the game gives the library, its first part: the buffers of the commands, the queue of those the
// ARM7 is done with, the callbacks and pointers to the three blocks that follow it (the flags, the status and an event)
// Black 2's; twinned code confirms the offsets of freeQueue, info, status, event, dmaNo, callbacks and indicate in
// Black
typedef struct NWMSys {
    NWMCmdBuf cmdBuf[NWM_CMD_BUF_COUNT];      // 0x0000
    OSMessageQueue freeQueue;                 // 0x2000
    OSMessage freeMsgs[NWM_CMD_BUF_COUNT];    // 0x2020
    u8 unk_20a0[0x80];                        // 0x20a0
    u8 unk_2120[0x154];                       // 0x2120: cleared when the library starts
    NWMInfo *info;                            // 0x2274
    NWMStatus *status;                        // 0x2278
    NWMEvent *event;                          // 0x227c
    u16 dmaNo;                                // 0x2280
    NWMCallback callbacks[NWM_APIID_ASYNC_KIND_MAX];     // 0x2284: of the commands
    NWMCallback indicate[NWM_INDICATE_COUNT]; // 0x22d4: of the frames received, by index
} NWMSys;

// nwm_setup.c: whether func_02702434 (NWM_Init) has set the library up, and whether the wireless is available and
// not turned off
extern u8 data_027341a0;
BOOL func_027026b4(void);

// nwm_events.c: the system area that func_02702434 was given, the callback that unloads the device before the
// application exits (func_02702b08 puts it), and the system area's getter, the PXI callback, the state check and
// the sleep callback's setup
extern NWMSys *data_027341a4;
extern PMExitCallbackInfo data_027341a8;
NWMSys *func_027026dc(void);
void func_027026ec(void);
void func_02702740(int tag, u32 data, BOOL err);
NWMRetCode func_027028e4(u32 count, ...);
void func_02702a40(void);

#endif // TWL_NWM_PRIVATE_H
