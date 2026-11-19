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
//   BWPORT_GL=mesa     the bundled Mesa, software (llvmpipe)
//   BWPORT_GL=zink     the bundled Mesa on the graphics card through Vulkan (below)
//   BWPORT_GL=d3d12    the bundled Mesa on the graphics card through Direct3D 12 (below)
// The bundled Mesa runs on the graphics card when it can: over Remote Desktop the system's GL is GDI Generic 1.1 while
// Vulkan and Direct3D still see the card, so a desktop viewed over RDP gets its GPU. Order, with the bundled Mesa in use
// and no GALLIUM_DRIVER / BWPORT_GL=mesa saying otherwise:
//   1. Zink (Mesa's GL on Vulkan; MIT, needs nothing but the graphics driver's Vulkan): when vulkan-1.dll has a physical
//      device that is a real GPU (discrete or integrated; FindVulkanGpu)
//   2. d3d12 (Mesa's GL on Direct3D 12): when mesa\dxil.dll is there (Microsoft's DXIL signer: not in the public zips, a
//      private folder may carry it) and DXGI has a hardware adapter that makes a D3D12 device (FindD3D12Adapter)
//   3. llvmpipe, the software GL
// A GPU driver that gives no usable context (no context, GL below 4.2, or a software renderer under it) is unloaded and the
// next one tried; only llvmpipe failing is fatal. The AMD desktop in an RDP session, Renderer=gpu: zink 650-740 fps,
// d3d12 570-645, llvmpipe 175.
// GALLIUM_DRIVER set by the user stands (zink, d3d12 or llvmpipe: that driver first, the later ones of the order as the
// fallback; llvmpipe: only it). Nothing on the system changes (no policy, no registry): for d3d12 the adapter is handed to
// this process's Mesa only, and the card's memory is used while the game runs.
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
#define COBJMACROS
#include <windows.h>
#include <dxgi1_6.h>
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

// The bundled Mesa on the graphics card. Mesa's d3d12 driver takes DXGI's first adapter unless its WGL front end gets
// an adapter from the ICD callback pfnGetAdapterLuid; in an RDP session the first adapter is the Microsoft Basic Render
// Driver (WARP, software: the GPU renderer at 53 fps). FindD3D12Adapter picks the first hardware adapter in DXGI's
// high-performance order that makes a D3D12 device; PointMesaAtAdapter hands its LUID to Mesa through
// DrvSetCallbackProcs (libgallium_wgl.dll's ICD entry point; Mesa checks each callback for NULL) before the first context.
static LUID sD3dLuid;
static char sD3dName[128];

typedef HRESULT(WINAPI *CreateDxgiFactoryFn)(UINT, REFIID, void **);
typedef HRESULT(WINAPI *CreateD3D12DeviceFn)(IUnknown *, int, REFIID, void **);
static const GUID kIidFactory6 = {0xc1b6694f, 0xff09, 0x44a9, {0xb0, 0x3c, 0x77, 0x90, 0x0a, 0x0a, 0x1d, 0x17}};
static const GUID kIidAdapter1 = {0x29038f61, 0x3839, 0x4626, {0x91, 0xfd, 0x08, 0x68, 0x79, 0x01, 0x1a, 0x05}};
static const GUID kIidD3D12Device = {0x189819f1, 0x1db6, 0x4b57, {0xbe, 0x54, 0x18, 0x21, 0x33, 0x9b, 0x85, 0xf7}};

static int FindD3D12Adapter(void) {
    HMODULE dx = LoadLibraryA("dxgi.dll");
    HMODULE d12 = LoadLibraryA("d3d12.dll");
    CreateDxgiFactoryFn makeFactory = dx != NULL ? (CreateDxgiFactoryFn)(void *)GetProcAddress(dx, "CreateDXGIFactory2") : NULL;
    CreateD3D12DeviceFn makeDevice = d12 != NULL ? (CreateD3D12DeviceFn)(void *)GetProcAddress(d12, "D3D12CreateDevice") : NULL;
    if (makeFactory == NULL || makeDevice == NULL) {
        GlLog("bwport: GL: no Direct3D 12 (%s)\n", makeFactory == NULL ? "dxgi.dll" : "d3d12.dll");
        return 0;
    }
    IDXGIFactory6 *f = NULL;
    if (FAILED(makeFactory(0, &kIidFactory6, (void **)&f))) {
        GlLog("bwport: GL: no DXGI 1.6 factory\n");
        return 0;
    }
    int found = 0;
    IDXGIAdapter1 *a = NULL;
    for (UINT i = 0; !found && SUCCEEDED(IDXGIFactory6_EnumAdapterByGpuPreference(f, i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                                                                                  &kIidAdapter1, (void **)&a)); i++) {
        DXGI_ADAPTER_DESC1 d;
        // a software adapter (WARP, the Basic Render Driver: vendor 0x1414) is no better than llvmpipe
        if (SUCCEEDED(IDXGIAdapter1_GetDesc1(a, &d)) && !(d.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) && d.VendorId != 0x1414 &&
            SUCCEEDED(makeDevice((IUnknown *)a, 0xb000 /* D3D_FEATURE_LEVEL_11_0 */, &kIidD3D12Device, NULL))) {
            sD3dLuid = d.AdapterLuid;
            WideCharToMultiByte(CP_UTF8, 0, d.Description, -1, sD3dName, sizeof(sD3dName), NULL, NULL);
            found = 1;
        }
        IDXGIAdapter1_Release(a);
    }
    IDXGIFactory6_Release(f);
    if (!found) {
        GlLog("bwport: GL: no hardware Direct3D 12 adapter\n");
    }
    return found;
}

static VOID APIENTRY D3dAdapterLuid(HDC hdc, LUID *luid) {
    (void)hdc;
    *luid = sD3dLuid;
}

static void PointMesaAtAdapter(void) {
    typedef void(APIENTRY * SetCallbackProcsFn)(INT, PROC *);
    HMODULE g = GetModuleHandleA("libgallium_wgl.dll");
    SetCallbackProcsFn set = g != NULL ? (SetCallbackProcsFn)(void *)GetProcAddress(g, "DrvSetCallbackProcs") : NULL;
    if (set == NULL) {
        GlLog("bwport: GL: libgallium_wgl.dll has no DrvSetCallbackProcs: Mesa picks the adapter itself\n");
        return;
    }
    // WGLCALLBACKS (Mesa's gldrv.h): SetCurrentValue, GetCurrentValue, GetDhglrc, unused, PresentBuffers, GetAdapterLuid
    PROC procs[6] = {NULL, NULL, NULL, NULL, NULL, (PROC)(void *)D3dAdapterLuid};
    set(6, procs);
}

// The bundled Mesa on the graphics card through Vulkan (Zink). Mesa's own Zink picks the Vulkan device; this only asks the
// Vulkan loader whether a real GPU is there, so a machine with no Vulkan (or only a software device: lavapipe) goes on to
// the next driver without making Zink fail. The few Vulkan types are declared here (no Vulkan SDK in the toolchain);
// layouts of the 64-bit ABI, checked against the spec: VkApplicationInfo 48 bytes, VkInstanceCreateInfo 64,
// VkPhysicalDeviceProperties 824 (only deviceType and deviceName are read; the buffer is bigger than the struct).
typedef struct VkAppInfoLite {
    int sType;   // VK_STRUCTURE_TYPE_APPLICATION_INFO = 0
    const void *pNext;
    const char *pApplicationName;
    unsigned applicationVersion;
    const char *pEngineName;
    unsigned engineVersion;
    unsigned apiVersion;
} VkAppInfoLite;
typedef struct VkInstanceInfoLite {
    int sType;   // VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO = 1
    const void *pNext;
    unsigned flags;
    const VkAppInfoLite *pApplicationInfo;
    unsigned enabledLayerCount;
    const char *const *ppEnabledLayerNames;
    unsigned enabledExtensionCount;
    const char *const *ppEnabledExtensionNames;
} VkInstanceInfoLite;
typedef struct VkPhysPropsLite {
    unsigned apiVersion, driverVersion, vendorID, deviceID;
    int deviceType;   // 0 other, 1 integrated GPU, 2 discrete GPU, 3 virtual GPU, 4 CPU
    char deviceName[256];
    unsigned char pipelineCacheUUID[16];
    unsigned long long rest[96];   // limits and sparseProperties (824 bytes in all): 1064 here, more than enough
} VkPhysPropsLite;
#ifdef _WIN64
_Static_assert(sizeof(VkAppInfoLite) == 48, "VkApplicationInfo");
_Static_assert(sizeof(VkInstanceInfoLite) == 64, "VkInstanceCreateInfo");
_Static_assert(__builtin_offsetof(VkPhysPropsLite, deviceName) == 20, "VkPhysicalDeviceProperties.deviceName");
_Static_assert(sizeof(VkPhysPropsLite) >= 824, "VkPhysicalDeviceProperties");
#endif

typedef void *(WINAPI *VkGetInstanceProcAddrFn)(void *, const char *);
typedef int(WINAPI *VkCreateInstanceFn)(const VkInstanceInfoLite *, const void *, void **);
typedef void(WINAPI *VkDestroyInstanceFn)(void *, const void *);
typedef int(WINAPI *VkEnumeratePhysicalDevicesFn)(void *, unsigned *, void **);
typedef void(WINAPI *VkGetPhysicalDevicePropertiesFn)(void *, VkPhysPropsLite *);

static char sVkName[260];

// 1 when vulkan-1.dll has a physical device that is a real GPU (discrete or integrated; not CPU, virtual or other): the
// first one wins and its name is in sVkName. An instance (Vulkan 1.0, no layers, no extensions) is made and destroyed
// and vulkan-1.dll released again; nothing else is called.
static int FindVulkanGpu(void) {
    HMODULE vk = LoadLibraryA("vulkan-1.dll");
    if (vk == NULL) {
        GlLog("bwport: GL: no Vulkan (no vulkan-1.dll)\n");
        return 0;
    }
    VkGetInstanceProcAddrFn getProc = (VkGetInstanceProcAddrFn)(void *)GetProcAddress(vk, "vkGetInstanceProcAddr");
    VkCreateInstanceFn create = getProc != NULL ? (VkCreateInstanceFn)getProc(NULL, "vkCreateInstance") : NULL;
    if (create == NULL) {
        GlLog("bwport: GL: no Vulkan (vulkan-1.dll has no vkCreateInstance)\n");
        FreeLibrary(vk);
        return 0;
    }
    VkAppInfoLite app = {0};
    app.apiVersion = 1u << 22;   // VK_API_VERSION_1_0
    VkInstanceInfoLite info = {0};
    info.sType = 1;
    info.pApplicationInfo = &app;
    void *inst = NULL;
    int res = create(&info, NULL, &inst);
    if (res != 0 || inst == NULL) {
        GlLog("bwport: GL: no Vulkan driver (vkCreateInstance: %d)\n", res);
        FreeLibrary(vk);
        return 0;
    }
    VkDestroyInstanceFn destroy = (VkDestroyInstanceFn)getProc(inst, "vkDestroyInstance");
    VkEnumeratePhysicalDevicesFn enumerate = (VkEnumeratePhysicalDevicesFn)getProc(inst, "vkEnumeratePhysicalDevices");
    VkGetPhysicalDevicePropertiesFn getProps =
        (VkGetPhysicalDevicePropertiesFn)getProc(inst, "vkGetPhysicalDeviceProperties");
    int found = 0;
    if (destroy == NULL || enumerate == NULL || getProps == NULL) {
        GlLog("bwport: GL: no Vulkan (the loader lacks a core function)\n");
    } else {
        void *devs[16];
        unsigned n = 16;
        res = enumerate(inst, &n, devs);   // 0 = success, 5 = incomplete (more than 16: the first 16 will do)
        if ((res != 0 && res != 5) || n == 0) {
            GlLog("bwport: GL: no Vulkan device (vkEnumeratePhysicalDevices: %d, %u)\n", res, n);
            n = 0;
        }
        for (unsigned i = 0; i < n; i++) {
            VkPhysPropsLite p;
            memset(&p, 0, sizeof(p));
            getProps(devs[i], &p);
            p.deviceName[sizeof(p.deviceName) - 1] = '\0';
            if (!found && (p.deviceType == 1 || p.deviceType == 2)) {
                snprintf(sVkName, sizeof(sVkName), "%s", p.deviceName);
                found = 1;
                GlLog("bwport: GL: Vulkan device: %s (%s GPU)\n", sVkName, p.deviceType == 2 ? "discrete" : "integrated");
            } else if (!found) {
                GlLog("bwport: GL: Vulkan device %s is not a GPU (type %d): skipped\n", p.deviceName, p.deviceType);
            }
        }
        if (!found && n > 0) {
            GlLog("bwport: GL: no Vulkan GPU\n");
        }
    }
    if (destroy != NULL) {
        destroy(inst, NULL);
    }
    FreeLibrary(vk);
    return found;
}

// the pre-loaded Mesa DLL (its handle is released with the rest by UnloadMesa)
static HMODULE sMesaPre;

// GALLIUM_DRIVER is set by the caller (SetEnv): Mesa (built against the UCRT) reads its environment as the DLL loads and at
// its first context
static int LoadMesa(const char *path, const char *dir, int d3d12Adapter) {
    wchar_t wdir[MAX_PATH];
    if (MultiByteToWideChar(CP_UTF8, 0, dir, -1, wdir, MAX_PATH) > 0) {
        SetDllDirectoryW(wdir);
    }
    // the adapter before SDL_GL_LoadLibrary: SDL makes a context there already (its WGL extension query), and Mesa's
    // screen, made at the first context, keeps the adapter it got then
    if (d3d12Adapter) {
        wchar_t wpath[MAX_PATH];
        if (MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, MAX_PATH) > 0 && (sMesaPre = LoadLibraryW(wpath)) != NULL) {
            PointMesaAtAdapter();
        }
    }
    if (SDL_GL_LoadLibrary(path) != 0) {
        GlLog("bwport: GL: the bundled Mesa (%s) did not load: %s\n", path, SDL_GetError());
        return -1;
    }
    return 0;
}

// Mesa's DLLs out of the process again, so the next driver starts from a fresh load (Mesa reads GALLIUM_DRIVER and makes its
// screen once per load). 1 when libgallium_wgl.dll is gone, 0 when something still holds it (a second load would then
// give the same driver again).
static int UnloadMesa(void) {
    SDL_GL_UnloadLibrary();
    if (sMesaPre != NULL) {
        FreeLibrary(sMesaPre);
        sMesaPre = NULL;
    }
    return GetModuleHandleA("libgallium_wgl.dll") == NULL;
}

typedef enum MesaDrv { DRV_ZINK, DRV_D3D12, DRV_LLVMPIPE, DRV_OTHER } MesaDrv;
static const char *const sDrvName[] = {"zink", "d3d12", "llvmpipe", "(its own)"};
static const char *const sDrvSource[] = {"bundled Mesa (Zink, Vulkan)", "bundled Mesa (d3d12, Direct3D 12)",
                                         "bundled Mesa (llvmpipe)", "bundled Mesa"};

// 1 when this GPU driver can be tried (d3d12: dxil.dll and a hardware adapter; zink: a Vulkan GPU unless forced); the log
// says why when not. Sets what the driver's name goes by in the log.
static int DriverUsable(MesaDrv d, const char *dir, int forced, char *where, size_t whereSize) {
    if (d == DRV_ZINK) {
        int have = FindVulkanGpu();
        snprintf(where, whereSize, "%s", have ? sVkName : "the Vulkan device Mesa picks");
        if (!have && forced) {
            GlLog("bwport: GL: zink was asked for (BWPORT_GL or GALLIUM_DRIVER): tried although no Vulkan GPU was found\n");
        }
        return have || forced;
    }
    if (d == DRV_D3D12) {
        // Mesa's d3d12 driver signs its shaders with mesa\dxil.dll (Microsoft's: not in the public zips, a user may put it
        // there): without it the first draw fails (exit 0x80070057)
        char dxil[1100];
        snprintf(dxil, sizeof(dxil), "%s\\dxil.dll", dir);
        FILE *fd = fopen(dxil, "rb");
        if (fd == NULL) {
            GlLog("bwport: GL: d3d12 skipped: no %s (optional, user-supplied)\n", dxil);
            return 0;
        }
        fclose(fd);
        if (!FindD3D12Adapter()) {
            return 0;
        }
        snprintf(where, whereSize, "%s", sD3dName);
        return 1;
    }
    return 1;
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
    if (force != NULL && force[0] != '\0' && strcmp(force, "system") != 0 && strcmp(force, "mesa") != 0 &&
        strcmp(force, "zink") != 0 && strcmp(force, "d3d12") != 0) {
        GlLog("bwport: GL: BWPORT_GL=%s is not system, mesa, zink or d3d12; ignored\n", force);
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
    if (force != NULL && (strcmp(force, "mesa") == 0 || strcmp(force, "zink") == 0 || strcmp(force, "d3d12") == 0)) {
        useMesa = 1;
        snprintf(reason, sizeof(reason), "BWPORT_GL=%s", force);
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
              force == NULL ? "" : strcmp(force, "system") != 0 ? "" : ": BWPORT_GL=system is not possible here");
        // The drivers to try, in order: the user's own GALLIUM_DRIVER first (it stands), else BWPORT_GL=zink|d3d12|mesa, else
        // zink -> d3d12 -> llvmpipe. A GPU driver that is skipped or gives no usable context hands over to the next one.
        const char *gd = getenv("GALLIUM_DRIVER");
        int userGd = gd != NULL && gd[0] != '\0';
        const char *want = userGd ? gd : (force != NULL && strcmp(force, "system") != 0 ? force : "");
        MesaDrv order[3];
        int n = 0, forced = 1;
        if (strcmp(want, "") == 0) {
            forced = 0;
            order[n++] = DRV_ZINK;
            order[n++] = DRV_D3D12;
            order[n++] = DRV_LLVMPIPE;
        } else if (strcmp(want, "zink") == 0) {
            order[n++] = DRV_ZINK;
            order[n++] = DRV_D3D12;
            order[n++] = DRV_LLVMPIPE;
        } else if (strcmp(want, "d3d12") == 0) {
            order[n++] = DRV_D3D12;
            order[n++] = DRV_LLVMPIPE;
        } else if (strcmp(want, "mesa") == 0 || strcmp(want, "llvmpipe") == 0) {
            order[n++] = DRV_LLVMPIPE;
        } else {
            order[n++] = DRV_OTHER;   // softpipe, a debug driver ...: the user's word, no fallback
        }
        int got = 0;
        memset(&sInfo, 0, sizeof(sInfo));
        for (int i = 0; i < n && !got; i++) {
            MesaDrv d = order[i];
            int last = i == n - 1;
            char where[300] = "";
            if (!DriverUsable(d, dir, forced && i == 0, where, sizeof(where))) {
                continue;   // the log says why; llvmpipe (always last) is never skipped
            }
            if (d == DRV_ZINK || d == DRV_D3D12) {
                SetEnv("GALLIUM_DRIVER", sDrvName[d]);
                GlLog("bwport: GL: the bundled Mesa on %s through %s (GALLIUM_DRIVER=%s; GALLIUM_DRIVER=llvmpipe or "
                      "BWPORT_GL=mesa for the software GL)\n", where, d == DRV_ZINK ? "Vulkan (Zink)" : "Direct3D 12",
                      sDrvName[d]);
            } else if (d == DRV_LLVMPIPE) {
                SetEnv("GALLIUM_DRIVER", "llvmpipe");   // the one Mesa driver that works with no GPU
                if (lowThreads) {
                    LowThreads();
                }
            }
            sSource = sDrvSource[d];
            char msg[512];
            int ok = LoadMesa(path, dir, d == DRV_D3D12) == 0;
            if (ok) {
                Probe(&sInfo);
                ok = sInfo.ok && (sInfo.hardware || d == DRV_LLVMPIPE || d == DRV_OTHER);
                if (!ok) {
                    snprintf(msg, sizeof(msg), "%s", sInfo.ok ? "a software renderer under a GPU driver" : sInfo.why);
                }
            } else {
                snprintf(msg, sizeof(msg), "the Mesa DLL did not load");
            }
            if (ok) {
                got = 1;
                break;
            }
            char fatal[700];
            if (last) {
                snprintf(fatal, sizeof(fatal), "The bundled Mesa gives no usable OpenGL (%s): %s", sDrvName[d], msg);
                Fatal(fatal);
            }
            GlLog("bwport: GL: the bundled Mesa on %s gives no usable OpenGL (%s; %s): unloading it, trying the next driver\n",
                  sDrvName[d], msg, sInfo.renderer[0] != '\0' ? sInfo.renderer : "no renderer");
            if (!UnloadMesa()) {
                snprintf(fatal, sizeof(fatal), "The bundled Mesa on %s gave no usable OpenGL (%s) and cannot be unloaded to "
                         "try another driver in this process. Start with BWPORT_GL=mesa (the software GL).", sDrvName[d],
                         msg);
                Fatal(fatal);
            }
            memset(&sInfo, 0, sizeof(sInfo));
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
