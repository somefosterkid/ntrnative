/* port/vcore: tasks on persistent threads. The frame used to create threads every frame (the 3D, engine B,
 * and the bridge's worker: ~1650 a second at 550 fps); on Windows that is a kernel thread create and exit with a DLL
 * attach/detach notification for every loaded DLL (the graphics driver's too), a fresh 8 MB stack and its page faults
 * each time. A task now runs on an idle thread of a small cache (a thread that finished its task waits for the next);
 * a new thread is made only when none is idle. The output is the same: a task is a function run on another thread,
 * joined by the caller. BWPORT_VCORE_POOL=0 creates a thread per task again (timing it against the pool). */
#define _POSIX_C_SOURCE 200809L
#if defined(__APPLE__)
#define _DARWIN_C_SOURCE /* pthread_setname_np */
#endif
#include "vcore.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct TWorker {
  pthread_t th;
  pthread_cond_t go;
  void *(*fn)(void *);
  void *arg;
  VCTask *task;
  struct TWorker *next; /* the idle list */
} TWorker;

static pthread_mutex_t sM = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sDone = PTHREAD_COND_INITIALIZER;
static TWorker *sIdle;
static int sPool = -1;

static int new_thread(pthread_t *t, void *(*fn)(void *), void *arg) {
  pthread_attr_t at;
  if (pthread_attr_init(&at) != 0) return 0;
  pthread_attr_setstacksize(&at, 8u << 20); /* the rasterizer's locals */
  int ok = pthread_create(t, &at, fn, arg) == 0;
  pthread_attr_destroy(&at);
  return ok;
}

static void *worker_main(void *a) {
  TWorker *w = (TWorker *)a;
#if defined(__APPLE__)
  pthread_setname_np("bwport-worker"); /* the per-thread CPU readout (port/src/sysmetrics.c) */
#endif
  pthread_mutex_lock(&sM);
  for (;;) {
    while (w->fn == NULL) pthread_cond_wait(&w->go, &sM);
    void *(*fn)(void *) = w->fn;
    void *arg = w->arg;
    VCTask *t = w->task;
    w->fn = NULL;
    pthread_mutex_unlock(&sM);
    fn(arg);
    pthread_mutex_lock(&sM);
    w->next = sIdle; /* idle before the join returns: the caller's next task finds this thread */
    sIdle = w;
    t->done = 1;
    pthread_cond_broadcast(&sDone);
  }
  return NULL;
}

int vcore_task_start(VCTask *t, void *(*fn)(void *), void *arg) {
  if (sPool < 0) {
    const char *e = getenv("BWPORT_VCORE_POOL");
    sPool = !(e != NULL && e[0] == '0');
  }
  memset(t, 0, sizeof *t);
  if (!sPool) {
    pthread_t *th = (pthread_t *)malloc(sizeof *th);
    if (th == NULL || !new_thread(th, fn, arg)) {
      free(th);
      return 0;
    }
    t->legacy = th;
    return 1;
  }
  pthread_mutex_lock(&sM);
  TWorker *w = sIdle;
  if (w != NULL) {
    sIdle = w->next;
    w->fn = fn;
    w->arg = arg;
    w->task = t;
    pthread_cond_signal(&w->go);
    pthread_mutex_unlock(&sM);
    return 1;
  }
  pthread_mutex_unlock(&sM);
  w = (TWorker *)calloc(1, sizeof *w);
  if (w == NULL) return 0;
  pthread_cond_init(&w->go, NULL);
  w->fn = fn; /* it starts with its task */
  w->arg = arg;
  w->task = t;
  if (!new_thread(&w->th, worker_main, w)) {
    free(w);
    return 0;
  }
  return 1;
}

void vcore_task_join(VCTask *t) {
  if (t->legacy != NULL) {
    pthread_join(*(pthread_t *)t->legacy, NULL);
    free(t->legacy);
    t->legacy = NULL;
    return;
  }
  pthread_mutex_lock(&sM);
  while (!t->done) pthread_cond_wait(&sDone, &sM);
  pthread_mutex_unlock(&sM);
}

int vcore_task_done(VCTask *t) {
  if (t->legacy != NULL) return 0;
  pthread_mutex_lock(&sM);
  int d = t->done;
  pthread_mutex_unlock(&sM);
  return d;
}
