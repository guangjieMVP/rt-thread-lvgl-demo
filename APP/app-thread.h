#ifndef _APP_THREAD_H_
#define _APP_THREAD_H_

#include "rtthread.h"

struct ApplicationThread {
	struct rt_thread lvgl_gui_thread;
	struct rt_thread print_thread;
	struct rt_thread touch_thread;
};
typedef struct ApplicationThread  stAppThread;

extern stAppThread  appthread;
void AppThread_Create(stAppThread *athrd);

#endif