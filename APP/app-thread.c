#include "rtthread.h"

#include "app-thread.h"
#include "lv_examples.h"
#include "gui_guider.h"

#include "bsp_lcd.h"
#include "gt9xx.h"


stAppThread appthread;

static void lvgl_gui_thread_entry(void *para)
{
	lv_demo_widgets();
//	setup_ui(&guider_ui);
	
	while(1)
	{
		lv_task_handler();
		rt_thread_delay(10); 
	}
}

static void lvgl_touch_thread_entry(void *para)
{
	while(1)
	{
		GT9xx_GetOnePiont();
		rt_thread_delay (20); 
	}
}

static void print_thread_entry(void* parameter)
{
	while(1)
	{
		rt_kprintf("rt-thread lvgl demo running\r\n");
		rt_thread_delay (2000); 
	}
}


void AppThread_Create(stAppThread *athrd)
{
#define LVGL_GUI_THREAD_STACK_SIZE    4096    
static rt_uint8_t lvgl_gui_stack[LVGL_GUI_THREAD_STACK_SIZE];	
	rt_thread_init(&athrd->lvgl_gui_thread,
                   "lvgl_gui",
                   lvgl_gui_thread_entry,
                   RT_NULL,
                   &lvgl_gui_stack[0],
                   LVGL_GUI_THREAD_STACK_SIZE,
                   3,
                   20);
    rt_thread_startup(&athrd->lvgl_gui_thread);
	
#define TOUCH_THREAD_PRI               3	
#define TOUCH_THREAD_STACK_SIZE        512       
static rt_uint8_t  touch_thread_stack[TOUCH_THREAD_STACK_SIZE];
	rt_thread_init(&athrd->touch_thread,
                   "touch_thread",
                   lvgl_touch_thread_entry,
                   RT_NULL,
                   &touch_thread_stack[0],
                   TOUCH_THREAD_STACK_SIZE,
                   TOUCH_THREAD_PRI,
                   20);
                   
     rt_thread_startup(&athrd->touch_thread);

#define PRINT_THREAD_PRI              7
#define PRINT_THREAD_STACK_SIZE       256         
static rt_uint8_t print_thread_stack[PRINT_THREAD_STACK_SIZE];
	rt_thread_init(&athrd->print_thread,
                   "print_thread",
                   print_thread_entry,
                   RT_NULL,
                   &print_thread_stack[0],
                   PRINT_THREAD_STACK_SIZE,
                   PRINT_THREAD_PRI,
                   20);
     rt_thread_startup(&athrd->print_thread);
}