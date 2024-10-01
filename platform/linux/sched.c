#include <errno.h>
#include <pthread.h>
#include <time.h>

#include "platform.h"

int sched_ctx_init(struct sched_ctx *ctx) { return 0; }

int sched_ctx_destroy(struct sched_ctx *ctx) { return 0; }

int sched_sleep(struct sched_ctx *ctx, mutex_t *mutex,
                const struct timespec *abstime) {
  return 0;
}

int sched_wakeup(struct sched_ctx *ctx) { return 0; }

int sched_interrupt(struct sched_ctx *ctx) { return 0; }