// The PC build's start (docs/port-runtime.md): what runs before libntr's main(), which sets up the simulated DS
// (SDL window, GL, the ARM7's PXI threads, sim_config.ini, header.bin) and then calls the game's entry, NitroMain
// (src/system/main.c's main: port_runtime.h names it so).
//
// A constructor, so it runs first, on the main thread:
// - the working directory: BWPORT_DIR if set (win64: else the run\ folder next to the exe). libntr finds everything in the working directory: the extracted ROM
//   (the game's files: a/0/0/0 ..., header.bin), save.bin, sim_config.ini, imgui.ini, DbgLog.txt;
// - sim_config.ini with the PC build's defaults when there is none (libntr then reads it instead of writing its own);
// - the ROM's files: extracted from BWPORT_ROM or the first *.nds in the directory when a/0/0/0 or header.bin is
//   missing (else libntr's FS asks for a ROM in its ImGui file dialog at the first file the game opens);
// - CARD_Enable: libntr's CARD_Init enables the card only for a card boot (OS_GetBootType is 0 on PC);
// - the game lock (main_threads.c), held by the main thread from here on.
#include <nitro.h>
#include <SDL2/SDL.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "launcher_core.h"
#include "port_game.h"
#ifdef __APPLE__
#include <TargetConditionals.h>
#endif
#if defined(__APPLE__) && TARGET_OS_IOS
#define PORT_IOS 1
#endif
#ifdef _WIN32
#include <direct.h>
#define PORT_MKDIR(path) _mkdir(path)
#else
#define PORT_MKDIR(path) mkdir(path, 0755)
#endif

void PORT_RuntimeInit(void);

static BOOL FileExists(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f != NULL) {
        fclose(f);
        return TRUE;
    }
    return FALSE;
}

// The PC build's keys and settings (libntr's own: A Z, B X, X C, Y V, L Q, R E, Start Enter, Select right Shift, and
// a 3D scale of 10, libntr's own renderer, now removed): the DS's buttons where they sit on a DS (B left of A, Y left of X) and where DS emulators put
// them (DeSmuME, melonDS: A X, B Z, X S, Y A, L Q, R W), and a Resolution of 1 (the DS's, which any machine keeps up with). The mouse is the touch screen (libntr: a button down on the bottom screen). libntr's other keys: ` the debug
// GUI, Tab the frame cap, Space fast forward
static void WriteDefaultConfig(void) {
    FILE *f;

    if (FileExists("sim_config.ini") || (f = fopen("sim_config.ini", "w")) == NULL) {
        return;
    }
    fprintf(f, "; bw-port's defaults (port/src/main_start.c); delete this file for them again\n");
    fprintf(f, "[General]\nRenderer=auto\nResolution=1\nSound=acore\nFullScreen=false\nMasterVolume=127\nScreenLayout=Vertical\n"
               "SwapScreens=false\nFrameRate=60\n\n");
#if defined(PORT_IOS) || defined(__ANDROID__)
    fprintf(f, "; iOS, Android: the side panels off, the picture gets the width (the menu bar stays: it is how the settings are reached)\n");
    fprintf(f, "[General]\nShowInputPanel=false\nShowMetrics=false\n\n");
#endif
#ifdef PORT_IOS
    fprintf(f, "; iOS speed model: Smooth motion on (the frame rate follows it), Skip frames when slow on, the fast-forward button 4x\n");
    fprintf(f, "[General]\nSmoothMotion=on\nFrameSkip=auto\nFastForwardSpeed=4\n\n");
#endif
    fprintf(f, "[PAD]\n");
    fprintf(f, "AKey=%d\nBKey=%d\nXKey=%d\nYKey=%d\n", SDLK_x, SDLK_z, SDLK_s, SDLK_a);
    fprintf(f, "UpKey=%d\nDownKey=%d\nLeftKey=%d\nRightKey=%d\n", SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
    fprintf(f, "LKey=%d\nRKey=%d\nStartKey=%d\nSelectKey=%d\n", SDLK_q, SDLK_w, SDLK_RETURN, SDLK_RSHIFT);
    fprintf(f, "GUIKey=%d\nFrameCapToggleKey=%d\nFrameCapKey=%d\n", SDLK_BACKQUOTE, SDLK_TAB, SDLK_SPACE);
    fclose(f);
}

#ifdef PORT_IOS
// iOS: the app's container. Documents is what the Files app shows (UIFileSharingEnabled): the user's ROM, save.bin and
// sim_config.ini live there. The working directory is Library/Application Support/bwport, which the Files app does not
// show: the ROM's extracted files, imgui.ini ... and a symlink there for each of the two files that stay in Documents.
static char sDocs[1024];
#define ROM_DIR sDocs
#else
#define ROM_DIR "."
#endif

static const char *FindRom(char *buf, size_t size) {
    const char *env = getenv("BWPORT_ROM");
    DIR *dir;
    struct dirent *e;

    if (env != NULL) {
        return env;
    }
    if ((dir = opendir(ROM_DIR)) == NULL) {
        return NULL;
    }
    while ((e = readdir(dir)) != NULL) {
        size_t n = strlen(e->d_name);
        if (n > 4 && (strcmp(e->d_name + n - 4, ".nds") == 0 || strcmp(e->d_name + n - 4, ".NDS") == 0)) {
            snprintf(buf, size, "%s/%s", ROM_DIR, e->d_name);
            closedir(dir);
            return buf;
        }
    }
    closedir(dir);
    return NULL;
}

// The game whose files are in the working directory, by header.bin's game code (0x0C): the flat layout (BWPORT_DIR, Android's
// test runs) has no launcher to say it; in a game's folder it is the game PORT_EnterGame chose
static void GameFromHeader(void) {
    char head[16];
    FILE *f = fopen("header.bin", "rb");

    if (f != NULL) {
        if (fread(head, 1, sizeof(head), f) == sizeof(head)) {
            char code[5] = {head[12], head[13], head[14], head[15], 0};
            PORT_SetGame(code);
        }
        fclose(f);
    }
}

// TRUE when the ROM's files are here (extracted now or before); FALSE: no ROM (port/src/ios_main.c asks for one)
BOOL PORT_ExtractRomIfMissing(void) {
    char name[1100];
    const char *rom;
    int files;

    if (FileExists("header.bin") && FileExists("a/0/0/0")) {
        return TRUE;
    }
    if ((rom = FindRom(name, sizeof(name))) == NULL || !FileExists(rom)) {
        fprintf(stderr, "bwport: no extracted ROM here (a/0/0/0, header.bin) and no ROM to extract: set BWPORT_ROM "
                        "or put the .nds in the working directory (else libntr asks for one)\n");
        return FALSE;
    }
    files = LC_ExtractRom(rom, ".");    // port/src/launcher_core.c, the ROM's FNT and FAT; its progress: PORT_ExtractPercent
    return files > 0;
}

void PORT_LogSettings(void);
void PORT_CrashTest(void);

#ifndef PORT_IOS
extern char bwport_sim_config_path[1100];    // libntr-ios-configpath.patch
#else
// SDL's UIKit start moves the working directory to the app bundle before IosRun: back to ours (PORT_EnterWorkDir)
static char sWorkDir[1100];
#endif

static char sRoot[1100];            // the launcher's root (iOS: Documents)
static BOOL sWantLauncher;          // iOS: the launcher runs before the game boots (machine.c's SIM_RunGame asks)
static const LC_Game *sAutoPlay;    // iOS: the last game's ROM is there but not its files: the launcher extracts them and plays

// Into a game (the start, or the launcher's Play): its folders made, a tiered backup of its save as the last session left it
// (launcher_core.c says why at start), LastGame, then its working directory; iOS: the links there to its save and to the
// settings in Documents. FALSE: the working directory stays where it was.
void PORT_SkipMenuArm(void);   // skip_menu.c
void PORT_AssetSetRom(const char *code, const char *path);   // asset_load.c

int PORT_EnterGame(const LC_Game *g) {
    char gameDir[1200], gameWork[1200];

    PORT_SetGame(g->code);   // games.c: PORT_Game, the game machine.c starts (its table: port/include/port_game.h)
    LC_GameDir(g, gameDir, sizeof(gameDir));
    LC_GameWorkDir(g, gameWork, sizeof(gameWork));
    {
        char rom[1300];
        snprintf(rom, sizeof(rom), "%s/%s.nds", gameDir, g->folder);
        PORT_AssetSetRom(g->code, rom);   // asset_load.c reads the game's data tables from it at the boot
    }
    PORT_MKDIR(gameDir);
    PORT_MKDIR(gameWork);
    LC_TierBackup(g);
    if (LC_HasRom(g) || LC_HasFiles(g)) {
        LC_SetLastGame(g);
    }
    if (chdir(gameWork) != 0) {
        fprintf(stderr, "bwport: can't enter %s: the root stays the working directory\n", gameWork);
        return FALSE;
    }
    fprintf(stderr, "bwport: launcher: %s, in %s\n", g->title, gameWork);
    PORT_SkipMenuArm();   // skip_menu.c: a start or the launcher's Play may skip the title (a Restart to title does not)
#ifdef PORT_IOS
    {
        char link[1300], target[1300];
        snprintf(link, sizeof(link), "%s/save.bin", gameWork);
        snprintf(target, sizeof(target), "%s/save.bin", gameDir);
        unlink(link);    // the container's path changes on a reinstall or an update: a link from before points nowhere
        symlink(target, link);    // a dangling link is fine: fopen "wb" creates the target
        snprintf(link, sizeof(link), "%s/sim_config.ini", gameWork);
        snprintf(target, sizeof(target), "%s/sim_config.ini", sRoot);
        unlink(link);
        symlink(target, link);
        snprintf(sDocs, sizeof(sDocs), "%s", gameDir);
        if (getcwd(sWorkDir, sizeof(sWorkDir)) == NULL) {
            sWorkDir[0] = 0;
        }
    }
#else
    snprintf(bwport_sim_config_path, sizeof(bwport_sim_config_path), "%s/sim_config.ini", sRoot);
#endif
    return TRUE;
}

// machine.c, before the game boots: the launcher first (iOS: no game chosen at the start, or the settings' Games row)
int PORT_LauncherWanted(void) {
    return sWantLauncher;
}

void PORT_SetLauncherWanted(int on) {
    sWantLauncher = on;
#if !defined(__APPLE__) && !defined(__ANDROID__)
    // Linux and Windows restart as a new process (reset.c) until their in-process restart exists: Game > Games... travels to it as
    // BWPORT_GAMES (LauncherStart reads and clears it); Restart to title clears it
#ifdef _WIN32
    _putenv(on ? "BWPORT_GAMES=1" : "BWPORT_GAMES=");
#elif defined(__linux__)
    if (on) {
        setenv("BWPORT_GAMES", "1", 1);
    } else {
        unsetenv("BWPORT_GAMES");
    }
#endif
#endif
}

const char *PORT_LauncherRoot(void) {    // the desktop's launcher works in the root (launcher_gui.cpp)
    return sRoot;
}

const LC_Game *PORT_LauncherAutoPlay(void) {
    const LC_Game *g = sAutoPlay;
    sAutoPlay = NULL;
    return g;
}

#ifdef __ANDROID__
// Android has no launcher screen yet: the game is always chosen at the start (iOS: ios_launcher.m; the desktop: launcher_gui.cpp)
void PORT_RunLauncher(void) {
    sWantLauncher = FALSE;
}
#endif

// The launcher's start (docs/launcher.md, sections 4 and 6; port/src/launcher_core.c), in the root, which is the working
// directory here (iOS: Documents is the root, the working directory the hidden one): the one-time migration of a flat
// layout, the game (launcher.ini's LastGame, else the first game folder, else Black's), a desktop's ROM imported by itself
// (BWPORT_ROM or a .nds in the root), then into the game's folder. iOS: launcher.ini's Start=launcher, or no game, leaves
// the working directory in the root and the launcher runs before the game boots (port/src/ios_launcher.m).
// FALSE: the flat layout stays (a failed migration), as before the launcher.
static BOOL LauncherStart(void) {
    char work[1100], err[600];
    const LC_Game *found[8], *g;
    int n, i;

    if (getcwd(work, sizeof(work)) == NULL) {
        return FALSE;
    }
#ifdef PORT_IOS
    snprintf(sRoot, sizeof(sRoot), "%s", sDocs);
#else
    snprintf(sRoot, sizeof(sRoot), "%s", work);
#endif
    LC_Init(sRoot, work);
#if !defined(__APPLE__) && !defined(__ANDROID__) && !defined(PORT_IOS)
    // the restart starts a new process in the game's folder (the cwd it had): it works in the root again (PortStart)
#ifdef _WIN32
    {
        static char env[1200];
        snprintf(env, sizeof(env), "BWPORT_ROOT=%s", sRoot);
        _putenv(env);
    }
#else
    setenv("BWPORT_ROOT", sRoot, 1);
#endif
#endif
    if (getenv("BWPORT_LCTEST") != NULL) {    // the core's test hook: its steps, then the exit
        int fails = LC_TestScript(getenv("BWPORT_LCTEST"));
        fprintf(stderr, "lctest: %d failed\n", fails);
        _Exit(fails != 0);    // the host's: exit is MSL's here (port_prefix.h), which returns
    }
    if (LC_IsFlat() && LC_Migrate(err, sizeof(err)) < 0) {
        return FALSE;
    }
    n = LC_Scan(found, 8);
    g = LC_LastGame();
    for (i = 0; g != NULL && i < n && found[i] != g; i++) {
    }
    if (g == NULL || i == n) {
        g = n > 0 ? found[0] : LC_Games(&i);    // no game yet: Black's folder (the table's first)
    }
    // Android: the launcher's Play (LauncherActivity.enter); elsewhere a test's choice (BWPORT_GAME=IRAO: White)
    if (getenv("BWPORT_GAME") != NULL && LC_GameByCode(getenv("BWPORT_GAME")) != NULL) {
        g = LC_GameByCode(getenv("BWPORT_GAME"));
    }
#ifdef PORT_IOS
    if (LC_StartMode() == LC_START_LAUNCHER || (!LC_HasRom(g) && !LC_HasFiles(g))) {
        fprintf(stderr, "bwport: launcher: %s: the launcher first\n", LC_StartMode() == LC_START_LAUNCHER ? "Start=launcher" : "no game");
        sWantLauncher = TRUE;
        return TRUE;
    }
    if (!LC_HasFiles(g)) {
        fprintf(stderr, "bwport: launcher: %s's files are missing: the launcher extracts them\n", g->title);
        sWantLauncher = TRUE;
        sAutoPlay = g;
        return TRUE;
    }
#else
    if (!LC_HasRom(g) && !LC_HasFiles(g)) {    // the desktop's drop-in: BWPORT_ROM, else a .nds in the root
        char gameDir[1200], name[1100];
        const char *rom = FindRom(name, sizeof(name));    // still in the root
        if (rom != NULL) {
            LC_GameDir(g, gameDir, sizeof(gameDir));
            PORT_MKDIR(gameDir);
            LC_ImportRom(rom, NULL, err, sizeof(err));
        }
    }
#ifndef __ANDROID__
    // the desktop: launcher.ini's Start=launcher, or no game, leaves the working directory in the root and the Games window
    // (port/src/launcher_gui.cpp) runs before the game boots; a ROM copy without its files is extracted by the launcher. Linux and
    // Windows restart as a new process (reset.c, BWPORT_RESET): the game's own reset goes back to its title, not here
    const char *games = getenv("BWPORT_GAMES");    // Game > Games... of the process before (a restart)
    int gamesAsked = games != NULL && games[0] != 0;
    if (gamesAsked || (LC_StartMode() == LC_START_LAUNCHER && getenv("BWPORT_RESET") == NULL) || (!LC_HasRom(g) && !LC_HasFiles(g))) {
        fprintf(stderr, "bwport: launcher: %s: the launcher first\n",
                gamesAsked ? "Game > Games..." : LC_StartMode() == LC_START_LAUNCHER ? "Start=launcher" : "no game");
        PORT_SetLauncherWanted(0);    // (clears BWPORT_GAMES: a later restart goes back to the title)
        sWantLauncher = TRUE;
        return TRUE;
    }
    if (!LC_HasFiles(g)) {
        fprintf(stderr, "bwport: launcher: %s's files are missing: the launcher extracts them\n", g->title);
        sWantLauncher = TRUE;
        sAutoPlay = g;
        return TRUE;
    }
#endif
#endif
    return PORT_EnterGame(g);
}

#ifdef PORT_IOS
void PORT_EnterWorkDir(void) {
    if (sWorkDir[0] != 0 && chdir(sWorkDir) != 0) {
        fprintf(stderr, "bwport: can't enter %s\n", sWorkDir);
    }
}
#endif

#ifdef __ANDROID__
// Android: System.loadLibrary runs a library's constructors on Java's UI thread, before SDL's thread (where the game runs and
// takes the game lock) exists and before the app's storage path is known: port/src/android_main.c calls this from SDL_main,
// with BWPORT_DIR set to the app's storage directory
void PORT_AndroidStart(void) {
#else
__attribute__((constructor)) static void PortStart(void) {
#endif
    const char *dir = getenv("BWPORT_DIR");
    BOOL launcher = dir == NULL;    // BWPORT_DIR: one game in a flat folder, no launcher, no migration (the test tools; Android)
#ifdef _WIN32
    // started directly (not through bwport.bat): the run\ folder next to the exe, as the .bat does
    char defDir[1024];
    if (dir == NULL) {
        char *base = SDL_GetBasePath();    // the exe's folder, with its trailing separator
        if (base != NULL && strlen(base) + 4 < sizeof(defDir)) {
            strcpy(defDir, base);
            strcat(defDir, "run");
            PORT_MKDIR(defDir);
            dir = defDir;
        }
        SDL_free(base);
    }
#endif

#ifdef PORT_IOS
    if (dir == NULL && getenv("HOME") != NULL) {
        static char work[1100];
        char link[1200], target[1200];

        snprintf(sDocs, sizeof(sDocs), "%s/Documents", getenv("HOME"));
        PORT_MKDIR(sDocs);
        snprintf(work, sizeof(work), "%s/Library", getenv("HOME"));
        PORT_MKDIR(work);
        snprintf(work, sizeof(work), "%s/Library/Application Support", getenv("HOME"));
        PORT_MKDIR(work);
        snprintf(work, sizeof(work), "%s/Library/Application Support/bwport", getenv("HOME"));
        PORT_MKDIR(work);
        snprintf(link, sizeof(link), "%s/sim_config.ini", work);
        snprintf(target, sizeof(target), "%s/sim_config.ini", sDocs);
        unlink(link);    // the container's path changes on a reinstall or an update: a link from before points nowhere
        symlink(target, link);    // a dangling link is fine: fopen "wb" creates the target
        dir = work;
    }
#endif
    if (dir != NULL && chdir(dir) != 0) {
        fprintf(stderr, "bwport: can't enter BWPORT_DIR %s\n", dir);
    }
#if !defined(__APPLE__) && !defined(__ANDROID__) && !defined(PORT_IOS)
    if (getenv("BWPORT_DIR") == NULL && getenv("BWPORT_ROOT") != NULL && getenv("BWPORT_ROOT")[0] != 0 && chdir(getenv("BWPORT_ROOT")) != 0) {
        fprintf(stderr, "bwport: can't enter BWPORT_ROOT %s\n", getenv("BWPORT_ROOT"));
    }
#endif
    WriteDefaultConfig();    // in the root
#ifdef PORT_IOS
    launcher = launcher && sDocs[0] != 0;
#endif
    if (launcher && !LauncherStart()) {
#ifdef PORT_IOS
        // the flat layout (Documents' save.bin, linked from the working directory, as before the launcher)
        char link[1200], target[1200];
        snprintf(link, sizeof(link), "%s/Library/Application Support/bwport/save.bin", getenv("HOME"));
        snprintf(target, sizeof(target), "%s/save.bin", sDocs);
        unlink(link);
        symlink(target, link);
#endif
    }
#ifdef PORT_IOS
    if (getcwd(sWorkDir, sizeof(sWorkDir)) == NULL) {
        sWorkDir[0] = 0;
    }
#endif
    PORT_LogSettings();    // port/src/ios_log.c: iOS only
    PORT_CrashTest();      // port/src/crash_bt.c: BWPORT_CRASHTEST
    if (!sWantLauncher) {
        PORT_ExtractRomIfMissing();    // (iOS: the launcher's game has its files; this is the flat layout's, BWPORT_DIR or a failed migration)
        GameFromHeader();
    }
    CARD_Enable(TRUE);
    PORT_RuntimeInit();
}
