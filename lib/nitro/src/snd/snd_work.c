// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/snd/src/snd_work.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/snd/common/work.h>

SNDSharedWork * SNDi_SharedWork;

static const void * ConvAddr(const void * p, const void * baseFrom, const void * baseTo);

#ifndef _MSC_VER
static inline
#else
__inline
#endif
const void * ConvAddr (const void * p, const void * baseFrom, const void * baseTo)
{
    if (p == NULL)
        return NULL;
    return (const char *)baseTo + ((const char *)p - (const char *)baseFrom);
}

u32 SND_GetPlayerStatus (void)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);

    DC_InvalidateRange((void *)&SNDi_SharedWork->playerStatus,
                       sizeof(SNDi_SharedWork->playerStatus));

    return SNDi_SharedWork->playerStatus;
}

u32 SND_GetChannelStatus (void)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);

    DC_InvalidateRange((void *)&SNDi_SharedWork->channelStatus,
                       sizeof(SNDi_SharedWork->channelStatus));

    return SNDi_SharedWork->channelStatus;
}

BOOL SND_ReadTrackInfo (const SNDDriverInfo * driverInfo, int playerNo, int trackNo,
                        SNDTrackInfo * trackInfo)
{
    const SNDPlayer * player;
    const SNDTrack * track;
    u8 trackID;

    SDK_NULL_ASSERT(driverInfo);
    SDK_NULL_ASSERT(trackInfo);

    if (playerNo < SND_PLAYER_MIN || SND_PLAYER_MAX < playerNo)
        return FALSE;

    if (trackNo < 0 || SND_TRACK_NUM_PER_PLAYER - 1 < trackNo)
        return FALSE;

    player = &driverInfo->work.player[playerNo];
    SDK_NULL_ASSERT(player);

    trackID = player->tracks[trackNo];
    if (trackID == SND_INVALID_TRACK_INDEX)
        return FALSE;

    track = &driverInfo->work.track[trackID];
    SDK_NULL_ASSERT(track);

    trackInfo->prgNo = track->prgNo;
    trackInfo->volume = track->volume;
    trackInfo->volume2 = track->volume2;
    trackInfo->pitchBend = track->pitch_bend;
    trackInfo->bendRange = track->bend_range;
    trackInfo->pan = (u8)(track->pan + SND_CHANNEL_PAN_CENTER);
    trackInfo->transpose = track->transpose;

    {
        const SNDExChannel * chp =
            (const SNDExChannel *)ConvAddr(track->channel_list, driverInfo->workAddress,
                                           &driverInfo->work);

        trackInfo->chCount = 0;
        while (chp != NULL) {
            trackInfo->channel[trackInfo->chCount] = chp->myNo;
            trackInfo->chCount++;

            chp =
                (const SNDExChannel *)ConvAddr(chp->nextLink, driverInfo->workAddress,
                                               &driverInfo->work);
        }
    }

    return TRUE;
}

u32 SNDi_GetFinishedCommandTag (void)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);

    DC_InvalidateRange((void *)&SNDi_SharedWork->finishCommandTag,
                       sizeof(SNDi_SharedWork->finishCommandTag));

    return SNDi_SharedWork->finishCommandTag;
}

    void SNDi_InitSharedWork (SNDSharedWork * work)
    {
        int playerNo, variableNo;

        work->playerStatus = 0;
        work->channelStatus = 0;
        work->captureStatus = 0;
        work->finishCommandTag = 0;

        for (playerNo = 0; playerNo < SND_PLAYER_NUM; playerNo++) {
            work->player[playerNo].tickCounter = 0;
            for (variableNo = 0; variableNo < SND_PLAYER_VARIABLE_NUM; variableNo++) {
                work->player[playerNo].variable[variableNo] = SND_DEFAULT_VARIABLE;
            }
        }
        for (variableNo = 0; variableNo < SND_GLOBAL_VARIABLE_NUM; variableNo++) {
            work->globalVariable[variableNo] = SND_DEFAULT_VARIABLE;
        }

        DC_FlushRange(work, sizeof(SNDSharedWork));
    }
