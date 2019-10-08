#pragma once
#include <stdint.h>
#include <semaphore.h>

#define SHM_NAME "/shmSANDBOX"
#define MTX_NAME "/mtxSANDBOX"

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

typedef struct ShmInfo {
	void *ptr;
	uint32_t size;
} ShmInfo;

typedef struct SharedData {
	volatile int32_t u;
} SharedData;
