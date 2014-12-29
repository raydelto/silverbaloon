#include "md2.h"

void process_AI(modelData *m, float distance, int col)
{
	if(distance<=100)
	{
		m->state=AI_RUNNING;
	}
		
	if(distance<=200 && distance > 100)
	{
		m->state=AI_IDLE2;
	}
		
	if(distance<=300 && distance > 200)
	{
		m->state=AI_IDLE3;
	}
		
	if(distance<=400 && distance > 300)
	{
		m->state=AI_IDLE4;
	}
	
	if(distance > 400)
	{
		m->state=AI_IDLE1;
	}
		

}
