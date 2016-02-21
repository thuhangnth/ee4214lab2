#include "displayBar.h"

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

void drawBar(int width, int length, int y0, int x0)
{
	int i = 0;
	for (i=0; i<width; i++)
	{
		drawHorLine(&TftInstance, y0+i, x0, x0+length, 0x000000ff); 
	{
}

void drawHorLine(XTft *Tft, int y, int x1, int x2, int col)
{
	int i = 0;
	if (x1<0)
	{
		x2 = x2-x1;
		x1=0;
	}
	if (x2>639)
	{
		x1=x1-(x2-639);
		x2=639;
	}
	if (y>=0 && y<=479)
	{
		for (i=x1; i<= x2; i++)
		{
			XTft_SetPixel(Tft, i, y, col);
		}
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