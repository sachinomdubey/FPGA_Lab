/*==========================================================
Code by G V V Sharma
March 7, 2021,
Released under GNU/GPL
https://www.gnu.org/licenses/gpl-3.0.en.html
/*==========================================================
 *
 *    File   : main.c
 *    Purpose: main for Pygmy blink onboard led
 *                                                          
 *=========================================================*/

#include "Fw_global_config.h"   // This defines application specific charactersitics

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "RtosTask.h"

/*    Include the generic headers required for QORC */
#include "eoss3_hal_gpio.h"
#include "eoss3_hal_rtc.h"
#include "eoss3_hal_timer.h"
#include "eoss3_hal_fpga_usbserial.h"
#include "ql_time.h"
#include "s3x_clock_hal.h"
#include "s3x_clock.h"
#include "s3x_pi.h"
#include "dbg_uart.h"
#include "eoss3_hal_uart.h"

#include "cli.h"


extern const struct cli_cmd_entry my_main_menu[];


const char *SOFTWARE_VERSION_STR;


/*
 * Global variable definition
 */


extern void qf_hardwareSetup();
static void nvic_init(void);

#define GPIO_OUTPUT_MODE (1)
#define GPIO_INPUT_MODE (0)
void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode);
int PyHal_GPIO_GetDir(uint8_t gpionum);
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval);
int PyHal_GPIO_Get(uint8_t gpionum);
float* convo(float x[], float h[], int length_x, int length_h, int length_y); 
float* correlate(float x[], float y[], int length_x, int length_y, int length_r);
void print_float(float* num);

int main(void)
{
    uint32_t i=0,j=0,k=0;
    SOFTWARE_VERSION_STR = "qorc-onion-apps/qf_hello-fpga-gpio-ctlr";
    
    qf_hardwareSetup();
    nvic_init();

    dbg_str("\n\n");
    dbg_str( "##########################\n");
    dbg_str( "Quicklogic QuickFeather FPGA GPIO CONTROLLER EXAMPLE\n");
    dbg_str( "SW Version: ");
    dbg_str( SOFTWARE_VERSION_STR );
    dbg_str( "\n" );
    dbg_str( __DATE__ " " __TIME__ "\n" );
    dbg_str( "##########################\n\n");

    dbg_str( "\n\nHello GPIO!!\n\n");	// <<<<<<<<<<<<<<<<<<<<<  Change me!

    CLI_start_task( my_main_menu );
	HAL_Delay_Init();
    
    //LED pins Output
    PyHal_GPIO_SetDir(18,1);
    PyHal_GPIO_SetDir(21,1);
    PyHal_GPIO_SetDir(22,1);
    
    //input pins
    PyHal_GPIO_SetDir(4,0);
    PyHal_GPIO_SetDir(5,0);
    PyHal_GPIO_SetDir(6,0);
    int count = 2;	
    int r;
    while(1)
    {
        char buffer[200];
        uint8_t ind = 0;
        r = 0;
        while (r!=10)
        {
            if(!(UART->UART_TFR & UART_TFR_RX_FIFO_EMPTY)) {
                r = UART->UART_DR;
                r = r & 0x0ff;
                buffer[ind] = (char)r;
                ind++;
            }
        }
        
        char* pEnd;
        char toPrint[20];
        float x1[20];
        int i=1;
        x1[0] = strtof((const char*)buffer,&pEnd);
        int lengthx = 19;
        while(lengthx--) {
            x1[i] = strtof((const char*)pEnd,&pEnd);
            i++;
        }
        int length_x = i;
        int length_y;
        float* y;
        float h1[] = { 0.6715, -1.2075, 0.7172, 1.6302, 0.4889, 1.0347, 0.7269, -0.3034, 0.2939, -0.7873, 0.8884, -1.1471, -1.0689, -0.8095, -2.9443 };
        int choice = PyHal_GPIO_Get(4);
        if(choice==0) {//convolution
            length_y = length_x + 15 - 1;
            y = convo(x1, h1, length_x, 15, length_y); //Calling convolving function.
        }
        else {//Correlation
            length_y = length_x + 15 - 1;
            y = correlate(x1, h1, length_x, 15, length_y); //Calling convolving function.
        }
        
        float *x = y;
        while(length_y--) {
            print_float(x);
            x++;
        }
        dbg_str("x\n");
        HAL_DelayUSec(500000);
    } 
}

static void nvic_init(void)
 {
    // To initialize system, this interrupt should be triggered at main.
    // So, we will set its priority just before calling vTaskStartScheduler(), not the time of enabling each irq.
    NVIC_SetPriority(Ffe0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SpiMs_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CfgDma_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Uart_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FbMsg_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
 }    

//needed for startup_EOSS3b.s asm file
void SystemInit(void)
{

}

//gpionum --> 0 --> 31 corresponding to the IO PADs
//gpioval --> 0 or 1
#define FGPIO_DIRECTION_REG (0x40024008)
#define FGPIO_OUTPUT_REG (0x40024004)
#define FGPIO_INPUT_REG (0x40024000)
//Set GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
//Before Set/Get GPIO value, the direction must be correctly set
void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);
    if (iomode)
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    else
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 & (~(0x1 << gpionum));

}


//Get current GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
int PyHal_GPIO_GetDir(uint8_t gpionum)
{
    uint32_t tempscratch32;
    int result = 0;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    result = ((tempscratch32 & (0x1 << gpionum)) ? GPIO_OUTPUT_MODE : GPIO_INPUT_MODE);

    return result;
}

//Set GPIO(=gpionum) to 0 or 1 (= gpioval)
//The direction must be set as Output for this GPIO already
//Return value = 0, success OR -1 if error.
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    //Setting Direction moved out as separate API, we will only check
    //*(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    if (!(tempscratch32 & (0x1 << gpionum)))
    {
        //Direction not Set to Output
        return -1;
    }
    
    tempscratch32 = *(uint32_t*)(FGPIO_OUTPUT_REG);

    if(gpioval > 0)
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 | (0x1 << gpionum);
    }
    else
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 & ~(0x1 << gpionum);
    }    

    return 0;
}
//Get GPIO(=gpionum): 0 or 1 returned (or in erros -1)
//The direction must be set as Input for this GPIO already
int PyHal_GPIO_Get(uint8_t gpionum)
{
    uint32_t tempscratch32;
    uint32_t gpioval_input;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_INPUT_REG);
    gpioval_input = (tempscratch32 >> gpionum) & 0x1;

    return ((int)gpioval_input);
}

float* convo(float x[], float h[], int length_x, int length_h, int length_y)
{
    int i, j, start_h, start_x, end_x;
    float* y = (float*) calloc(length_y, sizeof(float));
    for (i = 0; i < length_y; i++)
    {
        start_x = MAX(0, i - length_h + 1);
        end_x = MIN(i + 1, length_x);
        start_h = MIN(i, length_h - 1);
        for (j = start_x; j < end_x; j++)
        {
            y[i] += h[start_h--] * x[j];
        }
    }
    return y;
}

float* correlate(float x[], float y[], int length_x, int length_y, int length_r)
{
    int i, j, start_y, start_x, end_x;
    float* r = (float*)calloc(length_r, sizeof(float));
    for (i = 0; i < length_r; i++)
    {
        start_x = MAX(0, i - length_y + 1);
        end_x = MIN(i + 1, length_x);
        start_y = MAX(0, length_y - 1 - i);
        for (j = start_x; j < end_x; j++)
        {
            r[i] += y[start_y++] * x[j];
        }
    }
    return r;
}

void print_float(float* num)
{
    float a = *num;
    if(a<0 && a>-1) dbg_str("-");
    dbg_int((int)a);
    dbg_str(".");
    int afterDecimal = (int)((a - (int)a)*100);
    if(afterDecimal<0)
        dbg_int(-afterDecimal);
    else
        dbg_int(afterDecimal);
    dbg_str(" ");
}




