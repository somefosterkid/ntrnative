// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/snd/src/snd_main.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). The ARM9's file: SND_Init and the sound library's mutex.
// The port had SND_Init apart at first (pokebw2's nitro_snd_main.c), with snd_command.c's statics in its .bss.
#include <nitro/snd/common/main.h>
#include <nitro/os.h>
#include <nitro/snd/common/global.h>
#include <nitro/snd/common/exchannel.h>
#include <nitro/snd/common/seq.h>
#include <nitro/snd/common/alarm.h>
#include <nitro/snd/common/command.h>
#include <nitro/snd/common/util.h>
#include <nitro/snd/common/work.h>

    static OSMutex sSndMutex;

    void SND_Init (void)
    {
        {
            static BOOL initialized = FALSE;
            if (initialized)
                return;
            initialized = TRUE;
        }

        OS_InitMutex(&sSndMutex);
        SND_CommandInit();
        SND_AlarmInit();
    }

void SNDi_LockMutex (void)
{
    OS_LockMutex(&sSndMutex);
}

void SNDi_UnlockMutex (void)
{
    OS_UnlockMutex(&sSndMutex);
}
