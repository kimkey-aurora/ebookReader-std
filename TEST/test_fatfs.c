#include "src/ff.h"
#include "delay/delay.h"
#include "usart/usart.h"
#include "test_fatfs.h"

#include "string.h"

#include "debug_flag.h"


#define FRES_ERROR(msg, FORMAT, f_res) 	\
do {									\
	printf(FORMAT, (msg));				\
	print_fres(f_res); 					\
} while(0)


static void print_fres(FRESULT fr_index) {
	const char fr_str[] =
		"OK\0" 				"DISK_ERR\0" 			"INT_ERR\0" 				"NOT_READY\0"
		"NO_FILE\0"			"NO_PATH\0"				"INVALID_NAME\0" 			"DENIED\0" 			
		"EXIST\0" 			"INVALID_OBJECT\0" 		"WRITE_PROTECTED\0"			"INVALID_DRIVE\0"
		"NOT_ENABLED\0" 	"NO_FILE_SYSTEM\0" 		"MKFS_ABORTED\0" 			"TIMEOUT\0"
		"LOCKED\0" 			"NOT_ENOUGH_CORE\0" 	"TOO_MANY_OPEN_FILES\0" 	"INVALID_PARAMETER\0";
	printf("f_res = %u FR_%s\r\n", (UINT)fr_index, fr_str+fr_index);
}


void test_fatfs(void)
{
	FATFS fs;
	FIL file;
	FILINFO finfo;
	DIR dir;
	FRESULT f_res;
	BYTE work[FF_MAX_SS];
	UINT len, byte_num;
	char buffer[255];
	char path[30] = "sub1";
	const char* filename = "test.txt";
	
	//挂载SD卡
	f_res = f_mount(&fs, "0", 1);
	
	//确保建立文件系统
	if (f_res == FR_NO_FILESYSTEM) {
		printf("There is no filesystem in SD card.\r\n");
		printf("Try to make fat file system\r\n");
		f_res = f_mkfs("0", 0, work, sizeof(work));
		if (f_res == FR_OK) {
			printf("Succeed to make fat file system \r\n");
		}
		else {
			FRES_ERROR("Fail to mkfs.", "%s\r\n", f_res);
		}
	} else{
		printf("Fat file system already exists in SD card\r\n");
	}
	
	// 访问文件夹 SUB1
	printf("\r\nTry to open directory SUB1\r\n");
	f_res = f_opendir(&dir, path);
	if (f_res == FR_NO_PATH)	/* 若文件夹不存在则创建 */
	{
		printf("Direcotory doesn't exist!\r\n");
		printf("Try to create directory [%s]\r\n", path);
		f_res = f_mkdir(path);
		if (!f_res)
			printf("Succeed to create directory [%s]\r\n", path);
		else
		{
			FRES_ERROR("Directory create fail!", "%s\r\n", f_res);
		}
	}
	
	// 遍历目录 SUB1
	if (f_res == FR_OK) {
		while(1) {
            f_res = f_readdir(&dir, &finfo);  	
            if (f_res != FR_OK || finfo.fname[0] == 0) break;
			printf("/%s/%s", path, finfo.fname);
            if (finfo.fattrib & AM_DIR) {	// 目录
				printf("/");
            }
			printf("\r\n");
		}
	}
	
	// 打开 /%path/%filename 文件，进行写入测试
	len = strlen(path);
	//printf("strlen(path) = %d\r\n", len);
	sprintf(&path[len], "/%s", filename);
	sprintf(buffer, "%s", "Hello world! 你好 世界！");
	f_res = f_open(&file, "sub1/test.txt", FA_WRITE | FA_CREATE_ALWAYS);
	if (f_res == FR_OK) {
		printf("Open success! Running for write test...\r\n");
		printf("message: %s\r\ndest_file: %s\r\n", buffer, path);
		printf("Writing...\r\n");
		f_res = f_write(&file, buffer, strlen(buffer), &byte_num);
		if (f_res == FR_OK) {
			printf("Message has been written. Written bytes: %d\r\n", byte_num);
			printf("File closed\r\n");
		} else {
			FRES_ERROR("Write fail.", "%s\r\n", f_res);
		}
		f_res = f_close(&file);
	} else {
		FRES_ERROR("Open error.", "%s", f_res);
	}
	//path[len] = 0;	// 恢复目录路径
	
	// 打开 /%path/%filename 进行读出测试
	f_res = f_open(&file, path, FA_READ);
	if (f_res == FR_OK) {
		f_res = f_read(&file, buffer, sizeof(buffer), &byte_num);
		if (f_res == FR_OK) {
			printf("Read message success!\r\n");
			printf("%s\r\n", buffer);
		} else {
			FRES_ERROR("Read file fail.", "%s\r\n", f_res);
		}
		f_res = f_close(&file);
	} else {
		FRES_ERROR("Open file fail.", "%s\r\n", f_res);
	}
}
