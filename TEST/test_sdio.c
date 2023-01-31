#include "led/led.h"
#include "delay/delay.h"
#include "usart/usart.h"
#include "sdio/sdio_sdcard.h"
#include "test_sdio.h"

void test_sdio_init(void)
{
	delay_init();	    	 							//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为115200
 	LED_Init();					//初始化与LED连接的硬件接口
	
	while(SD_Init())			//若检测不到SD卡
	{
		printf("SD Card Error!\r\n");
		printf("Please Check!\r\n");
		printf("\r\n");
		delay_ms(1000);
		LED0=!LED0;				//DS0闪烁
	}
	//检测SD卡成功
	printf("\r\n\r\nGet SD card success!\r\n\r\n");
	printf("\r\nThe information of your SD card are as follows:\r\n\r\n");
	show_sdcard_info();	//打印SD卡相关信息
	while(1)
	{
		delay_ms(500);
		LED0=!LED0;		//DS0闪烁
	}
}

//通过串口打印SD卡相关信息
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:  	/*标准容量 V1.1 SD卡*/
			printf("Card Type:SDSC V1.1\r\n");
			break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:	/*标准容量 V2.0 SD卡*/
			printf("Card Type:SDSC V2.0\r\n");
			break;
		case SDIO_HIGH_CAPACITY_SD_CARD:		/*高容量SD卡*/
			printf("Card Type:SDHC V2.0\r\n");
			break;
		case SDIO_MULTIMEDIA_CARD:				/*MMC卡*/
			printf("Card Type:MMC Card\r\n");
			break;
	}
	printf("Card ManufacturerID:%d\r\n", SDCardInfo.SD_cid.ManufacturerID);		//制造商ID
 	printf("Card RCA:%d\r\n", SDCardInfo.RCA);									//卡相对地址
	printf("Card Capacity:%d MB\r\n", (u32)(SDCardInfo.CardCapacity>>20));		//显示容量
 	printf("Card BlockSize:%d\r\n\r\n", SDCardInfo.CardBlockSize);				//显示块大小
}
