// Which OpenGL the PC build draws with (docs/port-build.md, "Which OpenGL"). libntr opens a 4.2 core context for its
// window (the present, the debug GUI) and Renderer=gpu runs its passes on it; this file decides, before that window
// exists, which GL library the context comes from, and tells the rest of the program what it got.
//
// win64: the exe starts with the system's GL (a real graphics driver's opengl32.dll). A probe (a hidden window and a
// 4.2 core context, GL_RENDERER / GL_VERSION / the functions port/gpu loads) tells whether it is usable and whether it is
// hardware. Not usable (Windows' own "GDI Generic" 1.1, no 4.2 core context, a function missing), or software while the
// bundled Mesa is there (<exe folder>\mesa\opengl32.dll + libgallium_wgl.dll, llvmpipe): SDL_GL_LoadLibrary loads the
// bundled Mesa instead, before libntr creates its window. The log says which and why.
//   BWPORT_GL=system   the system's GL, whatever it is (its software GL too); only a GL that can't make a context at all
//                      still goes to Mesa (a message in the log, never a crash or a black window)
//   BWPORT_GL=mesa     the bundled Mesa
// The llvmpipe tuning (LP_NUM_THREADS, GALLIUM_DRIVER=llvmpipe) is set only when Mesa's software GL is the one in use.
// Linux: the system's GL always (Mesa or a vendor driver); only the report (hardware or not) and the LP_NUM_THREADS
// rule (a hardware driver ignores it) apply.
//
// libntr (libntr-glselect.patch) calls PreContext before SDL_CreateWindow and PostContext once the context exists.
// SIM_GLIsHardware / SIM_GLRendererName answer from the probe until then, from the live context after.
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef struct GlInfo {
    int ok;          // a 4.2 core context was made and has what port/gpu loads
    int hardware;    // a real GPU driver
    int major, minor;
    char renderer[160], vendor[96], version[128];
    char why[256];   // when !ok
} GlInfo;

typedef const unsigned char *(*GetStringFn)(unsigned int);

static GlInfo sInfo;
static int sHave;               // sInfo holds an answer
static int sSelected;           // the choice was made
static const char *sSource = "system";

#define GlLog(...) (printf(__VA_ARGS__), fflush(stdout))   // the run's log.txt is a file: no buffering

void SIM_SetGLHooks(void (*pre)(int), void (*post)(void));

static int ContainsNoCase(const char *s, const char *word) {
    size_t n = strlen(word);
    for (; *s != '\0'; s++) {
        if (SDL_strncasecmp(s, word, n) == 0) {
            return 1;
        }
    }
    return 0;
}

// the renderer strings of software GL: Mesa's llvmpipe / softpipe / swrast, Windows' GDI Generic and the Basic Render
// Driver (also under OpenGL-on-D3D12's "D3D12 (Microsoft Basic Render Driver)"), lavapipe
static int IsSoftware(const char *renderer) {
    static const char *const words[] = {"llvmpipe", "softpipe", "swrast", "software", "gdi generic", "basic render",
                                        "microsoft basic", "lavapipe", NULL};
    for (int i = 0; words[i] != NULL; i++) {
        if (ContainsNoCase(renderer, words[i])) {
            return 1;
        }
    }
    return 0;
}

// GL 3.3 core functions port/gpu and the bridges load by name, and libntr's present (a sample; one missing = Mesa)
static const char *const sNeeded[] = {
    "glActiveTexture", "glAttachShader", "glBindFramebuffer", "glBindVertexArray", "glBlitFramebuffer", "glBufferData",
    "glCheckFramebufferStatus", "glClearBufferfv", "glClearBufferuiv", "glCompileShader", "glCreateProgram",
    "glCreateShader", "glDrawBuffers", "glFramebufferTexture2D", "glFramebufferTextureLayer", "glGenBuffers",
    "glGenFramebuffers", "glGenVertexArrays", "glGetStringi", "glGetUniformBlockIndex", "glLinkProgram",
    "glTexImage3D", "glTexStorage2D", "glTexSubImage3D", "glUniformBlockBinding", "glBindBufferBase", NULL};

// A hidden window and a context with the attributes libntr uses (sim_main.cpp's SIM_RenderInit), the strings read, both
// gone again. The GL library (SDL_GL_LoadLibrary) is whatever is loaded now.
static void Probe(GlInfo *gi) {
    memset(gi, 0, sizeof(*gi));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window *w = SDL_CreateWindow("bwport GL probe", 0, 0, 64, 64, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (w == NULL) {
        snprintf(gi->why, sizeof(gi->why), "no window for a GL context: %s", SDL_GetError());
        return;
    }
    SDL_GLContext c = SDL_GL_CreateContext(w);
    if (c == NULL) {
        snprintf(gi->why, sizeof(gi->why), "no GL 4.2 core context: %s", SDL_GetError());
        SDL_DestroyWindow(w);
        return;
    }
    GetStringFn getString = (GetStringFn)SDL_GL_GetProcAddress("glGetString");
    const unsigned char *r = getString != NULL ? getString(0x1F01) : NULL;   // GL_RENDERER
    const unsigned char *v = getString != NULL ? getString(0x1F02) : NULL;   // GL_VERSION
    const unsigned char *n = getString != NULL ? getString(0x1F00) : NULL;   // GL_VENDOR
    snprintf(gi->renderer, sizeof(gi->renderer), "%s", r != NULL ? (const char *)r : "?");
    snprintf(gi->version, sizeof(gi->version), "%s", v != NULL ? (const char *)v : "?");
    snprintf(gi->vendor, sizeof(gi->vendor), "%s", n != NULL ? (const char *)n : "?");
    if (sscanf(gi->version, "%d.%d", &gi->major, &gi->minor) != 2) {
        gi->major = gi->minor = 0;
    }
    gi->hardware = !IsSoftware(gi->renderer);
    if (gi->major < 4 || (gi->major == 4 && gi->minor < 2)) {
        snprintf(gi->why, sizeof(gi->why), "GL %s (4.2 core needed)", gi->version);
    } else {
        for (int i = 0; sNeeded[i] != NULL; i++) {
            if (SDL_GL_GetProcAddress(sNeeded[i]) == NULL) {
                snprintf(gi->why, sizeof(gi->why), "%s missing", sNeeded[i]);
                break;
            }
        }
    }
    gi->ok = gi->why[0] == '\0';
    SDL_GL_DeleteContext(c);
    SDL_DestroyWindow(w);
}

static void SetEnv(const char *name, const char *value) {
#ifdef _WIN32
    SetEnvironmentVariableA(name, value);
    _putenv_s(name, value);
#else
    setenv(name, value, 0);
#endif
}

// Mesa makes its raster threads at its first context. Renderer=vcore's present is one quad, and waking llvmpipe's thread
// per CPU costs several times its work: 4 at most. A user's own LP_NUM_THREADS stands.
static void LowThreads(void) {
    if (getenv("LP_NUM_THREADS") != NULL) {
        return;
    }
    int n = SDL_GetCPUCount();
    char nbuf[8];
    n = n > 4 ? 4 : (n < 1 ? 1 : n);
    snprintf(nbuf, sizeof(nbuf), "%d", n);
    SetEnv("LP_NUM_THREADS", nbuf);
    GlLog("bwport: GL: LP_NUM_THREADS=%s (llvmpipe only; Renderer=vcore)\n", nbuf);
}

static void Fatal(const char *text) {
    GlLog("bwport: GL: %s\n", text);
    if (getenv("BWPORT_NOPAUSE") == NULL) {   // the .bat's switch for runs nobody watches
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Pokemon Black (bwport)", text, NULL);
    }
    _Exit(1);   // not exit(): the game's exit() (port/src/boot_exit.c) returns
}

#ifdef _WIN32
// <exe folder>\mesa\opengl32.dll: path, and the folder (libgallium_wgl.dll is found through it)
static int MesaPath(char *path, size_t size, char *dir, size_t dirsize) {
    char *base = SDL_GetBasePath();   // the exe's folder, with its trailing separator
    int ok = 0;
    if (base != NULL && strlen(base) + 24 < size && strlen(base) + 8 < dirsize) {
        snprintf(dir, dirsize, "%smesa", base);
        snprintf(path, size, "%smesa\\opengl32.dll", base);
        FILE *f = fopen(path, "rb");
        if (f != NULL) {
            fclose(f);
            ok = 1;
        }
    }
    SDL_free(base);
    return ok;
}

static int LoadMesa(const char *path, const char *dir, int lowThreads) {
    wchar_t wdir[MAX_PATH];
    if (MultiByteToWideChar(CP_UTF8, 0, dir, -1, wdir, MAX_PATH) > 0) {
        SetDllDirectoryW(wdir);
    }
    // Mesa (built against the UCRT) reads its environment as the DLL loads and at its first context: set before both
    if (getenv("GALLIUM_DRIVER") == NULL) {
        SetEnv("GALLIUM_DRIVER", "llvmpipe");   // the one Mesa driver that works with no GPU
    }
    if (lowThreads) {
        LowThreads();
    }
    if (SDL_GL_LoadLibrary(path) != 0) {
        GlLog("bwport: GL: the bundled Mesa (%s) did not load: %s\n", path, SDL_GetError());
        return -1;
    }
    sSource = "bundled Mesa";
    return 0;
}
#endif

static void Select(int lowThreads) {
    if (sSelected) {
        return;
    }
    sSelected = 1;
    if (!SDL_WasInit(SDL_INIT_VIDEO) && SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        GlLog("bwport: GL: SDL video: %s\n", SDL_GetError());
        return;
    }
    const char *force = getenv("BWPORT_GL");
    if (force != NULL && force[0] != '\0' && strcmp(force, "system") != 0 && strcmp(force, "mesa") != 0) {
        GlLog("bwport: GL: BWPORT_GL=%s is not system or mesa; ignored\n", force);
        force = NULL;
    }
    if (force != NULL && force[0] == '\0') {
        force = NULL;
    }
#ifdef _WIN32
    char path[1024], dir[1024];
    int haveMesa = MesaPath(path, sizeof(path), dir, sizeof(dir));
    int useMesa = 0;
    char reason[320] = "";
    if (force != NULL && strcmp(force, "mesa") == 0) {
        useMesa = 1;
        snprintf(reason, sizeof(reason), "BWPORT_GL=mesa");
    } else {
        if (SDL_GL_LoadLibrary(NULL) != 0) {   // the system's opengl32.dll
            snprintf(reason, sizeof(reason), "no system GL library: %s", SDL_GetError());
            GlLog("bwport: GL: %s\n", reason);
            useMesa = 1;
        } else {
            Probe(&sInfo);
            if (!sInfo.ok) {
                snprintf(reason, sizeof(reason), "the system's GL is not usable: %s (%s, %s)", sInfo.why,
                         sInfo.renderer, sInfo.version);
                GlLog("bwport: GL: %s\n", reason);
                useMesa = 1;
            } else if (!sInfo.hardware && force == NULL && haveMesa) {
                snprintf(reason, sizeof(reason), "the system's GL is software (%s, %s)", sInfo.renderer, sInfo.version);
                GlLog("bwport: GL: %s\n", reason);
                useMesa = 1;
            } else {
                sHave = 1;
                GlLog("bwport: GL: system: %s, %s (%s)%s%s\n", sInfo.renderer, sInfo.version, sInfo.vendor,
                    sInfo.hardware ? "" : ", software", force != NULL ? ", BWPORT_GL=system" : "");
            }
            if (useMesa) {
                SDL_GL_UnloadLibrary();
            }
        }
    }
    if (useMesa) {
        if (!haveMesa) {
            char msg[640];
            snprintf(msg, sizeof(msg), "No usable OpenGL (%s), and the bundled Mesa (mesa\\opengl32.dll next to the exe) is "
                     "missing. Install a graphics driver, or copy the mesa folder from a full bwport folder.", reason);
            Fatal(msg);
        }
        GlLog("bwport: GL: loading the bundled Mesa (%s)%s\n", path,
              force == NULL ? "" : strcmp(force, "mesa") == 0 ? ": BWPORT_GL=mesa" : ": BWPORT_GL=system is not possible here");
        if (LoadMesa(path, dir, lowThreads) != 0) {
            Fatal("The bundled Mesa (mesa\\opengl32.dll) could not be loaded. See the log.");
        }
        Probe(&sInfo);
        if (!sInfo.ok) {
            char msg[512];
            snprintf(msg, sizeof(msg), "The bundled Mesa gives no usable OpenGL: %s", sInfo.why);
            Fatal(msg);
        }
        sHave = 1;
        GlLog("bwport: GL: bundled Mesa: %s, %s (%s)\n", sInfo.renderer, sInfo.version, sInfo.vendor);
    }
#else
    // Linux: the system's GL; LP_NUM_THREADS before the first context (a hardware driver ignores it)
    if (force != NULL && strcmp(force, "mesa") == 0) {
        GlLog("bwport: GL: BWPORT_GL=mesa: this build uses the system's GL\n");
    }
    if (lowThreads) {
        LowThreads();
    }
#endif
}

static void PreContext(int lowThreads) {
    Select(lowThreads);
}

// libntr's context exists: the live strings are the answer from now on
static void PostContext(void) {
    GetStringFn getString = (GetStringFn)SDL_GL_GetProcAddress("glGetString");
    if (getString == NULL || getString(0x1F01) == NULL) {
        return;
    }
    GlInfo live;
    memset(&live, 0, sizeof(live));
    snprintf(live.renderer, sizeof(live.renderer), "%s", (const char *)getString(0x1F01));
    snprintf(live.version, sizeof(live.version), "%s", (const char *)getString(0x1F02));
    snprintf(live.vendor, sizeof(live.vendor), "%s", (const char *)getString(0x1F00));
    live.hardware = !IsSoftware(live.renderer);
    live.ok = 1;
    sscanf(live.version, "%d.%d", &live.major, &live.minor);
    sInfo = live;
    sHave = 1;
    GlLog("bwport: GL: %s: %s, %s (%s): %s\n", sSource, sInfo.renderer, sInfo.version, sInfo.vendor,
        sInfo.hardware ? "hardware" : "software");
}

// 1 = a real GPU driver; 0 = software GL (llvmpipe, GDI Generic, the Basic Render Driver ...) or none
int SIM_GLIsHardware(void) {
    if (!sHave) {
        Select(1);
        if (!sHave) {
            // Linux before libntr's context: a probe of the system's GL
            Probe(&sInfo);
            sHave = 1;
        }
    }
    return sInfo.ok && sInfo.hardware ? 1 : 0;
}

// GL_RENDERER of the GL in use ("none" when there is no usable GL)
const char *SIM_GLRendererName(void) {
    if (!sHave) {
        (void)SIM_GLIsHardware();
    }
    return sInfo.renderer[0] != '\0' ? sInfo.renderer : "none";
}

__attribute__((constructor)) static void GlSelectInit(void) {
    SIM_SetGLHooks(PreContext, PostContext);
}
