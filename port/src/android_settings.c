// Android: the settings screen's native side (the maintainers' notes, "Android"). The screen is Java: tools/android/app/src/main/java/com/bwport/app/
// SettingsScreen.java, a full-screen Dialog over SDL's activity with ios_settings.m's rows. libntr's touch menu button asks for it
// (libntr-android-settings.patch, SimTouchSettingsWait in sim_main.cpp): the game and its sound stand still while is_open says yes.
// Every choice goes through SIM_UISettingSet (libntr's sim_gui.cpp), as the iOS sheet's does, which also writes sim_config.ini.
//   present   asks Java to show the dialog (it is built on the UI thread; this thread is SIM_Render's)
//   is_open   until the dialog is dismissed (Done, back, a tap outside it) -- or never shown (10 s)
//   test_wants  BWPORT_SETTINGS_OPEN=S opens it once, S seconds after the first frame (a test aid: the headless emulator can also
//               tap the menu button, or the dialog's own views; this saves finding the button's pixels)
#ifdef __ANDROID__
#include <SDL2/SDL.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int SIM_UISettingGet(const char *key);
extern void SIM_UISettingSet(const char *key, int v);
extern const char *SIM_UIStatus(void);
extern int SIM_Renderer3DFastState(const char **how); // libntr-ios-fastgl.patch: 1 drawn, 0 cannot, -1 not tried; how or why
extern void SIM_PerfTestStart(void) __attribute__((weak));

static volatile int sOpen;       // present() was called and the dialog has not ended
static volatile int sShown;      // the dialog is on screen
static struct timespec sOpenedAt;

static double Now(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec * 1e-9;
}

void bwport_android_settings_present(void) {
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv();   // (attaches this thread)
    jobject act = (jobject)SDL_AndroidGetActivity();
    jclass cc, clsClass, loaderClass, scr;
    jobject loader;
    jmethodID getLoader, loadClass, show;
    jstring name;

    if (sOpen) {
        return;
    }
    sOpen = 1;
    sShown = 0;
    clock_gettime(CLOCK_MONOTONIC, &sOpenedAt);
    if (env == NULL || act == NULL) {
        fprintf(stderr, "bwport: settings: no JNI environment or activity\n");
        sOpen = 0;
        return;
    }
    // an app class is not found by FindClass on a thread SDL attached: ask the activity's class loader
    clsClass = (*env)->FindClass(env, "java/lang/Class");
    cc = (*env)->GetObjectClass(env, act);
    getLoader = (*env)->GetMethodID(env, clsClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    loader = (*env)->CallObjectMethod(env, cc, getLoader);
    loaderClass = (*env)->FindClass(env, "java/lang/ClassLoader");
    loadClass = (*env)->GetMethodID(env, loaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    name = (*env)->NewStringUTF(env, "com.bwport.app.SettingsScreen");
    scr = (jclass)(*env)->CallObjectMethod(env, loader, loadClass, name);
    if ((*env)->ExceptionCheck(env) || scr == NULL) {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        fprintf(stderr, "bwport: settings: SettingsScreen not found\n");
        sOpen = 0;
        return;
    }
    show = (*env)->GetStaticMethodID(env, scr, "show", "(Landroid/app/Activity;)V");
    (*env)->CallStaticVoidMethod(env, scr, show, act);
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        sOpen = 0;
        return;
    }
    (*env)->DeleteLocalRef(env, scr);
    (*env)->DeleteLocalRef(env, loader);
    (*env)->DeleteLocalRef(env, cc);
    (*env)->DeleteLocalRef(env, act);
}

int bwport_android_settings_is_open(void) {
    if (sOpen && !sShown) {   // Java never got it up (the activity going away, a refused window): the game goes on
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC, &t);
        if ((double)(t.tv_sec - sOpenedAt.tv_sec) > 10.0) {
            fprintf(stderr, "bwport: settings: the dialog did not show in 10 s; the game goes on without it\n");
            sOpen = 0;
        }
    }
    return sOpen;
}

int bwport_android_settings_test_wants(void) {
    static double at = -1, first;
    const char *e;

    if (at < 0) {
        e = getenv("BWPORT_SETTINGS_OPEN");
        at = e != NULL && *e ? atof(e) : 0.0;
        first = Now();
        if (at <= 0.0) {
            at = 1e30;
        }
    }
    if (at < 1e29 && Now() - first >= at) {
        at = 1e30;
        return 1;
    }
    return 0;
}

// ---- Java -> native (SettingsScreen's native methods) --------------------------------------------------------------------------


JNIEXPORT __attribute__((visibility("default"))) jint JNICALL Java_com_bwport_app_SettingsScreen_nativeGet(JNIEnv *env, jclass c, jstring key) {
    const char *k = (*env)->GetStringUTFChars(env, key, NULL);
    jint v = SIM_UISettingGet(k);

    (*env)->ReleaseStringUTFChars(env, key, k);
    return v;
}

JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_SettingsScreen_nativeSet(JNIEnv *env, jclass c, jstring key, jint v) {
    const char *k = (*env)->GetStringUTFChars(env, key, NULL);

    SIM_UISettingSet(k, v);
    (*env)->ReleaseStringUTFChars(env, key, k);
}

JNIEXPORT __attribute__((visibility("default"))) jstring JNICALL Java_com_bwport_app_SettingsScreen_nativeStatus(JNIEnv *env, jclass c) {
    return (*env)->NewStringUTF(env, SIM_UIStatus());
}

// the 3D renderer row's footer: why Renderer3D=fast cannot draw on this GPU, or NULL when it can or was not tried (port/gpu g10_fast_gl.c)
JNIEXPORT __attribute__((visibility("default"))) jstring JNICALL Java_com_bwport_app_SettingsScreen_nativeFast3DWhyNot(JNIEnv *env, jclass c) {
    const char *how = "";
    return SIM_Renderer3DFastState(&how) == 0 ? (*env)->NewStringUTF(env, how) : NULL;
}

JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_SettingsScreen_nativeShown(JNIEnv *env, jclass c) {
    sShown = 1;
}

JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_SettingsScreen_nativeClosed(JNIEnv *env, jclass c) {
    sOpen = 0;
}

// "Run performance test": libntr-ios-pace.patch's fixed list of settings, a few seconds each, a table in the log. It starts when the
// dialog closes (the game stands still while it is open), as on iOS.
JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_SettingsScreen_nativePerfTest(JNIEnv *env, jclass c) {
    if (SIM_PerfTestStart != NULL) {
        SIM_PerfTestStart();
    }
}

// "Restart to title": the game's own soft reset (port/src/restart_press.c), pressed after the screen is closed (the game stands still
// while it is open). The game's reset ends the app (port/src/reset.c): the next start is at the title.
extern void bwport_press_restart(void);
extern void bwport_press_games(void);
JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_SettingsScreen_nativeGames(JNIEnv *env, jclass c) {
    (void)env;
    (void)c;
    bwport_press_games();
}
JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_SettingsScreen_nativeRestart(JNIEnv *env, jclass c) {
    bwport_press_restart();
}
#endif
