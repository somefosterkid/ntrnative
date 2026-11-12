// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/sound.h, the lines in use so far.
#ifndef BWPORT_GFL_SOUND_H
#define BWPORT_GFL_SOUND_H

#include "types.h"
#include "struct_decls.h"
#include "nnsys/snd.h"
#include "gfl/heap.h"

void GFL_SndBGMFadeIn(u16 frames);
void GFL_SndBGMFadeOut(u16 frames);
u32 GFL_SndBGMGetID(void);
// Leaves every channel of the sequence enabled
#define SND_CHANNEL_MASK_ALL 0xffff
void GFL_SndBGMPlay(u32 bgm, u32 channelMask);
void GFL_SndBGMPop(void);
void GFL_SndBGMPush(void);
void GFL_SndBGMSetPaused(BOOL paused);
void GFL_SndBGMSetVolume(u16 trackMask, s32 volume);
// pokebw2's GFL_SndDestroyHeap
void func_02005c18(void);
void GFL_SndInit(void);
// The player that plays a sequence
s32 GFL_SndSeqGetPlayerIndex(u32 seq);
BOOL GFL_SndPlayerIsActive(s32 player);
void GFL_SndPlayerSetVolume(s32 player, u32 volume);
void GFL_SndPlayerStop(s32 player);
void GFL_SndSEPlay(u32 se);
BOOL GFL_SndIsPlaying(u32 seq);

#define SND_VOLUME_MAX 127
#define SND_PLAYER_MASK_ALL 0x3f

// The save's Chatot recording, which a cry of Chatot plays in place of its own
typedef struct {
    void *chatter;
} PokeVoiceChatterInfo;

// Sets the tempo ratio of the BGM (256 is normal), and the pitch and pan of its tracks in trackMask, each that is
// not -1
void GFL_SndBGMSetParams(u16 trackMask, s32 tempoRatio, s32 pitch, s32 pan);
BOOL GFL_SndBGMIsFading(void);
BOOL GFL_SndBGMIsPlaying(void);
BOOL GFL_SndPlayerIsActiveAny(void);
// Changes each value that is not -1: the tempo ratio, and the pitch and pan of every track
void GFL_SndPlayerSetParams(s32 player, s32 tempoRatio, s32 pitch, s32 pan);
void GFL_SndPlayerSetVolumeEx(u32 volume, u32 playerMask);
void GFL_SEPlayKeepVol(u32 se, s32 player);
void GFL_SndStop(void);
u32 GFL_SndGetLastResID(void);

// What PokeVoice_Init is given: the wave archive of a species and form's cry, and the cry's volume with, when the
// cry is not the wave archive's (a Chatot's recording), its samples, their count, sample rate and speed, returning
// TRUE. The game's are getResIDFunc and getWaveInfoFunc
typedef void (*PokeVoiceGetResIDFunc)(u32 species, u32 form, u32 *waveArc);

typedef BOOL (*PokeVoiceGetWaveInfoFunc)(u32 species, u32 form, const PokeVoiceChatterInfo *chatterInfo,
                                         void **samples, u32 *size, u32 *sampleRate, u32 *speed, s8 *volume);

void PokeVoice_Init(u16 a0, PokeVoiceGetResIDFunc getResID, PokeVoiceGetWaveInfoFunc getWaveInfo);
// Releases the handles whose cries have ended; called each frame
void PokeVoice_Update(void);
// Cries are played through handles. A cry has a pan (64 is the center), and with chorus a second voice whose volume
// and speed are relative to the first's; reverse plays it backwards. PokeVoice_Load only readies the cry
u32 PokeVoice_Load(u32 species, u32 form, u32 pan, u32 chorus, u32 chorusVolume, u32 chorusSpeed, u32 reverse,
                   const PokeVoiceChatterInfo *chatterInfo);
BOOL PokeVoice_StartPlayback(u32 handle);
// Room for count cries at once, and its release
void PokeVoice_AllocMulti(int count, HeapID heapId);
void PokeVoice_ResetMasterVolume(void);
// Whether the sound thread is still loading the next BGM
BOOL func_02005c64(void);

// The BGM info archive: which interactive sound subsystem each BGM uses (ISS_SUBSYSTEM_*), or 0 for none
BGMInfo *BGMInfo_Create(HeapID heapId);
void BGMInfo_Free(BGMInfo *info);

void GFL_SndUpdate(void);

// The BGM info archive: which interactive sound subsystem each BGM uses (ISS_SUBSYSTEM_*), or 0 for none

// The microphone, which records Chatot's cry: set up, free, and record into a buffer, calling back when done
void setupMic(HeapID heapId);
void func_02006d64(u32 a0);
BOOL func_02006d94(void);
// 0 once the recording has started
u32 func_02006dd8(void (*callback)(u32 result, u32 *done), u32 *done);

// The microphone, which records Chatot's cry: set up, free, and record into a buffer, calling back when done

BOOL GFL_SndIsVolumeControlCallbackSet(void);

// The microphone, which records Chatot's cry: set up, free, and record into a buffer, calling back when done

// Plays a BGM on the BGM2 player, over the BGM's (swan's name says otherwise)
void GFL_SndBGMStop(u32 bgm);

// Sets the output to stereo or mono
void func_02005c28(BOOL stereo);

// A sound player's handle
NNSSndHandle *func_02006138(s32 player);

// Loads sound sequences ahead of time, and frees them: the handle is the preload record in the sound heap
BGMStackPreload *func_02005a98(const u32 *seqs, u32 count);
void func_02005b04(BGMStackPreload *preload);

void func_02005888(u16 player);
NNSSndHandle *func_0200590c(void);
BOOL func_02005924(u32 seq);
void func_0200597c(void);
BOOL func_020059a0(u32 seq);
void func_020059c8(void);
BOOL func_02005a00(void);
BOOL func_02005a4c(void);

// Sets the players in playerMask to full volume when on, else silences them
void GFL_SndPlayerSetMuteStateEx(BOOL on, u32 playerMask);

void func_02006518(void);
// Sound the musical's stage plays: names unknown
BOOL func_02006448(void *seqData, void *bankData, u32 seq);
// Points wave index of the loaded wave archive waveArc at wave srcIndex of srcWaveArc
BOOL func_020064b8(u16 waveArc, u16 index, const SNDWaveArc *srcWaveArc, u16 srcIndex);

// Whether the card's data transfer interrupt is enabled
BOOL GFL_IRQCartDataTransferIsEnabled(void);
// The BGM about to play: the next one while the BGM changes with a fade, else the one playing
u32 func_02005c4c(void);
// pokebw2's GFL_SndSetSeqVerifyCallback
// Sets the callback that says whether a sequence may play while the sound thread is loading
void func_02005d08(BOOL (*callback)(u32 seq));
// Called once with start TRUE for a sequence, then each frame with FALSE until it returns TRUE
BOOL func_020063b4(u32 seq, u32 *step, BOOL start);
// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Plays a sound effect at a volume below 128, or at the sequence's own volume
void GFL_SndSEPlayEx(u32 se, u32 volume);
void PokeVoice_CreateChatterInfo(PokeVoiceChatterInfo *info);
u32 PokeVoice_Play(u32 species, u32 form, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6,
                   const PokeVoiceChatterInfo *chatterInfo);
void PokeVoice_AdjustVolume(u32 handle, s8 volume);
void PokeVoice_AdjustSpeed(u32 handle, s32 speed);
BOOL PokeVoice_IsPlaying(u32 handle);
BOOL PokeVoice_IsPlayingAny(void);
void PokeVoice_FreeMulti(void);
void PokeVoice_ReleaseAll(void);
void PokeVoice_Release(u32 handle);
// What a handle plays: its volume, speed, samples, count of samples and sample rate
s8 PokeVoice_GetVolume(u32 handle);
int PokeVoice_GetSpeed(u32 handle);
const s8 *PokeVoice_GetSamples(u32 handle);
u32 PokeVoice_GetSampleCount(u32 handle);
int PokeVoice_GetSampleRate(u32 handle);
void PokeVoice_SetMasterVolume(u32 volume);
// The BGM info archive: which interactive sound subsystem each BGM uses (ISS_SUBSYSTEM_*), or 0 for none
u8 BGMInfo_GetISSSubsystem(BGMInfo *info, u16 bgm);
// The sound handle that plays the BGM
NNSSndHandle *func_02005c3c(void);
void getResIDFunc(u32 species, u32 form, u32 *waveArc);
BOOL getWaveInfoFunc(u32 species, u32 form, const PokeVoiceChatterInfo *chatterInfo, void **samples, u32 *size,
                     u32 *sampleRate, u32 *speed, s8 *volume);
void ampOffFreeBlocks(void);
// Saves the recording as the Chatot's
void func_02006e18(void *chatter);
void func_02005d28(void);
void GFL_SndSetVolumeControlCallbacks(void);
void func_020057dc(NNSSndHeapHandle *heap);
s32 func_02005900(void);
void func_02006504(void);
void func_020064f4(u16 seq);
// Changes the BGM, fading the one playing out and the next one in over the frames given, while a thread loads it
void func_02005da4(u32 bgm, u16 channelMask, s32 fadeOutFrames, s32 fadeInFrames);

#endif // BWPORT_GFL_SOUND_H
