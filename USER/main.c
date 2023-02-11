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
	delay_init();		//延时函数初始化
	LED_Init();			//初始化与LED连接的硬件接口
#if DEV_TEST
	#if NEED_UART
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
		uart_init(115200);	 		//串口初始化为115200
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
