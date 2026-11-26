#include "global.h"

#include "overlay_00_thumb.h"
#include "unk_02037C94.h"
#include "overlay_00_nintendo_wfcmanager.h"

BOOL sub_02034098(int param0);

extern NintendoWFCManager *_0221A680;

void ov00_021E6388(int unused);

void ov00_021E6ED8(DWCError error, BOOL cancelled, BOOL isThisConsole, BOOL hostCancelled, int hostFriendIdx, void *userParam);

void ov00_021E6ED8(DWCError error, BOOL cancelled, BOOL isThisConsole, BOOL hostCancelled, int hostFriendIdx, void *userParam) {
    BOOL refuseNewConnections = 0;

    _0221A680->unused_1094 = 0;

    if (error == DWC_ERROR_NONE) {
        if (!cancelled) {
            if (sub_02034098(sub_0203993C()) && (hostFriendIdx == (-1))) {
                refuseNewConnections = 1;
            }

            if (_0221A680->hostMatchCallback) {
                if (0 == _0221A680->hostMatchCallback(hostFriendIdx)) {
                    refuseNewConnections = 1;
                }
            }

            if (_0221A680->matchmakingCancelState != MATCHMAKING_NOT_CANCELED || refuseNewConnections) {
                u32 newConnections = ~_0221A680->matchmakingAIDBitmap & DWC_GetAIDBitmap();
                u32 connections = DWC_GetAIDBitmap();

                if (newConnections) {
                    DWC_CloseConnectionHardBitmap(&newConnections);

                    if ((newConnections ^ connections) == 0x1) {
                        _0221A680->state = WFC_STATE_FORCE_CANCEL;
                    }

                    return;
                }
            }

            _0221A680->hostFriendIdx = hostFriendIdx;
            _0221A680->matchmakingAIDBitmap = DWC_GetAIDBitmap();

            if (_0221A680->matchmakingAIDBitmap == 0x1) {
                _0221A680->state = WFC_STATE_FORCE_CANCEL;
            } else {
                ov00_021E6388(hostFriendIdx);
            }
        } else if (!isThisConsole) {
            _0221A680->latestNewClientFriendIdx = (-1);
        }
    }
}
