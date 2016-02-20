#include "xmk.h"
#include "sys/init.h"
#include "xgpio.h"
#include "xuartlite.h"
#include "xparameters.h"
#include <sys/timer.h> //for using sleep. need to set config_time to true
#include <sys/intr.h> //xilkernel api for interrupts
#include <stdio.h>
#define XST_SUCCESS                     0L
#define XST_FAILURE                     1L
// TO DO : read and understand the xilkernel system calls as well as the driver APIs used.


XGpio gpPB; //PB device instance.
long last_int_time = 0;

static void gpPBIntHandler(void *arg) //Should be very short (in time). In a practical program, don't print etc.
{
	unsigned char val;
	//clear the interrupt flag. if this is not done, gpio will keep interrupting the microblaze.--
	// --Possible to use (XGpio*)arg instead of &gpPB
	XGpio_InterruptClear(&gpPB,1);
	long int_time = millis();
	if (int_time - last_int_time > 50)
	{
		//Read the state of the push buttons.
		val = XGpio_DiscreteRead(&gpPB, 1);
		xil_printf("PB event, val = %d \r\n", val); // for testing.
	}
	last_int_time = int_time;
}

void main_prog(void *arg)
{
	int Status;

	xil_printf("Initializing PB\r\n");
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

	while(1)
	{
		sleep(1000); //to kill time
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

// to think : press and release BTN_West few times. Press generates an interrupt, and you can see 4 being printed. Release, and you see 0 being printed. Do it a few times.
// Is 4 always followed by a 0 and vice versa? If not, why not?

// to do (not much related to interrupts) : find the difference between XGpio_DiscreteClear(), XGpio_DiscreteSet() and XGpio_DiscreteWrite() - useful with LEDs_6Bit peripheral !


