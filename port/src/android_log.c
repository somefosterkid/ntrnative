// Android: the log files, as iOS's port/src/ios_log.c keeps them. log.txt in the working directory is this run's log (android_main.c's
// LogThread writes it, tools/android_run.sh reads it); at each start the last run's log.txt moves to logs/bwport-<YYYYMMDD-HHMMSS>.log
// (its last-change time) and only the newest KEEP_LOGS of those stay. A log past LOG_CAP starts over (the settings screen's "Share latest
// log" sends log.txt and the newest of logs/: LogShare.java). PORT_LogSettings (main_start.c, once sim_config.ini exists) lists the
// settings in effect, like iOS: sim_config.ini's [General] lines and the BWPORT_ variables that override them.
#ifdef __ANDROID__
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define KEEP_LOGS 10

// the newest KEEP_LOGS-1 stay (names sort by time), so that with the one about to be made KEEP_LOGS do
static void PruneLogs(const char *dir) {
    static char names[64][64];
    int n = 0, i, j;
    DIR *d = opendir(dir);
    struct dirent *e;

    if (d == NULL) {
        return;
    }
    while ((e = readdir(d)) != NULL && n < 64) {
        if (strncmp(e->d_name, "bwport-", 7) == 0 && strlen(e->d_name) < 64) {
            strcpy(names[n++], e->d_name);
        }
    }
    closedir(d);
    for (i = 0; i < n; i++) {    // oldest first
        for (j = i + 1; j < n; j++) {
            if (strcmp(names[j], names[i]) < 0) {
                char t[64];
                strcpy(t, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], t);
            }
        }
    }
    for (i = 0; i < n - (KEEP_LOGS - 1); i++) {
        char path[1200];
        snprintf(path, sizeof(path), "%s/%s", dir, names[i]);
        unlink(path);
    }
}

// before this run's log.txt is opened: the last run's goes into logs/
void PORT_AndroidLogRotate(const char *dir) {
    char cur[1100], ldir[1100], path[1200], stamp[32];
    struct stat st;
    struct tm tm;

    snprintf(cur, sizeof(cur), "%s/log.txt", dir);
    if (stat(cur, &st) != 0 || st.st_size == 0) {
        return;
    }
    snprintf(ldir, sizeof(ldir), "%s/logs", dir);
    mkdir(ldir, 0755);
    PruneLogs(ldir);
    localtime_r(&st.st_mtime, &tm);
    strftime(stamp, sizeof(stamp), "%Y%m%d-%H%M%S", &tm);
    snprintf(path, sizeof(path), "%s/bwport-%s.log", ldir, stamp);
    rename(cur, path);
}

void PORT_LogSettings(void) {
    static const char *const vars[] = {"BWPORT_RENDERER", "BWPORT_RESOLUTION", "BWPORT_GAMESPEED", "BWPORT_FRAMESKIP", "BWPORT_KEYS",
                                       "BWPORT_HIDDEN", "BWPORT_SHOTS", "BWPORT_FPSLOG", "BWPORT_CRASHTEST"};
    FILE *f = fopen("sim_config.ini", "r");
    char line[256];
    int general = 0;
    unsigned i;

    printf("bwport: settings (sim_config.ini [General]; the renderer 'auto' is chosen at start, the status line says which):\n");
    while (f != NULL && fgets(line, sizeof(line), f) != NULL) {
        if (line[0] == '[') {
            general = strncmp(line, "[General]", 9) == 0;
        } else if (general && strchr(line, '=') != NULL) {
            printf("bwport:   %s", line);
        }
    }
    if (f != NULL) {
        fclose(f);
    }
    for (i = 0; i < sizeof(vars) / sizeof(vars[0]); i++) {
        if (getenv(vars[i]) != NULL) {
            printf("bwport:   env %s=%s\n", vars[i], getenv(vars[i]));
        }
    }
}
// ---- the heat state, Power Saver and the panel rate (Platform.java; iOS: apple_thermal.m, ios_input.m) -----------------------------
// PowerManager's thermal status (API 29+): none, light, moderate, severe, critical, emergency, shutdown. Logged at start (PORT_AndroidDiag
// asks for the line) and at each change; the system metrics line shows it (sysmetrics.c, "thermal ...").
#include <jni.h>
#include <time.h>

static volatile int sThermal = -1;      // -1 = not known (before API 29 or before Java told us)
static volatile int sLowPower = -1;
static time_t sStart;

static const char *ThermalName(int s) {
    switch (s) {
    case 0: return "none";
    case 1: return "light";
    case 2: return "moderate";
    case 3: return "severe";
    case 4: return "critical";
    case 5: return "emergency";
    case 6: return "shutdown";
    }
    return "unknown";
}

const char *bwport_thermal_name(void) {
    return sThermal < 0 ? NULL : ThermalName(sThermal);
}

// the log's head: the state as it is now
void PORT_AndroidThermalLine(void) {
    if (sThermal >= 0) {
        printf("thermal: %s\n", ThermalName(sThermal));
    }
    if (sLowPower >= 0) {
        printf("low power mode: %s\n", sLowPower ? "on" : "off");
    }
}

JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_Platform_nativeThermal(JNIEnv *env, jclass c, jint s) {
    int was = sThermal;
    int secs = sStart != 0 ? (int)(time(NULL) - sStart) : 0;

    (void)env;
    (void)c;
    if (sStart == 0) {
        sStart = time(NULL);
    }
    sThermal = s;
    if (was >= 0 && was != s) {
        fprintf(stderr, "thermal: %s (was %s) at %d s\n", ThermalName(s), ThermalName(was), secs);
    }
}

JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_Platform_nativeLowPower(JNIEnv *env, jclass c, jint on) {
    int was = sLowPower;
    int secs = sStart != 0 ? (int)(time(NULL) - sStart) : 0;

    (void)env;
    (void)c;
    if (sStart == 0) {
        sStart = time(NULL);
    }
    sLowPower = on;
    if (was >= 0 && was != on) {
        fprintf(stderr, "low power mode: %s (was %s) at %d s\n", on ? "on" : "off", was ? "on" : "off", secs);
    }
}

// the panel-rate vote (Surface.setFrameRate), once per change: iOS's CADisplayLink preferredFrameRateRange
JNIEXPORT __attribute__((visibility("default"))) void JNICALL Java_com_bwport_app_Platform_nativeRate(JNIEnv *env, jclass c, jfloat hz, jint mode) {
    (void)env;
    (void)c;
    fprintf(stderr, "bwport: display: asking the panel for %.0f Hz (%s)\n", (double)hz,
            mode == 2 ? "Smooth motion on" : mode == 1 ? "60 fps game" : "settings open");
}
#endif
