#pragma once

#include <semaphore.h>

sem_t *create_shared_memory();

struct SharedSemaphore {
  sem_t *ptr = nullptr;

  SharedSemaphore();
  ~SharedSemaphore();

  SharedSemaphore(const SharedSemaphore &) = delete;
  SharedSemaphore &operator=(const SharedSemaphore &) = delete;
};
