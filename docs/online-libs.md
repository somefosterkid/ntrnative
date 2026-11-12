# Online libraries: what is ROM code for now

lib/dwc (Nintendo Wi-Fi Connection and GameSpy), lib/dwcutil and lib/nitrowifi hold no code derived from ntrtwl's
NitroDWC, NitroWiFi or libvct repositories: those have no licence, so the public build can't carry C written from
them. The files that were (their header said "read, not copied", or tools/port_lib.py set them up from ntrtwl) were
deleted. Their delinks entries stay, without `complete`, so both DS builds link the ROM's own code there, the way
White's unfinished files do (docs/white.md): both ROMs still rebuild byte for byte. The port never compiles these
libraries (online is stubbed in port/src, port/meson.build), so nothing changes for it.

ROM code for now (60 files deleted, 2026-11-07):

- NitroDWC base: account/dwc_account.c, dwc_init.c; base/dwc_common.c, dwc_error.c, dwc_friend_base.c, dwc_login.c,
  dwc_main.c, dwc_match.c, dwc_memfunc.c, dwc_nasfunc.c, dwc_nd.c, dwc_transport.c; bm/dwc_backup.c, dwc_bm_init.c,
  util_wifiidtool.c.
- libvct: vct/fir.c, mixer.c, udp_buffer.c, vad.c (and fir.h).
- NitroWiFi: cps/cps.c (two functions and its packet macros came from ntrtwl).
- GameSpy, the old gs/ tree: available.c, gp/*.c, gt2/gt2Connection.c, gt2Main.c, gt2Message.c, gt2Socket.c,
  gt2Utility.c, gstats/*.c, hashtable.c, natneg/natneg.c, qr2/qr2.c, serverbrowsing/* but sb_crypt.c. pokebw2's ports
  of them (rp3) didn't match yet, so they aren't kept either.
- gdb/gdb.c (GameSpy storage, Sake): ROM code because its six Sake server URLs are ROM bytes the repo must not hold (a
  named static array moves ov175's layout; a string literal would need an asset type).
- gscommon/gsPlatformSocket.c, gsPlatformThread.c, gsPlatformUtil.c: our own decompilation, but written against the
  old GameSpy headers; it conflicts with pokebw2's (types of sockets, host lookups, threads).

Kept as C: pokebw2's ports that match, gamespy/darray.c, md5c.c, gt2/gt2Auth.c, gt2Buffer.c, gt2Callback.c,
serverbrowsing/sb_crypt.c (in place of gs/'s), and every file that was never ntrtwl-based. The GameSpy headers in
lib/dwc/include/dwc are pokebw2's with Black's names; gs/*.h only include them.

## Re-porting them later

One agent per include cluster, each from pokebw2's C (tools/port_file.py; delete nothing more), judged by the module
check: a file that matches gets its `complete` back.

1. GameSpy GP (gp/gp.c, gpi*.c): pokebw2's gp/ files and dwc/gp.h, gpi*.h are in place; most failed on missing
   prototypes (pokebw2's own headers lack some) and SOC* socket types.
2. GameSpy gt2 + gstats + hashtable: gt2Connection 13/14, gt2Message 48/51, hashtable 8/9 matched in rp3's try;
   gstats one relocation off; gbucket matched but pokebw2 splits its .bss differently from Black's file.
3. qr2, natneg, serverbrowsing, gsAvailable: pokebw2's files, blocked on gsPlatformSocket.h's socket types.
4. gscommon: make our own three files compile against pokebw2's gsPlatform*.h (or port pokebw2's: gsPlatformThread
   2/6 matched).
5. NitroDWC base and account/bm (main's TWLDWC files): pokebw2 has twins for most (tools/callee_names.py found them),
   but they need pokebw2's dwc/*.h base types first (dwc.h is still ours).
6. libvct (fir, mixer, udp_buffer, vad): pokebw2's vct_dsp.c, vct_mixer.c, vct_vad.c, vct_packet_pool.c.
7. cps.c: pokebw2's cps.c as ported, with pokebw2's own packet macros.

The headers that still had ntrtwl's text (dwc/dwc.h, bm.h, ac.h, nitrowifi/soc.h and cps.h) were put on pokebw2's text
(rp5, 2026-11-07): soc.h's types are pokebw2's SOC* (SOCConfig has its vendor and version words), the SDK's inline
functions and the unused packet macros are gone, dwc.h's callbacks, DWCError and DWCFriendControl are pokebw2's and what
no kept C used (DWC_ECODE_*, DWCAcc*, DWCUtilUni64, the allocator types) was dropped; a re-port that needs one takes it from
pokebw2's headers. lib/nitro and lib/twl hold nothing from ntrtwl (THIRD_PARTY.md "Provenance").
