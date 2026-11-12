/* port/acore piece 11: Channel allocation (docs/acore.md). OWNED BY PIECE 11.
 * p11_alloc.c/.h, tests/t11_alloc.c. `ac_exch_alloc(s, chmask, prio, strong, track)`, `ac_exch_start / release /
 * free`, `ac_exch_lock / unlock / stop_unlocked`, the track's channel list, `ac_exch_sync(s)` (UpdateExChannel).
 *
 * What the oracle shows (driver mode, streams of tools/mkacs.py's kind with up to 17 notes; the experiments are
 * summarised in tests/t11_alloc.c):
 *  - the allocation order over the allowed channels is 4 5 6 7 2 0 3 1 8 9 10 11 14 12 15 13;
 *  - the first inactive channel of that order is taken; with none free the victim is the one with the lowest
 *    priority, then the lowest envelope level (ac_env_level, larger = louder), the LATER one of the order on a full
 *    tie; a note whose priority is lower than the victim's gets no channel (equal steals); a channel in its release
 *    has priority 0 (a track priority 100 note in release is stolen before priority 64 ones);
 *  - LOCK_CHANNEL (mask, flags) writes `CNT & ~start` (4 bytes) for every channel of the mask at once, in channel
 *    order, locked or not, running or not; the lock goes into the set flags&1 (set 0 is what READ_DRIVER_INFO
 *    reports, both sets keep the channel from being allocated); UNLOCK clears the mask from the set flags&1, no
 *    write; STOP_UNLOCKED_CHANNEL writes the same stop for the channels of the mask not locked.
 *  - UpdateExChannel, per wake, channels in order 0-15: STOP: `CNT & ~start` (4 bytes). START: that stop, then CNT
 *    (without the start bit), TMR, and for a PCM/ADPCM channel PNT, LEN, SAD; the start bit goes in later, as a byte
 *    write at CNT+3, after the loop over all channels (so two channels started in one wake have their setups, then
 *    their start bytes). Otherwise the TIMER, VOLUME, PAN flags: TMR (2 bytes), CNT (2 bytes: the volume field),
 *    CNT+2 (1 byte: the pan).
 *
 * Conventions this piece takes from the other pieces' fields (AC_ExChannel, ac_types.h):
 *  - `volume` is the SOUNDxCNT low half (volume 0-127 | divider << 8), `pan` 0-127, `timer` the positive count
 *    (the register gets 0x10000 - timer), `data` the sample address (PCM) or the PSG duty, `wave.loopstart` and
 *    `wave.looplen` the PNT and LEN words, `wave.format` the SOUNDxCNT format, `wave.loopflag` loop (else one-shot).
 *  - the sync flags are set through ac_exch_set_sync; `AC_EXF_START` is set by ac_exch_start for piece 14, which
 *    turns it into AC_SYNC_START once the first values are computed.
 *  - `callback` is 1 while the channel belongs to a track (callback_data = the track), 0 otherwise.
 */
#ifndef AC_P11_H
#define AC_P11_H

/* the piece's own state, s->p11 */
typedef struct AC_P11 {
  unsigned lock[2];  /* locked channels by the lock flags' bit 0 */
  unsigned started;  /* channels whose hardware the sync has started (and not stopped since) */
} AC_P11;

/* the sync flags (AC_SYNC_*) of a channel: STOP replaces all others, anything else is added */
struct AcoreState;
void ac_exch_set_sync(struct AcoreState *s, int ch, int sync);
int ac_exch_sync_flags(const struct AcoreState *s, int ch);
/* the track's channels, newest first; -1 at the end */
int ac_track_first_channel(const struct AcoreState *s, int t);
int ac_track_next_channel(const struct AcoreState *s, int ch);
/* the allocation order (16 channels) */
extern const unsigned char ac_exch_order[16];

#endif
