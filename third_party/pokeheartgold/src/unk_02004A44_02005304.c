#include "global.h"

#include "sound.h"

s32 sub_02005B20(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
void BGM_SaveStateAndPlayNew(int a0);
#else
void BGM_SaveStateAndPlayNew(u8 a0);
#endif

s32 sub_02005328(s32);
void Sound_LoadHeapStateBGM(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
void BGM_SaveStateAndPlayNew(int a0) {
#else
void BGM_SaveStateAndPlayNew(u8 a0) {
#endif
    GetSoundDataPointer();
    sub_02005B20();
    PlayBGM((u16)a0);
}

void Sound_LoadHeapStateBGM(void) {
    GF_Snd_LoadState(sub_02005328(4));
}
