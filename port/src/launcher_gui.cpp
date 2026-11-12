// The desktop's launcher (docs/launcher.md, section 3): the Games window, drawn in libntr's ImGui. It comes up from the run loop
// before the game boots (port/src/machine.c's SIM_RunGame calls PORT_RunLauncher): at a start with no game, or with launcher.ini's
// Start=launcher, and on macOS after Game > Games... (the game's own soft reset, flagged "then the launcher"). Its frames are
// libntr's (SIM_LauncherFrame, libntr-ios-launcher.patch: the window's events, the menu bar and the settings windows, the present)
// with nothing of the game running. Play enters the game's folder (main_start.c's PORT_EnterGame) and returns; the game boots.
// Behaves as the iOS launcher (port/src/ios_launcher.m) and calls the same C core (port/src/launcher_core.c, the LC_* calls).
//
// Two layouts, switched from the gear menu and kept in launcher.ini (Layout=):
//   Library  every imported game as a card (a grid that wraps and scrolls), plus "Add Game". A click plays; each card's ... menu
//            has the save, backup, artwork and remove actions.
//   Tabs     a row of tabs at the top, one per game in its accent; the selected game fills the window: its showcase, title, save
//            summary, Play / Continue and Save, Backups, More. The page takes the game's theme (Black and Black 2 dark, White and
//            White 2 light).
//
// Content rules (the maintainer's): no Pokemon characters, no Nintendo / Game Freak / Pokemon logos or art, and the app ships none.
// The showcase is an abstract gradient in the game's colours (made here at run time), or the artwork the user picked for the
// game (Set Artwork..., stored as the game's folder's artwork.jpg, at most 1500 px on its long side). The scene slot (the game's
// own title-screen scene, drawn later from the user's ROM at run time) is left empty, as on iOS. The small icon is the ROM's
// own banner icon, read from the game's ROM copy at run time. Cover art (launcher_cover.c): with no artwork, the game's front
// cover, fetched on this machine into its folder as cover.jpg (gear menu: Download Artwork, on by default; the game's menu:
// Download Cover Again), aspect-fit over its own blurred copy, credited ("Cover art: GameTDB") on the tabs' showcase and in
// the game's menu, which opens the source's page.
//
// File pickers: ImGuiFileDialog (in libntr, MIT), one dialog of our own: it works the same on macOS, Windows and Linux, in the
// app's window and in hidden test runs, where a native panel (NSOpenPanel: an app with no Dock icon, a modal run loop) would not.
// Images: stb_image and stb_image_write (Sean Barrett, public domain / MIT, port/src/stb_image*.h, unchanged) decode the picked
// JPEG or PNG and write artwork.jpg; the textures go through ImGui's own texture list (RegisterUserTexture), so the Metal and
// the OpenGL backends upload them alike.
//
// Test hook (as iOS's): BWPORT_LAUNCHER_TEST="layout=tabs;game=1;shot=NAME;..." the first time the launcher comes up,
// BWPORT_LAUNCHER_AGAIN at every later time (and its Play flags the next reset "then the launcher" again, so cfgscript
// restart=1 comes back here). Steps run one second apart: layout=library|tabs, game=N|+1|next (the selected game; +1: the one
// after the selected one, at first the last played; next: the one after the game played last), size=WxH (the
// window), fullscreen=0|1, wait=S, idle (waits for an import or extraction to end), shot=NAME (the root's NAME.png; NAME-N.png the
// Nth time the launcher is up), importrom=PATH, importsave=PATH, exportsave=DIR, artwork=PATH, noartwork, restore=N (the Nth
// backup, newest first), remove, backups (the list), cover (Download Cover Again), covers=on|off (Download Artwork; idle also waits
// for the covers being fetched), menu=game|global (opens it), settings=N (a settings window), start=last|
// launcher, skipmenu=on|off (Skip Title and Menu), alert=ok|cancel (the open question's buttons), play, quit (ends the process).
#include "simulator/imgui/imgui.hpp"
#include "imgui_internal.h"
#include "ImGuiFileDialog.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <functional>
#include <string>
#include <vector>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <shobjidl.h>    // IFileOpenDialog: the pickers are Windows' own
#include <SDL2/SDL_syswm.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0755)
#endif

extern "C" {
#include "launcher_core.h"
void SIM_LauncherFrame(void);                  // libntr-ios-launcher.patch
void SIM_ShotRequest(const char *name);
void SIM_GuiSetLauncher(void (*draw)(float top));
void SIM_GuiOpenSettings(int which);
int PORT_EnterGame(const LC_Game *g);          // main_start.c
void PORT_SetLauncherWanted(int on);
const LC_Game *PORT_LauncherAutoPlay(void);
const char *PORT_LauncherRoot(void);
int PORT_ExtractPercent(void);                 // launcher_core.c
}

#include "stb_image.h"          // (their code: launcher_cover.c)
#include "stb_image_write.h"

namespace {

// ---- textures (ImGui's user textures: the backend makes and frees them) ------------------------------------------------------

struct Tex {
    ImTextureData *d = nullptr;
    int w = 0, h = 0;
};
std::vector<ImTextureData *> sDying;

Tex MakeTex(const unsigned char *rgba, int w, int h) {
    Tex t;
    t.d = IM_NEW(ImTextureData)();
    t.d->Create(ImTextureFormat_RGBA32, w, h);
    memcpy(t.d->Pixels, rgba, (size_t)w * h * 4);
    t.d->UseColors = true;
    ImGui::RegisterUserTexture(t.d);
    t.w = w;
    t.h = h;
    return t;
}

void FreeTex(Tex &t) {
    if (t.d != nullptr) {
        t.d->WantDestroyNextFrame = true;
        sDying.push_back(t.d);
    }
    t = Tex();
}

void Reap() {   // the freed textures the backend has let go of
    for (size_t i = 0; i < sDying.size();) {
        if (sDying[i]->Status == ImTextureStatus_Destroyed) {
            ImGui::UnregisterUserTexture(sDying[i]);
            IM_DELETE(sDying[i]);
            sDying.erase(sDying.begin() + (long)i);
        } else {
            i++;
        }
    }
}

// ---- the model ---------------------------------------------------------------------------------------------------------------

struct Col { float r, g, b; };
Col Hex(unsigned h) { return {(h >> 16 & 255) / 255.0f, (h >> 8 & 255) / 255.0f, (h & 255) / 255.0f}; }
ImU32 U32(Col c, float a = 1.0f) { return ImGui::ColorConvertFloat4ToU32(ImVec4(c.r, c.g, c.b, a)); }
Col Mix(Col a, Col b, float t) { return {a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t}; }

struct Game {
    const LC_Game *lc = nullptr;
    std::string title, shortTitle;
    bool hasSave = false, playable = false, dark = true;
    long playSeconds = -1;     // -1: the save has no play time (a new game's empty save)
    time_t lastSaved = 0;
    Col accent{}, deep{}, glow{};
    Tex icon, art, grad;       // the banner icon (4x), the artwork, the showcase's gradient (made for gradW x gradH)
    int gradW = 0, gradH = 0;
    Tex cover, coverBack;      // the downloaded cover (when there is no artwork) and its blurred, darkened backdrop
    std::string coverSource, coverLink;   // its cover.txt: "GameTDB" or "libretro", the page the credit links to
};

// Each game's colours and theme, by its game code (ios_launcher.m's)
void Theme(Game &x) {
    struct { const char *code; unsigned accent, deep, glow; bool dark; } t[] = {
        {"IRBO", 0x3d8bff, 0x0b0f1a, 0x1e5bd8, true},    // Black
        {"IRAO", 0xe8590c, 0xf4f1ec, 0xffb37a, false},   // White
        {"IREO", 0xf2b705, 0x10131c, 0x2e6f9e, true},    // Black 2
        {"IRDO", 0xd6336c, 0xeef3f7, 0x8fd3ff, false},   // White 2
    };
    size_t k = 0;
    for (size_t i = 0; i < sizeof t / sizeof t[0]; i++) {
        if (strcmp(t[i].code, x.lc->code) == 0) k = i;
    }
    x.accent = Hex(t[k].accent);
    x.deep = Hex(t[k].deep);
    x.glow = Hex(t[k].glow);
    x.dark = t[k].dark;
}

std::string GameDir(const LC_Game *g) {
    char d[LC_PATH * 2];
    LC_GameDir(g, d, sizeof(d));
    return d;
}
std::string RomPath(const LC_Game *g) { return GameDir(g) + "/" + g->folder + ".nds"; }
std::string ArtworkPath(const LC_Game *g) { return GameDir(g) + "/artwork.jpg"; }
std::string CoverPath(const LC_Game *g) { return GameDir(g) + "/cover.jpg"; }

// The ROM's banner icon: header word 0x68 is the banner's offset; the banner has a 4x4-tile 4bpp bitmap at +0x20 and 16 BGR555
// colours at +0x220 (colour 0 is transparent). Made 4x with nearest pixels, so the linear filter keeps it sharp.
Tex BannerIcon(const std::string &path) {
    FILE *f = fopen(path.c_str(), "rb");
    if (f == nullptr) return Tex();
    unsigned char hdr[0x6c], bmp[512], pal[32];
    unsigned off = 0;
    bool ok = fread(hdr, 1, sizeof hdr, f) == sizeof hdr;
    if (ok) {
        off = hdr[0x68] | hdr[0x69] << 8 | hdr[0x6a] << 16 | (unsigned)hdr[0x6b] << 24;
        ok = off != 0 && fseek(f, (long)off + 0x20, SEEK_SET) == 0 && fread(bmp, 1, 512, f) == 512 && fread(pal, 1, 32, f) == 32;
    }
    fclose(f);
    if (!ok) return Tex();
    std::vector<unsigned char> px(128 * 128 * 4);
    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 128; x++) {
            int sx = x / 4, sy = y / 4;
            unsigned char b = bmp[((sy / 8) * 4 + sx / 8) * 32 + (sy % 8) * 4 + (sx % 8) / 2];
            int i = sx & 1 ? b >> 4 : b & 15;
            unsigned c = pal[i * 2] | pal[i * 2 + 1] << 8;
            unsigned char *p = &px[(size_t)(y * 128 + x) * 4];
            p[0] = (unsigned char)((c & 31) * 255 / 31);
            p[1] = (unsigned char)((c >> 5 & 31) * 255 / 31);
            p[2] = (unsigned char)((c >> 10 & 31) * 255 / 31);
            p[3] = i == 0 ? 0 : 255;
        }
    }
    return MakeTex(px.data(), 128, 128);
}

Tex LoadArtwork(const std::string &path) {
    int w = 0, h = 0, n = 0;
    unsigned char *d = stbi_load(path.c_str(), &w, &h, &n, 4);
    if (d == nullptr) return Tex();
    Tex t = MakeTex(d, w, h);
    stbi_image_free(d);
    return t;
}

// The cover and its backdrop: the cover box-filtered down to 48 px on its long side, three box blurs, darkened; the linear
// filter spreads it over the showcase smoothly
void LoadCover(Game &x) {
    int w = 0, h = 0, n = 0;
    unsigned char *d = stbi_load(CoverPath(x.lc).c_str(), &w, &h, &n, 4);
    if (d == nullptr) return;
    x.cover = MakeTex(d, w, h);
    int bw = w >= h ? 48 : (w * 48 + h - 1) / h, bh = h >= w ? 48 : (h * 48 + w - 1) / w;
    std::vector<float> f((size_t)bw * bh * 3), t(f.size());
    for (int y = 0; y < bh; y++) {
        for (int x0 = 0; x0 < bw; x0++) {
            int sx0 = x0 * w / bw, sx1 = (x0 + 1) * w / bw, sy0 = y * h / bh, sy1 = (y + 1) * h / bh;
            float sum[3] = {0, 0, 0};
            int cnt = 0;
            for (int sy = sy0; sy < (sy1 > sy0 ? sy1 : sy0 + 1); sy++) {
                for (int sx = sx0; sx < (sx1 > sx0 ? sx1 : sx0 + 1); sx++) {
                    const unsigned char *p = &d[((size_t)sy * w + sx) * 4];
                    for (int k = 0; k < 3; k++) sum[k] += p[k];
                    cnt++;
                }
            }
            for (int k = 0; k < 3; k++) f[((size_t)y * bw + x0) * 3 + k] = sum[k] / cnt;
        }
    }
    stbi_image_free(d);
    for (int pass = 0; pass < 6; pass++) {   // a 5-wide box, across then down, three times
        bool across = pass % 2 == 0;
        for (int y = 0; y < bh; y++) {
            for (int x0 = 0; x0 < bw; x0++) {
                float sum[3] = {0, 0, 0};
                for (int o = -2; o <= 2; o++) {
                    int sx = across ? x0 + o : x0, sy = across ? y : y + o;
                    sx = sx < 0 ? 0 : sx >= bw ? bw - 1 : sx;
                    sy = sy < 0 ? 0 : sy >= bh ? bh - 1 : sy;
                    for (int k = 0; k < 3; k++) sum[k] += f[((size_t)sy * bw + sx) * 3 + k];
                }
                for (int k = 0; k < 3; k++) t[((size_t)y * bw + x0) * 3 + k] = sum[k] / 5.0f;
            }
        }
        f.swap(t);
    }
    float dim = x.dark ? 0.45f : 0.75f;
    std::vector<unsigned char> px((size_t)bw * bh * 4);
    for (size_t i = 0; i < (size_t)bw * bh; i++) {
        for (int k = 0; k < 3; k++) px[i * 4 + k] = (unsigned char)(f[i * 3 + k] * dim + 0.5f);
        px[i * 4 + 3] = 255;
    }
    x.coverBack = MakeTex(px.data(), bw, bh);
    char name[64], link[256];
    if (LC_CoverSource(x.lc, name, sizeof name, link, sizeof link) == 0) {
        x.coverSource = name;
        x.coverLink = link;
    }
}

// The showcase's gradient for a w x h showcase (ios_launcher.m's BWShowcase: a diagonal base from a lighter (dark theme) or darker
// (light) shade of the game's deep colour, the glow's light and the accent's light, both radial and fading to nothing at their
// edge; the sequels have the lights on the other diagonal). Made at half size: it is smooth.
void MakeGradient(Game &g, int w, int h) {
    int tw = w / 2 > 8 ? w / 2 : 8, th = h / 2 > 8 ? h / 2 : 8;
    if (tw > 640) { th = th * 640 / tw; tw = 640; }
    float hsv[3];
    ImGui::ColorConvertRGBtoHSV(g.deep.r, g.deep.g, g.deep.b, hsv[0], hsv[1], hsv[2]);
    float v2 = g.dark ? fminf(1.0f, hsv[2] + 0.10f) : hsv[2] * 0.90f;
    Col deep2;
    ImGui::ColorConvertHSVtoRGB(hsv[0], hsv[1], v2, deep2.r, deep2.g, deep2.b);
    bool two = g.lc != nullptr && strchr(g.lc->shortName, '2') != nullptr;
    float m = (float)(tw > th ? tw : th);
    float c1x = (two ? 0.05f : 0.35f) * tw, c1y = 0.55f * th, r1 = m * 0.45f, a1 = g.dark ? 0.85f : 0.75f;
    float c2x = (two ? 0.75f : 0.0f) * tw, c2y = 0.05f * th, r2 = m * 0.35f, a2 = g.dark ? 0.55f : 0.45f;
    std::vector<unsigned char> px((size_t)tw * th * 4);
    for (int y = 0; y < th; y++) {
        for (int x = 0; x < tw; x++) {
            float t = ((float)x / tw + (float)y / th) * 0.5f;
            Col c = Mix(deep2, g.deep, t);
            float d1 = sqrtf((x - c1x) * (x - c1x) + (y - c1y) * (y - c1y)) / r1;
            float d2 = sqrtf((x - c2x) * (x - c2x) + (y - c2y) * (y - c2y)) / r2;
            if (d1 < 1.0f) c = Mix(c, g.glow, a1 * (1.0f - d1));
            if (d2 < 1.0f) c = Mix(c, g.accent, a2 * (1.0f - d2));
            unsigned char *p = &px[(size_t)(y * tw + x) * 4];
            p[0] = (unsigned char)(c.r * 255.0f + 0.5f);
            p[1] = (unsigned char)(c.g * 255.0f + 0.5f);
            p[2] = (unsigned char)(c.b * 255.0f + 0.5f);
            p[3] = 255;
        }
    }
    FreeTex(g.grad);
    g.grad = MakeTex(px.data(), tw, th);
    g.gradW = w;
    g.gradH = h;
}

std::vector<Game> sGames;
int sSelected;          // the tabs' game
int sLayout;            // LC_LAYOUT_*
int sTimes;             // the times the launcher has come up
bool sDone;             // Play entered a game
void FetchCover(const LC_Game *g, bool manual);

void ReadSave(Game &x) {
    std::string path = GameDir(x.lc) + "/save.bin";
    long secs = -1;
    time_t when = 0;
    struct stat st;
    x.hasSave = LC_HasSave(x.lc) != 0;
    x.playSeconds = -1;
    x.lastSaved = 0;
    if (!x.hasSave) return;
    if (LC_SaveFileInfo(x.lc, path.c_str(), &secs, &when) == 0) {
        x.playSeconds = secs;
        x.lastSaved = when;
    } else if (stat(path.c_str(), &st) == 0) {
        x.lastSaved = st.st_mtime;
    }
}

// The games under the root that have a ROM copy or extracted files (a removed game's folder keeps its save and backups and is not
// listed), in the table's order
void Reload() {
    const LC_Game *keep = sSelected < (int)sGames.size() ? sGames[(size_t)sSelected].lc : LC_LastGame();
    for (Game &g : sGames) {
        FreeTex(g.icon);
        FreeTex(g.art);
        FreeTex(g.grad);
        FreeTex(g.cover);
        FreeTex(g.coverBack);
    }
    sGames.clear();
    const LC_Game *found[8];
    int n = LC_Scan(found, 8);
    for (int i = 0; i < n; i++) {
        if (!LC_HasRom(found[i]) && !LC_HasFiles(found[i])) continue;
        Game x;
        x.lc = found[i];
        x.title = found[i]->title;
        x.shortTitle = found[i]->shortName;
        x.playable = found[i]->sha1[0] != nullptr;
        Theme(x);
        ReadSave(x);
        x.icon = BannerIcon(RomPath(x.lc));
        x.art = LoadArtwork(ArtworkPath(x.lc));
        if (x.art.d == nullptr) LoadCover(x);
        sGames.push_back(x);
    }
    sSelected = 0;
    for (size_t i = 0; i < sGames.size(); i++) {
        if (sGames[i].lc == keep) sSelected = (int)i;
    }
    for (Game &g : sGames) {   // a cover for each game that has no picture yet: once a process (LC_CoverWanted)
        if (LC_CoverWanted(g.lc)) FetchCover(g.lc, false);
    }
}

// ---- text ----------------------------------------------------------------------------------------------------------------------

ImFont *sFont, *sBold;    // the system's UI font when there is one (never shipped), else ImGui's own

void LoadFonts() {
    static bool done;
    if (done) return;
    done = true;
    struct { const char *path; int regular, bold; } c[] = {
        {"/System/Library/Fonts/HelveticaNeue.ttc", 0, 1},
        {"C:/Windows/Fonts/segoeui.ttf", 0, -1},
        {"/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0, -1},
        {"/usr/share/fonts/TTF/DejaVuSans.ttf", 0, -1},
    };
    const char *bold[] = {"C:/Windows/Fonts/segoeuib.ttf", "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
                          "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf"};
    ImGuiIO &io = ImGui::GetIO();
    for (auto &f : c) {
        if (access(f.path, R_OK) != 0) continue;
        ImFontConfig cfg;
        cfg.FontNo = f.regular;
        sFont = io.Fonts->AddFontFromFileTTF(f.path, 16.0f, &cfg);
        if (f.bold >= 0) {
            ImFontConfig b;
            b.FontNo = f.bold;
            sBold = io.Fonts->AddFontFromFileTTF(f.path, 16.0f, &b);
        }
        break;
    }
    for (const char *b : bold) {
        if (sBold == nullptr && sFont != nullptr && access(b, R_OK) == 0) sBold = io.Fonts->AddFontFromFileTTF(b, 16.0f);
    }
    if (sFont == nullptr) sFont = io.Fonts->Fonts[0];
    if (sBold == nullptr) sBold = sFont;
}

// The launcher's scale: the UI's (UiScale, the display's DPI), made smaller in a window too small for it (sK, from the window at each
// frame). The ImGui widgets' fonts scale by UiScale themselves (PushFont's sizes times sK); the draw list's text and sizes by S().
float sK = 1.0f;
float S() { return ImGui::GetStyle().FontScaleMain * sK; }

std::string PlayText(long secs) {
    char b[48];
    long h = secs / 3600, m = secs / 60 % 60;
    if (h > 0) snprintf(b, sizeof b, "%ldh %ldm", h, m);
    else snprintf(b, sizeof b, "%ldm", m);
    return b;
}

std::string Ago(time_t when) {
    long d = (long)(time(nullptr) - when);
    char b[64];
    if (d < 60) return "just now";
    struct { long s; const char *one, *many; } u[] = {{31536000, "year", "years"}, {2592000, "month", "months"}, {604800, "week", "weeks"},
                                                     {86400, "day", "days"}, {3600, "hour", "hours"}, {60, "minute", "minutes"}};
    for (auto &x : u) {
        if (d >= x.s) {
            long n = d / x.s;
            snprintf(b, sizeof b, "%ld %s ago", n, n == 1 ? x.one : x.many);
            return b;
        }
    }
    return "";
}

std::string DateText(time_t when) {
    char b[64];
    struct tm tm;
#ifdef _WIN32
    localtime_s(&tm, &when);
#else
    localtime_r(&when, &tm);
#endif
    strftime(b, sizeof b, "%b %d, %Y at %H:%M", &tm);
    return b;
}

// "Saved 5 hours ago  ·  42h 17m played" (the save's own date and play time), or that there is no save
std::string Summary(const Game &g) {
    if (!g.hasSave) return "No save yet";
    std::string s = "Saved " + (g.lastSaved != 0 ? Ago(g.lastSaved) : std::string());
    if (g.playSeconds >= 0) s += "  \xc2\xb7  " + PlayText(g.playSeconds) + " played";
    return s;
}

// ---- drawing helpers -----------------------------------------------------------------------------------------------------------

ImDrawList *DL() { return ImGui::GetWindowDrawList(); }

void Text(ImFont *f, float size, ImVec2 at, ImU32 col, const char *s, float wrap = 0.0f) {
    DL()->AddText(f, size * S(), at, col, s, nullptr, wrap);
}
ImVec2 TextSize(ImFont *f, float size, const char *s) { return f->CalcTextSizeA(size * S(), FLT_MAX, 0.0f, s); }

// The aspect fill of a w x h picture into r: its UVs
void Fill(ImVec2 a, ImVec2 b, int w, int h, ImVec2 &uv0, ImVec2 &uv1) {
    float ra = (b.x - a.x) / fmaxf(1.0f, b.y - a.y), ta = (float)w / fmaxf(1.0f, (float)h);
    uv0 = ImVec2(0, 0);
    uv1 = ImVec2(1, 1);
    if (ra > ta) {
        float k = ta / ra;
        uv0.y = (1.0f - k) * 0.5f;
        uv1.y = 1.0f - uv0.y;
    } else {
        float k = ra / ta;
        uv0.x = (1.0f - k) * 0.5f;
        uv1.x = 1.0f - uv0.x;
    }
}

// The showcase: the artwork (aspect fill), else the gradient and its ring. The scene slot (later: the game's own title-screen
// scene, from the user's ROM at run time) would be drawn here over the gradient; nothing draws into it yet.
// A downloaded cover (covers are near square or portrait; showcases and cards are not): aspect-fit with a margin over its own
// blurred, darkened copy (aspect fill), above bottom (the card's info panel), with its credit when credit is set.
void Showcase(Game &g, ImVec2 a, ImVec2 b, float round, float bottom = 0.0f, bool credit = false) {
    int w = (int)(b.x - a.x), h = (int)(b.y - a.y);
    ImVec2 uv0, uv1;
    if (w < 4 || h < 4) return;
    if (g.art.d != nullptr) {
        Fill(a, b, g.art.w, g.art.h, uv0, uv1);
        DL()->AddImageRounded(g.art.d->GetTexRef(), a, b, uv0, uv1, IM_COL32_WHITE, round);
        return;
    }
    if (g.cover.d != nullptr) {
        float s = S(), m = fmaxf(10.0f * s, fminf((float)w, (float)h - bottom) * 0.06f);
        Fill(a, b, g.coverBack.w, g.coverBack.h, uv0, uv1);
        DL()->AddImageRounded(g.coverBack.d->GetTexRef(), a, b, uv0, uv1, IM_COL32_WHITE, round);
        float bw = (float)w - 2.0f * m, bh = (float)h - bottom - 2.0f * m - (credit ? 18.0f * s : 0.0f);
        if (bw > 8.0f && bh > 8.0f) {
            float k = fminf(bw / (float)g.cover.w, bh / (float)g.cover.h);
            float cw = g.cover.w * k, ch = g.cover.h * k;
            ImVec2 ca(a.x + ((float)w - cw) * 0.5f, a.y + m + (bh - ch) * 0.5f), cb(ca.x + cw, ca.y + ch);
            float r = fminf(10.0f * s, round * 0.5f);
            for (int i = 3; i >= 1; i--) {   // a soft shadow under it
                float o = (float)i * 3.0f * s;
                DL()->AddRectFilled(ImVec2(ca.x - o * 0.5f, ca.y), ImVec2(cb.x + o * 0.5f, cb.y + o), IM_COL32(0, 0, 0, 34), r + o);
            }
            DL()->AddImageRounded(g.cover.d->GetTexRef(), ca, cb, ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE, r);
        }
        if (credit && !g.coverSource.empty()) {
            std::string c = "Cover art: " + g.coverSource;
            ImVec2 ts = TextSize(sFont, 12.0f, c.c_str());
            Text(sFont, 12.0f, ImVec2(b.x - ts.x - 14.0f * s, b.y - ts.y - 10.0f * s), IM_COL32(255, 255, 255, 170), c.c_str());
        }
        return;
    }
    if (g.grad.d == nullptr || abs(g.gradW - w) > 6 || abs(g.gradH - h) > 6) MakeGradient(g, w, h);
    DL()->AddImageRounded(g.grad.d->GetTexRef(), a, b, ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE, round);
    bool two = strchr(g.lc->shortName, '2') != nullptr;
    float mn = (float)(w < h ? w : h);
    ImVec2 c(a.x + (two ? 0.3f : 0.68f) * w, a.y + 0.48f * h);
    ImU32 ring = g.dark ? IM_COL32(255, 255, 255, 56) : U32(g.accent, 0.22f);
    DL()->PushClipRect(a, b, true);
    DL()->AddCircle(c, mn * 0.36f, ring, 96, 1.5f * S());
    DL()->PathArcTo(c, mn * 0.25f, 0.3f, 2.0f * IM_PI - 0.9f, 72);
    DL()->PathStroke(ring, 0, 1.5f * S());
    DL()->PopClipRect();
}

// The game's icon: the ROM's banner icon on a rounded tile, else the tile in its accent with the game's initial
void Icon(const Game &g, ImVec2 a, float side) {
    ImVec2 b(a.x + side, a.y + side);
    float r = side * 0.22f;
    if (g.icon.d != nullptr) {
        DL()->AddRectFilled(a, b, IM_COL32(255, 255, 255, 255), r);
        DL()->AddImageRounded(g.icon.d->GetTexRef(), a, b, ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE, r);
        return;
    }
    DL()->AddRectFilled(a, b, U32(g.accent), r);
    char s[2] = {g.shortTitle.empty() ? '?' : g.shortTitle[0], 0};
    ImVec2 ts = TextSize(sBold, side * 0.55f / S(), s);
    Text(sBold, side * 0.55f / S(), ImVec2(a.x + (side - ts.x) * 0.5f, a.y + (side - ts.y) * 0.5f), IM_COL32_WHITE, s);
}

enum Glyph { G_PLUS, G_GEAR, G_MORE, G_PLAY, G_SHARE, G_CLOCK };

void DrawGlyph(Glyph k, ImVec2 c, float s, ImU32 col) {   // s: the glyph's half size
    ImDrawList *d = DL();
    float t = fmaxf(1.5f, s * 0.16f);
    switch (k) {
    case G_PLUS:
        d->AddLine(ImVec2(c.x - s, c.y), ImVec2(c.x + s, c.y), col, t);
        d->AddLine(ImVec2(c.x, c.y - s), ImVec2(c.x, c.y + s), col, t);
        break;
    case G_GEAR:
        for (int i = 0; i < 8; i++) {
            float an = i * IM_PI / 4.0f;
            d->AddLine(ImVec2(c.x + cosf(an) * s * 0.62f, c.y + sinf(an) * s * 0.62f), ImVec2(c.x + cosf(an) * s, c.y + sinf(an) * s), col, t * 1.5f);
        }
        d->AddCircle(c, s * 0.62f, col, 32, t);
        d->AddCircle(c, s * 0.26f, col, 24, t);
        break;
    case G_MORE:
        for (int i = -1; i <= 1; i++) d->AddCircleFilled(ImVec2(c.x + i * s * 0.75f, c.y), s * 0.17f, col, 12);
        break;
    case G_PLAY:
        d->AddTriangleFilled(ImVec2(c.x - s * 0.6f, c.y - s * 0.8f), ImVec2(c.x + s * 0.8f, c.y), ImVec2(c.x - s * 0.6f, c.y + s * 0.8f), col);
        break;
    case G_SHARE:
        d->AddLine(ImVec2(c.x, c.y - s), ImVec2(c.x, c.y + s * 0.25f), col, t);
        d->AddLine(ImVec2(c.x - s * 0.4f, c.y - s * 0.6f), ImVec2(c.x, c.y - s), col, t);
        d->AddLine(ImVec2(c.x + s * 0.4f, c.y - s * 0.6f), ImVec2(c.x, c.y - s), col, t);
        d->PathLineTo(ImVec2(c.x - s * 0.35f, c.y - s * 0.3f));
        d->PathLineTo(ImVec2(c.x - s * 0.8f, c.y - s * 0.3f));
        d->PathLineTo(ImVec2(c.x - s * 0.8f, c.y + s));
        d->PathLineTo(ImVec2(c.x + s * 0.8f, c.y + s));
        d->PathLineTo(ImVec2(c.x + s * 0.8f, c.y - s * 0.3f));
        d->PathLineTo(ImVec2(c.x + s * 0.35f, c.y - s * 0.3f));
        d->PathStroke(col, 0, t);
        break;
    case G_CLOCK:
        d->PathArcTo(c, s * 0.85f, -IM_PI * 0.95f, IM_PI * 0.75f, 40);
        d->PathStroke(col, 0, t);
        d->AddTriangleFilled(ImVec2(c.x - s * 0.85f - s * 0.3f, c.y - s * 0.25f), ImVec2(c.x - s * 0.85f + s * 0.3f, c.y - s * 0.25f),
                             ImVec2(c.x - s * 0.85f, c.y + s * 0.15f), col);
        d->AddLine(c, ImVec2(c.x, c.y - s * 0.5f), col, t);
        d->AddLine(c, ImVec2(c.x + s * 0.35f, c.y + s * 0.2f), col, t);
        break;
    }
}

// A clickable area: hovered and held states for the drawing; true when clicked
bool Hit(const char *id, ImVec2 a, ImVec2 b, bool *hover = nullptr, bool *held = nullptr, bool under = false) {
    ImGui::SetCursorScreenPos(a);
    if (under) ImGui::SetNextItemAllowOverlap();
    bool c = ImGui::InvisibleButton(id, ImVec2(fmaxf(1.0f, b.x - a.x), fmaxf(1.0f, b.y - a.y)));
    if (hover != nullptr) *hover = ImGui::IsItemHovered();
    if (held != nullptr) *held = ImGui::IsItemActive();
    return c;
}

// A round glass button with a glyph
bool RoundButton(const char *id, ImVec2 c, float r, Glyph k, bool dark) {
    bool hv, hd;
    bool clicked = Hit(id, ImVec2(c.x - r, c.y - r), ImVec2(c.x + r, c.y + r), &hv, &hd);
    int a = hd ? 90 : hv ? 70 : 46;
    DL()->AddCircleFilled(c, r, dark ? IM_COL32(255, 255, 255, a) : IM_COL32(0, 0, 0, a / 2 + 6), 48);
    DrawGlyph(k, c, r * 0.42f, dark ? IM_COL32(255, 255, 255, 235) : IM_COL32(28, 28, 30, 235));
    return clicked;
}

// A capsule button filled with col (Play, the selected tab)
bool Pill(const char *id, ImVec2 a, ImVec2 b, ImU32 fill, ImU32 text, const char *label, ImFont *f, float size, int glyph = -1) {
    bool hv, hd;
    bool clicked = Hit(id, a, b, &hv, &hd);
    float r = (b.y - a.y) * 0.5f;
    DL()->AddRectFilled(a, b, fill, r);
    if (hv || hd) DL()->AddRectFilled(a, b, hd ? IM_COL32(0, 0, 0, 40) : IM_COL32(255, 255, 255, 26), r);
    ImVec2 ts = TextSize(f, size, label);
    float gw = glyph >= 0 ? ts.y * 0.9f + 10.0f * S() : 0.0f;
    float x = a.x + (b.x - a.x - ts.x - gw) * 0.5f, y = a.y + (b.y - a.y - ts.y) * 0.5f;
    if (glyph >= 0) DrawGlyph((Glyph)glyph, ImVec2(x + ts.y * 0.45f, a.y + (b.y - a.y) * 0.5f), ts.y * 0.36f, text);
    Text(f, size, ImVec2(x + gw, y), text, label);
    return clicked;
}

// ---- the questions, notes, progress and the pickers ----------------------------------------------------------------------------

struct Ask {
    std::string title, text, button;
    bool destructive = false, note = false;
    std::function<void()> then;
};
std::vector<Ask> sAsks;          // the open question (the first), and any queued after it
bool sAskOpened;
int sTestAlert = -1;              // the test hook's alert=: 1 ok, 0 cancel

void Note(const std::string &title, const std::string &text) {
    Ask a;
    a.title = title;
    a.text = text;
    a.note = true;
    sAsks.push_back(a);
}

void Question(const std::string &title, const std::string &text, const std::string &button, bool destructive, std::function<void()> then) {
    Ask a;
    a.title = title;
    a.text = text;
    a.button = button;
    a.destructive = destructive;
    a.then = then;
    sAsks.push_back(a);
}

// the work on a thread (an import's copy and extraction, a Play's extraction) and its card
struct Busy {
    SDL_Thread *thread = nullptr;
    SDL_atomic_t done;
    std::function<void()> work, after;
    std::function<float(std::string *)> progress;
    std::string text;
} sBusy;

int BusyThread(void *) {
    sBusy.work();
    SDL_AtomicSet(&sBusy.done, 1);
    return 0;
}

void StartBusy(const std::string &text, std::function<float(std::string *)> progress, std::function<void()> work, std::function<void()> after) {
    sBusy.text = text;
    sBusy.progress = progress;
    sBusy.work = work;
    sBusy.after = after;
    SDL_AtomicSet(&sBusy.done, 0);
    sBusy.thread = SDL_CreateThread(BusyThread, "bwport-launcher", nullptr);
    if (sBusy.thread == nullptr) {   // (no thread: on this one)
        work();
        after();
    }
}

void BusyPoll() {
    if (sBusy.thread != nullptr && SDL_AtomicGet(&sBusy.done)) {
        SDL_WaitThread(sBusy.thread, nullptr);
        sBusy.thread = nullptr;
        sBusy.after();
    }
}

enum Pick { P_NONE, P_ROM, P_SAVE, P_EXPORT, P_ART };
IGFD::FileDialog sDialog;
Pick sPick;
const LC_Game *sPickFor;

std::string Home() {
    const char *h = getenv("HOME");
    if (h == nullptr) h = getenv("USERPROFILE");
    return h != nullptr ? h : ".";
}

#ifdef _WIN32
// Windows: the system's own Open dialog (IFileOpenDialog; a folder picker for Export Save), run from the frame that asked
// (sNativePick, in DrawPicker) so ImGui is not half-way through a widget. BWPORT_PICKER=imgui keeps ImGuiFileDialog (the tests).
bool sNativePick;

std::wstring Wide(const char *u) {
    int n = MultiByteToWideChar(CP_UTF8, 0, u, -1, nullptr, 0);
    std::wstring w((size_t)(n > 0 ? n : 1), L'\0');
    if (n > 0) MultiByteToWideChar(CP_UTF8, 0, u, -1, &w[0], n);
    w.resize(wcslen(w.c_str()));
    return w;
}

std::string Utf8(const wchar_t *w) {
    int n = WideCharToMultiByte(CP_UTF8, 0, w, -1, nullptr, 0, nullptr, nullptr);
    std::string s((size_t)(n > 0 ? n : 1), '\0');
    if (n > 0) WideCharToMultiByte(CP_UTF8, 0, w, -1, &s[0], n, nullptr, nullptr);
    s.resize(strlen(s.c_str()));
    return s;
}

// "" when cancelled or when the dialog could not open
std::string NativePick(Pick p) {
    static const wchar_t *title[] = {L"", L"Add Game: choose the ROM", L"Import Save: choose the save", L"Export Save: choose a folder",
                                     L"Set Artwork: choose a picture"};
    static const COMDLG_FILTERSPEC rom[] = {{L"Nintendo DS ROMs (*.nds)", L"*.nds"}, {L"All files", L"*.*"}};
    static const COMDLG_FILTERSPEC sav[] = {{L"Saves (*.sav, *.dsv, *.bin)", L"*.sav;*.dsv;*.bin"}, {L"All files", L"*.*"}};
    static const COMDLG_FILTERSPEC pic[] = {{L"Pictures (*.jpg, *.png)", L"*.jpg;*.jpeg;*.png"}, {L"All files", L"*.*"}};
    std::string out;
    HRESULT init = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    IFileOpenDialog *d = nullptr;
    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, (void **)&d)) && d != nullptr) {
        DWORD opts = 0;
        d->GetOptions(&opts);
        d->SetOptions(opts | FOS_FORCEFILESYSTEM | (p == P_EXPORT ? FOS_PICKFOLDERS : FOS_FILEMUSTEXIST));
        d->SetTitle(title[p]);
        if (p == P_ROM) d->SetFileTypes(2, rom);
        else if (p == P_SAVE) d->SetFileTypes(2, sav);
        else if (p == P_ART) d->SetFileTypes(2, pic);
        HWND owner = nullptr;
        SDL_Window *win = SDL_GetKeyboardFocus();
        SDL_SysWMinfo wm;
        SDL_VERSION(&wm.version);
        if (win != nullptr && SDL_GetWindowWMInfo(win, &wm)) owner = wm.info.win.window;
        if (SUCCEEDED(d->Show(owner))) {
            IShellItem *it = nullptr;
            if (SUCCEEDED(d->GetResult(&it)) && it != nullptr) {
                PWSTR name = nullptr;
                if (SUCCEEDED(it->GetDisplayName(SIGDN_FILESYSPATH, &name)) && name != nullptr) {
                    out = Utf8(name);
                    CoTaskMemFree(name);
                }
                it->Release();
            }
        }
        d->Release();
    }
    if (SUCCEEDED(init)) CoUninitialize();
    for (char &c : out) {
        if (c == '\\') c = '/';
    }
    return out;
}
#endif

void OpenPicker(Pick p, const LC_Game *g) {
    static const char *title[] = {"", "Add Game: choose the ROM", "Import Save: choose the save", "Export Save: choose a folder",
                                  "Set Artwork: choose a picture"};
    static const char *filter[] = {nullptr, "ROMs{.nds},.*", "Saves{.sav,.dsv,.bin},.*", nullptr, "Pictures{.jpg,.jpeg,.png}"};
    IGFD::FileDialogConfig cfg;
    cfg.path = Home();
    cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_DontShowHiddenFiles | ImGuiFileDialogFlags_DisableCreateDirectoryButton;
    sPick = p;
    sPickFor = g;
#ifdef _WIN32
    static const bool imgui = getenv("BWPORT_PICKER") != nullptr && strcmp(getenv("BWPORT_PICKER"), "imgui") == 0;
    if (!imgui) {
        sNativePick = true;
        return;
    }
#endif
    sDialog.OpenDialog("launcherpick", title[p], filter[p], cfg);
}

// a file dropped on the window (SDL_DROPFILE, libntr-pc-launcher.patch): handled in the next frame
std::string sDropped;

// ---- the actions both layouts share --------------------------------------------------------------------------------------------

Game *Find(const LC_Game *g) {
    for (Game &x : sGames) {
        if (x.lc == g) return &x;
    }
    return nullptr;
}

void Enter(const Game &g) {
    fprintf(stderr, "bwport: launcher: play %s\n", g.lc->title);
    if (!PORT_EnterGame(g.lc)) {
        Note(g.title, "The game's folder couldn't be opened.");
        return;
    }
    sDone = true;
}

void Play(Game &g) {
    if (!g.playable) {
        Note(g.title, g.title + " is not supported yet. This build plays Pokemon Black.");
        return;
    }
    if (LC_HasFiles(g.lc)) {
        Enter(g);
        return;
    }
    if (!LC_HasRom(g.lc)) {
        Note(g.title, "The game's files are missing. Remove the game and add its ROM again.");
        return;
    }
    // the ROM copy is there, its files are not: extract them first
    char work[LC_PATH * 2];
    LC_GameWorkDir(g.lc, work, sizeof(work));
    std::string rom = RomPath(g.lc), dir = work;
    const LC_Game *lc = g.lc;
    static int files;
    StartBusy("Preparing game files...", [](std::string *) { return (float)PORT_ExtractPercent() / 100.0f; },
              [rom, dir]() {
                  MKDIR(dir.c_str());
                  files = LC_ExtractRom(rom.c_str(), dir.c_str());
              },
              [lc]() {
                  fprintf(stderr, "bwport: launcher: extracted %d files of %s\n", files, lc->title);
                  Game *x = Find(lc);
                  if (files > 0 && x != nullptr) Enter(*x);
                  else Note(lc->title, "The ROM's files couldn't be extracted (is the disk full?).");
              });
}

void ImportRom(const std::string &path) {
    fprintf(stderr, "bwport: launcher: import ROM %s\n", path.c_str());
    static char err[600];
    static const LC_Game *got;
    static int r;
    StartBusy("Copying the ROM...",
              [](std::string *t) {
                  if (LC_CopyPercent() < 100) return (float)LC_CopyPercent() / 100.0f;
                  *t = "Preparing game files...";
                  return (float)PORT_ExtractPercent() / 100.0f;
              },
              [path]() {
                  err[0] = 0;
                  got = nullptr;
                  r = LC_ImportRom(path.c_str(), &got, err, sizeof(err));
              },
              []() {
                  fprintf(stderr, "bwport: launcher: import ROM -> %d%s%s\n", r, err[0] ? ": " : "", err);
                  if (r != 0) {
                      Note("Couldn't add the game", err);
                      return;
                  }
                  Reload();
                  for (size_t i = 0; i < sGames.size(); i++) {
                      if (sGames[i].lc == got) sSelected = (int)i;
                  }
              });
}

void ExportSave(const Game &g, const std::string &dir) {
    char out[LC_PATH * 2], err[600] = "";
    if (LC_ExportSave(g.lc, dir.c_str(), out, sizeof(out), err, sizeof(err)) != 0) {
        Note("Export Save", err);
        return;
    }
    fprintf(stderr, "bwport: launcher: exported %s's save to %s\n", g.lc->title, out);
    Note("Export Save", std::string("The save is in ") + out + ".");
}

void ImportSave(const Game &g, const std::string &path) {
    long secs = -1;
    time_t when = 0;
    std::string what;
    if (LC_SaveFileInfo(g.lc, path.c_str(), &secs, &when) == 0) {
        what = "saved " + DateText(when) + ", " + PlayText(secs) + " played";
    } else {
        char err[600] = "";
        if (LC_CheckSaveFile(g.lc, path.c_str(), err, sizeof(err)) != LC_SAVE_NEW) {
            Note("Import Save", err);
            return;
        }
        what = "an empty save (a new game)";
    }
    const LC_Game *lc = g.lc;
    Question("Import this save?", g.title + "'s save is replaced by this one: " + what + ".\n\nThe current save is backed up first (Backups).",
             "Import", true, [lc, path]() {
                 char err[600] = "";
                 if (LC_ImportSave(lc, path.c_str(), err, sizeof(err)) != 0) Note("Import Save", err);
                 else fprintf(stderr, "bwport: launcher: imported a save into %s\n", lc->title);
                 Reload();
             });
}

struct BackupRow { std::string name, kind; time_t when; };
std::vector<BackupRow> sBackups;
const LC_Game *sBackupsOf;
bool sBackupsOpen;

void ListBackups(const LC_Game *g) {
    static LC_BackupInfo b[256];
    int n = LC_ListBackups(g, b, 256);
    sBackups.clear();
    for (int i = 0; i < n; i++) {
        BackupRow r;
        r.name = b[i].name;
        r.when = b[i].when;
        if (r.when == 0) {   // (save-before-launcher.bin: the file's date)
            struct stat st;
            if (stat((GameDir(g) + "/backups/" + r.name).c_str(), &st) == 0) r.when = st.st_mtime;
        }
        r.kind = b[i].kind == LC_BACKUP_BEFORE ? "Before a change" : b[i].kind == LC_BACKUP_TIER ? "Kept copy" : "Before the library";
        sBackups.push_back(r);
    }
}

void OpenBackups(const Game &g) {
    sBackupsOf = g.lc;
    ListBackups(g.lc);
    sBackupsOpen = true;
}

void Restore(const LC_Game *g, const BackupRow &b) {
    std::string name = b.name;
    Question("Restore this backup?", std::string(g->title) + "'s save goes back to the backup from " + DateText(b.when) +
             ". The current save is backed up first.", "Restore", true, [g, name]() {
                 char err[600] = "";
                 if (LC_RestoreBackup(g, name.c_str(), err, sizeof(err)) != 0) Note("Restore", err);
                 else fprintf(stderr, "bwport: launcher: restored %s of %s\n", name.c_str(), g->title);
                 ListBackups(g);
                 Reload();
             });
}

void Remove(const Game &g) {
    const LC_Game *lc = g.lc;
    Question("Remove " + g.title + "?", "The game's files are deleted. Its save and backups stay in its folder, and come back if you add "
             "the game again.", "Remove", true, [lc]() {
                 char err[600] = "";
                 if (LC_RemoveRom(lc, err, sizeof(err)) != 0) Note("Remove Game", err);
                 else fprintf(stderr, "bwport: launcher: removed %s\n", lc->title);
                 Reload();
             });
}

// A JPEG's EXIF orientation, 1 to 8 (1 when it has none). Phone cameras store a photo as the sensor saw it, with this tag
// saying how to turn it upright; stb_image ignores it.
static int ExifOrientation(const std::string &path) {
    static unsigned char b[65536];
    FILE *f = fopen(path.c_str(), "rb");
    if (f == nullptr) return 1;
    size_t n = fread(b, 1, sizeof(b), f);
    fclose(f);
    if (n < 4 || b[0] != 0xFF || b[1] != 0xD8) return 1;
    size_t p = 2;
    while (p + 4 <= n && b[p] == 0xFF) {
        unsigned m = b[p + 1];
        size_t len = (size_t)b[p + 2] << 8 | b[p + 3];
        if (m == 0xDA || len < 2) break;    // the image data starts: the APP segments are all before it
        if (m == 0xE1 && len >= 16 && p + 2 + len <= n && memcmp(&b[p + 4], "Exif\0\0", 6) == 0) {
            const unsigned char *t = &b[p + 10];    // the TIFF header: II (little-endian) or MM
            size_t tn = len - 8;
            bool le = t[0] == 'I';
            auto u16 = [&](size_t o) -> unsigned {
                return o + 2 > tn ? 0u : le ? (unsigned)(t[o] | t[o + 1] << 8) : (unsigned)(t[o] << 8 | t[o + 1]);
            };
            auto u32 = [&](size_t o) -> size_t { return o + 4 > tn ? 0 : le ? (size_t)u16(o) | (size_t)u16(o + 2) << 16 : (size_t)u16(o) << 16 | u16(o + 2); };
            size_t ifd = u32(4);
            unsigned count = u16(ifd);
            for (unsigned i = 0; i < count; i++) {
                size_t e = ifd + 2 + (size_t)i * 12;
                if (u16(e) == 0x0112) {
                    unsigned v = u16(e + 8);
                    return v >= 1 && v <= 8 ? (int)v : 1;
                }
            }
            return 1;
        }
        p += 2 + len;
    }
    return 1;
}

// The pixels (3 bytes each) turned upright for EXIF orientation o; w and h become the upright size
static std::vector<unsigned char> Upright(const unsigned char *d, int &w, int &h, int o) {
    int W = w, H = h, ow = o >= 5 ? H : W, oh = o >= 5 ? W : H;
    std::vector<unsigned char> r((size_t)ow * oh * 3);
    for (int y = 0; y < oh; y++) {
        for (int x = 0; x < ow; x++) {
            int sx = x, sy = y;
            switch (o) {
            case 2: sx = W - 1 - x; break;
            case 3: sx = W - 1 - x; sy = H - 1 - y; break;
            case 4: sy = H - 1 - y; break;
            case 5: sx = y; sy = x; break;
            case 6: sx = y; sy = H - 1 - x; break;
            case 7: sx = W - 1 - y; sy = H - 1 - x; break;
            case 8: sx = W - 1 - y; sy = x; break;
            }
            memcpy(&r[((size_t)y * ow + x) * 3], &d[((size_t)sy * W + sx) * 3], 3);
        }
    }
    w = ow;
    h = oh;
    return r;
}

// The picture as artwork.jpg in the game's folder: upright, its long side at most 1500 px (a box filter), JPEG quality 85
void SetArtwork(const Game &g, const std::string &path) {
    int w = 0, h = 0, n = 0;
    unsigned char *raw = stbi_load(path.c_str(), &w, &h, &n, 3);
    if (raw == nullptr) {
        Note("Set Artwork", "That file isn't a picture this program can read (JPEG or PNG).");
        return;
    }
    int orient = ExifOrientation(path);
    std::vector<unsigned char> up;
    if (orient != 1) {
        up = Upright(raw, w, h, orient);
        stbi_image_free(raw);
        raw = nullptr;
    }
    const unsigned char *d = raw != nullptr ? raw : up.data();
    int ow = w, oh = h;
    int lng = w > h ? w : h;
    if (lng > 1500) {
        ow = (int)((long)w * 1500 / lng);
        oh = (int)((long)h * 1500 / lng);
        if (ow < 1) ow = 1;
        if (oh < 1) oh = 1;
    }
    std::vector<unsigned char> out((size_t)ow * oh * 3);
    for (int y = 0; y < oh; y++) {
        int y0 = (int)((long)y * h / oh), y1 = (int)((long)(y + 1) * h / oh);
        if (y1 <= y0) y1 = y0 + 1;
        for (int x = 0; x < ow; x++) {
            int x0 = (int)((long)x * w / ow), x1 = (int)((long)(x + 1) * w / ow);
            if (x1 <= x0) x1 = x0 + 1;
            unsigned sum[3] = {0, 0, 0}, cnt = 0;
            for (int sy = y0; sy < y1; sy++) {
                for (int sx = x0; sx < x1; sx++) {
                    const unsigned char *p = &d[((size_t)sy * w + sx) * 3];
                    sum[0] += p[0];
                    sum[1] += p[1];
                    sum[2] += p[2];
                    cnt++;
                }
            }
            for (int k = 0; k < 3; k++) out[((size_t)y * ow + x) * 3 + k] = (unsigned char)(sum[k] / cnt);
        }
    }
    stbi_image_free(raw);
    std::string dst = ArtworkPath(g.lc), tmp = dst + ".tmp";
    bool ok = stbi_write_jpg(tmp.c_str(), ow, oh, 3, out.data(), 85) != 0 && rename(tmp.c_str(), dst.c_str()) == 0;
    struct stat st;
    fprintf(stderr, "bwport: launcher: artwork for %s: %dx%d from %dx%d (EXIF orientation %d), %lld bytes%s\n", g.lc->title, ow, oh, w, h, orient,
            ok && stat(dst.c_str(), &st) == 0 ? (long long)st.st_size : 0LL, ok ? "" : ": not written");
    if (!ok) {
        unlink(tmp.c_str());
        Note("Set Artwork", "The picture couldn't be saved (is the disk full?).");
    }
    Reload();
}

void RemoveArtwork(const Game &g) {
    unlink(ArtworkPath(g.lc).c_str());
    fprintf(stderr, "bwport: launcher: artwork of %s removed\n", g.lc->title);
    Reload();
}

// ---- cover downloads: LC_FetchCover on a thread each; the launcher reloads when one ends ---------------------------------------

struct CoverJob {
    const LC_Game *g;
    bool manual;   // Download Cover Again: a failure is told; the automatic ones are only logged
    int r;
    char err[400];
};
SDL_mutex *sJobLock;
std::vector<CoverJob *> sJobsDone;   // under sJobLock
std::vector<const LC_Game *> sFetching;   // the main thread's: the games with a job running

bool Fetching(const LC_Game *g) {
    for (const LC_Game *x : sFetching) {
        if (x == g) return true;
    }
    return false;
}

int CoverThread(void *p) {
    CoverJob *j = (CoverJob *)p;
    j->err[0] = 0;
    j->r = LC_FetchCover(j->g, j->err, sizeof j->err);
    SDL_LockMutex(sJobLock);
    sJobsDone.push_back(j);
    SDL_UnlockMutex(sJobLock);
    return 0;
}

void FetchCover(const LC_Game *g, bool manual) {
    if (Fetching(g)) return;
    if (sJobLock == nullptr) sJobLock = SDL_CreateMutex();
    CoverJob *j = new CoverJob{g, manual, 0, ""};
    SDL_Thread *t = SDL_CreateThread(CoverThread, "bwport-cover", j);
    if (t == nullptr) {
        delete j;
        return;
    }
    SDL_DetachThread(t);
    sFetching.push_back(g);
}

void CoverPoll() {
    if (sJobLock == nullptr) return;
    std::vector<CoverJob *> done;
    SDL_LockMutex(sJobLock);
    done.swap(sJobsDone);
    SDL_UnlockMutex(sJobLock);
    if (done.empty()) return;
    for (CoverJob *j : done) {
        for (size_t i = 0; i < sFetching.size(); i++) {
            if (sFetching[i] == j->g) sFetching.erase(sFetching.begin() + (long)i);
        }
        if (j->manual && j->r != 0) Note("Download Cover Again", std::string("The cover couldn't be downloaded: ") + j->err);
        delete j;
    }
    Reload();
}

// ---- menus ---------------------------------------------------------------------------------------------------------------------

int sOpenMenu;          // the menu to open at Menus: 1 the game's, 2 the global one, 3 the saves, 4 the artwork and removal
ImVec2 sMenuAt, sMenuPivot;   // where it opens: its corner at the button that opened it
bool sCloseMenus;      // the test hook's menu=close
const LC_Game *sMenuFor;

void OpenMenu(int which, ImVec2 at, ImVec2 pivot) {
    sOpenMenu = which;
    sMenuAt = at;
    sMenuPivot = pivot;
}

void PushMenuStyle(bool dark) {
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 12.0f * S());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f * S(), 8.0f * S()));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12.0f * S(), 9.0f * S()));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f * S(), 6.0f * S()));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, dark ? ImVec4(0.13f, 0.13f, 0.15f, 0.98f) : ImVec4(0.97f, 0.97f, 0.98f, 0.98f));
    ImGui::PushStyleColor(ImGuiCol_Text, dark ? ImVec4(0.95f, 0.95f, 0.97f, 1) : ImVec4(0.11f, 0.11f, 0.12f, 1));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, dark ? ImVec4(0.55f, 0.55f, 0.6f, 1) : ImVec4(0.55f, 0.55f, 0.58f, 1));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, dark ? ImVec4(1, 1, 1, 0.10f) : ImVec4(0, 0, 0, 0.07f));
    ImGui::PushStyleColor(ImGuiCol_Border, dark ? ImVec4(1, 1, 1, 0.10f) : ImVec4(0, 0, 0, 0.10f));
    ImGui::PushStyleColor(ImGuiCol_Separator, dark ? ImVec4(1, 1, 1, 0.10f) : ImVec4(0, 0, 0, 0.10f));
    ImGui::PushFont(sFont, 16.0f * sK);
}
void PopMenuStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(6);
    ImGui::PopStyleVar(4);
}

// The secondary actions of a game (a card's ... menu; the tabs' Save and More): which=0 all, 1 the saves, 2 the artwork and removal
void GameMenuItems(Game &g, int which) {
    if (which != 2) {
        if (ImGui::MenuItem("Export Save...", nullptr, false, g.hasSave)) OpenPicker(P_EXPORT, g.lc);
        if (ImGui::MenuItem("Import Save...")) OpenPicker(P_SAVE, g.lc);
        if (ImGui::MenuItem("Backups")) OpenBackups(g);
    }
    if (which == 0) ImGui::Separator();
    if (which != 1) {
        if (ImGui::MenuItem("Set Artwork...")) OpenPicker(P_ART, g.lc);
        if (g.art.d != nullptr && ImGui::MenuItem("Remove Artwork")) RemoveArtwork(g);
        if (ImGui::MenuItem("Download Cover Again", nullptr, false, LC_CoverDownloads() != 0 && !Fetching(g.lc))) FetchCover(g.lc, true);
        if (!g.coverSource.empty() && ImGui::MenuItem(("Cover art: " + g.coverSource + "...").c_str())) SDL_OpenURL(g.coverLink.c_str());
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.27f, 0.23f, 1));
        if (ImGui::MenuItem("Remove Game...")) Remove(g);
        ImGui::PopStyleColor();
    }
}

// The launcher's own menu: the settings windows, what a start shows, the layout
void GlobalMenuItems() {
    if (ImGui::BeginMenu("Settings")) {
        const char *w[] = {"Video...", "Audio...", "Controls...", "View..."};
        for (int i = 0; i < 4; i++) {
            if (ImGui::MenuItem(w[i])) SIM_GuiOpenSettings(i);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("On Start, Open")) {
        bool last = LC_StartMode() == LC_START_LAST;
        if (ImGui::MenuItem("The Last Game", nullptr, last)) LC_SetStartMode(LC_START_LAST);
        if (ImGui::MenuItem("This Screen", nullptr, !last)) LC_SetStartMode(LC_START_LAUNCHER);
        ImGui::EndMenu();
    }
    bool covers = LC_CoverDownloads() != 0;
    if (ImGui::MenuItem("Download Artwork", nullptr, covers)) {   // (on: the covers missing are fetched now)
        LC_SetCoverDownloads(!covers);
        Reload();
    }
    bool skip = LC_SkipMenu() != 0;   // (skip_menu.c: a start goes into the save as CONTINUE does)
    if (ImGui::MenuItem("Skip Title and Menu", nullptr, skip)) LC_SetSkipMenu(!skip);
    if (ImGui::BeginMenu("Layout")) {
        if (ImGui::MenuItem("Library", nullptr, sLayout == LC_LAYOUT_LIBRARY)) { sLayout = LC_LAYOUT_LIBRARY; LC_SetLayout(sLayout); }
        if (ImGui::MenuItem("Tabs", nullptr, sLayout == LC_LAYOUT_TABS)) { sLayout = LC_LAYOUT_TABS; LC_SetLayout(sLayout); }
        ImGui::EndMenu();
    }
}

void Menus(bool dark) {
    const char *names[] = {"", "##gamemenu", "##globalmenu", "##savemenu", "##moremenu"};
    int open = sOpenMenu > 0 && sOpenMenu < 5 ? sOpenMenu : 0;
    if (open) ImGui::OpenPopup(names[open]);
    sOpenMenu = 0;
    PushMenuStyle(dark);
    // (the position only for the one opened now: it is the next Begin's, and a closed popup has none)
    auto at = [open](int k) { if (open == k) ImGui::SetNextWindowPos(sMenuAt, ImGuiCond_Always, sMenuPivot); };
    at(1);
    if (ImGui::BeginPopup("##gamemenu")) {
        if (sCloseMenus) ImGui::CloseCurrentPopup();
        Game *g = Find(sMenuFor);
        if (g != nullptr) {
            ImGui::TextDisabled("%s", g->title.c_str());
            GameMenuItems(*g, 0);
        }
        ImGui::EndPopup();
    }
    at(3);
    if (ImGui::BeginPopup("##savemenu")) {
        if (sCloseMenus) ImGui::CloseCurrentPopup();
        Game *g = Find(sMenuFor);
        if (g != nullptr) GameMenuItems(*g, 1);
        ImGui::EndPopup();
    }
    at(4);
    if (ImGui::BeginPopup("##moremenu")) {
        if (sCloseMenus) ImGui::CloseCurrentPopup();
        Game *g = Find(sMenuFor);
        if (g != nullptr) GameMenuItems(*g, 2);
        ImGui::EndPopup();
    }
    at(2);
    if (ImGui::BeginPopup("##globalmenu")) {
        if (sCloseMenus) ImGui::CloseCurrentPopup();
        GlobalMenuItems();
        ImGui::EndPopup();
    }
    sCloseMenus = false;
    PopMenuStyle();
}

// The top right: Add Game and the gear
void TopButtons(ImVec2 at, float r, bool dark) {   // at: the right edge's centre
    if (RoundButton("##add", ImVec2(at.x - r * 3.4f, at.y), r, G_PLUS, dark)) OpenPicker(P_ROM, nullptr);
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Game");
    if (RoundButton("##gear", ImVec2(at.x - r, at.y), r, G_GEAR, dark)) OpenMenu(2, ImVec2(at.x, at.y + r + 6.0f * S()), ImVec2(1, 0));
}

// ---- the empty state -----------------------------------------------------------------------------------------------------------

void Empty(ImVec2 a, ImVec2 b, bool dark) {
    ImU32 fg = dark ? IM_COL32(245, 245, 247, 255) : IM_COL32(28, 28, 30, 255), dim = dark ? IM_COL32(160, 160, 168, 255) : IM_COL32(110, 110, 115, 255);
    float cx = (a.x + b.x) * 0.5f, cy = (a.y + b.y) * 0.5f;
    const char *t1 = "No games yet", *t2 = "Add your own Pokemon Black ROM (USA / Europe) to start.";
    ImVec2 s1 = TextSize(sBold, 26.0f, t1), s2 = TextSize(sFont, 15.0f, t2);
    Text(sBold, 26.0f, ImVec2(cx - s1.x * 0.5f, cy - s1.y - 40.0f * S()), fg, t1);
    Text(sFont, 15.0f, ImVec2(cx - s2.x * 0.5f, cy - 30.0f * S()), dim, t2);
    float w = 200.0f * S(), h = 46.0f * S();
    if (Pill("##addempty", ImVec2(cx - w * 0.5f, cy + 4.0f * S()), ImVec2(cx + w * 0.5f, cy + 4.0f * S() + h), U32(Hex(0x3d8bff)),
             IM_COL32_WHITE, "Add Game...", sBold, 17.0f, G_PLUS)) {
        OpenPicker(P_ROM, nullptr);
    }
}

// ---- layout (a): the library ---------------------------------------------------------------------------------------------------

void Library(ImVec2 a, ImVec2 b) {
    const float s = S();
    DL()->AddRectFilled(a, b, IM_COL32(10, 10, 12, 255));
    float pad = 28.0f * s, top = a.y + 18.0f * s, r = 19.0f * s;
    const char *title = "Games";
    Text(sBold, 28.0f, ImVec2(a.x + pad, top), IM_COL32(245, 245, 247, 255), title);
    TopButtons(ImVec2(b.x - pad, top + 17.0f * s), r, true);
    float y0 = top + 64.0f * s;
    if (sGames.empty()) {
        Empty(ImVec2(a.x, y0), b, true);
        return;
    }
    // the cards: as many columns as fit at 200-280 points each; portrait cards (4:5); "Add Game" last
    float gap = 20.0f * s, avail = b.x - a.x - pad * 2;
    int cols = (int)((avail + gap) / (220.0f * s + gap));
    if (cols < 1) cols = 1;
    int count = (int)sGames.size() + 1;
    if (cols > count) cols = count;
    float cw = (avail - gap * (cols - 1)) / cols;
    if (cw > 300.0f * s) cw = 300.0f * s;
    float ch = cw * 1.25f;
    float x0 = a.x + pad;
    ImGui::SetCursorScreenPos(ImVec2(a.x, y0));
    ImGui::BeginChild("##cards", ImVec2(b.x - a.x, b.y - y0), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
    ImVec2 o = ImGui::GetCursorScreenPos();
    float round = 22.0f * s;
    for (int i = 0; i < count; i++) {
        int row = i / cols, col = i % cols;
        ImVec2 p(x0 + col * (cw + gap), o.y + row * (ch + gap));
        ImVec2 q(p.x + cw, p.y + ch);
        ImGui::PushID(i);
        if (i == count - 1) {   // Add Game
            bool hv;
            if (Hit("##addcard", p, q, &hv)) OpenPicker(P_ROM, nullptr);
            DL()->AddRect(p, q, IM_COL32(255, 255, 255, hv ? 90 : 50), round, 0, 1.5f * s);
            ImVec2 c((p.x + q.x) * 0.5f, (p.y + q.y) * 0.5f - 14.0f * s);
            DL()->AddCircleFilled(c, 26.0f * s, IM_COL32(255, 255, 255, hv ? 46 : 30), 48);
            DrawGlyph(G_PLUS, c, 11.0f * s, IM_COL32(255, 255, 255, 230));
            ImVec2 ts = TextSize(sBold, 16.0f, "Add Game");
            Text(sBold, 16.0f, ImVec2(c.x - ts.x * 0.5f, c.y + 38.0f * s), IM_COL32(235, 235, 240, 255), "Add Game");
            ImGui::PopID();
            continue;
        }
        Game &g = sGames[(size_t)i];
        bool hv, hd;
        bool clicked = Hit("##card", p, q, &hv, &hd, true);
        Showcase(g, p, q, round, 84.0f * s);
        if (hv) DL()->AddRect(p, q, U32(g.accent, 0.9f), round, 0, 2.0f * s);
        Icon(g, ImVec2(p.x + 14.0f * s, p.y + 14.0f * s), 36.0f * s);
        // the info panel at the bottom: glass in the game's theme
        float ph = 74.0f * s;
        ImVec2 pa(p.x + 10.0f * s, q.y - ph - 10.0f * s), pb(q.x - 10.0f * s, q.y - 10.0f * s);
        DL()->AddRectFilled(pa, pb, g.dark ? IM_COL32(40, 42, 50, 200) : IM_COL32(255, 255, 255, 215), round - 8.0f * s);
        DL()->AddRect(pa, pb, g.dark ? IM_COL32(255, 255, 255, 30) : IM_COL32(0, 0, 0, 18), round - 8.0f * s);
        ImU32 fg = g.dark ? IM_COL32(255, 255, 255, 255) : IM_COL32(20, 20, 22, 255);
        ImU32 dim = g.dark ? IM_COL32(200, 200, 210, 220) : IM_COL32(90, 90, 96, 255);
        Text(sBold, 20.0f, ImVec2(pa.x + 12.0f * s, pa.y + 10.0f * s), fg, g.shortTitle.c_str());
        std::string sum = Summary(g);
        DL()->PushClipRect(pa, pb, true);
        Text(sFont, 13.0f, ImVec2(pa.x + 12.0f * s, pa.y + 38.0f * s), dim, sum.c_str(), pb.x - pa.x - 24.0f * s);
        DL()->PopClipRect();
        // the ... button (over the card: its own click)
        ImVec2 mc(q.x - 28.0f * s, p.y + 32.0f * s);
        if (RoundButton("##more", mc, 16.0f * s, G_MORE, g.dark)) {
            sMenuFor = g.lc;
            OpenMenu(1, ImVec2(mc.x + 16.0f * s, mc.y + 22.0f * s), ImVec2(1, 0));
        } else if (clicked) {
            Play(g);
        }
        ImGui::PopID();
    }
    int rows = (count + cols - 1) / cols;
    ImGui::SetCursorScreenPos(ImVec2(o.x, o.y + rows * (ch + gap)));
    ImGui::Dummy(ImVec2(1, pad));
    ImGui::EndChild();
    Menus(true);
}

// ---- layout (b): the tabs ------------------------------------------------------------------------------------------------------

void Tabs(ImVec2 a, ImVec2 b) {
    const float s = S();
    if (sGames.empty()) {
        DL()->AddRectFilled(a, b, IM_COL32(10, 10, 12, 255));
        TopButtons(ImVec2(b.x - 24.0f * s, a.y + 36.0f * s), 19.0f * s, true);
        Empty(a, b, true);
        Menus(true);
        return;
    }
    if (sSelected >= (int)sGames.size()) sSelected = 0;
    Game &g = sGames[(size_t)sSelected];
    bool dark = g.dark;
    // the page in the game's theme: its colours from the top, into its deep colour
    Col top = dark ? Mix(g.deep, g.glow, 0.38f) : Mix(g.deep, g.glow, 0.30f);
    DL()->AddRectFilledMultiColor(a, ImVec2(b.x, a.y + (b.y - a.y) * 0.55f), U32(top), U32(top), U32(g.deep), U32(g.deep));
    DL()->AddRectFilled(ImVec2(a.x, a.y + (b.y - a.y) * 0.55f - 1.0f), b, U32(g.deep));
    ImU32 fg = dark ? IM_COL32(255, 255, 255, 255) : IM_COL32(20, 20, 22, 255);
    ImU32 dim = dark ? IM_COL32(200, 200, 210, 230) : IM_COL32(80, 80, 86, 255);
    ImU32 dim2 = dark ? IM_COL32(150, 150, 160, 220) : IM_COL32(120, 120, 126, 255);
    float pad = 24.0f * s, r = 19.0f * s;
    float ty = a.y + 16.0f * s, th = 38.0f * s;
    // the tab row: the selected one filled with its accent, the others glass with a dot of theirs; it fades out under the buttons
    float x = a.x + pad, limit = b.x - pad - r * 5.0f;
    DL()->PushClipRect(ImVec2(a.x, a.y), ImVec2(limit, b.y), true);
    for (size_t i = 0; i < sGames.size(); i++) {
        Game &t = sGames[i];
        bool on = (int)i == sSelected;
        ImVec2 ts = TextSize(on ? sBold : sFont, 15.0f, t.shortTitle.c_str());
        float w = ts.x + 30.0f * s + 26.0f * s;
        ImVec2 p(x, ty), q(x + w, ty + th);
        ImGui::PushID((int)i);
        bool hv;
        if (Hit("##tab", p, q, &hv)) sSelected = (int)i;
        ImGui::PopID();
        DL()->AddRectFilled(p, q, on ? U32(t.accent) : dark ? IM_COL32(255, 255, 255, hv ? 44 : 28) : IM_COL32(0, 0, 0, hv ? 26 : 14), th * 0.5f);
        float ix = p.x + 12.0f * s, iy = p.y + (th - 22.0f * s) * 0.5f;
        if (t.icon.d != nullptr) DL()->AddImageRounded(t.icon.d->GetTexRef(), ImVec2(ix, iy), ImVec2(ix + 22.0f * s, iy + 22.0f * s), ImVec2(0, 0),
                                                      ImVec2(1, 1), IM_COL32_WHITE, 5.0f * s);
        else DL()->AddCircleFilled(ImVec2(ix + 11.0f * s, iy + 11.0f * s), 5.0f * s, on ? IM_COL32_WHITE : U32(t.accent), 16);
        Text(on ? sBold : sFont, 15.0f, ImVec2(ix + 30.0f * s, p.y + (th - ts.y) * 0.5f), on ? IM_COL32_WHITE : fg, t.shortTitle.c_str());
        x += w + 10.0f * s;
    }
    DL()->PopClipRect();
    if (x > limit) {   // the row's fade under the buttons
        Col bg = top;
        DL()->AddRectFilledMultiColor(ImVec2(limit - 40.0f * s, ty), ImVec2(limit, ty + th), U32(bg, 0.0f), U32(bg, 1.0f), U32(bg, 1.0f), U32(bg, 0.0f));
    }
    TopButtons(ImVec2(b.x - pad, ty + th * 0.5f), r, dark);

    // the page: wide, the showcase on the left and the info column on the right; narrow, the showcase over the info
    float y0 = ty + th + 22.0f * s;
    float W = b.x - a.x - pad * 2, H = b.y - y0 - pad;
    bool wide = W > H * 1.15f;
    ImVec2 sa, sb, ia;
    float iw;
    if (wide) {
        float sw = W * 0.56f;
        sa = ImVec2(a.x + pad, y0);
        sb = ImVec2(sa.x + sw, y0 + H);
        ia = ImVec2(sb.x + 32.0f * s, y0 + fmaxf(0.0f, (H - 300.0f * s) * 0.5f));
        iw = b.x - pad - ia.x;
    } else {
        float sh = fmaxf(120.0f * s, H - 300.0f * s);
        sa = ImVec2(a.x + pad, y0);
        sb = ImVec2(b.x - pad, y0 + sh);
        ia = ImVec2(a.x + pad, sb.y + 22.0f * s);
        iw = W;
    }
    Showcase(g, sa, sb, 24.0f * s, 0.0f, true);
    // the info column: icon and title, the save, the edition, Play / Continue, then Save, Backups, More
    float iy = ia.y;
    Icon(g, ImVec2(ia.x, iy), 48.0f * s);
    Text(sBold, 34.0f, ImVec2(ia.x + 62.0f * s, iy + 3.0f * s), fg, g.shortTitle.c_str());
    iy += 62.0f * s;
    std::string sum = Summary(g);
    Text(sFont, 16.0f, ImVec2(ia.x, iy), dim, sum.c_str(), iw);
    iy += 26.0f * s;
    Text(sFont, 14.0f, ImVec2(ia.x, iy), dim2, g.playable ? "USA / Europe  \xc2\xb7  NDSi Enhanced" : "Not supported yet");
    iy += 36.0f * s;
    float bw = fminf(iw, 420.0f * s);
    if (Pill("##play", ImVec2(ia.x, iy), ImVec2(ia.x + bw, iy + 54.0f * s), U32(g.accent), IM_COL32_WHITE, g.hasSave ? "Continue" : "Play",
             sBold, 20.0f, G_PLAY)) {
        Play(g);
    }
    iy += 54.0f * s + 14.0f * s;
    float sq = (bw - 2 * 12.0f * s) / 3.0f, sh2 = 64.0f * s;
    const char *lab[3] = {"Save", "Backups", "More"};
    Glyph gl[3] = {G_SHARE, G_CLOCK, G_MORE};
    for (int k = 0; k < 3; k++) {
        ImVec2 p(ia.x + k * (sq + 12.0f * s), iy), q(p.x + sq, iy + sh2);
        bool hv, hd;
        ImGui::PushID(k);
        bool c = Hit("##sec", p, q, &hv, &hd);
        ImGui::PopID();
        DL()->AddRectFilled(p, q, dark ? IM_COL32(255, 255, 255, hd ? 50 : hv ? 38 : 26) : IM_COL32(0, 0, 0, hd ? 34 : hv ? 24 : 14), 14.0f * s);
        DrawGlyph(gl[k], ImVec2((p.x + q.x) * 0.5f, p.y + 24.0f * s), 9.0f * s, fg);
        ImVec2 ts = TextSize(sFont, 13.0f, lab[k]);
        Text(sFont, 13.0f, ImVec2((p.x + q.x - ts.x) * 0.5f, q.y - ts.y - 9.0f * s), fg, lab[k]);
        if (c) {
            sMenuFor = g.lc;
            if (k == 0) OpenMenu(3, ImVec2(p.x, q.y + 6.0f * s), ImVec2(0, 0));
            else if (k == 1) OpenBackups(g);
            else OpenMenu(4, ImVec2(q.x, q.y + 6.0f * s), ImVec2(1, 0));
        }
    }
    Menus(dark);
}

// ---- the dialogs ---------------------------------------------------------------------------------------------------------------

void PushDialogStyle() {
    const float s = S();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f * s);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(22.0f * s, 20.0f * s));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f * s);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14.0f * s, 8.0f * s));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f * s, 10.0f * s));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.12f, 0.12f, 0.14f, 0.99f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.08f));
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0, 0, 0, 0.45f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 1, 1, 0.10f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.16f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.22f));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.24f, 0.55f, 1.0f, 1));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1, 1, 1, 0.08f));
    ImGui::PushFont(sFont, 15.0f * sK);
}
void PopDialogStyle() {
    ImGui::PopFont();
    ImGui::PopStyleColor(8);
    ImGui::PopStyleVar(5);
}

bool DialogButton(const char *label, bool primary, bool destructive, float w) {
    if (primary) {
        ImVec4 c = destructive ? ImVec4(0.92f, 0.26f, 0.22f, 1) : ImVec4(0.24f, 0.55f, 1.0f, 1);
        ImGui::PushStyleColor(ImGuiCol_Button, c);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(c.x * 1.1f, c.y * 1.1f, c.z * 1.1f, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(c.x * 0.9f, c.y * 0.9f, c.z * 0.9f, 1));
    }
    bool r = ImGui::Button(label, ImVec2(w, 0));
    if (primary) ImGui::PopStyleColor(3);
    return r;
}

void Dialogs() {
    const float s = S();
    ImGuiViewport *vp = ImGui::GetMainViewport();
    ImVec2 mid(vp->Pos.x + vp->Size.x * 0.5f, vp->Pos.y + vp->Size.y * 0.5f);
    PushDialogStyle();
    // the busy card: an import's copy and extraction, a Play's extraction
    if (sBusy.thread != nullptr) {
        ImGui::OpenPopup("##busy");
        ImGui::SetNextWindowPos(mid, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(340.0f * s, 0));
        if (ImGui::BeginPopupModal("##busy", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            std::string t;
            float f = sBusy.progress(&t);
            if (!t.empty()) sBusy.text = t;
            ImGui::PushFont(sBold, 17.0f * sK);
            ImGui::TextUnformatted(sBusy.text.c_str());
            ImGui::PopFont();
            ImGui::ProgressBar(f, ImVec2(-1, 8.0f * s), "");
            ImGui::EndPopup();
        }
    } else if (!sAsks.empty()) {
        Ask &q = sAsks.front();
        if (!sAskOpened) {
            ImGui::OpenPopup("##ask");
            sAskOpened = true;
        }
        ImGui::SetNextWindowPos(mid, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(fminf(420.0f * s, vp->Size.x - 40.0f), 0));
        if (ImGui::BeginPopupModal("##ask", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            ImGui::PushFont(sBold, 18.0f * sK);
            ImGui::TextWrapped("%s", q.title.c_str());
            ImGui::PopFont();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.80f, 0.84f, 1));
            ImGui::TextWrapped("%s", q.text.c_str());
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(1, 4.0f * s));
            float w = (ImGui::GetContentRegionAvail().x - 10.0f * s) * 0.5f;
            int answer = -1;
            if (q.note) {
                if (DialogButton("OK", true, false, -1) || ImGui::IsKeyPressed(ImGuiKey_Enter) || sTestAlert >= 0) answer = 0;
            } else {
                if (DialogButton("Cancel", false, false, w) || ImGui::IsKeyPressed(ImGuiKey_Escape) || sTestAlert == 0) answer = 0;
                ImGui::SameLine();
                if (DialogButton(q.button.c_str(), true, q.destructive, w) || sTestAlert == 1) answer = 1;
            }
            if (answer >= 0) {
                std::function<void()> then = answer == 1 ? q.then : nullptr;
                sTestAlert = -1;
                ImGui::CloseCurrentPopup();
                sAsks.erase(sAsks.begin());
                sAskOpened = false;
                if (then) then();
            }
            ImGui::EndPopup();
        }
    } else if (sBackupsOpen) {
        ImGui::OpenPopup("##backups");
        ImGui::SetNextWindowPos(mid, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(fminf(520.0f * s, vp->Size.x - 40.0f), fminf(480.0f * s, vp->Size.y - 60.0f)));
        if (ImGui::BeginPopupModal("##backups", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            ImGui::PushFont(sBold, 20.0f * sK);
            ImGui::TextUnformatted("Backups");
            ImGui::PopFont();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.7f, 1));
            ImGui::TextWrapped("A copy before every import, restore or removal (the last 20), one a day for a week, one a week for a month, "
                               "one a month for a year.");
            ImGui::PopStyleColor();
            float rowsH = ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing() - 6.0f * s;
            ImGui::BeginChild("##rows", ImVec2(0, rowsH), ImGuiChildFlags_Borders);
            if (sBackups.empty()) ImGui::TextDisabled("No backups yet.");
            for (size_t i = 0; i < sBackups.size(); i++) {
                ImGui::PushID((int)i);
                float right = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 90.0f * s;
                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted(DateText(sBackups[i].when).c_str());
                ImGui::SameLine();
                ImGui::TextDisabled("%s", sBackups[i].kind.c_str());
                ImGui::SameLine(right);
                if (ImGui::Button("Restore", ImVec2(90.0f * s, 0))) Restore(sBackupsOf, sBackups[i]);
                ImGui::PopID();
            }
            ImGui::EndChild();
            if (DialogButton("Done", true, false, -1) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                sBackupsOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    PopDialogStyle();
    // the picker
    ImVec2 mn(fminf(720.0f * s, vp->Size.x - 40.0f), fminf(460.0f * s, vp->Size.y - 60.0f));
    ImGui::SetNextWindowPos(mid, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (!sDropped.empty() && sBusy.thread == nullptr) {
        std::string path = sDropped;
        sDropped.clear();
        size_t n = path.size();
        bool nds = n > 4 && path[n - 4] == '.' && (path[n - 3] | 32) == 'n' && (path[n - 2] | 32) == 'd' && (path[n - 1] | 32) == 's';
        fprintf(stderr, "bwport: launcher: dropped %s\n", path.c_str());
        if (nds) ImportRom(path);
        else Note("Add Game", "Only a Nintendo DS ROM (.nds) can be dropped here.");
    }
    auto picked = [](Pick p, const LC_Game *pf, const std::string &path) {
        Game *g = Find(pf);
        fprintf(stderr, "bwport: launcher: picked %s\n", path.c_str());
        if (p == P_ROM) ImportRom(path);
        else if (g != nullptr && p == P_SAVE) ImportSave(*g, path);
        else if (g != nullptr && p == P_EXPORT) ExportSave(*g, path);
        else if (g != nullptr && p == P_ART) SetArtwork(*g, path);
    };
#ifdef _WIN32
    if (sNativePick) {
        Pick p = sPick;
        sNativePick = false;
        sPick = P_NONE;
        std::string path = NativePick(p);
        ImGui::GetIO().ClearInputKeys();    // (the dialog ate the button's release)
        if (!path.empty()) picked(p, sPickFor, path);
    }
#endif
    if (sDialog.Display("launcherpick", ImGuiWindowFlags_NoCollapse, mn)) {
        Pick p = sPick;
        if (sDialog.IsOk()) {
            picked(p, sPickFor, p == P_EXPORT ? sDialog.GetCurrentPath() : sDialog.GetFilePathName(IGFD_ResultMode_KeepInputFile));
        }
        sDialog.Close();
        sPick = P_NONE;
    }
}

// ---- the window ----------------------------------------------------------------------------------------------------------------

void Draw(float top) {
    ImGuiViewport *vp = ImGui::GetMainViewport();
    LoadFonts();
    float ui = ImGui::GetStyle().FontScaleMain;
    sK = fminf(1.0f, fminf(vp->Size.x / (780.0f * ui), (vp->Size.y - top) / (560.0f * ui)));
    if (sK < 0.4f) sK = 0.4f;
    ImGui::SetNextWindowPos(ImVec2(vp->Pos.x, vp->Pos.y + top));
    ImGui::SetNextWindowSize(ImVec2(vp->Size.x, vp->Size.y - top));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("##games", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar(3);
    ImVec2 a = ImGui::GetWindowPos(), b(a.x + ImGui::GetWindowWidth(), a.y + ImGui::GetWindowHeight());
    ImGui::PushFont(sFont, 15.0f * sK);
    if (sLayout == LC_LAYOUT_TABS) Tabs(a, b);
    else Library(a, b);
    Dialogs();
    ImGui::PopFont();
    ImGui::End();
}

// ---- the test hook -------------------------------------------------------------------------------------------------------------

std::vector<std::string> sSteps;
size_t sStep;
Uint64 sDue;

SDL_Window *Window() { return SDL_GetWindowFromID((Uint32)(intptr_t)ImGui::GetMainViewport()->PlatformHandle); }

void TestStep() {
    if (sStep >= sSteps.size() || SDL_GetTicks64() < sDue) return;
    std::string st = sSteps[sStep];
    size_t eq = st.find('=');
    std::string k = st.substr(0, eq), v = eq == std::string::npos ? "" : st.substr(eq + 1);
    Game *g = sSelected < (int)sGames.size() ? &sGames[(size_t)sSelected] : nullptr;
    if (k == "idle" && (sBusy.thread != nullptr || !sFetching.empty())) return;   // (again at the next frame)
    sStep++;
    sDue = SDL_GetTicks64() + 1000;
    fprintf(stderr, "launchertest: %s\n", st.c_str());
    if (k == "idle") {
    } else if (k == "layout") {
        sLayout = v == "tabs" ? LC_LAYOUT_TABS : LC_LAYOUT_LIBRARY;
        LC_SetLayout(sLayout);
    } else if (k == "game") {
        // +1: the game after the selected one (at first the last played), round the list: a test that switches each time;
        // next: the game after the one played last (a switching test's BWPORT_LAUNCHER_AGAIN="game=next;play")
        const LC_Game *last = LC_LastGame();
        sSelected = v == "+1" && !sGames.empty() ? (sSelected + 1) % (int)sGames.size() : atoi(v.c_str());
        for (size_t i = 0; v == "next" && i < sGames.size(); i++) {
            if (sGames[i].lc == last) sSelected = (int)((i + 1) % sGames.size());
        }
    } else if (k == "size") {
        int w = 0, h = 0;
        if (sscanf(v.c_str(), "%dx%d", &w, &h) == 2) SDL_SetWindowSize(Window(), w, h);
    } else if (k == "fullscreen") {
        SDL_SetWindowFullscreen(Window(), v == "1" ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    } else if (k == "wait") {
        sDue = SDL_GetTicks64() + (Uint64)(atof(v.c_str()) * 1000.0);
    } else if (k == "shot") {
        int w = 0, h = 0;
        SDL_GetWindowSize(Window(), &w, &h);
        std::string name = v + (sTimes > 1 ? "-" + std::to_string(sTimes) : "");   // (in the root: the working directory here)
        fprintf(stderr, "launchertest: shot %s/%s.png (window %dx%d)\n", PORT_LauncherRoot(), name.c_str(), w, h);
        SIM_ShotRequest(name.c_str());
    } else if (k == "drop") {   // (what SDL_DROPFILE does: bwport_launcher_drop)
        sDropped = v;
    } else if (k == "importrom") {
        ImportRom(v);
    } else if (k == "importsave" && g != nullptr) {
        ImportSave(*g, v);
    } else if (k == "exportsave" && g != nullptr) {
        ExportSave(*g, v);
    } else if (k == "artwork" && g != nullptr) {
        SetArtwork(*g, v);
    } else if (k == "noartwork" && g != nullptr) {
        RemoveArtwork(*g);
    } else if (k == "cover" && g != nullptr) {
        FetchCover(g->lc, true);
    } else if (k == "covers") {
        LC_SetCoverDownloads(v != "off");
        Reload();
    } else if (k == "skipmenu") {
        LC_SetSkipMenu(v != "off");
    } else if (k == "restore" && g != nullptr) {
        ListBackups(g->lc);
        size_t i = (size_t)atoi(v.c_str());
        if (i < sBackups.size()) Restore(g->lc, sBackups[i]);
    } else if (k == "remove" && g != nullptr) {
        Remove(*g);
    } else if (k == "backups" && g != nullptr) {
        if (v == "close") sBackupsOpen = false;
        else OpenBackups(*g);
    } else if (k == "menu") {
        sMenuFor = g != nullptr ? g->lc : nullptr;
        ImVec2 sz = ImGui::GetMainViewport()->Size;   // (the test's menus: the gear's corner, or the window's middle)
        if (v == "global") OpenMenu(2, ImVec2(sz.x - 24.0f * S(), 84.0f * S()), ImVec2(1, 0));
        else if (v == "close") sCloseMenus = true;
        else OpenMenu(v == "save" ? 3 : v == "more" ? 4 : 1, ImVec2(sz.x * 0.5f, sz.y * 0.5f), ImVec2(0.5f, 0.5f));
    } else if (k == "settings") {
        SIM_GuiOpenSettings(atoi(v.c_str()));
    } else if (k == "start") {
        LC_SetStartMode(v == "launcher" ? LC_START_LAUNCHER : LC_START_LAST);
    } else if (k == "alert") {
        if (sAsks.empty()) fprintf(stderr, "launchertest: alert: no question is open\n");   // (a later one would take it)
        else sTestAlert = v == "ok" ? 1 : 0;
    } else if (k == "play" && g != nullptr) {
        Play(*g);
    } else if (k == "quit") {
        fprintf(stderr, "launchertest: quit\n");
        fflush(stderr);
        _Exit(0);
    } else {
        fprintf(stderr, "launchertest: unknown step '%s'\n", st.c_str());
    }
}

}  // namespace

// machine.c's run loop, before the game boots: the Games window until Play has entered a game
extern "C" void PORT_RunLauncher(void) {
    Uint64 t0 = SDL_GetTicks64();
    PORT_SetLauncherWanted(0);
    sTimes++;
    sDone = false;
    if (chdir(PORT_LauncherRoot()) != 0) {
        fprintf(stderr, "bwport: launcher: can't enter the root %s\n", PORT_LauncherRoot());
    }
    sLayout = LC_Layout();
    sSelected = 1 << 20;   // (Reload: the last game)
    Reload();
    const char *e = getenv(sTimes == 1 ? "BWPORT_LAUNCHER_TEST" : "BWPORT_LAUNCHER_AGAIN");
    sSteps.clear();
    sStep = 0;
    sDue = SDL_GetTicks64() + 1500;
    for (std::string all = e != nullptr ? e : ""; !all.empty();) {
        size_t semi = all.find(';');
        if (all.substr(0, semi) != "") sSteps.push_back(all.substr(0, semi));
        all = semi == std::string::npos ? "" : all.substr(semi + 1);
    }
    SIM_GuiSetLauncher(Draw);
    fprintf(stderr, "bwport: launcher: up (%d), %zu games\n", sTimes, sGames.size());
    const LC_Game *autoPlay = PORT_LauncherAutoPlay();
    if (autoPlay != nullptr && Find(autoPlay) != nullptr) Play(*Find(autoPlay));
    while (!sDone) {
        TestStep();
        SIM_LauncherFrame();
        BusyPoll();
        CoverPoll();
        Reap();
    }
    SIM_GuiSetLauncher(nullptr);
    for (Game &g : sGames) {
        FreeTex(g.icon);
        FreeTex(g.art);
        FreeTex(g.grad);
        FreeTex(g.cover);
        FreeTex(g.coverBack);
    }
    sGames.clear();
    sAsks.clear();
    sBackupsOpen = false;
    if (getenv("BWPORT_LAUNCHER_AGAIN") != nullptr) PORT_SetLauncherWanted(1);   // (the test hook: the next reset comes back here)
    fprintf(stderr, "bwport: launcher: down after %.1f s\n", (double)(SDL_GetTicks64() - t0) / 1000.0);
}

// the window's SDL_DROPFILE (libntr-pc-launcher.patch: SIM_LauncherFrame): the path waits for the next frame
extern "C" void bwport_launcher_drop(const char *path) {
    sDropped = path;
}
