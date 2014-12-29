#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_opengl.h>
#include "part.h"

float xxg, yyg, zzg, xxsp, yysp, zzsp;
float fad;

void init_particles (PART *p, float fade, int max, float xg, float yg, float zg, int autocol, float r, float g, float b, float spx, float spy, float spz)
{

	int loop;

	xxg=xg;
	yyg=xg;
	zzg=zg;

	xxsp=spx;
	yysp=spy;
	zzsp=spz;

	fad=fade;


	for(loop=0; loop < max; loop++)
	{
		p[loop].life=1.0;
		p[loop].fade=((rand()%100)/fad);

		if(p[loop].fade <= 0.0)
		{
			p[loop].fade=0.03;
		}

		

		if(autocol)
		{
			p[loop].r=r+(rand()%100)/1000;
			p[loop].g=g+(rand()%100)/1000;
			p[loop].b=b+(rand()%100)/1000;
		} else {
			p[loop].r=r;
			p[loop].g=g;
			p[loop].b=b;
		}

		p[loop].xg=xg;
		p[loop].yg=yg;
		p[loop].zg=zg;

		p[loop].xsp=(spx*(rand()%50)-25.0);
		p[loop].ysp=(spy*(rand()%50)-25.0);
		p[loop].zsp=(spz*(rand()%50)-25.0);
		
	}
}

void process_particles (PART *p, int max, float rot)
{
	int loop;
	float x, y, z;

	for(loop=0; loop < max; loop++)
	{

		if(p[loop].life < 0.0)
		{
			p[loop].life=1.0;
			p[loop].fade=((rand()%1000)/fad);


			if(p[loop].fade <= 0.0)
				{
					p[loop].fade=0.03;
				}
			
			p[loop].x=0.0;
			p[loop].y=0.0;
			p[loop].z=0.0;

			p[loop].xsp=((rand()%50)-25.0);
			p[loop].ysp=((rand()%50)-25.0);
			p[loop].zsp=((rand()%50)-25.0);
			
		}

		x=p[loop].x;
		y=p[loop].y;
		z=p[loop].z;

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glColor4f(p[loop].r, p[loop].g, p[loop].b, p[loop].life);
		glPushMatrix();
		glRotatef(rot, 1.0, 1.0, 1.0);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1,1); glVertex3f(x+2.5f,y+2.5f,z);
		glTexCoord2d(0,1); glVertex3f(x-2.5f,y+2.5f,z);
		glTexCoord2d(1,0); glVertex3f(x+2.5f,y-2.5f,z);
		glTexCoord2d(0,0); glVertex3f(x-2.5f,y-2.5f,z);
		glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glColor3f(1.0, 1.0, 1.0);


		p[loop].xsp+=p[loop].xg;
		p[loop].ysp+=p[loop].yg;
		p[loop].zsp+=p[loop].zg;

		p[loop].x+=(p[loop].xsp*xxsp);
		p[loop].y+=(p[loop].ysp*yysp);
		p[loop].z+=(p[loop].zsp*zzsp);

		p[loop].life-=(p[loop].fade);

	}
}
