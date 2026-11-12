// The launcher's core (docs/launcher.md, sections 4 to 6): the game table, the game folders under the root, ROM import and
// removal, save export and import, backups and restore, launcher.ini and the one-time migration of the flat layout.
// Plain C (stdio, dirent, POSIX file calls; _WIN32 branches where Windows differs), with no SDK or UI in it: the UIs
// (UIKit on iOS, the ImGui window on the desktop, Android's LauncherScreen) list and call, and main_start.c uses it at
// start. Every function returns 0 on success; on failure a negative number and, when err is not NULL, one sentence for
// the user in err.
#ifndef LAUNCHER_CORE_H
#define LAUNCHER_CORE_H

#include <stddef.h>
#include <time.h>

#define LC_SAVE_SIZE 524288    // the DS card's flash: CARD_BACKUP_TYPE_FLASH_4MBITS_EX
#define LC_PATH 1100

typedef struct {
    const char *code;         // the header's game code at 0x0C
    const char *title;        // the game's title
    const char *shortName;    // "Black"
    const char *folder;       // the game's folder under the root, and its ROM copy's name with .nds
    const char *sha1[2];      // the supported dumps, hashed padded with 0xFF to the chip size; NULL ends; none = not playable yet
    const char *noIntro;      // the No-Intro name: libretro-thumbnails' file name for its cover (NULL: none)
    unsigned saveEnd;         // where the save's footer ends in a copy: the size the footer states (Black, White 0x23f9c; Black 2, White 2 0x25fa4)
    unsigned saveCopy;        // the backup copy's offset on the card (0x24000; 0x26000)
} LC_Game;

typedef struct {
    char name[64];    // the file's name in backups/
    time_t when;      // from the name (local time)
    int kind;         // LC_BACKUP_*
} LC_BackupInfo;

enum { LC_BACKUP_BEFORE, LC_BACKUP_TIER, LC_BACKUP_MIGRATION };
enum { LC_SAVE_GOOD, LC_SAVE_NEW };    // LC_CheckSave's answers (a bad save is < 0)
enum { LC_START_LAST, LC_START_LAUNCHER };
enum { LC_LAYOUT_LIBRARY, LC_LAYOUT_TABS };    // the iOS launcher's two looks

// --- the table and the folders
const LC_Game *LC_Games(int *count);                 // every game the table knows (playable or not)
const LC_Game *LC_GameByCode(const char *code);      // NULL when not in the table
// root: the folder the user sees (iOS Documents, Android's files dir, Windows run\, the working directory elsewhere);
// work: where the extracted files go (iOS Library/Application Support/bwport; the same as root elsewhere)
void LC_Init(const char *root, const char *work);
int LC_Scan(const LC_Game **out, int max);           // the games that have a folder under the root, in table order
void LC_GameDir(const LC_Game *g, char *out, size_t n);     // root/<folder>: the ROM copy, save.bin, backups/
void LC_GameWorkDir(const LC_Game *g, char *out, size_t n); // work/<folder>: the extracted files; the game's working directory
int LC_HasRom(const LC_Game *g);                     // the ROM copy is there
int LC_HasFiles(const LC_Game *g);                   // the extracted files are there (header.bin, a/0/0/0)
int LC_HasSave(const LC_Game *g);

// --- ROMs
// Check the header, copy into the game's folder while hashing, refuse a wrong sha1 (the copy deleted), extract (progress:
// PORT_ExtractPercent; the copy's: LC_CopyPercent). A failed import leaves nothing behind. *game gets the game.
int LC_ImportRom(const char *path, const LC_Game **game, char *err, size_t n);
int LC_CopyPercent(void);
int LC_ExtractRom(const char *rom, const char *dir);    // the ROM's files into dir; the count of files (0: none)
// Delete the ROM copy and the extracted files by the ROM's own file list; save.bin and backups/ stay
int LC_RemoveRom(const LC_Game *g, char *err, size_t n);

// --- saves
// g's own layout; a save of another game is refused by name
int LC_CheckSave(const LC_Game *g, const unsigned char *data, size_t size, char *err, size_t n);    // LC_SAVE_GOOD, LC_SAVE_NEW or < 0
int LC_CheckSaveFile(const LC_Game *g, const char *path, char *err, size_t n);    // the same on a .sav or .dsv file
int LC_ExportSave(const LC_Game *g, const char *dir, char *out, size_t outN, char *err, size_t n);  // dir/<title>.sav
int LC_ImportSave(const LC_Game *g, const char *path, char *err, size_t n);    // .sav (raw) or DeSmuME .dsv
int LC_DeleteSave(const LC_Game *g, char *err, size_t n);
// the play time (seconds) and the date of the last save the game wrote into a save file (.sav or .dsv) of g (NULL: any game); < 0: none
int LC_SaveFileInfo(const LC_Game *g, const char *path, long *seconds, time_t *saved);

// --- backups (backups/: save-YYYY-MM-DD-HHMMSS-before.bin, the last 20; save-YYYY-MM-DD-HHMMSS.bin, the tiered copies;
// save-before-launcher.bin, the migration's, kept for good)
int LC_BackupBefore(const LC_Game *g, char *err, size_t n);    // before an import, restore or deletion; no save: nothing
int LC_TierBackup(const LC_Game *g);       // a tiered copy when save.bin changed since the newest one; 1 = taken
int LC_ListBackups(const LC_Game *g, LC_BackupInfo *out, int max);    // newest first
int LC_RestoreBackup(const LC_Game *g, const char *name, char *err, size_t n);
void LC_SetClock(time_t now);              // tests: the time backups are named and pruned by (0: the real clock)

// --- launcher.ini
const LC_Game *LC_LastGame(void);
void LC_SetLastGame(const LC_Game *g);
int LC_StartMode(void);                    // LC_START_LAST or LC_START_LAUNCHER
void LC_SetStartMode(int mode);
int LC_Layout(void);                       // LC_LAYOUT_LIBRARY or LC_LAYOUT_TABS
void LC_SetLayout(int layout);

int LC_CoverDownloads(void);               // launcher.ini's DownloadArtwork=on|off (on by default)
void LC_SetCoverDownloads(int on);
int LC_SkipMenu(void);                     // launcher.ini's SkipMenu=on|off (off by default; port/src/skip_menu.c)
void LC_SetSkipMenu(int on);

// --- cover art (launcher_cover.c; docs/launcher.md, "Cover art"): fetched on the user's device, never shipped. GameTDB by the
// header's code, else libretro-thumbnails by the No-Intro name, into the game's folder as cover.jpg, its source in cover.txt.
// Shown after the user's artwork.jpg and before the gradient.
#define LC_USER_AGENT "bw-port-launcher/1.0 (a Pokemon Black/White port's game launcher; one cover per game)"
// One GET over https (the platform's: http_get.c's libcurl on macOS, ios_http.m's NSURLSession; a stub elsewhere): 15 s, at
// most 10 MB, LC_USER_AGENT. 0: a 200 and its body in path; > 0: another HTTP status (nothing in path); < 0: no answer (err)
int PORT_HttpGet(const char *url, const char *path, char *err, size_t n);
// 1 when a cover should be fetched now: the setting is on, the game has neither artwork.jpg nor cover.jpg, and it was not tried
// yet in this process (it is marked tried). The UIs call it on their main thread and LC_FetchCover on another.
int LC_CoverWanted(const LC_Game *g);
// Fetch, check that it decodes, write cover.jpg (and cover.txt); blocking. Nothing replaced unless a good image came.
// 0 = written; < 0 = none (err says why: off, offline, not found)
int LC_FetchCover(const LC_Game *g, char *err, size_t n);
// The cover's source ("GameTDB", "libretro") and the page its credit links to; < 0: no cover.jpg
int LC_CoverSource(const LC_Game *g, char *name, size_t nameN, char *link, size_t linkN);
// RGB pixels to a JPEG (quality 85), the long side at most 1500 px (a box filter), through dst.tmp; 0 = written
int LC_WriteImage(const unsigned char *rgb, int w, int h, const char *dst);

// --- the migration (section 6): 1 = nothing to do, 0 = done, < 0 = failed and everything left in place
int LC_Migrate(char *err, size_t n);
int LC_IsFlat(void);                       // save.bin or header.bin at the root's top and no game folder

// --- tests: BWPORT_LCTEST="clock=2026-01-01;importrom=PATH;exportsave=DIR;importsave=PATH;restore=NAME;tier;list;..."
int LC_TestScript(const char *script);

#endif
