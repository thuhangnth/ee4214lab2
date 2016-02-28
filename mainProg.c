#include "xmk.h"
#include "sys/init.h"
#include "xgpio.h"
#include "pthread.h"
#include "xuartlite.h"
#include "xparameters.h"
#include <sys/timer.h> //for using sleep. need to set config_time to true
#include <sys/intr.h> //xilkernel api for interrupts
#include <stdio.h>
#include "displayBar.h"
#define XST_SUCCESS                     0L
#define XST_FAILURE                     1L
// TO DO : read and understand the xilkernel system calls as well as the driver APIs used.

pthread_t tid1;
const int LEFT_BTN = 4;
const int RIGHT_BTN = 8;
const int LENGTH = 80;
const int DIST=10;
XGpio gpPB; //PB device instance.
long last_int_time1 = 0, last_int_time0 = 0;
int btn;
int x=310;

//TODO: Implement button debounce
static void gpPBIntHandler(void *arg) //Should be very short (in time). In a practical program, don't print etc.
{
	unsigned char val;
	//clear the interrupt flag. if this is not done, gpio will keep interrupting the microblaze.--
	// --Possible to use (XGpio*)arg instead of &gpPB
	XGpio_InterruptClear(&gpPB,1);
	long int_time = xget_clock_ticks();
	val = XGpio_DiscreteRead(&gpPB, 1);
	if (val>0) {
		if (int_time - last_int_time1>20 && int_time - last_int_time0> 20)
		{
			//Read the state of the push buttons.
			btn = val;
		}
		last_int_time1 = int_time;
	}
	else {
		if (int_time - last_int_time0 > 20)
		{
			//Read the state of the push buttons.
			btn = val;
		}
		last_int_time0 = int_time;
	}
}

void moveBar()
{
	if (btn==LEFT_BTN)
	{
		//Decrement x coordinate
		x=x-DIST;
		if (x<0) x=0;
		drawBar(5,LENGTH,409,x,0x000000ff);
		drawBar(5,DIST,409,x+LENGTH,0x00ffffff);
	}
	else if (btn==RIGHT_BTN)
	{
		//Increment x coordinate
		x=x+DIST;
		if (x>639-LENGTH) x=639-LENGTH;
		drawBar(5,LENGTH,409,x,0x000000ff);
		drawBar(5,DIST,409,x-DIST,0x00ffffff);
	}
	/*else
	{
		drawBar(5,LENGTH,409,x,0x000000ff);
	}*/
}

void* thread_func_1()
{
	while (1)
	{
		//clearScreen();
		moveBar();
		sleep(40);
	}
}

void main_prog(void *arg)
{
	int Status;
	int ret;

	xil_printf("Initializing PB\r\n");
	initTFT();
	xil_printf("Finish initializing TFT\r\n");
	fillScreen();
	// Initialise the PB instance
	Status = XGpio_Initialize(&gpPB, XPAR_GPIO_0_DEVICE_ID);
	// set PB gpio direction to input.
	XGpio_SetDataDirection(&gpPB, 1, 0x000000FF);

	xil_printf("Enabling PB interrupts\r\n");
	//global enable
	XGpio_InterruptGlobalEnable(&gpPB);
	// interrupt enable. both global enable and this function should be called to enable gpio interrupts.
	XGpio_InterruptEnable(&gpPB,1);
	//register the handler with xilkernel
	register_int_handler(XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR, gpPBIntHandler, &gpPB);
	//enable the interrupt in xilkernel
	enable_interrupt(XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
	drawBar(5,LENGTH,409,x, 0x000000ff);
	//start thread 1
	ret = pthread_create (&tid1, NULL, (void*)thread_func_1, NULL);
	if (ret != 0) {
		xil_printf ("-- ERROR (%d) launching thread_func_1...\r\n", ret);
	}
	else {
		xil_printf ("Thread 1 launched with ID %d \r\n",tid1);
	}
}

int main()
{
	print("-- Entering main() --\r\n");
	//Initialize Xilkernel
	xilkernel_init();

	//Add main_prog as the static thread that will be invoked by Xilkernel
	xmk_add_static_thread(main_prog, 0);

	//Start Xilkernel
	xilkernel_start();

	/* Never reached */
	return 0;
}



