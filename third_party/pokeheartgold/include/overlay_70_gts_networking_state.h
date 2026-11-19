#ifndef POKEHEARTGOLD_OVERLAY_70_GTS_NETWORKING_STATE_H
#define POKEHEARTGOLD_OVERLAY_70_GTS_NETWORKING_STATE_H

// One BSS block at 0x02246800 (asm/overlay_70.s): the GTS HTTP request/response state. The asm splits it at the
// addresses the code's literal pools hit: _02246800 (status..friendKey) | ov70_02246814 = requestData |
// ov70_02246900 = requestData + 0xEC | ov70_0224693C = responseStatus (0x13C) | responseData (0x140).
// Over-index fix (hk67): the original code addresses the tail of the block from ov70_02246900 (+0x38 / +0x40), which
// on 64-bit is another object. The byte-identical build must keep that base label.

typedef enum GTSNetworkStatus {
    NETWORK_STATUS_INACTIVE,
    NETWORK_STATUS_IDLE,
    NETWORK_STATUS_POST_PENDING,
    NETWORK_STATUS_POST_PENDING_ABORTED,
    NETWORK_STATUS_POST_FINISH_PENDING,
    NETWORK_STATUS_POST_FINISH_PENDING_ABORTED,
    NETWORK_STATUS_GET_LISTED_POKEMON_PENDING,
    NETWORK_STATUS_GET_LISTED_POKEMON_PENDING_ABORTED,
    NETWORK_STATUS_GET_LISTING_STATUS_PENDING,
    NETWORK_STATUS_GET_LISTING_STATUS_PENDING_ABORTED,
    NETWORK_STATUS_DELETE_PENDING,
    NETWORK_STATUS_DELETE_PENDING_ABORTED,
    NETWORK_STATUS_RETURN_PENDING,
    NETWORK_STATUS_RETURN_PENDING_ABORTED,
    NETWORK_STATUS_SEARCH_PENDING,
    NETWORK_STATUS_SEARCH_PENDING_ABORTED,
    NETWORK_STATUS_EXCHANGE_PENDING,
    NETWORK_STATUS_EXCHANGE_PENDING_ABORTED,
    NETWORK_STATUS_EXCHANGE_FINISH_PENDING,
    NETWORK_STATUS_EXCHANGE_FINISH_PENDING_ABORTED,
    NETWORK_STATUS_INIT_PENDING,
    NETWORK_STATUS_INIT_PENDING_ABORTED,
    NETWORK_STATUS_SET_PROFILE_PENDING,
    NETWORK_STATUS_SET_PROFILE_PENDING_ABORTED,
    NETWORK_STATUS_COMPLETE
} GTSNetworkStatus;

typedef struct GTSNetworkingState {
    GTSNetworkStatus status;
    s32 result;
    s32 dwcProfileId;
    u64 friendKey;
    u8 requestData[296];
    u8 responseStatus[4];
    u8 *responseData;
} GTSNetworkingState;

// The same storage as the tail of GTSNetworkingState (_02246800 + 0x100): requestData[0xEC..0x124), then the last
// word of requestData, responseStatus and responseData. A native (ntrnative) build must alias it field by field:
// ov70_02246900.requestDataEnd == _02246800.requestData[0x124 .. +4), .responseStatus == _02246800.responseStatus,
// .responseData == _02246800.responseData.
typedef struct GTSNetworkingStateTail {
    u8 unk_00[0x38];
    s32 requestDataEnd;   // 0x38
    u8 responseStatus[4]; // 0x3C
    u8 *responseData;     // 0x40
} GTSNetworkingStateTail;

extern GTSNetworkingState _02246800;
extern GTSNetworkingStateTail ov70_02246900;

#endif // POKEHEARTGOLD_OVERLAY_70_GTS_NETWORKING_STATE_H
