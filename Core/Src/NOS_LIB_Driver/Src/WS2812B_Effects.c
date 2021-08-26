#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "WS2812B_Core.h"
#include <stdlib.h>
extern WS2812_Struct ws2812b;
// RGB Framebuffers
uint8_t frameBuffer1[3*8*96];
uint8_t frameBuffer2[3*8*64];
uint8_t frameBuffer3[3*8*32];

// Helper defines
#define newColor(r, g, b) (((uint32_t)(r) << 16) | ((uint32_t)(g) <<  8) | (b))
#define Red(c) ((uint8_t)((c >> 16) & 0xFF))
#define Green(c) ((uint8_t)((c >> 8) & 0xFF))
#define Blue(c) ((uint8_t)(c & 0xFF))

void visInit()
{
		// Set output channel/pin, GPIO_PIN_0 = 0, for GPIO_PIN_5 = 5 - this has to correspond to WS2812B_PINS
		ws2812b.item[0].channel = 1;
		ws2812b.item[0].frameBufferPointer = frameBuffer1;
	    ws2812b.item[0].frameBufferSize = sizeof(frameBuffer1);
		ws2812b.item[1].channel = 2;
		ws2812b.item[1].frameBufferPointer = frameBuffer2;
	    ws2812b.item[1].frameBufferSize = sizeof(frameBuffer2);
		ws2812b.item[2].channel = 3;
		ws2812b.item[2].frameBufferPointer = frameBuffer3;
	    ws2812b.item[2].frameBufferSize = sizeof(frameBuffer3);
	ws2812b_init();
}


void visHandle()
{

	if(ws2812b.transferComplete)
	{
		// Update your framebuffer here or swap buffers

		// Signal that buffer is changed and transfer new data
		ws2812b.startTransfer = 1;
		ws2812b_handle();
	}
	
}