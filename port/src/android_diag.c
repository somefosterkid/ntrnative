// Android only: the first lines of log.txt that tell a remote tester's device apart: the model, the Android release, the ABI,
// the SoC, whether plain char is signed (the game is built -fsigned-char, the libraries are not) and the requested renderer. The GL
// vendor, renderer and version are the "bwport: GL: system: ..." line (gl_select.c), the renderer that ended up drawing is
// "bwport: renderer: ..." (libntr's status line) and "bwport: Renderer=gpu: ..." (gpu_bridge.c).
#ifdef __ANDROID__
#include <stdio.h>
#include <stdlib.h>
#include <sys/system_properties.h>

static void Prop(const char *name, char *out, size_t n) {
    out[0] = 0;
    char v[PROP_VALUE_MAX] = "";
    if (__system_property_get(name, v) > 0) snprintf(out, n, "%s", v);
}

void PORT_AndroidThermalLine(void);   // android_log.c

void PORT_AndroidDiag(void) {
    char man[PROP_VALUE_MAX], model[PROP_VALUE_MAX], rel[PROP_VALUE_MAX], sdk[PROP_VALUE_MAX], abi[PROP_VALUE_MAX], soc[PROP_VALUE_MAX],
        hw[PROP_VALUE_MAX], build[PROP_VALUE_MAX];
    Prop("ro.product.manufacturer", man, sizeof man);
    Prop("ro.product.model", model, sizeof model);
    Prop("ro.build.version.release", rel, sizeof rel);
    Prop("ro.build.version.sdk", sdk, sizeof sdk);
    Prop("ro.product.cpu.abi", abi, sizeof abi);
    Prop("ro.soc.model", soc, sizeof soc);
    Prop("ro.hardware", hw, sizeof hw);
    Prop("ro.build.fingerprint", build, sizeof build);
    const char *r = getenv("BWPORT_RENDERER");
    printf("bwport: app build %s\n", getenv("BWPORT_APP_BUILD") != NULL ? getenv("BWPORT_APP_BUILD") : "?");
    printf("bwport: device: %s %s, Android %s (API %s), ABI %s (built for %s), SoC %s, hardware %s\n", man, model, rel, sdk, abi,
#if defined(__aarch64__)
           "arm64-v8a",
#elif defined(__x86_64__)
           "x86_64",
#else
           "other",
#endif
           soc[0] ? soc : "?", hw);
    printf("bwport: device: build %s; plain char is %s here (the game is -fsigned-char); BWPORT_RENDERER=%s (sim_config.ini Renderer is "
           "read later: the line \"bwport: renderer:\" says what draws)\n", build, (char)0xff < 0 ? "signed" : "unsigned", r != NULL ? r : "(unset)");
    PORT_AndroidThermalLine();
    fflush(stdout);
}
#endif
