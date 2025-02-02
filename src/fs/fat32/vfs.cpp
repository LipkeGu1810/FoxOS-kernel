#include <fs/fat32/vfs.h>
#include <fs/fat32/ff.h>

#include <memory/memory.h>
#include <memory/heap.h>

#include <paging/page_frame_allocator.h>

#include <scheduling/scheduler/errno.h>

#include <string.h>
#include <assert.h>

vfs_mount* initialise_fat32(int disk_id) {
	vfs_mount* mount = new vfs_mount;
	memset(mount, 0, sizeof(mount));

	mount->data2 = disk_id;

	mount->mount = fat32_mount;
	mount->unmount = fat32_unmount;

	mount->open = fat32_open;
	mount->close = fat32_close;
	mount->write = fat32_write;
	mount->read = fat32_read;

	mount->opendir = fat32_opendir;
	mount->closedir = fat32_closedir;
	mount->readdir = fat32_readdir;
	mount->rewinddir = fat32_rewinddir;

	mount->mkdir = fat32_mkdir;
	mount->unlink = fat32_unlink;
	mount->rename = fat32_rename;

	return mount;
}


void fat32_mount(vfs_mount* node) {
	assert(node->data2 < 10);

	FATFS* fs = (FATFS*) global_allocator.request_page(); // dont ask why but fatfs doesent like heap addresses
	fs->pdrv = node->data2;

	char path[3] = "0:";
	path[0] += node->data2;
	f_mount(fs, path, 1);

	node->data = fs;
}

void fat32_unmount(vfs_mount* node) {
	global_allocator.free_page(node->data);
}

FILE* fat32_open(vfs_mount* node, const char* file, const char* mode) {
	FIL fil;
	BYTE fatmode;
	FILE* fp = new FILE;

	if (strcmp((char*)mode, (char*)"r") == 0) {
		fatmode = FA_READ;
		fp->is_readable = 1;
		fp->is_writable = 0;
	} else if (strcmp((char*)mode, (char*)"w") == 0) {
		fatmode = FA_WRITE;
		fp->is_readable = 0;
		fp->is_writable = 1;
	} else if (strcmp((char*)mode, (char*)"a") == 0) {
		fatmode = FA_OPEN_APPEND;
		fp->is_readable = 0;
		fp->is_writable = 1;
	} else if (strcmp((char*)mode, (char*)"r+") == 0) {
		fatmode = FA_OPEN_ALWAYS | FA_WRITE | FA_READ;
		fp->is_readable = 1;
		fp->is_writable = 1;
	} else if (strcmp((char*)mode, (char*)"w+") == 0) {
		fatmode = FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
		fp->is_readable = 1;
		fp->is_writable = 1;
	} else if (strcmp((char*)mode, (char*)"a+") == 0) {
		fatmode = FA_OPEN_ALWAYS | FA_OPEN_APPEND | FA_READ;
		fp->is_readable = 1;
		fp->is_writable = 1;
	}

	char* filepath = (char*) "0:";
	filepath[0] += node->data2;
	filepath[2] = '\0';
	strcat(filepath, file);

	FRESULT fr = f_open(&fil, filepath, fatmode);
	if (fr != FR_OK) {
		fp->is_error = 1;
		set_task_errno(vfs_result::VFS_FILE_NOT_FOUND);
		return fp;
	}

	fp->size = f_size(&fil);
	fp->data = (void*) malloc(sizeof(FIL));
	memcpy(fp->data, &fil, sizeof(FIL));

	return fp;
}

int fat32_close(vfs_mount* node, file_t* stream) {
	f_close((FIL*) stream->data);
	free(stream->data);

	return 0;
}

size_t fat32_read(vfs_mount*, void* buffer, size_t size, size_t nmemb, file_t* stream) {
	unsigned int has_read;
	FRESULT res = f_read((FIL*)stream->data, buffer, size, &has_read);
	return has_read;
}

size_t fat32_write(vfs_mount*, void* buffer, size_t size, size_t nmemb, file_t* stream) {
	unsigned int has_written;
	FRESULT res = f_write((FIL*)stream->data, buffer, size, &has_written);
	return has_written;
}

DIR* fat32_opendir(vfs_mount* node, const char* name) {
	FATDIR dir;
	DIR* dp = new DIR;

	FRESULT fr = f_opendir(&dir, name);
	if (fr != FR_OK) {
		dp->is_error = 1;
		set_task_errno(vfs_result::VFS_FILE_NOT_FOUND);
		return dp;
	}

	dp->data = (void*) malloc(sizeof(FATDIR));
	memcpy(dp->data, &dir, sizeof(FATDIR));

	return dp;
}

int fat32_closedir(vfs_mount*, DIR* stream) {
	f_closedir((FATDIR*) stream->data);
	free(stream->data);

	return 0;
}

struct dirent* fat32_readdir(vfs_mount*, DIR* stream) {
	FILINFO flinf;
	FRESULT fr = f_readdir((FATDIR*)stream->data, &flinf);
	if (fr != FR_OK) {
		return NULL;
	}

	dirent* out = new dirent;
	out->ino = -1;
	for (int i = 0; i < 13; i++) {
		out->name[i] = flinf.fname[i];
	}

	return out;
}

void fat32_rewinddir(vfs_mount*, DIR* stream) {
	f_readdir((FATDIR*) stream->data, 0);
}

int fat32_mkdir(vfs_mount*, const char* name, uint32_t) {
	FRESULT fr = f_mkdir(name);
	if (fr != FR_OK) {
		return -1;
	}

	return 0;
}

int fat32_unlink(vfs_mount*, const char* name) {
	FRESULT fr = f_unlink(name);
	if (fr != FR_OK) {
		return -1;
	}

	return 0;
}

int fat32_rename(vfs_mount*, const char* old_name, const char* new_name) {
	FRESULT fr = f_rename(old_name, new_name);
	if (fr != FR_OK) {
		return -1;
	}

	return 0;
}