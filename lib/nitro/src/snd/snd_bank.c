// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/snd/src/snd_bank.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/snd/common/bank.h>

    #include <nitro/misc.h>
    #include <nitro/mi.h>
    #include <nitro/snd/common/main.h>

static const SNDWaveData * GetWaveData(const SNDBankData * bank, int waveArcNo, int waveIndex);

void SND_AssignWaveArc (SNDBankData * bank, int index, SNDWaveArc * waveArc)
{
    SNDWaveArcLink * next;
    SNDWaveArcLink * prev;

    SDK_NULL_ASSERT(bank);
    SDK_NULL_ASSERT(waveArc);
    SDK_ASSERTMSG(((u32)bank & 0x1f) == 0, "bank address must be aligned 32 bytes boundary.");
    SDK_ASSERTMSG(((u32)waveArc & 0x1f) == 0, "waveArc address must be aligned 32 bytes boundary.");
    SDK_MINMAX_ASSERT(index, 0, SND_BANK_TO_WAVEARC_MAX - 1);

    SNDi_LockMutex();

    if (bank->waveArcLink[index].waveArc != NULL) {
        if (waveArc == bank->waveArcLink[index].waveArc) {
            SNDi_UnlockMutex();
            return;
        }

        if (&bank->waveArcLink[index] == bank->waveArcLink[index].waveArc->topLink) {
            bank->waveArcLink[index].waveArc->topLink = bank->waveArcLink[index].next;

            DC_StoreRange(bank->waveArcLink[index].waveArc, sizeof(SNDWaveArc));
        } else {
            prev = bank->waveArcLink[index].waveArc->topLink;
            while (prev != NULL) {
                if (&bank->waveArcLink[index] == prev->next)
                    break;
                prev = prev->next;
            }
            SDK_NULL_ASSERT(prev);
            prev->next = bank->waveArcLink[index].next;

            DC_StoreRange(prev, sizeof(SNDWaveArcLink));
        }
    }

    next = waveArc->topLink;
    waveArc->topLink = &bank->waveArcLink[index];
    bank->waveArcLink[index].next = next;
    bank->waveArcLink[index].waveArc = waveArc;

    SNDi_UnlockMutex();

    DC_StoreRange(bank, sizeof(SNDBankData));
    DC_StoreRange(waveArc, sizeof(SNDWaveArc));
}

void SND_DestroyBank (SNDBankData * bank)
{
    SNDWaveArc * waveArc;
    SNDWaveArcLink * prev;
    int i;

    SDK_NULL_ASSERT(bank);
    SDK_ASSERTMSG(((u32)bank & 0x1f) == 0, "bank address must be aligned 32 bytes boundary.");

    SNDi_LockMutex();

    for (i = 0; i < SND_BANK_TO_WAVEARC_MAX; i++) {
        waveArc = bank->waveArcLink[i].waveArc;
        if (waveArc == NULL)
            continue;

        if (&bank->waveArcLink[i] == waveArc->topLink) {
            waveArc->topLink = bank->waveArcLink[i].next;

            DC_StoreRange(waveArc, sizeof(SNDWaveArc));
        } else {
            prev = waveArc->topLink;
            while (prev != NULL) {
                if (&bank->waveArcLink[i] == prev->next)
                    break;
                prev = prev->next;
            }
            SDK_NULL_ASSERT(prev);
            prev->next = bank->waveArcLink[i].next;

            DC_StoreRange(prev, sizeof(SNDWaveArcLink));
        }
    }

    SNDi_UnlockMutex();
}

void SND_DestroyWaveArc (SNDWaveArc * waveArc)
{
    SNDWaveArcLink * link;
    SNDWaveArcLink * next;

    SDK_NULL_ASSERT(waveArc);
    SDK_ASSERTMSG(((u32)waveArc & 0x1f) == 0, "waveArc address must be aligned 32 bytes boundary.");

    SNDi_LockMutex();

    link = waveArc->topLink;
    while (link != NULL) {
        next = link->next;

        link->waveArc = NULL;
        link->next = NULL;

        DC_StoreRange(link, sizeof(SNDWaveArcLink));
        link = next;
    }

    SNDi_UnlockMutex();
}

SNDInstPos SND_GetFirstInstDataPos (const SNDBankData * bank)
{
    SNDInstPos pos;

    SDK_NULL_ASSERT(bank);

    (void)bank;

    pos.prgNo = 0;
    pos.index = 0;

    return pos;
}

BOOL SND_GetNextInstData (const SNDBankData * bank, SNDInstData * inst, SNDInstPos * pos)
{
    SDK_NULL_ASSERT(bank);
    SDK_NULL_ASSERT(inst);
    SDK_NULL_ASSERT(pos);

    for (; pos->prgNo < bank->instCount; pos->prgNo++, pos->index = 0) {
        u32 instOffset = bank->instOffset[pos->prgNo];
        inst->type = (u8)(instOffset & 0xff);
        instOffset >>= 8;

        switch (inst->type) {
        case SND_INST_PCM:
        case SND_INST_PSG:
        case SND_INST_NOISE:
        case SND_INST_DIRECTPCM:
        case SND_INST_NULL:
        {
            const SNDInstParam * param = (const SNDInstParam *)((u8 *)bank + instOffset);
            inst->param = *param;
            pos->prgNo++;
            return TRUE;
        }
        case SND_INST_DRUM_SET:
        {
            const SNDDrumSet * drumSet = (const SNDDrumSet *)((u8 *)bank + instOffset);
            for (; pos->index < drumSet->max - drumSet->min + 1; pos->index++) {
                *inst = drumSet->instOffset[pos->index];
                pos->index++;
                return TRUE;
            }
            break;
        }
        case SND_INST_KEY_SPLIT:
        {
            const SNDKeySplit * keySplit = (const SNDKeySplit *)((u8 *)bank + instOffset);
            for (; pos->index < SND_INST_KEYSPLIT_MAX; pos->index++) {
                if (keySplit->key[pos->index] == 0)
                    break;
                *inst = keySplit->instOffset[pos->index];
                pos->index++;
                return TRUE;
            }
            break;
        }
        default:
            break;
        }
    }

    return FALSE;
}

u32 SND_GetWaveDataCount (const struct SNDWaveArc * waveArc)
{
    SDK_NULL_ASSERT(waveArc);
    return waveArc->waveCount;
}

void SND_SetWaveDataAddress (SNDWaveArc * waveArc, int index, const SNDWaveData * address)
{
    SDK_NULL_ASSERT(waveArc);
    SDK_MINMAX_ASSERT(index, 0, waveArc->waveCount);
    SDK_ASSERT(address == NULL || address >= (void *)HW_MAIN_MEM);

    SNDi_LockMutex();

    waveArc->waveOffset[index] = (u32)address;

    DC_StoreRange(&waveArc->waveOffset[index], sizeof(u32));

    SNDi_UnlockMutex();
}

const SNDWaveData * SND_GetWaveDataAddress (const SNDWaveArc * waveArc, int index)
{
    const SNDWaveData * wave;
    u32 offset;

    SDK_NULL_ASSERT(waveArc);
    SDK_MINMAX_ASSERT(index, 0, waveArc->waveCount);

    SNDi_LockMutex();

    offset = waveArc->waveOffset[index];

    if (offset != 0) {
        if (offset < HW_MAIN_MEM)
            wave = (const SNDWaveData *)((u8 *)waveArc + offset);
        else
            wave = (const SNDWaveData *)offset;
    } else {
        wave = NULL;
    }

    SNDi_UnlockMutex();

    return wave;
}
