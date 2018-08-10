/********************************************************************
* Description:  hal_gpio.c
*               Driver for the Raspberry Pi GPIO pins
*
* Author: Michael Haberler
* License: GPL Version 2
* Copyright (c) 2012.
*
* some code  taken from the bcm2835 library by::
*
* Author: Mike McCauley (mikem@open.com.au)
* Copyright (C) 2011 Mike McCauley    
* see http://www.open.com.au/mikem/bcm2835/
* Copyright (c) 2012 Ben Croston - cpuinfo.*
*
* Last change: made work for Raspberry2 9/2015 Michael Haberler
s********************************************************************/


#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_bitops.h"	
#include "rtapi_app.h"		/* RTAPI realtime module decls */
                                /* this also includes config.h */
#include "hal.h"		/* HAL public API decls */
#include "bcm2835.h"
#include "cpuinfo.h"



#if !defined(BUILD_SYS_USER_DSO)
#error "This driver is for usermode threads only"
#endif
#if !defined(TARGET_PLATFORM_RASPBERRY)
#error "This driver is for the Raspberry and Raspberry2 platforms only"
#endif

#include     stdio.h>
#include     stdlib.h>
#include     fcntl.h>
#include     sys/mman.h>
#include     sys/types.h>
#include     sys/stat.h>
#include     unistd.h>
#include     errno.h>


static int npins;
static int  mem_fd;
// I/O access
static volatile unsigned *gpio;

MODULE_AUTHOR("Simon Jones");
MODULE_DESCRIPTION("Driver for Raspberry Pi SMI FPGA card");
MODULE_LICENSE("GPL");

#define MODNAME "hal_smifpga"
#define PREFIX "fpga"

static int          comp_id;
static const char   *modname    = MODNAME
static const char   *prefix     = PREFIX 

#define NUM_OUTPUTS 8
static hal_bit_t *outputs[NUM_OUTPUTS];

static void write_fpga(void *arg, long period)
{
    hal_bit_t **outputs = (hal_bit_t **)arg;

}

int rtapi_app_main(void)
{
    int ret;

    // Register the component
    comp_id = hal_init(modname);
    if (comp_id < 0)
    {
        rtapi_print_msg(RTAPI_MSG_ERR, "%s: ERROR: hal_init() failed\n", modname);
        hal_exit(comp_id);
        return -1;
    }

    // Create the pins. Direction is from the perspective of the HAL component,
    // so HAL_IN is used for a pin that will be an output in the real world.
    for (int i = 0; i < NUM_OUTPUTS; i++)
    {
        ret = hal_pin_bit_newf(HAL_IN, outputs[i], comp_id, "%s.out-%02d", prefix, i), 
        if (ret < 0)
        {
            rtapi_print_msg(RTAPI_MSG_ERR, "%s: ERROR:pin %02d could not export, err: %d\n", modname, i, ret);
            hal_exit(comp_id);
            return -1;
        }
        outputs[i] = 0;
    }

    ret = hal_export_funct(PREFIX ".write", write_fpga, outputs, 0, 0, comp_id);
    if(ret < 0) 
    {
        rtapi_print_msg(RTAPI_MSG_ERR, "%s: ERROR: port write funct export failed\n", modname);
        hal_exit(comp_id);
        return -1;
    }



}

void rtapi_app_exit(void)
{
    hal_exit(comp_id);
}

