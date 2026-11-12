# port/acore: a DS sound core from the hardware and the driver's behaviour

port/acore is the PC port's own sound implementation, built the way port/vcore was (docs/vcore.md): written from the
public descriptions (GBATEK, the SDAT format docs), checked automatically against a reference that runs the game's own
ARM7 sound driver in melonDS. It replaces libntr's ARM7 side (libraries/sim7's snd: the sequencer, channels and
sim_audio.cpp's mixer), which is incomplete (SETUP_CHANNEL_PCM, CHANNEL_TIMER/VOLUME/PAN, capture, master volume and
output selection do nothing there; the Pokemon cries go through those). The ARM9 side (the SDK's SND library and
NitroSystem's NNS_Snd, libntr's libraries/snd and libntrsystem's snd) builds command lists and stays for now; a later
piece (18) puts acore behind it in the port.

The groundwork: this file and the skeleton port/acore/ are BUILT. The capture (BWPORT_ACAPTURE), the
reference (<ref>/aoracle/), the compare tool (tools/acore_compare.py) and the tools mkacs.py / acs_dump.py are
DESIGNED below but NOT BUILT YET (see "Open items").

## What the game uses

From `grep -rn 'NNS_Snd\|SND_' src lib/gfl` (Black's own code; lib/nitro/src/snd and lib/nnsys/src/snd are the SDK's):

| feature | used | where | on the ARM7 |
|---|---|---|---|
| sequences (SSEQ), BGM | yes | snd_sys.c, snd_bgm_stack.c, field_sound_system.c: NNS_SndArcPlayerStartSeq(Ex), NNS_SndArcLoadSeq(Ex) | START_SEQ / PREPARE_SEQ + START_PREPARED_SEQ, the sequencer |
| sequence archives (SSAR), sound effects | yes | NNS_SndArcLoadSeqArc, NNS_SndArcPlayerStartSeqArc (via snd_sys.c) | the same commands: base = the SSAR's data, offset = the sequence's |
| banks (SBNK) and wave archives (SWAR) | yes | loaded with each sequence (NNS_SndArcLoadSeq/Group/Bank), SND_AssignWaveArc; SND_SetWaveDataAddress (snd_sys.c func_020064b8, the musical) | instrument and wave lookup at each note |
| player and track parameters | yes | NNS_SndPlayerSetVolume/PlayerVolume/TrackPitch/TrackPan/TrackMute(Ex)/TrackModDepth/Speed/TempoRatio/MoveVolume; player_volume_fader.c | PLAYER_PARAM, TRACK_PARAM (raw offsets into the ARM7's SNDPlayer / SNDTrack), MUTE_TRACK |
| sequence variables | yes | the SDK's SND_SetPlayerLocal/GlobalVariable via NNS | PLAYER_LOCAL_VAR / GLOBAL_VAR, the shared work |
| direct channels (wave out) | yes: the Pokemon cries | pm_voice.c (NNS_SndWaveOutStart/SetVolume/SetSpeed, a "chorus" second channel), ctvt_mic.c (Xtransceiver) | LOCK_CHANNEL, SETUP_CHANNEL_PCM, CHANNEL_TIMER/VOLUME/PAN, START_TIMER/STOP_TIMER |
| master volume | yes | NNS_SndSetMasterVolume (6 calls), NNS_SndSetMonoFlag | MASTER_VOLUME, MASTER_PAN, OUTPUT_SELECTOR |
| driver info | yes | NNS_SndUpdateDriverInfo, NNS_SndPlayerReadDriverTrackInfo | READ_DRIVER_INFO (the ARM7 copies its work to the ARM9) |
| streams (STRM) | Black, White: no (online only); Black 2, White 2: the title music | NNS_SndStrm* only in ov013/dwc_rap_vchat.c (voice chat, stubbed); B2/W2: NNS_SndArcStrm* in snd_strm.c | not on the ARM7 here: ac_strm.c plays it host side ("The archive stream") |
| sound capture (reverb, effects) | no | no NNS_SndCapture* call in the game | SETUP_CAPTURE: in the hardware spec, tested synthetically |
| microphone | sampling only | snd_mic.c (Chatot), mictest.c, ctvt_mic.c | the MIC library, not the sound unit: out of scope |
| PSG / noise instruments | in banks | SBNK instrument types 2 and 3 | channels 8-13 / 14-15 |

## The archive stream (STRM; Black 2 and White 2, 2026-11-06)

Black 2's and White 2's title music is their sound archive's only stream (stream 0; pokebw2's src/system/snd_strm.c, called
by ov162/title.c). The port plays it with its own player instead of libntrsystem's (the maintainers' notes, "The title's music"):
port/src/strm_arc.c is the game's side (PORT_SndArcStrm*, which port/include_b2w2/port_b2w2.h puts in place of the
NNS_SndArcStrm* / NNS_SndStrmHandle* names, for pokebw2's C only), port/bridge/acore_bridge.c's ab_strm_* hold the decoded
samples, and port/acore/src/ac_strm.c decodes and plays them: host side, added to the mixer's sum (p04's `ac_mix` and
`ac_mix_block`) as the DS's two stream channels would add, so the master volume, the DAC and the quality setting apply; on
the music bus when it is split (domains 0 and 2, not 1). No ring buffer, ARM7 alarm or timer: nothing the game reads needs
one (it reads only the position in ms, and only to see 0 once its fade has ended). Nothing goes into the sound heap: the file
is read with NNS_SndArcReadFile into host memory, decoded and freed; the decoded samples are freed when the title frees the
stream player's channels. Those channels (the archive's stream player 0: channels 0 and 6) are locked from the sequences
with SND_LockChannel while the title runs. Sound=libntr: no acore, the stream is silent (its position reads 0, so the title
goes on).

The stream in both games' archives: stream 0 = archive file 5196, volume 127, player 0; PCM16, stereo, 32728 Hz (timer 512:
one sample per mixer sample), no loop, 3665535 samples (112.0 s), one block per channel (7.3 MB each).

Format facts used (sources: kiwi.ds's "Nitro Composer File (*.sdat) Specification", 2007, with loveemu's strm2wave notes,
web.archive.org/web/2008/http://kiwi.ds.googlepages.com/sdat.html; GBATEK "DS Sound"):
- SDAT: the INFO block's record 7 (STRM) entries are {u16 fileId, u16, u8 volume, u8 priority, u8 player, u8[5]}, record 6
  (PLAYER2) {u8 count, u8 channel[16], ...}; the FAT has {u32 offset, u32 size, u32[2]} per file. The port reads them through
  the archive's public calls (NNS_SndArcGetStrmInfo, NNS_SndArcGetStrmPlayerInfo, NNS_SndArcGetFileSize, NNS_SndArcReadFile).
- STRM: a 16-byte "STRM" header; HEAD at 0x10, from 0x18: u8 type (0 PCM8, 1 PCM16, 2 IMA-ADPCM), u8 loop, u8 channels, u8,
  u16 rate, u16 timer / 32, u32 loop start (samples), u32 samples, u32 data offset (0x68), u32 blocks, u32 block length, u32
  samples per block, u32 last block length, u32 samples in the last block (lengths and counts per channel). The samples
  start at the data offset, block by block, each block's channels one after the other (the last block's at its own length).
- A block is SWAV-like: PCM8 signed bytes, PCM16 little-endian, IMA-ADPCM a header word (s16 first sample, the index in bits
  16-22) then 4-bit codes, low nibble first, decoded as the hardware does (GBATEK; p02's `ac_adpcm_step`, shared).
- A channel's timer period is 16756991 / rate counts, 512 counts a mixer sample (GBATEK); played nearest-sample, as the DS.
- Assumed (not in the documents): the volume is the archive's 0-127 through the driver's dB table (`ac_decibel`, as a
  sequence's volume); a fade lowers a 0-127 fade volume linearly, one step a game frame (560190 cycles of the sound's time),
  and adds its dB; a stereo stream's channels are panned full left and full right, a mono one centred.
- Checked (Mac, 2026-11-06): with BWPORT_SOUNDCLOCK=device (its BWPORT_AUDIO_WAV is acore's own DS-rate output; the frame
  clock's WAV with SDL's dummy driver is the ring's, with its drops and +-3 % rate control, no good for a sample compare) the
  title's capture equals a decode straight from the ROM: gain 1.0000 both channels, no lag drift over 40 s, residual -47 dB
  (the DAC's 10 bits); START's 4-frame fade steps 0.75 / 0.50 / 0.25 / 0 a frame; a restart to title plays it again. Black: an
  acore_render of a Black capture (title, town) is byte-identical before and after (DS and Enhanced).

## The DS's sound side

```
ARM9 (the game, NNS_Snd, the SDK's SND)                ARM7 (the SDK's sound driver, its own thread)
  SND_PushCommand: SNDCommand {next, id, arg[4]}         PXI callback: data >= 0x02000000 -> the list queued;
  in main memory, linked                                    data == 0 -> wake the thread
  SND_FlushCommand: PXI FIFO tag 7, the list's    --->   every wake (a list + wakeup, or the periodic alarm every
  address (26 bits); SND_COMMAND_IMMEDIATE adds           0xAA8 OS ticks = 174592 cycles = 5.2095 ms):
  a wakeup word 0                                           UpdateExChannel; CommandProc (each queued list in order,
                                                            finishCommandTag++); SeqMain(periodic); ExChannelMain(
  reads SNDSharedWork (main memory): finishCommand-  <---   periodic); UpdateSharedWork; CalcRandom
  Tag, player status, channel status, variables,          alarms: a PXI message to the ARM9 when they fire
  tick counters                                            writes the sound registers 0x04000400-0x0400051F
                                                                    |
                                                         the sound unit: 16 channels (PCM8/16, IMA-ADPCM, PSG 8-13,
                                                         noise 14-15), 2 capture units, the mixer, master volume,
                                                         bias -> 10 bits, one stereo sample per 1024 cycles
                                                         (33513982 / 1024 = 32728.5 Hz)
```

acore copies both ARM7 layers: **the driver** (pieces 6-17: what the SDK's ARM7 code does with commands and sequence
data, down to the register values it writes) and **the hardware** (pieces 1-5: the sound unit, register values ->
samples). The two are checked apart: the driver by its register writes (a register trace, exact), the hardware by
its samples from a given register stream (exact).

### Sources

- GBATEK (<ref>/gbatek/, INDEX.txt; tools/gbatek_split.py now also splits the sound chapters): 57 channels, 58
  SOUNDCNT/SOUNDBIAS, 59 capture, 60 block diagrams, 61 sound notes (start delays, the mixer's bit widths, ADPCM, PSG
  duty, noise), 63-69 the SDAT, SSEQ, SSAR, SBNK, SWAR, SWAV, STRM formats (the SSEQ command list with parameters), 70
  timers, 71 IPC (the FIFO), 72 BIOS sound functions. The ARM7 BIOS's GetSineTable / GetPitchTable / GetVolumeTable
  (SWI 1Ah-1Ch, their ranges only) are in <ref>/gbatek.htm (grep GetPitchTable).
- <ref>/sound/: nds-sdat.html (Kiwi's "NDS Sound Data (SDAT) Format" spec, feshrine.net: the SSEQ events, the
  SBNK instrument records, the ADSR fields), vgmtrans/ (MIT, its NDS SSEQ/SBNK/SWAR readers under src/main/formats:
  how the community reads the formats), see <ref>/sound/README.
- The reference: <ref>/aoracle/ runs the ROM's own ARM7 binary (its sound driver) in melonDS with the captured
  command stream; melonDS's SPU renders it (below).
- Rules as for vcore: no libntr code, no NitroSDK headers or decompiled SDK code (lib/nitro/src/snd, the ARM7 decomps
  in <ref>/pokediamond/arm7, libntr's sim7), no GPL code (melonDS) copied into port/acore. Reading the decomps or
  melonDS to settle a detail the documents leave open (an envelope's rounding, a table's values) is allowed only to
  understand the behaviour; write the code yourself, say in a comment which document it follows, and let the oracle
  decide. The DS's data layouts (SNDCommand, SNDPlayer offsets, SNDSharedWork) are the protocol and are written out
  in ac_types.h from the SDK's interface, as vcore's register offsets are.

## The stream: .acs

acore's input, the oracle's input and the capture's output are one format: a DS sound session as the ARM7 sees it.
"ACST", u32 version 1, then chunks (4-byte tag, u32 size, data padded to 4), in time order. Times are ARM7 cycles
(33513982 Hz, 64-bit) from the stream's start; a frame is 560190 cycles; a capture's events at frame f are at
`f * 560190` (+ the order within the frame: events at one time run in file order).

| chunk | data | meaning |
|---|---|---|
| INFO | text, key=value lines | source, frames, notes, `tick0=` (the driver's first periodic wake, cycles; default 0), `mode=driver\|hw` |
| MEM  | u64 time, u32 addr, bytes | write bytes to main memory (DS address 0x02000000-0x023FFFFF) at time: sequence, bank and wave files, wave-out samples, the shared work's initial value |
| CMDS | u64 time, u32 frame, u32 n, n x {u32 id, u32 arg[4]} | one command list sent through the FIFO (the SDK's SNDCommand ids 0-33, DS-layout arguments: pointers are DS addresses) |
| WAKE | u64 time | a FIFO wakeup word (0): the driver wakes and runs the queued lists |
| REGS | u64 time, u32 n, n x {u32 addr, u32 value, u32 size} | ARM7 sound register writes (mode=hw streams for pieces 1-5; the oracle's trace has the same records) |
| END  | u64 time | the stream's length: render up to here |

The address plan (both the capture and synthetic streams keep to it; the oracle's ARM7 owns the rest): command lists
0x02040000-0x020FFFFF (the oracle writes each CMDS list there as linked SNDCommands), data 0x02100000-0x0237FFFF (the
capture maps the port's sound heap here), the shared work and driver info buffers 0x02380000 down (below the ARM7's
binary at 0x02380000: 0x0237F000 and 0x0237D000).

`python3 port/acore/tools/acs_dump.py FILE.acs [--cmds] [--mem]` lists a stream: chunks, commands by name with their
arguments, the files a MEM writes (magic, size).

## The outputs

- PCM: stereo s16 at 32728.5 Hz (one sample per 1024 cycles), the DS's 10-bit output value v (0-0x3FF after the bias
  and the clip) written as `(v << 6) - 0x8000`, as melonDS with 10-bit degrade and the bias applied. A WAV file says
  32728 Hz.
- The register trace (`--regs FILE`): one line per sound register write `T ADDR SIZE VALUE` (T in cycles, hex
  address and value), the driver's writes in order. The oracle writes the same from the real driver.
- The shared work (DS layout) and READ_DRIVER_INFO's buffer are written into acore's main memory, as the DS does; the
  tests read them there.

## Layout and ownership

```
port/acore/
  meson.build, list_sources.py   its own meson project (not part of the port's build); sources = src/*.c, tests/*.c
  include/acore.h                the public interface                                       SKELETON
  src/ac_types.h                 constants, register offsets, command ids, the DS layouts   SKELETON
  src/ac_shared.h                AcoreState, memory and register access, every cross-piece
                                 function's declaration with its owner                      SKELETON
  src/ac_state.c                 the .acs loader, the event loop (time, samples, wakes)     SKELETON (piece 17's)
  src/ac_wav.c                   WAV write/read                                             SKELETON
  src/pNN_<name>.c / .h          piece NN: its code / its own state struct AC_PNN           PIECE NN
  tests/act.h, tests/act.c       the unit-test framework and runner, stream builders        SKELETON
  tests/t00_core.c               the skeleton's tests                                       SKELETON
  tests/tNN_<name>.c             piece NN's unit tests                                      PIECE NN
  tools/acore_render.c           acore_render FILE.acs OUT.wav [--regs T.txt] [--to CYCLES] SKELETON
  tools/mkacs.py                 synthetic streams (hardware and driver)                    SKELETON
  tools/acs_dump.py              read a stream                                              SKELETON
```

The rules are vcore's: a piece edits only its own files (pNN_*.c/.h, tests/tNN_*.c, more files named pNN_*), the
SKELETON files are frozen while the pieces run (a piece that needs a shared type changed writes the request in its
report and keeps its data in its AC_PNN struct meanwhile, `s->pNN`), a function another piece calls is declared in
ac_shared.h with its owner, the stubs compile and do nothing (return 0). C11, gcc, clang and mingw; no extensions
beyond the tests' `__attribute__((constructor))`.

Cross-piece calls: a piece that needs another piece to call it (once per sequencer tick, at note-on, at start) appends one line to <scratch> (caller piece, callee function, when, why) as well as saying so in its report; the integration step (the main session or an integration agent) wires every line.

## The interface

`include/acore.h`: `acore_create`, `acore_destroy`, `acore_load_acs(s, path, err, len)` / `_mem`, `acore_run(s,
until)` (cycles; renders samples into the state's buffer), `acore_pcm(s, &nsamples)` (interleaved s16 L R),
`acore_render_all(s)` (to END), `acore_set_trace(s, FILE *)`, `acore_mem(s)` (the 4 MB main memory),
`acore_write_wav(path, pcm, n, rate)`.

## The time base (ac_state.c; piece 17 owns it from now on)

`acore_run` takes events in time order; between them it renders samples. Sample k is taken at cycle `k * 1024 +
sample_phase` (INFO `sphase=`, default 0). At each sample: `ac_hw_sample(s, out)` (the skeleton's: each channel's
output `ac_ch_output` (1, through 2/3 by format), `ac_mix` (4), `ac_cap_sample` (5), then `ac_ch_step` (1) advances
each running channel by 512 timer ticks, the 16.76 MHz channel clock's count in 1024 cycles). Events:

- MEM: copied into `s->mem`. REGS: `ac_hw_write` each (the skeleton's register file; it calls `ac_ch_start` /
  `ac_ch_stop` (1) on a SOUNDxCNT start bit edge and `ac_cap_start` / `ac_cap_stop` (5) on SNDCAPxCNT bit 7).
- CMDS: the list is queued (`s->drv.queue`). WAKE: `ac_driver_wake(s, 0)` (17).
- The periodic alarm: at `tick0 + k * 174592`, `ac_driver_wake(s, 1)`.

`ac_driver_wake(s, periodic)` (17) runs the driver's steps in the SDK's order: `ac_exch_sync(s)` (11: start/stop the
hardware channels the last step flagged), `ac_cmd_proc(s)` (15: every queued list, then finishCommandTag++),
`ac_seq_main(s, periodic)` (9), `ac_exch_main(s, periodic)` (14), `ac_shared_update(s)` (16), `ac_random(s)` (6).
All register writes of one wake land at the wake's cycle (the real driver's take a few hundred cycles: the trace
shows by how much; compare with the tool's lag search).

### The live clock

The live port's sound time is the game's frames, not the audio device's: `SIM_Audio_FrameTick` (libntr, end of each
frame in SIM_Render, not while the game is paused) runs the sound side for 560190 ARM7 cycles: sim7's driver and
channels, or acore (`SIM_SoundBackend.run` = acore_bridge.c's `ab_run(to_cycle)`: acore_run to the cycle, the DS-rate
samples back). The samples go into a ring of DS-rate frames (4096, ~125 ms) that the audio callback plays, cubic-resampled
to the device rate with a rate that follows the ring's fill (since 2026-11-01 a slow PI controller, "The ring's rate, cuts and fades"): silence when it runs dry (until 1.5 frames are
in), samples dropped when it is full (uncapped). Before each step the PXI thread is drained (SIM_PxiDrain: every command
the game sent has reached acore's queue, so which frame a command lands in does not depend on thread timing).
A command wait (libntr's SND_RecvCommandReply with SND_COMMAND_BLOCK) calls SIM_Audio_WaitPump: iteration 0 only
drains, later ones advance the sound by one driver tick (a frame cannot come while the game spins there).
- WAV time 0 (BWPORT_AUDIO_WAV, written by libntr's sim_audio.cpp for both backends, DS rate, header 32728 Hz) is cycle 0
  of the sound clock = the first frame tick; sample k is at cycle k*1024 (+ acore's sphase); the .acs of
  BWPORT_ACAPTURE stamps frame N at N*560190, so the two line up (live vs oracle best lag -0.06 ms).
- `ab_render` (the device clock, BWPORT_SOUNDCLOCK=device) still writes its own DS-rate WAV, opened at its first call;
  with the frame clock `ab_render` is not called.
- BWPORT_SOUNDCLOCK=device: the old behaviour (the callback runs the sound in real time). BWPORT_AUDIOLOG=1: the
  ring's fill, drops and underruns once a second on stderr.
- Measured: route1 lockstep dumps byte-identical at 60 fps and LSTEP_FAST=1 (first divergence f=413, then f=598
  as before); title from boot, live acore vs oracle: spectral 0.77 / best lag 198 ms -> 0.9987 / -0.06 ms,
  exact 36 % -> 39.5 % (50.4 % at the best lag: the frame stamps put a command up to a frame off its real time).
  60 fps on the dummy driver: 0 underruns, 0 drops in 2400 frames.
- Open: acglue_cmds still waits (500 ms) when 64 lists queue up within one frame; the queue is drained once per frame now.

## Build, run, test

```
meson setup build/acore port/acore              # once (--reconfigure after adding a file)
ninja -C build/acore                            # libacore, acore_tests, acore_render
build/acore/acore_tests [p07 ...] [-q]          # unit tests: PASS / FAIL / TODO and a summary; exit 1 on a FAIL
build/acore/acore_render F.acs OUT.wav [--regs R.txt] [--to CYCLES] [--info]
python3 port/acore/tools/mkacs.py --all DIR     # synthetic streams: hw_pcm16, hw_pcm8, hw_adpcm, hw_psg, hw_noise,
                                                #   hw_mix, drv_note (one note of a hand-made bank), drv_cry (wave out)
python3 port/acore/tools/acs_dump.py F.acs --cmds
<ref>/aoracle/aoracle F.acs OUT.wav [--regs R.txt] [-v]          # the reference (below)
python3 tools/acore_compare.py compare REF.wav OUT.wav [--lag MS]    # match % sample-exact, max error, RMS / spectral
python3 tools/acore_compare.py regs REF.txt OUT.txt                  # register traces: first difference, counts
python3 tools/acore_compare.py corpus --acore build/acore/acore_render --oracle <ref>/aoracle/aoracle \
        --corpus DIR --out OUTDIR                                    # every stream of a directory
```

### Unit tests

tests/act.h: `ACT_TEST(pNN_name) { ... }`, `ACT_CHECK(cond)`, `ACT_EQ(a, b)`, `ACT_TODO("...")`. Helpers:
`act_state()` (a zeroed state with the driver initialised as the SDK's SND_Init leaves it: master volume 127, the
channels off), `act_mem(s, addr, data, n)`, `act_reg(s, addr, value, size)` (a register write now),
`act_cmd(s, id, a0, a1, a2, a3)` (append a command to the pending list), `act_flush(s)` (queue the list and wake),
`act_run(s, cycles)`, `act_sample(s, k)` (output sample k, L and R), the builders `act_sseq(s, addr, bytes, n)`,
`act_sbnk_pcm(s, addr, key, wave_arc_addr, ...)`, `act_swar(s, addr, waves...)` (minimal valid files), and
`act_emit(s, "pNN_scene")` (with `ACT_EMIT=DIR`, writes the test's stream as DIR/pNN_scene.acs for the oracle).
Expected values come from GBATEK and the format docs, worked by hand; the oracle settles what they leave open.

## The done-check (every piece)

1. **Unit tests**: `acore_tests pNN` passes, no TODO left, every case of the piece's spec covered.
2. **Synthetic streams vs the oracle**: the piece's emitted streams (`ACT_EMIT=<scratch> acore_tests pNN`)
   rendered by acore and by the oracle: hardware pieces (1-5) 100% sample-exact (`acore_compare.py compare`, lag 0);
   driver pieces (6-16) the register trace identical in values and order (`acore_compare.py regs`; times may differ
   by the driver's run time inside one wake) and the PCM >= 99% exact.
3. **Corpus**: the clips of <ref>/acorpus/ (captured from the port, below) whose features include the piece's:
   register traces equal for the first N wakes (report N and the first difference), the PCM's tolerant score
   (RMS error per channel, the spectral match) above the bar the compare tool prints. Piece 17 reports the whole
   table.

Dependencies: 1 and 4 are needed for any sound (wave 1, with 17's loop, already working in the skeleton); 6 is needed
by every driver piece's oracle check; the sequencer chain is 7 -> 8 -> 9 -> 10 -> 11 -> 14 (notes are heard once all
six exist), 12 and 13 add to it; 15 and 16 make the wave-out path (cries) work with 1, 4 and 11. All start together on
unit tests; the main session merges 1, 4, 6, 17 first, then the chain.

## The pieces

GBATEK numbers are <ref>/gbatek/INDEX.txt's; "kiwi" is <ref>/sound/nds-sdat.html.

### The hardware

**1. PCM channels** -- p01_pcm.c/.h, tests/t01_pcm.c. `ac_ch_start(s, ch)`, `ac_ch_stop(s, ch)`, `ac_ch_step(s, ch)`,
`ac_ch_output(s, ch)` (the channel's current sample, s16 scale, before volume; dispatches ADPCM to 2 and PSG/noise to
3). SOUNDxCNT/SAD/TMR/PNT/LEN as latched at the start, the timer (TMR counting up at 16.76 MHz, overflow -> next sample,
reload), PCM8 (NN -> NN00) and PCM16 fetched from `s->mem` by SAD, the start delays (PCM 3 samples, GBATEK 61), the
repeat modes: manual, loop (PNT then LEN repeated), one-shot (busy cleared at the last sample, the last sample held
with Hold), the busy bit in SOUNDxCNT. GBATEK 57, 61. Tests: a PCM16 ramp at TMR 0xFE00 (one sample per 512 ticks:
exactly one per output sample), TMR 0xFF00 (two per output sample), PCM8 scaling, a loop's wrap (PNT + LEN words),
one-shot end and busy, Hold, a restart while running, the minimum length rule (PNT+LEN < 4 words).

**2. IMA-ADPCM** -- p02_adpcm.c/.h, tests/t02_adpcm.c. `ac_adpcm_start(s, ch)` (the header word: PCM16 value and
index), `ac_adpcm_next(s, ch)` (one 4-bit sample: GBATEK's "in practice" diff with its rounding, the clipping quirks,
the index table), the loop: at loop start the state is saved, at the loop end restored. The 11-sample start delay (3
+ 8 for the header). GBATEK 61, 68. Tests: a hand-decoded block of 16 nibbles, the -8000h/+7FFFh clip asymmetry, the
index clamps 0/88, a loop restoring the saved state, the delay.

**3. PSG and noise** -- p03_psg_noise.c/.h, tests/t03_psg_noise.c. `ac_psg_start(s, ch)`, `ac_psg_next(s, ch)`: duty
cycles on channels 8-13 (8 steps, starting LOW, duty 7 = always LOW), the noise LFSR on 14-15 (X = 7FFFh at start,
`X >>= 1; carry ? (LOW, X ^= 6000h) : HIGH`), levels +-7FFFh, 1-sample start delay, format 3 on channels 0-7 silent.
GBATEK 57, 61. Tests: each duty's 8-step pattern, the first 32 noise outputs worked by hand, PSG ignoring LEN/repeat.

**4. The mixer** -- p04_mixer.c/.h, tests/t04_mixer.c. `ac_mix(s, const s32 chout[16], s16 out[2])`: per channel the
volume divider (shift 0/1/2/4), the volume (N/128, register 127 = 128), the pan (left (128-N)/128, right N/128), the
GBATEK 61 bit widths and the rounding at each step, the sum, SOUNDCNT bits 8-13 (output from mixer / ch1 / ch3 /
ch1+ch3, ch1 and ch3 to the mixer or not), the master volume (N/128/64), the master enable (bit 15), the bias
(SOUNDBIAS) and the clip to 0-3FFh; out = (v << 6) - 0x8000. GBATEK 58, 60, 61. Tests: one channel full left/right,
centre pan, each divider, master volume 0/64/127, the clip with 16 channels at full, the output selectors, bias 0 and
3FFh.

**5. Capture** -- p05_capture.c/.h, tests/t05_capture.c. `ac_cap_start(s, n)`, `ac_cap_stop(s, n)`, `ac_cap_sample(s,
mix_l, mix_r, chout)`: SNDCAPxCNT (add mode, source: mixer or channel 0/2, one-shot/loop, PCM8/16, start), DAD, LEN
(words), the capture timer (it runs on channel 1's / 3's timer), the clipping and the PCM8 rounding (GBATEK 61
"capture clipping"), the writes into `s->mem`, the add mode (channel 1/3 plus the capture input); the documented
capture bugs (GBATEK 59). Not used by the game (no NNS_SndCapture): synthetic streams only. GBATEK 59, 60, 61. Tests:
PCM16 capture of a constant mixer, PCM8 rounding, loop wrap, add mode.

### The driver

**6. Tables and arithmetic** -- p06_tables.c/.h, tests/t06_tables.c. `ac_decibel(s, x)` (the 0-127 -> dB*10 table the
driver uses for volumes, velocity and the sustain level), `ac_volume_table(i)` (BIOS GetVolumeTable, 724 entries),
`ac_pitch_table(i)` (BIOS GetPitchTable, 768 entries), `ac_sine(i)` (the LFO's sine, a quarter wave of 33 values),
`ac_calc_timer(base_timer, pitch)` (pitch in 1/64 semitones: octave shift and the pitch table), `ac_calc_volume(db,
&vol, &shift)` (dB*10 -> the register volume and divider), `ac_random(s)` (the driver's LCG, advanced every wake).
Sources: GBATEK 72 + the BIOS SWIs' ranges, kiwi; the tables are not in GBATEK: write them from their formulas and
check every entry against the oracle (a driver stream that plays each pitch / volume and the trace's TMR / volume
values). Tests: known entries (the table ends, the octave boundaries), calc_timer at +-1 octave, calc_volume at 0,
-6 dB, the minimum.

**7. The sequence reader and flow** -- p07_seq_flow.c/.h, tests/t07_seq_flow.c. `ac_track_tick(s, p, t)`: one tick
of a track: the wait countdown, then commands until a wait: notes 0x00-0x7F (key, velocity, VLQ length: to 10 via
`ac_note_on`), 0x80 wait, 0x81 program, 0x93 open track, 0x94 jump, 0x95 call, 0xFD return, 0xD4 loop start, 0xFC
loop end, 0xA0 random / 0xA1 variable / 0xA2 if prefixes, the variable ops 0xB0-0xBD (set, add, sub, mul, div,
shift, random, compare -> the track's condition flag), 0xFE (tracks used), 0xFF end; the reads through `ac_rd8` from
the track's DS addresses; parameter commands passed to 8 (`ac_track_param`). GBATEK 64, kiwi. Tests: each command on a
hand-made sequence in memory (the track's cur, wait, call stack, loop counts, variables after N ticks), VLQ lengths,
the if-prefix skipping, nested calls to depth 3.

**8. Track parameters** -- p08_seq_param.c/.h, tests/t08_seq_param.c. `ac_track_param(s, p, t, cmd, value, ...)`: pan
0xC0, volume 0xC1, player volume 0xC2, transpose 0xC3, pitch bend 0xC4, bend range 0xC5, priority 0xC6, mono/poly
0xC7, tie 0xC8, portamento key 0xC9 / on-off 0xCE / time 0xCF, modulation depth 0xCA, speed 0xCB, type 0xCC, range
0xCD, delay 0xE0, attack/decay/sustain/release 0xD0-0xD3, expression 0xD5, tempo 0xE1, sweep pitch 0xE3, the print
0xD6 (ignored); and `ac_track_update_channels(s, p, t, release)`: the track's volume (volume, expression, player
volume, faders, in dB through 6), pan (pan, pan range, the player/ext pan), pitch (bend x range, transpose, ext pitch)
and modulation pushed into each of its channels' user fields every tick. GBATEK 64, kiwi. Tests: each command's
field, the dB composition of volume 127/64/0 with expression, a bend of +-127 at range 2.

**9. Players** -- p09_player.c/.h, tests/t09_player.c. `ac_seq_main(s, periodic)` (each active player: the tempo
counter `+= tempo * tempo_ratio / 256`, a tick per 240, the ticks' track updates, the tick counter), `ac_player_start
/ prepare / start_prepared / stop / pause / skip`, track allocation from the pool of 32 (`ac_track_alloc`), the
player's prio and volume, mute (`ac_track_mute`: off, no-stop, release, stop), the raw PLAYER_PARAM / TRACK_PARAM
writes into the DS-layout structs, a player's end when its tracks end. GBATEK 64, kiwi (tempo 0xE1, 0xFE). Tests:
ticks per wake at tempo 120/240 and ratio 256/128, start/stop, prepare + start, pause, skip N ticks, track pool
exhaustion, mute modes.

**10. Banks and note-on** -- p10_bank.c/.h, tests/t10_bank.c. `ac_note_on(s, p, t, key, velocity, length)`: the
instrument from the player's bank (SBNK: the record for the program, types 1 PCM, 2 PSG, 3 noise, 4 direct PCM,
5 null, 16 drum set, 17 key split), the wave from the bank's wave archive links (SWAR: the wave's SWAV header: format,
loop, rate, timer, loop start, length), the channel set up through 11 (`ac_exch_alloc`) and its start (`ac_exch_start`:
key, base key, velocity, length, ADSR from the instrument unless the track's are set, the instrument's pan). GBATEK
66, 67, 68, kiwi. Tests: each instrument type on a hand-made bank, a drum set's range, a key split's regions, a missing
wave archive (no note), the PSG duty from the record.

**11. Channel allocation** -- p11_alloc.c/.h, tests/t11_alloc.c. `ac_exch_alloc(s, chmask, prio, strong, callback)`
(the channel kind's allowed set: PCM any, PSG 8-13, noise 14-15; the SDK's channel order; a free channel first, else
the lowest priority one, ties by envelope level; locked channels never), `ac_exch_free`, `ac_exch_release`,
`ac_exch_lock / unlock` (LOCK_CHANNEL, UNLOCK_CHANNEL and its "stop" flag), the track's channel list (mono mode,
tie), `ac_exch_sync(s)` (UpdateExChannel: the start/stop/timer/volume/pan sync flags -> register writes through
`ac_hw_write`, the order of a channel's writes as the trace shows). Tests: allocation order, stealing by priority,
the lock mask, sync flags producing the expected register writes.

**12. Envelopes** -- p12_envelope.c/.h, tests/t12_envelope.c. `ac_env_init`, `ac_env_set_attack/decay/sustain/release`
(the 0-127 values -> the rates the driver uses: the attack table for 109-127, the decay/release rate conversion, the
sustain level in dB), `ac_env_update(s, ch)` (per periodic wake: attack multiplies towards 0, decay and release
subtract, the release's end at the floor -> the channel stops), `ac_env_level`. kiwi (SBNK ADSR), GBATEK 66. Tests:
the conversions at their ends, an attack of 127 (instant), a decay to a sustain level, a release to the end in N
wakes.

**13. LFO, sweep and portamento** -- p13_lfo_sweep.c/.h, tests/t13_lfo_sweep.c. `ac_lfo_init`, `ac_lfo_start`,
`ac_lfo_update(s, lfo)` (delay, the counter advanced by speed, the sine via 6, depth x range -> the pitch / volume /
pan modulation by type), the sweep (`sweep_pitch`, its length and counter, auto sweep), portamento (from porta_key
or the last key over porta_time). GBATEK 64, kiwi. Tests: the delay, one LFO period's values at speed 16, depth 0,
each target, a sweep's pitch at half its length, portamento time 0.

**14. Channel update** -- p14_exchannel.c/.h, tests/t14_exchannel.c. `ac_exch_main(s, periodic)`: per active ExChannel
the envelope (12) and LFO / sweep (13) steps, the volume (velocity, envelope, user decay -> dB -> 6's calc_volume),
the pitch (key - base key in 1/64 semitones, user pitch, sweep, LFO -> calc_timer from the wave's timer), the pan
(init pan, user pan, LFO, pan range), the length countdown and the end callback (the track's note finished), the
result into the channel's registers with sync flags for 11. Tests: a channel's register values for key = base key,
an octave up, velocity 127/64, pan -64/0/63, a length running out.

**15. Commands** -- p15_commands.c/.h, tests/t15_commands.c. `ac_cmd_proc(s)`: each queued list (`s->drv.queue`, in
order) and each command of it (ids 0-33 of ac_types.h): START/STOP/PREPARE/START_PREPARED/PAUSE/SKIP_SEQ (to 9),
PLAYER_PARAM, TRACK_PARAM (raw offsets), MUTE_TRACK, ALLOCATABLE_CHANNEL, PLAYER_LOCAL_VAR / GLOBAL_VAR (the shared
work's variables, 16), START_TIMER / STOP_TIMER (channels, captures and alarms by mask: the locked wave-out channels
start here), SETUP_CHANNEL_PCM / PSG / NOISE (the argument packing of ac_types.h), SETUP_CAPTURE, SETUP_ALARM,
CHANNEL_TIMER / VOLUME / PAN, SURROUND_DECAY, MASTER_VOLUME, MASTER_PAN, OUTPUT_SELECTOR, LOCK / UNLOCK_CHANNEL,
STOP_UNLOCKED_CHANNEL, SHARED_WORK, INVALIDATE_SEQ / BANK / WAVE (stop what reads the range), READ_DRIVER_INFO (the
DS-layout work, the channels' SOUNDxCNT, the work's address, the locked mask into memory); then finishCommandTag++.
Alarms: SETUP_ALARM + START_TIMER record their firing times in `s->drv.alarm_log` (no ARM9 to message). Tests: each
command's effect on the state or the registers, the wave-out sequence of pm_voice.c (lock, setup PCM, timer, volume,
pan, start timer, stop timer).

**16. Shared work and variables** -- p16_shared.c/.h, tests/t16_shared.c. `ac_shared_update(s)` (UpdateSharedWork:
player status bits, channel status bits (the busy bits), capture status, each player's tick counter, at the shared
work's DS address), `ac_var_get / ac_var_set(s, p, n)` (local variables per player, global ones: they live in the
shared work, as on the DS; before SHARED_WORK a fallback store). Tests: the layout (offsets of ac_types.h), the status
bits after a start and an end, variables read back by a sequence's 0xA1/0xB0 ops (with 7).

**17. The driver loop and integration** -- p17_driver.c/.h, tests/t17_driver.c (and ac_state.c, tools/acore_render.c
from now on). `ac_driver_wake` (the order above), `ac_driver_init` (SND_Init's state: channels off, master volume
0x7F, the output selector 0), the periodic alarm's phase (`tick0`: from the oracle's trace of the stream), the
register trace, and the whole-core score: every corpus clip through acore and the oracle, register traces and PCM,
triaged to the pieces. Tests: the wake order (a command and a tick in one wake), tick0, the end of a stream, an
end-to-end note.

**18. The port (later, after the corpus scores)** -- port/src/acore_glue.c and a libntr patch: acore behind libntr's
PXI sound path instead of sim7's driver, the capture's host-to-DS translation as the live path, SDL output from
acore's PCM (resampled to the device rate), the shared work and driver info copied back to the game's (PC-layout)
structs.
BUILT, default OFF: sim_config.ini `Sound=acore|libntr` (BWPORT_SOUND overrides; default libntr).
libntr-acore.patch: a SIM_SoundBackend the program registers (start at SIM_Audio_Init, mix in the audio callback; the PXI
thread skips sim7's sound commands while it is on). port/src/acapture.c's hook is the translation, now with a live sink
(acglue_mem / acglue_cmds / acglue_wake); port/src/acore_glue.c (game types: the shared work copied back after each
callback chunk, READ_DRIVER_INFO translated into the host SNDDriverInfo when its list has finished, tags counted as the
driver's) and port/bridge/acore_bridge.c (acore types: one AcoreState, acore_run per callback chunk at 32728.5 Hz,
cubic resampling to the device rate, a mutex). Commands take effect at the next callback chunk. BWPORT_AUDIO_WAV=FILE
writes acore's samples at the DS rate (compare tool format), BWPORT_ACORE_REGS=FILE the register trace.

## The capture (BWPORT_ACAPTURE)

BUILT. `BWPORT_ACAPTURE=FILE.acs[,FRAMES|,FROM-TO]` (frames of OS_GetVBlankCount; default all; N = 0-N) in the port (port/src/acapture.c with
libntr-acapture.patch: a hook in sim7's PXI sound path before the commands run) writes the command stream as the ARM7
receives it: each list as a CMDS chunk at its frame's time, each wakeup word as WAKE, and before a command that points
at memory a MEM chunk with what it points at, translated to the DS's layout at the DS address of the plan above:

- the port's sound heap (NNS_SndHeap's block) is mapped linearly at 0x02100000 (DS address = 0x02100000 + host offset;
  the heap is twice the DS's size on x86_64, 0x12B5A0 bytes, so it ends below 0x0222C000);
- START_SEQ / PREPARE_SEQ: the sequence file holding the base pointer (an SSEQ / SSAR found by its header) as it is in
  memory; the bank in the DS layout (libntrsystem's PC copy moves the file's bytes from 0x38 on up by 32 bytes for its
  64-bit links: the capture puts them back and writes the links as the DS addresses of the wave archives); each linked
  wave archive (topLink cleared; waveOffset entries that are absolute pointers translated);
- SETUP_CHANNEL_PCM: the samples (PNT + LEN words) from the wave-out buffer, placed at a DS address of their own
  (0x02230000-0x0237CFFF, a ring);
- SHARED_WORK / READ_DRIVER_INFO: their buffers' DS addresses (0x0237F000 / 0x0237D000), the shared work's initial
  bytes as a MEM;
- PLAYER_PARAM / TRACK_PARAM offsets need no translation: SNDPlayer's and SNDTrack's fields up to the first pointer
  have the same offsets on the PC and the DS (checked in acapture.c with static asserts).

Frames before FROM keep only the global state commands (SHARED_WORK, MASTER_VOLUME / PAN, OUTPUT_SELECTOR) at time 0, so
a clip starts quiet: FROM wants to be before the music it should hold begins. The stream's time is (frame - FROM) * 560190 and END is (TO - FROM) * 560190. The
capture closes itself at the first sound message after TO (and at exit) and prints one `acapture:` line to stderr.
What the capture does not have: a periodic wakeup. The PC's SND library sends the wakeup word only for commands flushed
IMMEDIATE (SND_Init's, a few starts), so a capture has a handful of WAKE chunks and the commands of a frame arrive in
CMDS lists without one: the driver's periodic alarm (tick0 + k * 174592) runs them. The game sends READ_DRIVER_INFO
every frame (NNS_SndUpdateDriverInfo). Wave-out (the cries: LOCK_CHANNEL, SETUP_CHANNEL_PCM, START_TIMER, UNLOCK_CHANNEL
in one list; a cry's samples are 8-bit, loop flag 2) has its samples in a MEM at the ring. Command ids in a stream are
the SDK's (SND_COMMAND_START_SEQ = 0 ... READ_DRIVER_INFO = 33); a player's sound effects come as PREPARE_SEQ +
ALLOCATABLE_CHANNEL + START_PREPARED_SEQ with base = the SSAR.
A file is written again only when its bytes change (a hash per DS range; the banks and their wave archives are
re-checked at every START_SEQ / PREPARE_SEQ). The corpus: <ref>/acorpus/*.acs
(game-derived, never in git) with an index.txt (clip, frames, what is heard, features).

## The reference: <ref>/aoracle

melonDS (<ref>/melonDS, the commit <ref>/oracle uses, unmodified, GPL: never copy it into the repo) with the
ROM (<repo>/build/pokeblack_us.nds), direct boot, FreeBIOS. The ARM9 runs the game's boot until NNS_SndInit
(0x020726D0, from config/b_us/arm9/symbols.txt), where the oracle has written `b .` once the decompressed static
module holds NNS_SndInit's first word; then the ARM9's IME is cleared: it never sends a sound command. The ARM7 has
booted the SDK's driver and waits. From then on the oracle plays the ARM9: each MEM chunk into main memory, each CMDS
as linked SNDCommands in the command area and its address into the FIFO (ARM9 IPCFIFOSEND), each WAKE a 0 word, the
ARM7's FIFO words drained, all at the stream's times (a scheduler event of its own). It records every sound register
write the ARM7 makes (an NDS subclass's ARM7IOWrite8/16/32) with its time, and the SPU's output once per 1024 cycles
(the SPU's last output sample, 10-bit degrade on), so the WAV has one sample per mixer step, no resampling. mode=hw
streams skip the ROM: both CPUs halted, the REGS chunks written as ARM7 I/O writes at their times. README there.

## Compare tool: tools/acore_compare.py

`compare REF.wav OUT.wav [--lag MS] [--from S --to S]`: per channel (L, R): sample-exact match %, max error, RMS
error relative to the reference's RMS, a spectral score (the magnitude spectra of 2048-sample windows, the
correlation averaged over windows), and the best lag within +-MS (default 10 ms) with the scores at that lag. `regs
A.txt B.txt`: two register traces aligned by order: equal writes, the first difference with its time, counts per
register. `corpus`: every .acs of a directory through both renderers, a table. Pure Python (no numpy in WSL).

## Readiness for Sound=acore as the default

Corpus (offline, acore_render vs the oracle, same .acs): battle_cry 87.5 -> 95.4 % exact (spectral 0.885 -> 0.994), intro
64.6 %, se_menu 100 %, title 78 % (spectral 0.999); mean 82.6 -> 84.5 %. Two fixes: a PSG / noise note had timer base 0 (every
such note ran at TMR 0xFFF0; the base is 8006 = 0x1F46, the oracle's 0xF4F7 for key 78), and a track that ended stayed in
its player's slot, so when the pool reused the track the OLD player ticked the new sequence with its own bank (the second
move effect of the battle clip never played).

Live (Sound=acore, BWPORT_AUDIO_WAV + BWPORT_ACAPTURE in one run, the capture rendered by the oracle):

| clip | live vs oracle | offline core vs oracle | verdict |
|---|---|---|---|
| title from boot (frames 0-1000, 56 fps) | 36 % exact, spectral 0.77 (lag 198 ms best) | 81.5 %, spectral 0.999 | core right; the live clock is not: NOT READY |
| route1 save, wild battle (lockstep inputs, 0-2600) | not aligned (core 3-6 s behind the stream) | 70 %, spectral 0.995 | same |
| battle cry (frame 2293) | LOCK 2293, UNLOCK 2346 (53 frames = 0.88 s for a 0.87 s cry): busy for the cry's length | cry window spectral 0.997 | cry works |

The live core runs on the audio callback's clock, the stream (and the oracle) on game frames; at 35-56 fps the live
register times are 5-9 M cycles late and drifting (title: 372 equal writes, then different notes). So (a) is not
measurable until the sound clock follows game frames (lc62, acore_bridge.c); the core itself is within 0.99 spectral on
every clip. Rerun then: tools below. Not ready to be the default before that.

Method: `BWPORT_SOUND=acore BWPORT_AUDIO_WAV=live.wav BWPORT_ACORE_REGS=live.regs.txt BWPORT_ACAPTURE=live.acs,0-1000
python3 tools/port_run.py -t 30 --keys 'start@f600 start@f900 start@f1300 a@f1700'`; `aoracle live.acs o.wav`;
`acore_compare.py compare o.wav live.wav --lag 300`. A lockstep run (BWPORT_LSTEP) now closes the capture and the WAV
(ab_wav_close, acapture_close) before its _exit.

Left: intro (free-channel order: when every channel is a released note, the oracle takes ch8 before ch9 where acore's
amplitude order picks ch9, 5 dB-tenths apart; velocity weight < 0.5 fits this case but breaks the title), title pan/volume at
write 2257, battle_cry at write 540.

### Readiness table

What changed: (1) the live core ran with none of the timing a stream sets (acore_create has `timed` off: writes landed at the
wake's cycle, no WAKE-word lag): `acore_set_live` (ab_init) gives it the driver-mode stream defaults. (2) acapture.c stamps each
captured list (and MEM) at the live core's clock (`acglue_now` = acore's `now`, the end of its last frame step, where the next
alarm takes the list), not at the PXI thread's frame: the oracle replays the live timeline. Result: live acore vs offline
acore_render on the same capture is 100.000 % exact (title, intro, route1 battle; `--fast` and 60 fps), so live vs oracle IS
core vs oracle now; what is left is the core's.

Core fix: a steal (every channel busy) takes the channel with the quietest HARDWARE volume (the last tick's SOUNDxCNT
volume field: mantissa / divider 1, 2, 4, 16), a tie going to the FIRST channel of the order (it was the amplitude sum of
velocity, envelope and decays as the envelope stands now, a tie to the last). Measured by forcing the oracle's choices into the
core (the set of channels the oracle started in the next wake, 57 steals of title / intro / battle_cry): old sum 54 right,
hardware volume 55, the sum with other weights <= 54. Corpus (acorp.sh) mean exact 84.5 -> 88.6 %: title 78 -> 81,
intro 65 -> 78 (registers now identical, all 20422 writes), battle_cry 95.4, se_menu 100.

Live vs oracle, acore (fresh capture of each clip, WAV time 0 = capture time 0; spectral over the whole clip):

| clip | live vs oracle exact | spectral | registers | verdict |
|---|---|---|---|---|
| title from boot (0-1000 frames) | 75 % (before the fixes 38 %, 70 % with the stamps only) | 0.999 | first difference at write ~2100 (a note taken by the wrong channel) | sounds the same |
| intro / NEW GAME (0-3400 frames, title + intro + talk) | 50 % (73 % before the steal change: the longer capture diverges at write 718 on a note start the oracle takes one wake later, then everything shifts) | 0.99 | diverges at 718 | musically right, notes shift |
| route1 save, wild battle with the cry (battle_long, 0-5400) | 39 % | 0.986 | diverges at 2094 (title music at boot, the same channel choice) | right instruments, notes shift |
| menu / message-box effects (corpus se_menu) | 100 % | 1.000 | identical | identical |
| battle cry (corpus battle_cry) | 95.6 % | 0.994 | diverges at 540 | cries work |

The default libntr backend vs the same oracle: title spectral 0.83, route1 battle 0.97 (exact 36 % / 21 %): acore is closer to
the SDK driver than libntr's sim7 on every clip. Verdict: (a) the live output follows the oracle as closely as the core does
offline (100 % identical to acore_render); (b) cries work; (c) the maintainer listens: C:\stress\shots\ar65_*.wav
(title and the route1 battle, each _acore / _libntr / _oracle, same start). Not made the default here: the intro and
battle are 40-50 % sample-exact (spectral 0.99), the differences are notes shifted by a wake or a channel choice.

Left (next agent): (1) the first difference of the live intro/route1 captures (write 718 / 2094): a note starts in a wake
the oracle starts it one later, or on another free channel (when a channel is INACTIVE the order picks the first one; the
oracle may hold a channel busy that acore freed, e.g. a one-shot whose hardware ended: p14's `ended` check against the
oracle's stop time). (2) the title steal where the oracle takes a channel with a larger hardware volume (W=429845504 of the
corpus title: chose ch2, lin 5.5, over ch7 2.25 and ch0 2.88): probably a channel the oracle has already freed. (3) battle_cry
write 540. Tools: C:\stress\ar65\ (live.sh, livesave.sh, cmp.sh, force.sh: AC_FORCE=oracle regs + AC_DBGALLOC debug
patch of p11, not committed; ana.py / grid.py fit allocation keys).

## Open items (groundwork)

Built (a53a): GBATEK's sound chapters split (<ref>/gbatek/56-72), <ref>/sound/ (nds-sdat.html, vgmtrans),
this design, port/acore's skeleton (the .acs loader, the register file, the event loop with the periodic wakes and
the 1024-cycle samples, WAV I/O, the test framework with stream recording and act_emit, acore_render; 17 piece stubs
and 79 TODO tests from the specs above; 5 skeleton tests pass).

Not built, in this order (1 is BUILT: port/src/acapture.c, libntr-acapture.patch, corpus in
<ref>/acorpus/ with index.txt):
1. The capture: port/src/acapture.c + libntr-acapture.patch (a hook pointer in sim7's pxi.c PXI_FIFO_TAG_SOUND case,
   called with each list before it runs and for each 0 wakeup word; frame time from OS_GetVBlankCount), the
   translation of the section "The capture" (the bank's PC layout: header 0x18 bytes, 64 bytes of host links, then
   the file's bytes from 0x38 on at +0x58; NNS_SndArcGetFileAddress over the archive's file ids finds a file's start
   and size), then clips in <ref>/acorpus/ (title, a battle with a cry, a sound effect).
2. The oracle <ref>/aoracle/ (outside the repo, GPL): melonDS's libcore.a from <ref>/oracle/build.sh, an NDS
   subclass overriding ARM7IOWrite8/16/32 (the register trace), the ROM direct-booted; the ARM9 stopped at
   NNS_SndInit (0x020726D0: the ARM9 static module is compressed, so write `b .` only once that address holds the
   decompressed first word, checked from a periodic scheduler event of 64 cycles; RegisterEventFuncs/ScheduleEvent
   are public, use an event id a DS never schedules, e.g. a DSi one); then the ARM9's IME = 0 and the stream
   injected through ARM9IOWrite32(0x04000188) with ARM9IORead32(0x04100000) draining the ARM7's words; the samples
   read once per 1024 cycles (SPU's OutputLastSamples after each Mix, Degrade10Bit on: melonDS's ReadOutput is
   blip-resampled, not raw). The ARM7 driver queues a FIFO word >= 0x02000000 as a list and wakes on a 0 word.
   mode=hw: no ROM, the REGS as ARM7IOWrite at their times. Validate on drv_note / hw_pcm16 streams.
   BUILT (o54a); mode=driver VALIDATED (o55a: drv_note TMR FE00/FE1D/FF00 for keys 60/61/72, drv_cry, the four
   corpus clips, ~1.5 s each; see <ref>/aoracle/README STATUS).
3. BUILT (t54a): tools/acore_compare.py (compare / regs / corpus; scores: sample-exact %, max error, RMS relative to the
   reference, spectral and envelope 0..1, best lag +-10 ms; tests tools/acore_compare_tests.py, corpus mode checked with
   mock binaries). Corpus mode runs `BIN F.acs OUT.wav --regs R.txt` for both renderers; a clip's table row has the
   exact % at lag 0, the tolerant scores at the best lag and the register verdict; features come from the corpus index.txt.
4. BUILT (t54a): port/acore/tools/mkacs.py (hw_pcm16/pcm8/adpcm/psg/noise/mix, drv_note, drv_cry, drv_psg, drv_noise; a
   library: Acs, build_sseq/sbnk/swar), acs_dump.py (--cmds --mem --regs --json), tests port/acore/tools/acs_tools_tests.py.
   drv_psg / drv_noise use the public NitroSDK's SND_SetupChannelPsg/Noise packing (arg1 = shift<<8|volume, arg2 =
   timer<<8|pan, arg3 = duty): unchecked against the oracle (piece 15's open question).
Open questions for the pieces: the size of the DS's OSAlarm inside SNDAlarm (AC_Alarm assumes 0x40 bytes; check
READ_DRIVER_INFO's buffer against the oracle), the PSG / NOISE / CAPTURE command argument packing (piece 15 reads
lib/nitro/src/snd/snd_interface.c's PushCommand calls: the ARM9 side's protocol, which is the game's own code), and
whether FreeBIOS's GetPitchTable / GetVolumeTable match the real BIOS (the oracle uses FreeBIOS).

## Sound quality: DS (exact) / Enhanced (2026-11-01, libntr-ios-soundq.patch)

The maintainer thought the sound "low quality or distorted" next to YouTube recordings (the official soundtrack, or emulators that
smooth). Faults looked for first: clipping, the resampling to the device, underruns, volume truncation. Found:
- Clipping: no. Town BGM at 1x peaks at 19.8k of 32767 (rms 2.7k), 0 samples at full scale in 18 s; only the title/boot part
  (first 19 s of the capture) touches the DAC rail, 12 samples of 620k, the game's own mix (a real DS clips the same).
  The buses add with a clamp (RingOutM), the music bus at speed > 1 included (FF numbers below).
- The device resampling WAS a fault: libntr's ring player used a 4-point cubic from 32.7 kHz to 44.1 kHz. As a resampler it is
  poor in the top octave: a 10 kHz tone came out with -17 dB (everything that is not the tone, relative to the tone), a 14 kHz
  tone -6 dB, a 15.5 kHz tone -2 dB (images and distortion). Replaced by a 64-tap Kaiser sinc (beta 8, cutoff 15.2 kHz, 512
  phases, linear between rows): -83 dB at 1-14 kHz, -69 dB at 15.5 kHz. Cost 3.3 ms per second of output (0.33 % of a core)
  against 0.8 for the cubic. In both quality modes. The rate followed the ring's raw fill (+-5 %): that was a fault of its own, see "The ring's rate, cuts and fades".
- SDL and CoreAudio: the app asks for 44100 Hz, 2 ch, s16, 230 frames (5.2 ms) and takes what it gets (NULL obtained spec), so SDL
  converts to the device's rate (48 kHz on iPhones, usually) with its own resampler (not read; assume decent). Two conversions
  in a row. The iOS session is SDL's default: Playback,
  MixWithOthers (plays with the silent switch on). Underruns: 0 on the Mac in a quiet run (CoreAudio, ring fill 1060-1320 of a 1100
  target); the many seen under load 100 are the machine's. The 5 ms callback has the whole ring (820 frames) behind it; not
  measurable without the phone.
- Volume: the master volume is integer-scaled (v * vol / 127, truncation toward zero) only below 127; the maintainer's 127 skips it. The
  new resampler scales in float and rounds.
- What is left is how the DS sounds: samples held until the next (images of every sample: the "crunch"), 8-bit PCM and ADPCM
  sources, a 10-bit output (step 64 of 65536 = -60 dBFS), no interpolation.

The setting `SoundQuality=enhanced|ds` (sim_config.ini [General], BWPORT_SOUNDQUALITY, default enhanced; desktop Config > Audio
"Sound quality"; iOS sheet Sound > Sound quality; SIM_UISettingGet/Set "soundquality") reaches acore as `acore_set_quality(s, 1|0)`
(SIM_SoundBackend.setQuality, called by SIM_Audio_FrameTick with the audio locked when it changes; acore_glue.c ->
ab_set_quality). Game logic is identical; only the sample values change:
- DS (exact), the default of acore_create and of every test: as before, bit for bit (the 206 older tests are unchanged).
- Enhanced (`s->hq`): (1) p01: a PCM8/PCM16/ADPCM channel's output is the sinc-interpolated curve through its last 16 samples
  (16 taps, Kaiser beta 8, cutoff 0.92 of the channel's Nyquist, 128 phases, linear between rows) at the timer's position in the
  period, eight periods late (a millisecond at 8 kHz; the sequencer's timing is untouched). PSG and noise stay as they are (they
  are square and noise by design). (2) p04: the mix keeps the DAC's six truncated bits (`(src * master >> 15) + (bias << 6)`, clipped to
  0..0xFFFF, minus 0x8000) instead of `v << 6`. The capture units see the same mix. The music bus uses the same two functions.
  t19_quality.c: DS holds samples, Enhanced's ramp is straight, the mix keeps low bits, and a 4 kHz tone in an 11 kHz sample:
  everything but the tone is -2.4 dB (DS) vs -79.3 dB (Enhanced); the old cubic would have been -12.

Measured (tools/wav_stats.py, a numpy venv: `python3 -m venv ~/bw-cache/sndvenv && pip install numpy`; the offline
render is deterministic: `BWPORT_ACAPTURE=town.acs,0-2500` from the town save, then `acore_render town.acs out.wav [--enhanced] --info`):
- Same passage, town BGM 22-40 s of the capture (18 s, 41.8 s stream, offline renders at the DS rate):
  | | DS (exact) | Enhanced |
  | --- | --- | --- |
  | peak | 19840 | 20079 |
  | samples at full scale | 0 | 0 |
  | energy above 10 kHz (of total) | -28.9 dB | -30.0 dB |
  | above 16 kHz | -41.6 dB | -43.1 dB |
  | band power 10-12 / 12-14 / 14-16 kHz (dB) | 35.8 / 34.8 / 32.9 | 35.1 / 34.3 / 29.2 |
  The town BGM is mostly not image-dominated, so a spectrum of the music shows only 1-4 dB less above 10 kHz: what the ear gets is
  mostly in the notes whose samples play far below the mixer rate (low-rate instruments, cries), where the held-sample images fall
  in 4-16 kHz (the 4 kHz tone test above: -2.4 dB vs -79 dB). The DS's own noise floor (the DAC's 6 bits dropped) is 25 dB of power
  across the whole band, 16 dB per 2 kHz band: under the 35 dB the music itself has there.
- Device-rate output (BWPORT_AUDIO_OUT, 44.1 kHz, dummy driver, passages not aligned): above 16 kHz -40.7 dB (cubic, DS) vs -57.7 dB
  (sinc, Enhanced).
- Resampler alone (a 32.7 kHz tone ring to 44.1 kHz, everything but the tone relative to it): cubic -79 / -38 / -17 / -6 / -2 dB at
  1 / 5 / 10 / 14 / 15.5 kHz; sinc -83 / -83 / -83 / -83 / -69 dB.
- Fast forward 4x with Music at normal speed on (the music bus split; Enhanced): peak 14.8k, 0 samples at the rail in 9 s of
  music after the switch; the machine (load 100) could not hold 4x, so the 95 underruns there are the load's.
- CPU, acore alone per second of audio (offline render of the 41.8 s town stream, M2, machine at load 100 so noisy; three runs):
  DS 11.6-12.9 ms, Enhanced 16.8-20.8 ms (+5-8 ms, the interpolation of ~8 active channels); the resampler in the audio callback
  3.3 ms per second of output (cubic: 0.8). Enhanced in all: about 2 % of one M2 core. DS mode's samples are byte-identical to before
  (a 41.8 s render compared with `cmp` against the pre-change acore).
- Listen-free checks to rerun: `build/acore/acore_tests p19` (the tone test prints its two numbers), tools/wav_stats.py on two renders.

## The ring's rate, cuts and fades (2026-11-01, libntr-ios-sound.patch)

A read-only review (12 findings) blamed the maintainer's "low quality or distorted" on the ring player, not the cores. Proved and
fixed:
- **Pitch wobble (the main one).** The callback set the playback rate from the ring's raw fill (`(fill - 1100) / 1100 * 6 %`,
  clamped +-5 %). The fill saw-tooths by a whole game frame (547 DS frames pushed at once, ~171 taken per 5 ms callback), so
  every note was frequency-modulated at the frame rhythm: +-25 cents. Now `RateUpdate` (once per frame tick, audio locked)
  low-passes the fill sampled just after the push (two poles at 0.2 s: the 60 Hz saw-tooth is ~70 dB down) and runs a PI
  controller on it: the proportional part at most +-0.25 % (4 cents); the integral part (Ki 0.003/s, so at most 0.3 % a second,
  plus 0.1 % per dry ring while the game ran) follows a producer that really runs slow or fast, up to +-3 %, and restarts at 0 on
  every speed / fast-forward change. Nothing in it moves faster than a slow glide.
- **85 cents sharp at Max, then a 2-3 s sag.** With the ring full the old clamp played everything (the music bus too) 5 % fast.
  Now a fill at SND_CAP (3100) skips incoming frames until it is back at SND_RESUME (1100); returning from Max (or a lasting
  fill 600 over the target) cuts to the target at once instead of draining by pitch.
- **Max chopped into 5 ms slices (a buzz).** Single frames were dropped whenever the ring was full: pieces of ~171 frames with hard
  joins. Now the pieces are ~2000 frames (60 ms) and the first 128 frames after a skip are cross-faded (raised cosine) into the
  last 128 in the ring; `s_ringDry` keeps each frame without the music bus's share, so the fade swaps only the sound effects and
  the music (one bus frame per appended frame) stays continuous.
- **Underrun clicks.** A dry ring cut to 0 mid-buffer and the restart replayed the interpolator's 64 old samples. Now the output
  fades from its last value to 0 in 64 samples, the interpolator restarts from zeros, and the output fades in over 128 samples.
- **Sheet / background pause clicks.** `SIM_Audio_Pause` (sim_main.cpp's four SDL_PauseAudio calls) fades the output out over 128
  samples, waits for it (at most ~40 ms), then pauses; resuming fades in.
- **Two resamplers.** The device was opened at 44.1 kHz with a NULL obtained spec; this Mac's output is 48 kHz, so SDL resampled
  our sinc's output again. Now (frame clock) the default device's rate is asked for (`SDL_GetDefaultAudioInfo`) and the obtained
  rate taken (`SDL_OpenAudio` with an obtained spec; a changed format or channel count reopens as before). Log: `audio: device
  48000 Hz, 250 frames a callback`; the iOS Simulator reports 48000 too (not yet seen on the phone). Simulator check of the fades
  (tone, `BWPORT_SETTINGS_TEST` opening and closing the sheet, a loaded Mac so 209 dropouts in 25 s): the biggest sample step in
  the whole output is 1934 (a 1 kHz sine at 48 kHz steps 1047 by itself; a cut is 8000).

Measuring switches (kept): `BWPORT_AUDIO_TONE=HZ` replaces every frame the ring appends with a sine that stays continuous in
the appended frames (as the music bus is), so `BWPORT_AUDIO_OUT` shows what the ring and its rate do to a steady pitch;
`BWPORT_AUDIO_TONEPUSH=HZ` puts the sine in the game's time, before the stretch and the cuts (joins show up as steps);
`BWPORT_AUDIO_MUTE=1` plays silence on a real device (the OUT file still gets the output); `BWPORT_AUDIO_SLOW=PCT` paces the
frame tick PCT % slower (a device that cannot keep up); BWPORT_AUDIOLOG adds skips, the rate, its integral and the longest
time a frame tick held the audio lock, plus an `audio: mode speed S unpaced U at out frame N` line per change.

Pitch, live path (port/acore/tools/ring_pitch.py; `--save town`, cfgscript `f1300` 1x, `p900` 4x, `p1800` 1x, `p600` Max, `p1200` off; tone 1000 Hz through the
real ring, CoreAudio at its own rate, muted; instantaneous frequency from the analytic signal, 2 ms averaged, 80 ms around each
dropout excluded; fast = the part above ~10 Hz, slow = the 100 ms average). **All at load averages 80-600** (five agents building),
so the game itself missed its pace (underruns, the 4x segment short of 4x); the fast column is what matters:

| segment | before: fast sd / p1-p99 | after: fast sd / p1-p99 | before: median, slow p1-p99 | after: median, slow p1-p99 |
| --- | --- | --- | --- | --- |
| 1x | 16.5 / 66 cents | 0.02 / 0.10 | +1.6, 67 | +0.1, 1.6 |
| FF 4x | 16.3 / 76 (155 dropouts) | 0.04 / 0.26 (2) | -4.1, 46 | -16.5, 17.6 |
| 1x after 4x | 16.5 / 66 | 0.02 / 0.10 | +1.0, 37 | +1.9, 8.2 |
| FF Max | 0.01 / 0.05 | 0.01 / 0.04 | **+84.5** | +0.2 |
| 1x after Max | 16.0 / 66 | 0.02 / 0.10 | +3.2, 90 | +0.6, 10.5 |

The 4x and after-4x medians of the "after" run are the integral following a game that could not hold 4x at that load (it now
restarts at each speed change: the row above is that run, -8.2 cents and 1.1 cents p1-p99 before the restart was added). Simulated slow device (`BWPORT_AUDIO_SLOW=1` and `3`, at
load ~600): the fast part stays at 0.04-0.3 cents sd; the integral went to its 3 % limit because the loaded game ran slower still
(-45 to -57 cents), with dropouts beyond that: at that load the number measures the machine, not the controller. Under 3 % short
the music plays flat by the shortfall rather than with gaps; past it, gaps with fades.

Joins and dropouts (`BWPORT_AUDIO_TONEPUSH=1000`, `BWPORT_ACORE_SOLO=rest`, sample steps above 1600 a second; a 1 kHz sine at
48 kHz steps at most 1047): Max 235.6/s before (hard joins, biggest step 12095), 0.0/s after (biggest 1026). At the dropouts of
the loaded 1x segments the biggest step was 8000 (full scale to 0) before, 2016 after (the fade).

Not reproduced / not changed: a full command queue (#6: 0 "command queue stayed full" lines in every run; the town capture has
at most 4 lists in a frame against 64; the "leaked info slot" is not one: a dropped list leaves the tag at lists + 1, which the
next list completes); the audio lock (#10: a frame tick holds it 0.3-1.4 ms, 3.9 at most at load 600, against ~30 ms of
CoreAudio buffers); music skipping the capture units while split (#12: the game never sets up capture; no capture command in
the captures). The live hook's re-hash per sequence start (#7) is 200-250 KB per START_SEQ in the town; the hash now takes 8
bytes a step (246 KB: 4.45 -> 0.82 ms at load ~300, byte-wise FNV was ~4 cycles a byte).

## Game speed and sound

At GameSpeed S > 1 the sound side makes S times as many samples a second as the device plays and libntr-sp76's WSOLA stretches
them back. Before the bus (main 3996a63c6, Music at normal speed on) the music's tempo was divided by S so that after the stretch
it came out at its normal tempo: the music itself went through the stretch, which smears every attack and note decay (the maintainer's
"music is a bit distorted when game speed is increased"). With Music at normal speed on and Sound=acore, libntr-mu83.patch keeps
the music out of that stretch:
- acore (ac_shared.h `AC_Bus`, ac_state.c, p17's `ac_music_wake`): the sequences started on the music players (libntr's
  `SIM_Snd_SeqPlayerAssign` + `MusicPlayer`, told per start through `SIM_Audio_SlotMusic` -> `ab_slot_music`) are tagged
  (`music_players`; their channels `music_ch`, set in `ac_exch_alloc`, cleared by `ac_exch_lock` and the direct channel setups).
  `acore_bus_split(1)` gives them a clock of their own: `acore_bus_run(frames)` makes music samples with a periodic wake that
  ticks only the music players' sequencer, envelopes, LFOs and sweeps (`ac_dom_pl` / `ac_dom_ch` filter `ac_seq_main`,
  `ac_exch_main`, `ac_exch_sync`), mixing only their channels; `acore_run` (the frame clock) then makes everything else (commands,
  SEs, cries, the shared work). The join (`acore_bus_split(0)`) keeps the music wake's phase (before, the first tick
  after it jumped 17-55 samples).
- libntr (sim_audio.cpp): the music frames are asked for by the output. Every output frame the ring takes gets the next music
  frame added after the WSOLA (`RingOutM`, a FIFO topped up by `MusNeed` before each block: exactly as many as the block plays), never
  stretched; a frame the ring drops (Space / Tab: the game runs faster than the device plays) takes no music frame, so the music
  stays in real time. The bus is split at S > 1 and also while the game runs unpaced at Speed 1 (`SIM_FastForwarding`: Space held,
  Frame rate Unlimited; frames then go straight from `RingPush` to `RingOutM`). S changes and the split toggling go through
  StDisengage/StEngage, the tail played with the music added. Backends without a bus (Sound=libntr) keep sp77's tempo division.
- Switches for measuring: BWPORT_MUSICBUS=off (no bus: main's tempo division, music still tagged), BWPORT_ACORE_SOLO=music|rest
  (mutes the other channels' share of the mix; they still run), BWPORT_AUDIO_WAV (the output after the stretch, the frames the
  ring takes, 32728 Hz), BWPORT_SNDLOG=1 (each sequence start, with its slot, NitroSystem player and the WAV frame it started at:
  the marker that aligns two runs), BWPORT_SPEEDSCRIPT='TICK:SPEED,...' (live speed changes), BWPORT_AUDIOLOG=1 (the ring's fill
  and the underruns once a second).

Measured (Linux, SDL dummy audio, 60 fps cap, 4 runs at once; every file is the music alone, SOLO=music, cut at the
sequence's start and aligned on the music onset; 1x is deterministic: two 1x runs are identical). The 1x WAV of route1 (40 s) and
the town (60 s, a walk with texts) is byte-identical to main's over the common length. Each speed against the 1x music
(first 20 s of the tune), `before` = BWPORT_MUSICBUS=off (main's behaviour), `after` = the bus. lsd = median log-spectral
distance (dB, 0 = the same spectrum), env = correlation of the 10 ms RMS envelopes, corr_w = median per-second waveform
correlation after the best +-30 ms lag, tempo = onset-envelope tempo ratio (1.000 = same), decay = time from an envelope peak to
-10 dB over the 1x's:

| tune | S | lsd before / after | env before / after | corr_w before / after | tempo before / after | decay before / after |
|---|---|---|---|---|---|---|
| route1 | 2 | 9.3 / 3.1 | 0.87 / 0.99 | 0.28 / 0.98 | 1.000 / 1.000 | 1.50 / 1.25 |
| route1 | 3 | 12.5 / 3.1 | 0.53 / 0.98 | 0.19 / 0.98 | 0.972 / 1.000 | 1.03 / 1.25 |
| route1 | 4 | 11.2 / 2.7 | 0.54 / 0.98 | 0.18 / 0.99 | 1.000 / 1.000 | 0.63 / 1.25 |
| town | 2 | 9.3 / 3.6 | 0.54 / 0.99 | 0.25 / 1.00 | 0.988 / 1.000 | 1.00 / 1.00 |
| town | 3 | 10.3 / 3.6 | 0.47 / 0.98 | 0.19 / 1.00 | 0.966 / 1.000 | 0.98 / 1.00 |
| town | 4 | 10.5 / 3.3 | 0.48 / 0.98 | 0.22 / 1.00 | 0.966 / 1.000 | 0.78 / 1.00 |
| wild battle | 2 | 8.6 / 3.2 | 0.44 / 0.99 | 0.20 / 0.99 | 1.000 / 1.000 | 1.00 / 1.00 |
| wild battle | 3 | 10.7 / 3.2 | 0.35 / 0.99 | 0.13 / 1.00 | 0.994 / 1.000 | 1.00 / 1.00 |
| wild battle | 4 | 11.0 / 3.1 | 0.34 / 0.99 | 0.12 / 0.99 | 0.990 / 1.000 | 1.00 / 1.00 |

(Waveform correlation of the whole 20 s, after: 0.97-0.99 at every speed; before: 0.0-0.1. The decay column is 10 ms-quantized: 1.25
is one step; the 0.63-0.78 of the before is the stretch cutting the notes short.) What stays of the 1x difference after the bus is
the music's own timing at sample level: the bus lets its register writes land at the wake (the main clock models the wake's write
latency, a few hundred cycles), so a note's start is up to a few samples apart and the DS's nearest-sample PCM playback picks
other source samples: -20 dB of residual, constant lag, inaudible. The title music is the only NitroSystem player 6 sequence
the saves reach (no real fanfare could be started from a save cheaply); 1 s of it: lsd 11.3 before / 0.0 after at 2x, 2.3 after at 3x (the 4x run is cut by the game's own next command).
- Underruns at 4x (mu83: 15 in 40 s): main has the same (15 in 60 s alone, 17-21 with other runs), the bus did not add them. They
  come in a burst while the game boots and loads (the game cannot keep 4 x 60 fps through a load: a tick longer than the ring's
  ~25 ms of audio); after that 0-3 more in 5 minutes (2x: 3 in 300 s; 4x: 17 in 300 s, 15 of them in the first 30 s). The ring's
  fill stays 260-900 frames for 5 minutes at 2x and 4x (no growth: the music frames are made as the output plays them, so music and
  SEs cannot drift apart). Not fixed: a larger ring target at S > 1 or a music-only fill of a dry ring (the music clock is real
  time, so the callback could keep the music going through a stall). The dummy driver calls back every 5 ms for a 5.2 ms buffer, so
  the ring's rate controller (+-5 %) works harder here than on a real device: the underrun counts need a real device to settle.
- SE and cry durations (SOLO=rest, wild battle fought with A presses, same keys at 1x / 2x / 4x): a cry of 1.28 s lasts 0.64 s at 2x
  and 0.33 s at 4x, the menu blips 0.46 -> 0.24 / 0.12; events paired by start time (t, t/S): length ratio median 1.96 at 2x and
  3.65 at 4x over 13 events (5 ms resolution; the shortest of them merge with their neighbours).
- Live speed changes (route1 music alone against the 1x music; BWPORT_SPEEDSCRIPT): 1 -> 4 -> 1 (ticks 2400, 4800), 1 -> 3 -> 2 -> 4
  -> 1, and 4 -> 2: the lag against the 1x music stays within 4 samples through every split change and every S change (the 4 -> 1
  join was +55 and -17 samples before the phase fix, 0 after), the correlation dips to 0.90 in the half second of a change and is
  0.98-1.00 elsewhere; the longest near-silence is the 1x's (2.2 ms); no level jump (the biggest sample step is 7264 against 7136).
  Space (fast forward) held for 600 ticks at 1x and 1800 at 4x: the music alone matches the 1x music throughout (lag within 10
  samples, no gap over 2.2 ms), the rest of the sound is dropped by the ring as before (BWPORT_AUDIO_WAV holds what the ring takes).
- Risks: (1) the game uses no sound capture or reverb (no NNS_SndCapture* / SND_SetupCapture call in src/ or lib/), so the music bus
  skipping `ac_cap_*` loses nothing. (2) A music note stolen by an SE: the steal happens at the main clock's time, which is ahead of the
  music clock by the stretch's latency, so the note loses up to ~30 ms of tail; it needs every channel busy, not seen in the runs.
  (3) Two starts of different bus class on one slot within one driver wake: the flag FIFO (`ac_bus_slot` / `ac_bus_player_started`,
  8 deep, the slot's last flag when empty) keeps the order, tested by p18_slot_flag_fifo; a START command dropped by a full queue would
  shift the FIFO by one for that slot (not seen). (4) Commands (pause, stop, fades) are run by the main clock, so they reach the music
  up to the stretch latency (a few tens of ms) before the music clock's time. (5) Sound=libntr keeps the tempo division.
- Recipe: `bash port/acore/tools/bus/mx.sh NAME SAVE SECS [MOVES]` (output in $OUT, default <scratch>; WT = the worktree) runs one scenario at 1x, 1x again, and 2x 3x 4x with and
  without BWPORT_MUSICBUS=off, music alone (mrun.sh sets the env, the keys, the WAV and the log); `cmpall.sh NAME 20 PLAYER N`
  cuts each file at the N-th start on the NitroSystem player (BWPORT_SNDLOG's `wav` field) and runs `an84.py` (numpy; waveform
  correlation, spectra, envelope, tempo (an77), decay) on each speed against 1x; `ev.py` pairs SE events by start time, `sc.py`
  checks live changes against the 1x music. All of them are in port/acore/tools/bus/ (an77.py is sp77's tempo analysis, walk.py the key-script maker).
