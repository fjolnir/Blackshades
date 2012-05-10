#ifndef _TIMER_H_
#define _TIMER_H_


/**> HEADER FILES <**/
class timer			 							
{
	public:
  	unsigned long mm_timer_start;			
  	unsigned long mm_timer_elapsed;					
};


void TimerInit(timer* theTimer);
float TimerGetTime(timer* theTimer);

#endif

