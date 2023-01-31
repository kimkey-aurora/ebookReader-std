#include "stm32f10x.h"
#include "delay/delay.h"
#include "usart/usart.h"

// Hardware
#include "led/led.h"
#include "sdio/sdio_sdcard.h"

#define DEV_TEST 1
#define TEST_SDIO 1

#if DEV_TEST
	#if TEST_SDIO
		#include "test_sdio.h"
	#endif
#endif


int main(void)
{
#if DEV_TEST
	#if TEST_SDIO
		test_sdio_init();
	#endif
#else
	
#endif
}
