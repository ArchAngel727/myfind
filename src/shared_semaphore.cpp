#include "../headers/shared_semaphore.hpp"
#include <cstdio>
#include <semaphore.h>
#include <sys/mman.h>

sem_t *create_shared_memory() {
  void *memory = mmap(nullptr, sizeof(sem_t), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  if (memory == MAP_FAILED) {
    perror("mmap");
    return nullptr;
  }

  auto *output_lock = static_cast<sem_t *>(memory);

  if (sem_init(output_lock, 1, 1) == -1) {
    perror("sem_init");
    munmap(memory, sizeof(sem_t));
    return nullptr;
  }

  return output_lock;
}

SharedSemaphore::SharedSemaphore() : ptr(create_shared_memory()) {}

SharedSemaphore::~SharedSemaphore() {
  if (ptr != nullptr) {
    sem_destroy(ptr);
    munmap(ptr, sizeof(sem_t));
  }
}
