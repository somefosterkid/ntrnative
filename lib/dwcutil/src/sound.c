// libdwcutil's sound.c (Black's ov016 0x021cfebc-0x021d0040): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void taskSound(void);

typedef struct SoundWork {
    NNSSndArc arc;
    u32 unk_98[2];
    NNSSndHandle handle;
    void *info;
    s32 *task;
} SoundWork;

static SoundWork *data_ov016_021de648;


void DWCi_SNDlInit(void) {
    u32 size;
    s32 playerNo;

    data_ov016_021de648 = DWCi_HEAPlAllocEx(0xac, 4);
    data_ov016_021de648->info = DWCi_ARClReadEx("sound/sound_data.sdat.l", &size, 0x20);
    NNS_SndInit();
    NNS_SndArcInitOnMemory(&data_ov016_021de648->arc, data_ov016_021de648->info);
    playerNo = 0;
    NNS_SndArcPlayerSetup(NULL);
    NNS_SndHandleInit(&data_ov016_021de648->handle);
    do {
        NNS_SndPlayerSetPlayerVolume(playerNo, 0x7f);
        NNS_SndPlayerSetPlayableSeqCount(playerNo, 1);
        NNS_SndPlayerSetAllocatableChannel(playerNo, 0);
        playerNo++;
    } while (playerNo < 0x20);
    data_ov016_021de648->task = DWCi_TSKlForm(0, (s32)taskSound, 0, 200);
}

void DWCi_SNDlEnd(void) {
    DWCi_TSKlDelete(0, (u32)data_ov016_021de648->task);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de648);
}

void DWCi_SNDlPlay(s32 no) {
    NNS_SndArcPlayerStartSeqArc((NNSSndHandle *)&data_ov016_021de648->handle, 0, no);
}

void DWCi_SNDlSetVolume(s32 volume) {
    NNS_SndPlayerSetVolume((NNSSndHandle *)&data_ov016_021de648->handle, volume);
}

void DWCi_SNDlSetPitch(u32 mask, s32 pitch) {
    NNS_SndPlayerSetTrackPitch((NNSSndHandle *)&data_ov016_021de648->handle, mask, pitch);
}

void DWCi_SNDlStop(void) {
    NNS_SndPlayerStopSeq((NNSSndHandle *)&data_ov016_021de648->handle, 0);
}

void taskSound(void) {
    NNS_SndMain();
}
