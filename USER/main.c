#include "stm32f10x.h"
#include "delay/delay.h"
#include "usart/usart.h"

// Hardware
#include "led/led.h"
#include "sdio/sdio_sdcard.h"

// TEST
#include "debug_flag.h"

#if DEV_TEST
	#if TEST_SDIO
		#include "test_sdio.h"
	#endif
	
	#if TEST_FATFS
		#include "test_fatfs.h"
	#endif
#endif


int main(void)
{
	delay_init();		//��ʱ������ʼ��
	LED_Init();			//��ʼ����LED���ӵ�Ӳ���ӿ�
#if DEV_TEST
	#if NEED_UART
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
	#endif
	
	#if TEST_SDIO
		test_sdio_init();
	#endif
	
	#if TEST_FATFS
		test_fatfs();
	#endif
#else
	
#endif
	while(1)
	{
		
	}
}
