/***************************** Include Files ********************************/

#include "xtft.h"
#include "xparameters.h"
#include <string.h>

/************************** Constant Definitions ****************************/
/**
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define TFT_DEVICE_ID    XPAR_TFT_0_DEVICE_ID
#define DDR_HIGH_ADDR    XPAR_PS7_DDR_0_S_AXI_HIGHADDR 

#ifndef DDR_HIGH_ADDR
#warning "CHECK FOR THE VALID DDR ADDRESS IN XPARAMETERS.H"
#endif

#define DISPLAY_COLUMNS  640
#define DISPLAY_ROWS     480


/**
 * User has to specify a 2MB memory space for filling the frame data.
 * This constant has to be updated based on the memory map of the
 * system.
 */
#define TFT_FRAME_ADDR        0x10000000

/**************************** Type Definitions ******************************/

/************************** Function Prototypes *****************************/

int initTFT();
void drawColumn(int width, int length, int y0, int x0, int col); //(x0,y0) is the top left corner of the column
void drawVerLine(XTft *Tft, int x, int y1, int y2, int col);
void clearScreen();
void fillScreen();

/************************** Variable Definitions ****************************/

static XTft TftInstance;
