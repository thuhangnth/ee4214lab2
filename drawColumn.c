#include "drawColumn.h"

int initTFT()
{
	int Status;
	XTft_Config *TftConfigPtr;

	/*
	 * Get address of the XTft_Config structure for the given device id.
	 */
	TftConfigPtr = XTft_LookupConfig(TFT_DEVICE_ID);
	if (TftConfigPtr == (XTft_Config *)NULL) 
	{
		return XST_FAILURE;
	}

	/*
	 * Initialize all the TftInstance members and fills the screen with
	 * default background color.
	 */
	Status = XTft_CfgInitialize(&TftInstance, TftConfigPtr,
			TftConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) 
	{
		return XST_FAILURE;
	}

	/*
	 * Wait till Vsync(Video address latch) status bit is set before writing
	 * the frame address into the Address Register. This ensures that the
	 * current frame has been displayed and we can display a new frame of
	 * data. Checking the Vsync state ensures that there is no data flicker
	 * when displaying frames in real time though there is some delay due to
	 * polling.
	 */
	while (XTft_GetVsyncStatus(&TftInstance) !=
			XTFT_IESR_VADDRLATCH_STATUS_MASK);

	/*
	 * Change the Video Memory Base Address from default value to
	 * a valid Memory Address and clear the screen.
	 */
	XTft_SetFrameBaseAddr(&TftInstance, TFT_FRAME_ADDR);
	XTft_ClearScreen(&TftInstance);

	XTft_SetPos(&TftInstance, 0,0);
	XTft_SetPosChar(&TftInstance, 0,0);

	XTft_SetColor(&TftInstance, 0x00000000, 0x00ffffff);

	XTft_FillScreen(&TftInstance, 0, 0,639,479,0x00ffffff); // white

	return XST_SUCCESS;
}

//(x0,y0) is the top left corner of the column
void drawColumn(int width, int length, int y0, int x0, int col)
{
	int i = 0;
	for (i=0; i<width; i++)
	{
		drawVerLine(&TftInstance, x0+i, y0, y0+length, col);
	}
}

void drawVerLine(XTft *Tft, int x, int y1, int y2, int col)
{
	int i = 0;
	for (i=y1; i<= y2; i++)
	{
		XTft_SetPixel(Tft, x, i, col);
	}
}

void clearScreen()
{
	XTft_ClearScreen(&TftInstance);
}

void fillScreen()
{
	XTft_SetColor(&TftInstance, 0x00000000, 0x00ffffff);
}