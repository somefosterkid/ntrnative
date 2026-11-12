// System metrics (port/src/sysmetrics.c): CPU use, a clock estimate, GPU busy time. -1 / 0 = not measurable here.
#ifndef BWPORT_SYSMETRICS_H
#define BWPORT_SYSMETRICS_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    float cores;          // the process's CPU time over wall time, last second
    float mainCores;      // the game thread (-1: not measurable)
    float otherCores;     // cores - main - audio (-1: not measurable)
    float audioCores;     // the audio thread (-1: not measured here)
    float ghz;            // estimated clock of the game thread's core (0: no estimate)
    float gpuMs;          // GPU ms a frame
    float gpuBusy;        // % of the time
    float fenceMs;        // the fence latency (swap to GPU done), 0 = no fences
    float framesPerSec;   // frame loop turns a second
    int gpuKind;          // 0 none, 1 GL timer queries, 2 fence latency (an upper bound)
} bwport_sys_t;

int bwport_sys_frame(void);                 // once a frame, on the game thread; 1 = a second has passed (then call bwport_sys_fence)
void bwport_sys_fence(float meanMs);
int bwport_sys_want_gpu(void);              // run GPU timer queries this frame
void bwport_sys_gpu_timers(int ok);
void bwport_sys_gpu_add(double ms);
void bwport_sys_gpu_end(void);
const bwport_sys_t *bwport_sys_get(void);
void bwport_sys_line(int i, char *buf, int n);   // 0 cpu, 1 clock, 2 gpu, 3 heat state
void bwport_sys_format(char *buf, int n);   // "cpu 2.3 cores (main 0.9, ...), ~3.4 GHz (game thread), gpu ..., thermal fair"
void bwport_sys_threads_mark(void);         // a bench row starts (the game thread)
void bwport_sys_threads_row(const char *name, double frames, double secs);   // it ends: the "threads:" line, CPU a drawn frame by thread name
#ifdef __cplusplus
}
#endif
#endif
