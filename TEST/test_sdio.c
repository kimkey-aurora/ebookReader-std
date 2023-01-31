#include "led/led.h"
#include "delay/delay.h"
#include "usart/usart.h"
#include "sdio/sdio_sdcard.h"
#include "test_sdio.h"

void test_sdio_init(void)
{
	delay_init();	    	 							//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
 	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	
	while(SD_Init())			//����ⲻ��SD��
	{
		printf("SD Card Error!\r\n");
		printf("Please Check!\r\n");
		printf("\r\n");
		delay_ms(1000);
		LED0=!LED0;				//DS0��˸
	}
	//���SD���ɹ�
	printf("\r\n\r\nGet SD card success!\r\n\r\n");
	printf("\r\nThe information of your SD card are as follows:\r\n\r\n");
	show_sdcard_info();	//��ӡSD�������Ϣ
	while(1)
	{
		delay_ms(500);
		LED0=!LED0;		//DS0��˸
	}
}

//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:  	/*��׼���� V1.1 SD��*/
			printf("Card Type:SDSC V1.1\r\n");
			break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:	/*��׼���� V2.0 SD��*/
			printf("Card Type:SDSC V2.0\r\n");
			break;
		case SDIO_HIGH_CAPACITY_SD_CARD:		/*������SD��*/
			printf("Card Type:SDHC V2.0\r\n");
			break;
		case SDIO_MULTIMEDIA_CARD:				/*MMC��*/
			printf("Card Type:MMC Card\r\n");
			break;
	}
	printf("Card ManufacturerID:%d\r\n", SDCardInfo.SD_cid.ManufacturerID);		//������ID
 	printf("Card RCA:%d\r\n", SDCardInfo.RCA);									//����Ե�ַ
	printf("Card Capacity:%d MB\r\n", (u32)(SDCardInfo.CardCapacity>>20));		//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n", SDCardInfo.CardBlockSize);				//��ʾ���С
}
