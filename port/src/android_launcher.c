// Android: the launcher's bridge (docs/launcher.md, "Android launcher"). LauncherCore.java (tools/android/app/src/main/java/com/bwport/app/)
// calls the C core (launcher_core.c, launcher_cover.c) through these functions, in the app's MAIN process, where LauncherActivity runs:
// the launcher loads libmain.so for them without starting the game (the game runs in the ":game" process, BwportActivity: SDL's
// SDL_main is called only there). The UI lists and calls; the work (game table, import with validation and extraction, removal, saves,
// backups, covers, launcher.ini, the migration) is the same C as iOS's and the desktop's.
// PORT_HttpGet is here too: HttpURLConnection through LauncherCore.httpGet (port/src/launcher_core.h), https only, 15 s, 10 MB.
#ifdef __ANDROID__
#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "launcher_core.h"

int PORT_ExtractPercent(void);

#define JFN(name) JNIEXPORT __attribute__((visibility("default"))) JNICALL Java_com_bwport_app_LauncherCore_##name

static JavaVM *sVm;
static jclass sCore;    // LauncherCore (a global reference), for PORT_HttpGet's call back into Java

static void Log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
static void Log(const char *fmt, ...) {
    char b[600];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(b, sizeof(b), fmt, ap);
    va_end(ap);
    __android_log_write(ANDROID_LOG_INFO, "bwport", b);
}

static const char *Str(JNIEnv *env, jstring s, char *buf, size_t n) {
    const char *u;

    buf[0] = 0;
    if (s == NULL) {
        return buf;
    }
    u = (*env)->GetStringUTFChars(env, s, NULL);
    if (u != NULL) {
        snprintf(buf, n, "%s", u);
        (*env)->ReleaseStringUTFChars(env, s, u);
    }
    return buf;
}

static jstring JStr(JNIEnv *env, const char *s) {
    return (*env)->NewStringUTF(env, s != NULL ? s : "");
}

static const LC_Game *ByCode(JNIEnv *env, jstring code) {
    char c[16];

    return LC_GameByCode(Str(env, code, c, sizeof(c)));
}

// The core's folders: the root is the app's external files directory (adb push reaches it); the extracted files go in the game's
// own folder there too (the desktop's and the old APK's layout). Then the one-time migration of the old flat layout (docs/launcher.md,
// section 6): "" = nothing to do or done, else why it failed (the flat layout stays; the game runs from it as before)
jstring JFN(nativeInit)(JNIEnv *env, jclass cls, jstring root) {
    char r[1100], err[600] = "";

    (void)cls;
    Str(env, root, r, sizeof(r));
    LC_Init(r, r);
    if (LC_IsFlat()) {
        int m = LC_Migrate(err, sizeof(err));
        Log("launcher: migration of the flat layout: %d %s", m, err);
        if (m < 0) {
            return JStr(env, err[0] ? err : "The files could not be moved into the game's folder.");
        }
    }
    return JStr(env, "");
}

// One line per game under the root that has a ROM copy or extracted files, tab-separated:
// code title shortName playable(0/1) hasRom hasFiles hasSave playSeconds(-1) savedEpoch(0) coverSource coverLink
jobjectArray JFN(nativeGames)(JNIEnv *env, jclass cls) {
    const LC_Game *found[8];
    char line[1800];
    int n = LC_Scan(found, 8), i, k = 0;
    jclass sc = (*env)->FindClass(env, "java/lang/String");
    jobjectArray a;

    (void)cls;
    a = (*env)->NewObjectArray(env, n, sc, NULL);
    for (i = 0; i < n; i++) {
        const LC_Game *g = found[i];
        char dir[1200], path[1300], name[64] = "", link[256] = "";
        long secs = -1;
        time_t when = 0;
        int hasSave = LC_HasSave(g) != 0;

        if (!LC_HasRom(g) && !LC_HasFiles(g)) {
            continue;
        }
        LC_GameDir(g, dir, sizeof(dir));
        snprintf(path, sizeof(path), "%s/save.bin", dir);
        if (hasSave && LC_SaveFileInfo(g, path, &secs, &when) != 0) {
            struct stat st;

            secs = -1;
            when = stat(path, &st) == 0 ? st.st_mtime : 0;
        }
        LC_CoverSource(g, name, sizeof(name), link, sizeof(link));
        snprintf(line, sizeof(line), "%s\t%s\t%s\t%d\t%d\t%d\t%d\t%ld\t%lld\t%s\t%s", g->code, g->title, g->shortName, g->sha1[0] != NULL,
                 LC_HasRom(g) != 0, LC_HasFiles(g) != 0, hasSave, secs, (long long)when, name, link);
        (*env)->SetObjectArrayElement(env, a, k++, JStr(env, line));
    }
    // (a shorter array when a folder was skipped: the unused tail is NULL, Java stops at it)
    return a;
}

jstring JFN(nativeGameDir)(JNIEnv *env, jclass cls, jstring code) {
    const LC_Game *g = ByCode(env, code);
    char dir[1200] = "";

    (void)cls;
    if (g != NULL) {
        LC_GameDir(g, dir, sizeof(dir));
    }
    return JStr(env, dir);
}

// "" = added (the game's code in out[0]), else the sentence for the user
jstring JFN(nativeImportRom)(JNIEnv *env, jclass cls, jstring path, jobjectArray out) {
    char p[1100], err[600] = "";
    const LC_Game *g = NULL;
    int r;

    (void)cls;
    Str(env, path, p, sizeof(p));
    Log("launcher: import ROM %s", p);
    r = LC_ImportRom(p, &g, err, sizeof(err));
    Log("launcher: import ROM -> %d %s", r, err);
    if (r != 0) {
        return JStr(env, err[0] ? err : "The ROM could not be added.");
    }
    if (g != NULL && out != NULL) {
        (*env)->SetObjectArrayElement(env, out, 0, JStr(env, g->code));
    }
    return JStr(env, "");
}

jint JFN(nativeCopyPercent)(JNIEnv *env, jclass cls) {
    (void)env;
    (void)cls;
    return LC_CopyPercent();
}

jint JFN(nativeExtractPercent)(JNIEnv *env, jclass cls) {
    (void)env;
    (void)cls;
    return PORT_ExtractPercent();
}

// Extract a game's files from its ROM copy (a move from the old layout that had none): the count of files
jint JFN(nativeExtract)(JNIEnv *env, jclass cls, jstring code) {
    const LC_Game *g = ByCode(env, code);
    char rom[1300], dir[1200], work[1200];

    (void)cls;
    if (g == NULL) {
        return 0;
    }
    LC_GameDir(g, dir, sizeof(dir));
    LC_GameWorkDir(g, work, sizeof(work));
    snprintf(rom, sizeof(rom), "%s/%s.nds", dir, g->folder);
    mkdir(work, 0755);
    return LC_ExtractRom(rom, work);
}

jstring JFN(nativeRemove)(JNIEnv *env, jclass cls, jstring code) {
    const LC_Game *g = ByCode(env, code);
    char err[600] = "";

    (void)cls;
    if (g == NULL || LC_RemoveRom(g, err, sizeof(err)) != 0) {
        return JStr(env, err[0] ? err : "The game is not known.");
    }
    Log("launcher: removed %s", g->title);
    return JStr(env, "");
}

// Export: the file's path in dir, or "!" and then the error in out[0]
jstring JFN(nativeExportSave)(JNIEnv *env, jclass cls, jstring code, jstring dir, jobjectArray out) {
    const LC_Game *g = ByCode(env, code);
    char d[1100], path[1300], err[600] = "";

    (void)cls;
    Str(env, dir, d, sizeof(d));
    if (g == NULL || LC_ExportSave(g, d, path, sizeof(path), err, sizeof(err)) != 0) {
        if (out != NULL) {
            (*env)->SetObjectArrayElement(env, out, 0, JStr(env, err[0] ? err : "The save could not be exported."));
        }
        return JStr(env, "!");
    }
    return JStr(env, path);
}

jstring JFN(nativeImportSave)(JNIEnv *env, jclass cls, jstring code, jstring path) {
    const LC_Game *g = ByCode(env, code);
    char p[1100], err[600] = "";

    (void)cls;
    Str(env, path, p, sizeof(p));
    if (g == NULL || LC_ImportSave(g, p, err, sizeof(err)) != 0) {
        Log("launcher: import save: %s", err);
        return JStr(env, err[0] ? err : "The save could not be imported.");
    }
    Log("launcher: save imported into %s", g->title);
    return JStr(env, "");
}

// The play time and date of a save file the user is about to import: "seconds\tepoch", "" when it has none
jstring JFN(nativeSaveFileInfo)(JNIEnv *env, jclass cls, jstring path) {
    char p[1100], b[80];
    long secs = 0;
    time_t when = 0;

    (void)cls;
    Str(env, path, p, sizeof(p));
    if (LC_SaveFileInfo(NULL, p, &secs, &when) != 0) {
        return JStr(env, "");
    }
    snprintf(b, sizeof(b), "%ld\t%lld", secs, (long long)when);
    return JStr(env, b);
}

// Backups, newest first: name \t epoch \t kind (0 before a change, 1 kept copy, 2 before the library)
jobjectArray JFN(nativeBackups)(JNIEnv *env, jclass cls, jstring code) {
    const LC_Game *g = ByCode(env, code);
    static LC_BackupInfo b[256];
    int n = g != NULL ? LC_ListBackups(g, b, 256) : 0, i;
    jobjectArray a = (*env)->NewObjectArray(env, n, (*env)->FindClass(env, "java/lang/String"), NULL);

    (void)cls;
    for (i = 0; i < n; i++) {
        char line[200];

        snprintf(line, sizeof(line), "%s\t%lld\t%d", b[i].name, (long long)b[i].when, b[i].kind);
        (*env)->SetObjectArrayElement(env, a, i, JStr(env, line));
    }
    return a;
}

jstring JFN(nativeRestore)(JNIEnv *env, jclass cls, jstring code, jstring name) {
    const LC_Game *g = ByCode(env, code);
    char nm[100], err[600] = "";

    (void)cls;
    Str(env, name, nm, sizeof(nm));
    if (g == NULL || LC_RestoreBackup(g, nm, err, sizeof(err)) != 0) {
        return JStr(env, err[0] ? err : "The backup could not be restored.");
    }
    Log("launcher: restored %s of %s", nm, g->title);
    return JStr(env, "");
}

// launcher.ini: the last game, the start mode, the layout, the cover setting
void JFN(nativeSetLastGame)(JNIEnv *env, jclass cls, jstring code) {
    const LC_Game *g = ByCode(env, code);

    (void)cls;
    if (g != NULL) {
        LC_SetLastGame(g);
    }
}

jstring JFN(nativeLastGame)(JNIEnv *env, jclass cls) {
    const LC_Game *g = LC_LastGame();

    (void)cls;
    return JStr(env, g != NULL ? g->code : "");
}

jint JFN(nativeGet)(JNIEnv *env, jclass cls, jint what) {
    (void)env;
    (void)cls;
    return what == 0 ? LC_StartMode() : what == 1 ? LC_Layout() : what == 2 ? LC_CoverDownloads() : LC_SkipMenu();
}

void JFN(nativeSet)(JNIEnv *env, jclass cls, jint what, jint v) {
    (void)env;
    (void)cls;
    if (what == 0) {
        LC_SetStartMode(v);
    } else if (what == 1) {
        LC_SetLayout(v);
    } else if (what == 2) {
        LC_SetCoverDownloads(v);
    } else {
        LC_SetSkipMenu(v);
    }
}

jboolean JFN(nativeCoverWanted)(JNIEnv *env, jclass cls, jstring code) {
    const LC_Game *g = ByCode(env, code);

    (void)cls;
    return g != NULL && LC_CoverWanted(g);
}

// Blocking (a thread of Java's): "" = written, else why not
jstring JFN(nativeFetchCover)(JNIEnv *env, jclass cls, jstring code) {
    const LC_Game *g = ByCode(env, code);
    char err[400] = "";

    if (sCore == NULL) {
        sCore = (*env)->NewGlobalRef(env, cls);
    }
    if (g == NULL || LC_FetchCover(g, err, sizeof(err)) != 0) {
        return JStr(env, err[0] ? err : "No cover.");
    }
    return JStr(env, "");
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    (void)reserved;
    sVm = vm;
    return JNI_VERSION_1_6;
}

// PORT_HttpGet (launcher_core.h): LauncherCore.httpGet(url, path) does it with HttpURLConnection and returns the status (200 and the
// body in path), or a negative number with the reason in LauncherCore.sWhy. Called on a Java thread (nativeFetchCover's)
int PORT_HttpGet(const char *url, const char *path, char *err, size_t n) {
    JNIEnv *env = NULL;
    jmethodID m, w;
    jstring ju, jp, why;
    jint r;

    if (sVm == NULL || sCore == NULL || (*sVm)->GetEnv(sVm, (void **)&env, JNI_VERSION_1_6) != JNI_OK || env == NULL) {
        snprintf(err, n, "not available here");
        return -1;
    }
    m = (*env)->GetStaticMethodID(env, sCore, "httpGet", "(Ljava/lang/String;Ljava/lang/String;)I");
    w = (*env)->GetStaticMethodID(env, sCore, "httpWhy", "()Ljava/lang/String;");
    if (m == NULL || w == NULL) {
        (*env)->ExceptionClear(env);
        snprintf(err, n, "not available here");
        return -1;
    }
    ju = (*env)->NewStringUTF(env, url);
    jp = (*env)->NewStringUTF(env, path);
    r = (*env)->CallStaticIntMethod(env, sCore, m, ju, jp);
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        r = -1;
    }
    why = (jstring)(*env)->CallStaticObjectMethod(env, sCore, w);
    err[0] = 0;
    if (why != NULL) {
        Str(env, why, err, n);
    }
    return r == 200 ? 0 : (r > 0 ? r : -1);
}
#endif
