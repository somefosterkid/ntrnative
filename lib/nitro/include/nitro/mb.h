// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/mb.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_MB_H
#define BWPORT_NITRO_MB_H

#include "types.h"

#define MB_USER_NAME_LENGTH 10
#define MB_DOWNLOAD_PARAMETER_SIZE 32

// A tgid that MB_Init makes up from the OS_GetTick
#define MB_TGID_AUTO 0x10000

// The size of the library's work
#define MB_SYSTEM_BUF_SIZE 0xb100

// A player, as the parent and the children show each other
typedef struct {
    u8 favoriteColor : 4;
    u8 playerNo : 4;
    u8 nameLength;
    u16 name[MB_USER_NAME_LENGTH];
} MBUserInfo;

// A game that the parent offers for download
typedef struct {
    const char *romFilePathp;
    u16 *gameNamep;
    u16 *gameIntroductionp;
    const char *iconCharPathp;
    const char *iconPalettePathp;
    u32 ggid;
    u8 maxPlayerNum;
    u8 pad[3];
    u8 userParam[MB_DOWNLOAD_PARAMETER_SIZE];
} MBGameRegistry;

// The smallest packet the parent sends
#define MB_COMM_PARENT_SEND_MIN 256

// The results of the library's calls (MB_SUCCESS) and the error codes of MB_COMM_PSTATE_ERROR
enum {
    MB_SUCCESS,
    MB_ERRCODE_INVALID_PARAM,
    MB_ERRCODE_INVALID_STATE,
    MB_ERRCODE_INVALID_DLFILEINFO,
    MB_ERRCODE_INVALID_BLOCK_NO,
    MB_ERRCODE_INVALID_BLOCK_NUM,
    MB_ERRCODE_INVALID_FILE,
    MB_ERRCODE_INVALID_RECV_ADDR,
    MB_ERRCODE_WM_FAILURE,
    MB_ERRCODE_FATAL,
};

// What the parent's state callback reports
enum {
    MB_COMM_PSTATE_NONE,
    MB_COMM_PSTATE_INIT_COMPLETE,
    MB_COMM_PSTATE_CONNECTED,
    MB_COMM_PSTATE_DISCONNECTED,
    MB_COMM_PSTATE_KICKED,
    MB_COMM_PSTATE_REQ_ACCEPTED,
    MB_COMM_PSTATE_SEND_PROCEED,
    MB_COMM_PSTATE_SEND_COMPLETE,
    MB_COMM_PSTATE_BOOT_REQUEST,
    MB_COMM_PSTATE_BOOT_STARTABLE,
    MB_COMM_PSTATE_REQUESTED,
    MB_COMM_PSTATE_MEMBER_FULL,
    MB_COMM_PSTATE_END,
    MB_COMM_PSTATE_ERROR,
    MB_COMM_PSTATE_WAIT_TO_SEND,
};

// The parent's answers to a child
typedef enum {
    MB_COMM_RESPONSE_REQUEST_KICK,
    MB_COMM_RESPONSE_REQUEST_ACCEPT,
    MB_COMM_RESPONSE_REQUEST_DOWNLOAD,
    MB_COMM_RESPONSE_REQUEST_BOOT,
} MBCommResponseRequestType;

// The argument of MB_COMM_PSTATE_ERROR
typedef struct {
    u16 errcode;
} MBErrorStatus;

typedef void (*MBCommPStateCallback)(u16 childAid, u32 status, void *arg);

void MB_EndToIdle(void);
void MB_DisconnectChild(u16 aid);
BOOL MB_CommIsBootable(u16 childAid);
BOOL MB_CommResponseRequest(u16 childAid, MBCommResponseRequestType ack);

static inline BOOL MB_CommBootRequest(u16 childAid) {
    return MB_CommResponseRequest(childAid, MB_COMM_RESPONSE_REQUEST_BOOT);
}

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/mb/mb.h
static inline BOOL MB_CommStartSending (u16 child_aid)
{
	return MB_CommResponseRequest(child_aid, MB_COMM_RESPONSE_REQUEST_DOWNLOAD);
}

#endif // BWPORT_NITRO_MB_H
