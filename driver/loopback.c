#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "platform.h"

#include "net.h"
#include "util.h"

#define LOOPBACK_MTU UINT16_MAX /* maximum size of IP datagram */
#define LOOPBACK_QUEUE_LIMIT 16
#define LOOPBACK_IRQ (INTR_IRQ_BASE + 1)

#define PRIV(x) ((struct loopback *)x->priv)

struct loopback {
  int irq;
  mutex_t mutex;
  struct queue_head queue;
};

struct loopback_queue_query {
  uint16_t type;
  size_t len;
  uint8_t data[]; /* flexible array member */
};

static int loopback_transmit(struct net_device *dev, uint16_t type,
                             const uint8_t *data, size_t len, const void *dst) {
}

static int loopback_isr(unsigned int irq, void *id) {}

static struct net_device_ops loopback_ops = {
    .transmit = loopback_transmit,
};

struct net_device *loopback_init(void) {
  struct net_device *dev;
  struct loopback *lo;

  dev = net_device_alloc();
  dev->type = NET_DEVICE_FLAG_LOOPBACK;
  dev->mtu = LOOPBACK_MTU;
  dev->alen = 0;
  dev->hlen = 0;
  dev->flags = NET_DEVICE_FLAG_LOOPBACK;
  dev->ops = &loopback_ops;

  lo = memory_alloc(sizeof(*lo));
  if (!lo) {
    errorf("memory_alloc() failure");
    return NULL;
  }
  lo->irq = LOOPBACK_IRQ;
  mutex_init(&lo->mutex);
  queue_init(&lo->queue);
  dev->priv = lo;

  if (net_device_register(dev) == -1) {
    errorf("net_device_register() failure");
    return NULL;
  }
  intr_request_irq(LOOPBACK_IRQ, loopback_isr, INTR_IRQ_SHARED, dev->name, dev);

  debugf("initialized, dev=%s", dev->name);
  return dev;
}