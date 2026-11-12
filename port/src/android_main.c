// Android: the program's entry (the maintainers' notes, "Android"). SDLActivity (tools/android/app/src/main/java/com/bwport/app/
// BwportActivity.java) loads libSDL2.so and this library (libmain.so) and calls SDL_main on SDL's own thread. Before libntr's main
// (libntr-android-gles.patch calls it SIM_main):
// - the working directory: BWPORT_DIR, else the app's external files directory (/storage/emulated/0/Android/data/com.bwport.app/
//   files: adb push reaches it without root), else the internal one. The ROM (*.nds), save.bin, sim_config.ini and the ROM's
//   extracted files live there. main_start.c's start (PORT_AndroidStart, a constructor elsewhere) runs here, on SDL's thread.
// - stdout and stderr: Android sends them nowhere. A thread copies each line to logcat (tag "bwport") and to log.txt in the working
//   directory (emptied at each start).
// - the orientations iOS allows (portrait and both landscapes): SDL's Java side follows the hint; the sound driver order.
#ifdef __ANDROID__
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int SIM_main(int argc, char *argv[]);
void PORT_AndroidStart(void);
void PORT_AndroidDiag(void);   // android_diag.c: the device lines of log.txt

void PORT_AndroidLogRotate(const char *dir);   // android_log.c: the last run's log.txt goes to logs/
static int sLogPipe[2];
static FILE *sLogFile;
static unsigned long sLogBytes;
static char sLogHead[1024];   // the first lines, written again after the file starts over
#define LOG_CAP (2ul * 1024 * 1024)

static void LogLine(const char *line) {
    __android_log_write(ANDROID_LOG_INFO, "bwport", line);
    if (sLogFile != NULL) {
        size_t n = strlen(line) + 1;

        if (sLogBytes + n > LOG_CAP) {   // a long session or a line that repeats: the file starts over, with its head and a note
            if (ftruncate(fileno(sLogFile), 0) == 0) {
                rewind(sLogFile);
                fprintf(sLogFile, "%sbwport: this log passed %lu MB; the earlier lines were dropped\n", sLogHead, LOG_CAP / (1024 * 1024));
                sLogBytes = strlen(sLogHead);
            }
        }
        fputs(line, sLogFile);
        fputc('\n', sLogFile);
        fflush(sLogFile);
        sLogBytes += n;
        if (sLogBytes < sizeof(sLogHead) && strlen(sLogHead) + n < sizeof(sLogHead) && strncmp(line, "bwport: ", 8) == 0 && (strstr(line, "device") != NULL || strstr(line, "app build") != NULL)) {
            strcat(sLogHead, line);
            strcat(sLogHead, "\n");
        }
    }
}

static void *LogThread(void *arg) {
    static char buf[4096];
    size_t have = 0;
    ssize_t n;

    (void)arg;
    while ((n = read(sLogPipe[0], buf + have, sizeof(buf) - 1 - have)) > 0) {
        char *line = buf, *nl;

        have += (size_t)n;
        buf[have] = 0;
        while ((nl = strchr(line, '\n')) != NULL) {
            *nl = 0;
            LogLine(line);
            line = nl + 1;
        }
        have -= (size_t)(line - buf);
        memmove(buf, line, have);
        if (have == sizeof(buf) - 1) {    // a line longer than the buffer goes out in pieces
            buf[have] = 0;
            LogLine(buf);
            have = 0;
        }
    }
    return NULL;
}

static void LogStart(const char *dir) {
    char path[1100];
    pthread_t th;

    PORT_AndroidLogRotate(dir);
    snprintf(path, sizeof(path), "%s/log.txt", dir);
    sLogFile = fopen(path, "w");
    if (pipe(sLogPipe) != 0) {
        return;
    }
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    dup2(sLogPipe[1], 1);
    dup2(sLogPipe[1], 2);
    if (pthread_create(&th, NULL, LogThread, NULL) == 0) {
        pthread_detach(th);
    }
}

// The display's density and insets, from the Java activity (BwportActivity.reportInsets, on the UI thread; the density first through the
// BWPORT_DENSITY variable before SDL starts). sim_touch.cpp (libntr-android-touch.patch) lays the touch controls out in dp (1/160 in)
// and asks for the insets in pixels: the display cutout and the navigation bar, as iOS's safe area.
static volatile float sDensity;
static volatile float sInsets[4];
static volatile int sInsetsKnown;

JNIEXPORT void JNICALL Java_com_bwport_app_BwportActivity_nativeInsets(JNIEnv *env, jclass cls, jint l, jint t, jint r, jint b,
                                                                       jfloat density) {
    (void)env;
    (void)cls;
    sInsets[0] = (float)l;
    sInsets[1] = (float)t;
    sInsets[2] = (float)r;
    sInsets[3] = (float)b;
    if (density > 0.0f) {
        sDensity = density;
    }
    sInsetsKnown = 1;
}

// The panel's refreshes (BwportActivity.nativeVsync, Choreographer's frame times): the last one and the period, from the median of the
// last five gaps (a stalled UI thread makes one long gap). The game's SpLink asks (sim_main.cpp, libntr-android-touch.patch).
static volatile unsigned long long sVsLast;
static volatile unsigned long long sVsPeriod;
static unsigned long long sVsGaps[5];
static int sVsN;

JNIEXPORT void JNICALL Java_com_bwport_app_BwportActivity_nativeVsync(JNIEnv *env, jclass cls, jlong frameTimeNanos) {
    unsigned long long t = (unsigned long long)frameTimeNanos, prev = sVsLast;

    (void)env;
    (void)cls;
    if (prev != 0 && t > prev && t - prev < 100000000ull) {
        unsigned long long g[5], tmp;
        int n, i, j;

        sVsGaps[sVsN++ % 5] = t - prev;
        n = sVsN < 5 ? sVsN : 5;
        for (i = 0; i < n; i++) {
            g[i] = sVsGaps[i];
        }
        for (i = 1; i < n; i++) {
            for (j = i; j > 0 && g[j - 1] > g[j]; j--) {
                tmp = g[j];
                g[j] = g[j - 1];
                g[j - 1] = tmp;
            }
        }
        sVsPeriod = g[n / 2];
    }
    sVsLast = t;
}

int bwport_android_vsync(unsigned long long *lastNs, unsigned long long *periodNs) {
    *lastNs = sVsLast;
    *periodNs = sVsPeriod;
    return sVsLast != 0 && sVsPeriod != 0;
}

float bwport_android_density(void) {
    if (sDensity <= 0.0f) {
        const char *e = getenv("BWPORT_DENSITY");
        float d = e != NULL ? (float)atof(e) : 0.0f;
        sDensity = d > 0.0f ? d : 1.0f;
    }
    return sDensity;
}

int bwport_android_insets(float *l, float *t, float *r, float *b) {
    *l = sInsets[0];
    *t = sInsets[1];
    *r = sInsets[2];
    *b = sInsets[3];
    return sInsetsKnown;
}

// Leaves the game for the launcher (reset.c: Restart to title restarts the same game, Games goes to the library): BwportActivity.leaveGame
// starts the launcher activity and ends this process. 0 when this run was not started by the launcher (the caller keeps its old way).
int bwport_android_leave(int play) {
    const char *game = getenv("BWPORT_GAME");
    JNIEnv *env;
    jobject act;
    jmethodID m;
    jstring arg = NULL;

    if (game == NULL || game[0] == 0 || getenv("BWPORT_DIR") != NULL) {
        return 0;
    }
    env = (JNIEnv *)SDL_AndroidGetJNIEnv();
    act = (jobject)SDL_AndroidGetActivity();
    if (env == NULL || act == NULL) {
        return 0;
    }
    m = (*env)->GetMethodID(env, (*env)->GetObjectClass(env, act), "leaveGame", "(Ljava/lang/String;)V");
    if (m == NULL) {
        (*env)->ExceptionClear(env);
        return 0;
    }
    if (play) {
        arg = (*env)->NewStringUTF(env, game);
    }
    (*env)->CallVoidMethod(env, act, m, arg);
    return 1;
}

#undef main    // port_prefix.h names the game's main NitroMain; this is the program's
int SDL_main(int argc, char *argv[]) {
    const char *dir = getenv("BWPORT_DIR");

    if (dir == NULL || dir[0] == 0) {
        dir = SDL_AndroidGetExternalStoragePath();    // (creates the directory)
    }
    if (dir == NULL) {
        dir = SDL_AndroidGetInternalStoragePath();
    }
    if (dir != NULL) {
        const char *game = getenv("BWPORT_GAME");

        if (game != NULL && game[0] != 0 && getenv("BWPORT_DIR") == NULL) {
            // started by the launcher (LauncherActivity.enter): the root is the working directory, and main_start.c's launcher start
            // picks the game's folder (BWPORT_GAME) and goes into it, keeping sim_config.ini in the root
            if (chdir(dir) != 0) {
                fprintf(stderr, "bwport: can't enter %s\n", dir);
            }
        } else {
            setenv("BWPORT_DIR", dir, 1);    // a test run or an old layout: one game, the flat folder, no launcher
        }
        LogStart(dir);
    }
    printf("bwport: Android, working directory %s\n", dir != NULL ? dir : "(none)");
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait LandscapeLeft LandscapeRight");
    // sound: AAudio first (Android 8+). SDL 2.32's default, OpenSL ES, ran dry with the game's 256-frame callbacks and restarted
    // its AudioTrack ~90 times a second in the emulator (logcat "AudioTrack: stop"); AAudio did not once
    if (getenv("SDL_AUDIODRIVER") == NULL) {
        setenv("SDL_AUDIODRIVER", "aaudio,openslES,android", 1);
    }
    PORT_AndroidDiag();
    PORT_AndroidStart();
    return SIM_main(argc, argv);
}
#endif
