#include "xmk.h"
#include <pthread.h>
#include "semaphore.h"
#include <stdio.h>
#include "drawColumn.h"
#define XST_SUCCESS                     0L
#define XST_FAILURE                     1L

pthread_t tid1, tid2, tid3, tid4,tid5,tid6,tid7,tid8;

// declare the semaphore
sem_t sem;

const int WIDTH = 20;
const int LENGTH = 60;

void drawRedColumn(int tid)
{
	drawColumn(WIDTH, LENGTH, 30, 40*tid, 0x00ff0000);
}

void drawBlueColumn(int tid)
{
	drawColumn(WIDTH, LENGTH, 30, 40*tid, 0x000000ff);
}

void* thread_func_1() {
  while(1) {
	drawBlueColumn(1);
	sem_wait(&sem);
	drawRedColumn(1);
	sleep(500);
	sem_post(&sem);
  }
}

void* thread_func_2() {
  while(1) {
	drawBlueColumn(2);
	sem_wait(&sem);
	drawRedColumn(2);
	sleep(500);
	sem_post(&sem);
  }
}

void* thread_func_3() {
  while(1) {
	drawBlueColumn(3);
	sem_wait(&sem);
	drawRedColumn(3);
	sleep(500);
	sem_post(&sem);
  }
}

void* thread_func_4() {
  while(1) {
	drawBlueColumn(4);
	sem_wait(&sem);
	drawRedColumn(4);
	sleep(500);
	sem_post(&sem);
  }
}

void* thread_func_5() {
  while(1) {
	drawBlueColumn(5);
	sem_wait(&sem);
	drawRedColumn(5);
	sem_post(&sem);
  }
}

void* thread_func_6() {
  while(1) {
	drawBlueColumn(6);
	sem_wait(&sem);
	drawRedColumn(6);
	sleep(500);
	sem_post(&sem);
  }
}

void* thread_func_7() {
  while(1) {
	drawBlueColumn(7);
	sem_wait(&sem);
	drawRedColumn(7);
	sleep(500);
	sem_post(&sem);
  }
}

void* thread_func_8() {
  while(1) {
	drawBlueColumn(8);
	sem_wait(&sem);
	drawRedColumn(8);
	sleep(500);
	sem_post(&sem);
  }
}

int main_prog(void) {  // This thread is statically created (as configured in the kernel configuration) and has priority 0 (This is the highest possible)

  int ret;
  
  print("-- Entering main() --\r\n");
  
  initTFT();
  xil_printf("Finish initializing TFT\r\n");
  fillScreen();

  // initialize the semaphore
  if( sem_init(&sem, 1, 2) < 0 ) {
	print("Error while initializing semaphore sem.\r\n");
  }

  //start thread 1
  ret = pthread_create (&tid1, NULL, (void*)thread_func_1, NULL);
  if (ret != 0) {
      xil_printf ("-- ERROR (%d) launching thread_func_1...\r\n", ret);
  }
  else { 
    xil_printf ("Thread 1 launched with ID %d \r\n",tid1);
  }

  //start thread 2
  ret = pthread_create (&tid2, NULL, (void*)thread_func_2, NULL);
  if (ret != 0) {
      xil_printf ("-- ERROR (%d) launching thread_func_2...\r\n", ret);
  }
  else { 
    xil_printf ("Thread 2 launched with ID %d \r\n",tid2);
  }

  //start thread 3
  ret = pthread_create (&tid3, NULL, (void*)thread_func_3, NULL);
  if (ret != 0) {
      xil_printf ("-- ERROR (%d) launching thread_func_3...\r\n", ret);
  }
  else { 
    xil_printf ("Thread 3 launched with ID %d \r\n",tid3);
  }
  
  //start thread 4
  ret = pthread_create (&tid4, NULL, (void*)thread_func_4, NULL);
  if (ret != 0) {
      xil_printf ("-- ERROR (%d) launching thread_func_4...\r\n", ret);
  }
  else { 
    xil_printf ("Thread 4 launched with ID %d \r\n",tid4);
  }
  
  //start thread 5
  ret = pthread_create (&tid5, NULL, (void*)thread_func_5, NULL);
  if (ret != 0) {
      xil_printf ("-- ERROR (%d) launching thread_func_5...\r\n", ret);
  }
  else { 
    xil_printf ("Thread 5 launched with ID %d \r\n",tid5);
  }
  
   ret = pthread_create (&tid6, NULL, (void*)thread_func_6, NULL);
  if (ret != 0) {
      xil_printf ("-- ERROR (%d) launching thread_func_6...\r\n", ret);
  }
  else { 
    xil_printf ("Thread 6 launched with ID %d \r\n",tid6);
  }
 
  ret = pthread_create (&tid7, NULL, (void*)thread_func_7, NULL);
  if (ret != 0) {
      xil_printf ("-- ERROR (%d) launching thread_func_7...\r\n", ret);
  }
  else { 
    xil_printf ("Thread 7 launched with ID %d \r\n",tid7);
  }
 
  ret = pthread_create (&tid8, NULL, (void*)thread_func_8, NULL);
  if (ret != 0) {
      xil_printf ("-- ERROR (%d) launching thread_func_8...\r\n", ret);
  }
  else { 
    xil_printf ("Thread 8 launched with ID %d \r\n",tid8);
  }
}

int main (void) {

	print("-- Entering main() uB0--\r\n");
		xilkernel_init();
		xmk_add_static_thread(main_prog,0);
		xilkernel_start();
  //Start Xilkernel
  xilkernel_main ();
  
  //Control does not reach here
	return 0;
}