/* What the Windows frame pacer's last-bit spin costs (SpSleepUntil in libntr's sim_main.cpp): the same sleep, alone.
 *   x86_64-w64-mingw32-gcc -O2 -o win_sleep_bench.exe win_sleep_bench.c
 *   win_sleep_bench.exe [frames] [work_us]        (a console program: nothing on the screen)
 * Each row: a pacing variant, 60 Hz ticks (16666.7 us), `work_us` of busy "game work" per tick, then the sleep to the next
 * tick as SpSleepUntil does (a waitable timer for (remaining - spin), then a spin on the counter up to the target).
 * Prints the thread CPU (QueryThreadCycleTime) as a share of the tick (CPU %), the CPU a tick beyond the work (extra us: the spin,
 * the timer calls), and how late the tick began after its target (us: mean / p99 / max; the wake-up jitter the spin has to cover). */
#define _WIN32_WINNT 0x0A00
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static LARGE_INTEGER freq;
static double NowUs(void) { LARGE_INTEGER c; QueryPerformanceCounter(&c); return (double)c.QuadPart * 1e6 / (double)freq.QuadPart; }
typedef LONG (NTAPI *NtSetTimerResolutionFn)(ULONG, BOOLEAN, PULONG);

static HANDLE sTimer;
/* the pacer's sleep: coarse timer to (target - spin), then a spin; returns the us spent spinning */
static void SleepUntil(double target, double spin) {
    for (;;) {
        double now = NowUs();
        if (now >= target) return;
        double rem = target - now;
        if (rem > spin) {
            LARGE_INTEGER due;
            due.QuadPart = -(LONGLONG)((rem - spin) * 10.0);
            if (SetWaitableTimer(sTimer, &due, 0, NULL, NULL, FALSE)) { WaitForSingleObject(sTimer, 100); continue; }
            Sleep((DWORD)((rem - spin) / 1000.0));
        }
    }
}
static int Cmp(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return x < y ? -1 : x > y; }

int main(int argc, char **argv) {
    int frames = argc > 1 ? atoi(argv[1]) : 600;
    double work = argc > 2 ? atof(argv[2]) : 2000.0;
    QueryPerformanceFrequency(&freq);
    HMODULE winmm = LoadLibraryA("winmm.dll");
    FARPROC tbp = GetProcAddress(winmm, "timeBeginPeriod");
    HMODULE nt = LoadLibraryA("ntdll.dll");
    NtSetTimerResolutionFn ntres = (NtSetTimerResolutionFn)GetProcAddress(nt, "NtSetTimerResolution");
    ULONG minr, maxr, curr;
    typedef LONG (NTAPI *NtQ)(PULONG, PULONG, PULONG);
    NtQ q = (NtQ)GetProcAddress(nt, "NtQueryTimerResolution");
    q(&minr, &maxr, &curr);
    printf("timer resolution at start: %.3f ms (finest %.3f ms)\n", curr / 1e4, maxr / 1e4);
    /* cycles a second of this thread, to turn cycles into time */
    ULONG64 c0, c1;
    QueryThreadCycleTime(GetCurrentThread(), &c0);
    double t0 = NowUs();
    while (NowUs() - t0 < 200000.0) {}
    QueryThreadCycleTime(GetCurrentThread(), &c1);
    double cps = (double)(c1 - c0) / ((NowUs() - t0) * 1e-6);
    struct { const char *name; int hires; int period1; int ntres5; double spin; } v[] = {
        {"hires timer + timeBeginPeriod(1), spin 600 (the game)", 1, 1, 0, 600},
        {"  same, spin 300", 1, 1, 0, 300},
        {"  same, spin 150", 1, 1, 0, 150},
        {"  same, spin 0", 1, 1, 0, 0},
        {"hires timer, resolution 0.5 ms, spin 600", 1, 1, 1, 600},
        {"  same, spin 300", 1, 1, 1, 300},
        {"  same, spin 150", 1, 1, 1, 150},
        {"plain timer + timeBeginPeriod(1), spin 2500 (no hires)", 0, 1, 0, 2500},
        {"  same, spin 1500", 0, 1, 0, 1500},
    };
    printf("%d ticks, %.0f us of work a tick; the CPU share is cycles / (cycles a second) / 16666.7 us\n", frames, work);
    printf("%-58s %8s %10s %9s %8s %8s %8s %8s\n", "variant", "CPU %", "extra us", "late mean", "p50", "p90", "p99", "max");
    for (unsigned k = 0; k < sizeof v / sizeof v[0]; k++) {
        if (sTimer) CloseHandle(sTimer);
        sTimer = v[k].hires ? CreateWaitableTimerExW(NULL, NULL, 0x2, 0x1F0003) : CreateWaitableTimerW(NULL, TRUE, NULL);
        if (v[k].period1) ((UINT(WINAPI *)(UINT))tbp)(1);
        if (v[k].ntres5 && ntres) { ULONG a; ntres(5000, TRUE, &a); }
        double *late = malloc(sizeof(double) * frames);
        ULONG64 cs0, cs1;
        double next = NowUs() + 16666.7, wall0 = NowUs(), spinTot = 0;
        QueryThreadCycleTime(GetCurrentThread(), &cs0);
        for (int i = 0; i < frames; i++) {
            double w0 = NowUs();
            while (NowUs() - w0 < work) {}
            SleepUntil(next, v[k].spin);
            double now = NowUs();
            late[i] = now - next;
            next += 16666.7;
            if (late[i] > 16666.7) next = now + 16666.7;   /* fell behind: restart the clock like the pacer */
        }
        QueryThreadCycleTime(GetCurrentThread(), &cs1);
        double wall = NowUs() - wall0;
        double cpuUs = (double)(cs1 - cs0) / cps * 1e6;
        double sum = 0;
        for (int i = 0; i < frames; i++) sum += late[i];
        qsort(late, frames, sizeof(double), Cmp);
        printf("%-58s %8.2f %10.0f %9.1f %8.1f %8.1f %8.1f %8.1f\n", v[k].name, 100.0 * cpuUs / wall, (cpuUs / frames) - work,
               sum / frames, late[frames / 2], late[(int)(frames * 0.9)], late[(int)(frames * 0.99)], late[frames - 1]);
        free(late);
        if (v[k].ntres5 && ntres) { ULONG a; ntres(5000, FALSE, &a); }
        if (v[k].period1) ((UINT(WINAPI *)(UINT))GetProcAddress(winmm, "timeEndPeriod"))(1);
        (void)spinTot;
    }
    return 0;
}
