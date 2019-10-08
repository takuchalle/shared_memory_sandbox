#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <unistd.h>

#include "shared_memory.h"

/* Server で作成した共有メモリをマップする */
int mapShm(char *name, ShmInfo* info)
{
	int ret;
	struct stat stat;

	int fd = shm_open(name, O_RDWR, FILE_MODE);
	if(fd < 0) {
		printf("open error\n");
	}

	ret = fstat(fd, &stat);
	if(ret < 0) {
		printf("unlink error\n");
	}

	void* ptr = mmap(NULL,
			   stat.st_size,
			   PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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

int unmapShm(ShmInfo *info) {
	int ret = munmap(info->ptr, info->size);
	if(ret < 0) {
		printf("munmap error\n");
	}
	return ret;
}

int main(int argc, char* argv[])
{
	printf("client start\n");
	ShmInfo info;
	
	mapShm(SHM_NAME, &info);
	SharedData *data = info.ptr;

	data->u = 1;
	
	unmapShm(&info);

	printf("client stop\n");
	
	return 0;
}
