#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>

#include "shared_memory.h"

/* 共有メモリを作成してマップする */
int createShm(char *name, int size, ShmInfo* info)
{
	int ret;
	struct stat stat;

	int fd = shm_open(name, O_RDWR | O_CREAT, FILE_MODE);
	if(fd < 0) {
		printf("open error\n");
	}

	ret = ftruncate(fd, size);
	if(ret < 0) {
		printf("ftruncate error\n");
	}

	ret = fstat(fd, &stat);
	if(ret < 0) {
		printf("unlink error\n");
	}
	
	void *ptr = mmap(NULL,
					 stat.st_size,
					 PROT_READ | PROT_WRITE,
					 MAP_SHARED, fd, 0);
	if(ptr == NULL) {
		printf("mmap error\n");
	}

	ret = close(fd);
	if(ret < 0) {
		printf("close error\n");
	}

	info->ptr = ptr;
	info->size = stat.st_size;

	return ret;
}

int destroyShm(char *name, ShmInfo* info)
{
	int ret = shm_unlink(name);
	if(ret < 0) {
		printf("unlink error\n");
	}
	
	ret = munmap(info->ptr, info->size);
	if(ret < 0) {
		printf("munmap error\n");
	}

	return ret;
}

int main(int argc, char* argv[])
{
	printf("server start\n");
	ShmInfo info;
	
	createShm(SHM_NAME, sizeof(SharedData), &info);
	SharedData *data = info.ptr;
	
	data->u = 0;
	
	while(1) {
		if(data->u == 1) break;
	}

	destroyShm(SHM_NAME, &info);

	printf("server stop\n");

	return 0;
}
