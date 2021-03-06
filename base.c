#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_mixer.h>

#include <time.h>
#include <string.h>
#include <math.h>
#include "rawtex.h"
#include "utgatex.h"
#include "ctgatex.h"
#include "models.h"
#include "map.h"
#include "md2.h"
#include "part.h"
#include "console.h"

#define MAX_CASCADE 100
#define NUM_SOUNDS 10
#define FRAME_RATE_SAMPLES 50

int rot;
int mouse_grab=1;
int draw_fpsc=0;

float k;
float xrot=0.0, yrot=0.0;
float xpos=0.0, ypos=0.0, zpos=0.0;
float nextXpos=0.0, nextYpos=0.0, nextZpos=0.0;
float sensitivity=15.0;
float walk_speed=1.0;
float space=2.0;
float dist;

static int number[11];
static int letter[27];
static int consy;
static int colon;
static int li;
static int grunt;

float ambientlight[] = {0.5, 0.5, 0.5, 1.0};
float diffuselight[] = {0.8, 0.8, 0.8, 1.0};
float lightPos[] = {0.0, 0.0, 0.0, 1.0};
float matspec[] = {1.0, 1.0, 0.0, 1.0};

int frameCount=0;
float FrameRate=10;

char buf[20];
int cen, dec, caca;
float fa;
int pushb;
float t;
int hehe;
int ret, ret2, ret3;

IMAGE_CTGA number_tex[11];
IMAGE_CTGA letter_tex[27];
IMAGE_CTGA lis;
IMAGE_UTGA grunt_d;
IMAGE_CTGA console_tex;
IMAGE_CTGA colon_tex;

MAP map[100];

SDL_Event event;
SDL_AudioSpec fmt;

CONS console;

modelData *mod;

struct sample {
	Uint8 *data;
	Uint32 dpos;
	Uint32 dlen;
} sounds[NUM_SOUNDS];

	
void load_rawtex(IMAGE_RAW *img, char *filename);
void load_models(MODEL *model, char *filename);
void init_particles (PART *p, float fade, int max, float xg, float yg, float zg, int autocol, float r, float g, float b, float spx, float spy, float spz);
void process_particles (PART *p, int max, float rot);
void load_utgatex(IMAGE_UTGA *img, char *filename);
void load_ctgatex(IMAGE_CTGA *img, char *filename);
void draw_string(char *string, int numbero);
void process_commands(char *com);
int test_in_polygon(MAP *map, float currx, float curry, float currz);
void load_map(MAP *map, char *filename);
void render_map(MAP *map, int active);
modelData *load_md2(char *filename, char *texturename);
void render_md2_interpol(modelData *model, int startFrame, int endFrame, int t);
int test_sphere_colision(modelData *m, float xpos, float ypos, float zpos, float radius);
void process_AI(modelData *m, float distance, int col);

float distPlayerToEnemy(modelData *m, float x, float y, float z);

void fps()

{
	float delta;
	static clock_t last=0;
	clock_t now;

	if (++frameCount >= FRAME_RATE_SAMPLES) 

	{

		now  = clock();
		delta= (now - last) / (float) CLOCKS_PER_SEC;
		last = now;
		FrameRate = FRAME_RATE_SAMPLES / delta;
		frameCount = 0;
	}

}

void load_fonts(void)
{
	int foo;
	load_ctgatex(&number_tex[0], "data/0.tga");
	load_ctgatex(&number_tex[1], "data/1.tga");
	load_ctgatex(&number_tex[2], "data/2.tga");
	load_ctgatex(&number_tex[3], "data/3.tga");
	load_ctgatex(&number_tex[4], "data/4.tga");
	load_ctgatex(&number_tex[5], "data/5.tga");
	load_ctgatex(&number_tex[6], "data/6.tga");
	load_ctgatex(&number_tex[7], "data/7.tga");
	load_ctgatex(&number_tex[8], "data/8.tga");
	load_ctgatex(&number_tex[9], "data/9.tga");
	load_ctgatex(&number_tex[10], "data/point.tga");
	load_ctgatex(&letter_tex[0], "data/a.tga");
	load_ctgatex(&letter_tex[1], "data/b.tga");
	load_ctgatex(&letter_tex[2], "data/c.tga");
	load_ctgatex(&letter_tex[3], "data/d.tga");
	load_ctgatex(&letter_tex[4], "data/e.tga");
	load_ctgatex(&letter_tex[5], "data/f.tga");
	load_ctgatex(&letter_tex[6], "data/g.tga");
	load_ctgatex(&letter_tex[7], "data/h.tga");
	load_ctgatex(&letter_tex[8], "data/i.tga");
	load_ctgatex(&letter_tex[9], "data/j.tga");
	load_ctgatex(&letter_tex[10], "data/k.tga");
	load_ctgatex(&letter_tex[11], "data/l.tga");
	load_ctgatex(&letter_tex[12], "data/m.tga");
	load_ctgatex(&letter_tex[13], "data/n.tga");
	load_ctgatex(&letter_tex[14], "data/o.tga");
	load_ctgatex(&letter_tex[15], "data/p.tga");
	load_ctgatex(&letter_tex[16], "data/q.tga");
	load_ctgatex(&letter_tex[17], "data/r.tga");
	load_ctgatex(&letter_tex[18], "data/s.tga");
	load_ctgatex(&letter_tex[19], "data/t.tga");
	load_ctgatex(&letter_tex[20], "data/u.tga");
	load_ctgatex(&letter_tex[21], "data/v.tga");
	load_ctgatex(&letter_tex[22], "data/w.tga");
	load_ctgatex(&letter_tex[23], "data/x.tga");
	load_ctgatex(&letter_tex[24], "data/y.tga");
	load_ctgatex(&letter_tex[25], "data/z.tga");
	load_ctgatex(&letter_tex[26], "data/space.tga");

	load_ctgatex(&console_tex, "data/console.tga");
	load_ctgatex(&colon_tex, "data/colon.tga");


	glGenTextures(1, &consy);
	glBindTexture(GL_TEXTURE_2D, consy);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, console_tex.width, console_tex.height, GL_RGB, GL_UNSIGNED_BYTE, console_tex.data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	
	glGenTextures(1, &colon);
	glBindTexture(GL_TEXTURE_2D, colon);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, colon_tex.width, colon_tex.height, GL_RGBA, GL_UNSIGNED_BYTE, colon_tex.data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	for(foo=0; foo<11; foo++)
	{

		glGenTextures(1, &number[foo]);

		glBindTexture(GL_TEXTURE_2D, number[foo]);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, number_tex[foo].width, number_tex[foo].height, GL_RGBA, GL_UNSIGNED_BYTE, number_tex[foo].data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	}

	for(foo=0; foo<27; foo++)
	{

		glGenTextures(1, &letter[foo]);
		glBindTexture(GL_TEXTURE_2D, letter[foo]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, letter_tex[foo].width, letter_tex[foo].height, GL_RGBA, GL_UNSIGNED_BYTE, letter_tex[foo].data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	load_ctgatex(&lis, "data/map.tga");
	glGenTextures(1, &li);
	glBindTexture(GL_TEXTURE_2D, li);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, lis.width, lis.height, GL_RGB, GL_UNSIGNED_BYTE, lis.data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	load_utgatex(&grunt_d, "data/skin.tga");
	glGenTextures(1, &grunt);
	glBindTexture(GL_TEXTURE_2D, grunt);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, grunt_d.width, grunt_d.height, GL_RGB, GL_UNSIGNED_BYTE, grunt_d.data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

}

void Init(void)
{
	GLubyte *vend;
	GLubyte *rend;
	GLubyte *ver;
	GLubyte *ext;
	vend=(GLubyte *)glGetString(GL_VENDOR);
	rend=(GLubyte *)glGetString(GL_RENDERER);
	ver=(GLubyte *)glGetString(GL_VERSION);
	ext=(GLubyte *)glGetString(GL_EXTENSIONS);

	printf("GL_VENDOR: %s\n", vend);
	printf("GL_RENDERER: %s\n", rend);
	printf("GL_VERSION: %s\n", ver);
	printf("GL_EXTENSIONS: %s\n", ext);



	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0, 2.0, 20000);


	console.active=1;

	glShadeModel(GL_SMOOTH);	
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuselight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);


	load_fonts();
	mod=load_md2("data/tris.md2", "data/skin.tga");

	
	mod->x=50.0;
	mod->y=0.0;
	mod->z=0.0;
	mod->nextX=50.0;
	mod->radius=10.0;
	mod->state=AI_IDLE1;
}

void draw_fps(void)

{
	fps();
	fa=FrameRate;

	caca=(int)fa % 10;
	fa/=10;
	dec=(int)fa % 10;
	fa/=10;
	cen=(int)fa % 10;


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-100, 100, -100, 100, -100, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(draw_fpsc)
	{
		glPushMatrix();
		glTranslatef(70, 90, 0);
		glBindTexture(GL_TEXTURE_2D, number[cen]);
		glBegin(GL_QUADS);
		glTexCoord2d(0,1); glVertex2f(0,0);
		glTexCoord2d(1,1); glVertex2f(10,0);
		glTexCoord2d(1,0); glVertex2f(10,10);
		glTexCoord2d(0,0); glVertex2f(0,10);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(80, 90, 0);
		glBindTexture(GL_TEXTURE_2D, number[dec]);
		glBegin(GL_QUADS);
		glTexCoord2d(0,1); glVertex2f(0, 0);
		glTexCoord2d(1,1); glVertex2f(10, 0);
		glTexCoord2d(1,0); glVertex2f(10, 10);
		glTexCoord2d(0,0); glVertex2f(0, 10);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(90, 90, 0);
		glBindTexture(GL_TEXTURE_2D, number[caca]);

		glBegin(GL_QUADS);
		glTexCoord2d(0,1); glVertex2f(0, 0);
		glTexCoord2d(1,1); glVertex2f(10, 0);
		glTexCoord2d(1,0); glVertex2f(10, 10);
		glTexCoord2d(0,0); glVertex2f(0, 10);
		glEnd();

		glPopMatrix();
	}

	if(console.active)
	{
		glDisable(GL_LIGHTING);
		glBindTexture(GL_TEXTURE_2D, consy);
		glPushMatrix();
		glTranslatef(-90, -90, 0);
		glBegin(GL_QUADS);
		glTexCoord2d(0,1); glVertex3f(0, 0, 0);
		glTexCoord2d(1,1); glVertex3f(180, 0, 0);
		glTexCoord2d(1,0); glVertex3f(180, 30, 0);
		glTexCoord2d(0,0); glVertex3f(0, 30, 0);
		glEnd();
		glPopMatrix();
		draw_string(console.buff, console.number);
		glEnable(GL_LIGHTING);
	}

	glDisable(GL_BLEND);
}

void display(void)
{
	ret=test_in_polygon(map, -nextXpos, -nextYpos, -nextZpos);
	ret2=test_sphere_colision(mod, -nextXpos, -nextYpos, -nextZpos, mod->radius);
	ret3=test_in_polygon(map, -mod->nextX, -mod->nextY, -mod->nextZ);
	dist=distPlayerToEnemy(mod, -xpos, -ypos, -zpos);

	if(!ret && !ret2)
	{

		xpos=nextXpos;
		ypos=nextYpos;
		zpos=nextZpos;
	} else 
	{
		nextXpos=xpos;
		nextYpos=ypos;
		nextZpos=zpos;
	}

	if(!ret3)
	{

		if(mod->state==AI_RUNNING)
		{
			mod->nextX=mod->nextX+0.1;
		}

		mod->x=mod->nextX;
		mod->y=mod->nextY;
		mod->z=mod->nextZ;
	} 
	else
	{
		mod->nextX=mod->x;
		mod->nextY=mod->y;
		mod->nextZ=mod->z;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glRotatef(yrot, 1.0, 0.0, 0.0);
	glRotatef(xrot, 0.0, 1.0, 0.0);
	glTranslatef(xpos, ypos, zpos);

	render_map(map, console.active);
	glDisable(GL_BLEND);

	glPushMatrix();
	process_AI(mod, dist, ret2);
	glTranslatef(mod->x, mod->y, mod->z);

	if(mod->state==AI_IDLE1)
	{
		render_md2_interpol(mod, 0, 39, grunt);
	}

	if(mod->state==AI_IDLE2)
	{
		render_md2_interpol(mod, 123, 135, grunt);
	}

	if(mod->state==AI_IDLE3)
	{
		render_md2_interpol(mod, 74, 95, grunt);
	}

	if(mod->state==AI_IDLE4)
	{
		render_md2_interpol(mod, 113, 122, grunt);
	}

	if(mod->state==AI_RUNNING)
	{
		render_md2_interpol(mod, 40, 46, grunt);
	}

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	draw_fps();
	glFlush();
	SDL_GL_SwapBuffers();
}

void mixaudio(void *unused, Uint8 *stream, int len)
{
	size_t i;
	Uint32 amount;

	for ( i=0; i<NUM_SOUNDS; ++i ) 
	{
		amount = (sounds[i].dlen-sounds[i].dpos);
		if ( amount > len ) 
		{
			amount = len;
		}
	}

	SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
	sounds[i].dpos += amount;
}

void PlaySound(char *file)
{
	unsigned int index;
	SDL_AudioSpec wave;
	Uint8 *data;
	Uint32 dlen;
	SDL_AudioCVT cvt;

	for ( index=0; index<NUM_SOUNDS; ++index ) 
	{

		if ( sounds[index].dpos == sounds[index].dlen ) 
		{
			break;
		}
	}

	if ( index == NUM_SOUNDS )

	return;

	if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL )
	{
		fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
		return;
	}

	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq, AUDIO_S16, 2, 44100);

	cvt.buf = malloc(dlen*cvt.len_mult);

	memcpy(cvt.buf, data, dlen);

	cvt.len = dlen;

	SDL_ConvertAudio(&cvt);

	SDL_FreeWAV(data);


	if ( sounds[index].data )
	{
		free(sounds[index].data);
	}

	SDL_LockAudio();

	sounds[index].data = cvt.buf;

	sounds[index].dlen = cvt.len_cvt;

	sounds[index].dpos = 0;

	SDL_UnlockAudio();

}

void draw_string(char *string, int numbero)
{

unsigned int teta; 
int omega=1;
int a=0, b=1, c=2, d=3, e=4, f=5, g=6, h=7, i=8, j=9, k=10, l=11, m=12, n=13, o=14, 
	p=15, q=16, r=17, s=18, t=19, u=20, v=21, w=22, x=23, y=24, z=25, space=26;



glBindTexture(GL_TEXTURE_2D, colon);
glPushMatrix();
glTranslatef(-80, -80, 1.0);
glBegin(GL_QUADS);
glTexCoord2d(0,1); glVertex2f(0,0);
glTexCoord2d(1,1); glVertex2f(10,0);
glTexCoord2d(1,0); glVertex2f(10,10);
glTexCoord2d(0,0); glVertex2f(0,10);
glEnd();
glPopMatrix();

	for(teta=0; teta<numbero; teta++)
	{

		switch(string[teta])
		{

		case 'a':

		glBindTexture(GL_TEXTURE_2D, letter[a]);

		omega++;

		break;

		case 'b':

		glBindTexture(GL_TEXTURE_2D, letter[b]);

		omega++;

		break;

		case 'c':

		glBindTexture(GL_TEXTURE_2D, letter[c]);

		omega++;

		break;

		case 'd':

		glBindTexture(GL_TEXTURE_2D, letter[d]);

		omega++;

		break;

		case 'e':

		glBindTexture(GL_TEXTURE_2D, letter[e]);

		omega++;

		break;

		case 'f':

		glBindTexture(GL_TEXTURE_2D, letter[f]);

		omega++;

		break;

		case 'g':

		glBindTexture(GL_TEXTURE_2D, letter[g]);

		omega++;

		break;

		case 'h':

		glBindTexture(GL_TEXTURE_2D, letter[h]);

		omega++;

		break;

		case 'i':

		glBindTexture(GL_TEXTURE_2D, letter[i]);

		omega++;

		break;

		case 'j':

		glBindTexture(GL_TEXTURE_2D, letter[j]);

		omega++;

		break;

		case 'k':

		glBindTexture(GL_TEXTURE_2D, letter[k]);

		omega++;

		break;

		case 'l':

		glBindTexture(GL_TEXTURE_2D, letter[l]);

		omega++;

		break;

		case 'm':

		glBindTexture(GL_TEXTURE_2D, letter[m]);

		omega++;

		break;

		case 'n':

		glBindTexture(GL_TEXTURE_2D, letter[n]);

		omega++;

		break;

		case 'o':

		glBindTexture(GL_TEXTURE_2D, letter[o]);

		omega++;

		break;

		case 'p':

		glBindTexture(GL_TEXTURE_2D, letter[p]);

		omega++;

		break;

		case 'q':

		glBindTexture(GL_TEXTURE_2D, letter[q]);

		omega++;

		break;

		case 'r':

		glBindTexture(GL_TEXTURE_2D, letter[r]);

		omega++;

		break;

		case 's':

		glBindTexture(GL_TEXTURE_2D, letter[s]);

		omega++;

		break;

		case 't':

		glBindTexture(GL_TEXTURE_2D, letter[t]);

		omega++;

		break;

		case 'u':

		glBindTexture(GL_TEXTURE_2D, letter[u]);

		omega++;

		break;

		case 'v':

		glBindTexture(GL_TEXTURE_2D, letter[v]);

		omega++;

		break;

		case 'w':

		glBindTexture(GL_TEXTURE_2D, letter[w]);

		omega++;

		break;

		case 'x':

		glBindTexture(GL_TEXTURE_2D, letter[x]);

		omega++;

		break;

		case 'y':

		glBindTexture(GL_TEXTURE_2D, letter[y]);

		omega++;

		break;

		case 'z':

		glBindTexture(GL_TEXTURE_2D, letter[z]);

		omega++;

		break;

		case ' ':

		glBindTexture(GL_TEXTURE_2D, letter[space]);

		omega++;

		break;


		case '0':

		glBindTexture(GL_TEXTURE_2D, number[0]);

		omega++;

		break;

		case '1':

		glBindTexture(GL_TEXTURE_2D, number[1]);

		omega++;

		break;

		case '2':

		glBindTexture(GL_TEXTURE_2D, number[2]);

		omega++;

		break;

		case '3':

		glBindTexture(GL_TEXTURE_2D, number[3]);

		omega++;

		break;

		case '4':

		glBindTexture(GL_TEXTURE_2D, number[4]);

		omega++;

		break;

		case '5':

		glBindTexture(GL_TEXTURE_2D, number[5]);

		omega++;

		break;

		case '6':

		glBindTexture(GL_TEXTURE_2D, number[6]);

		omega++;

		break;

		case '7':

		glBindTexture(GL_TEXTURE_2D, number[7]);

		omega++;

		break;

		case '8':

		glBindTexture(GL_TEXTURE_2D, number[8]);

		omega++;

		break;

		case '9':

		glBindTexture(GL_TEXTURE_2D, number[9]);

		omega++;

		case '\n':

		omega=1;

		console.number=0;

		break;

		}

		glPushMatrix();
		glTranslatef((omega*10)-90, -80, 1.0);
		glBegin(GL_QUADS);
		glTexCoord2d(0,1); glVertex2f(0,0);
		glTexCoord2d(1,1); glVertex2f(10,0);
		glTexCoord2d(1,0); glVertex2f(10,10);
		glTexCoord2d(0,0); glVertex2f(0,10);
		glEnd();
		glPopMatrix();
	}

}


int ohh_my_god=0;

void pass_char(char c)
{
	if(c == (char)NULL)
	{
		console.buff[ohh_my_god]=c;
		ohh_my_god--;
		console.number--;
	} else 
	{
		console.buff[ohh_my_god]=c;
		ohh_my_god++;
		console.number++;
	}
}

void process_commands(char *com)

{
	int loopy;
	int exitt;
	int sens;
	int speed;
	int fps_status;
	int gaby;
	int map_change;
	int lev;
	char trash[50];
	char next[50];

	exitt=(int)strstr(com, "exit");
	sens=(int)strstr(com, "sensitivity ");
	speed=(int)strstr(com, "walkspeed ");
	fps_status=(int)strstr(com, "drawfps ");
	gaby=(int)strstr(com, "mousegrab ");
	map_change=(int)strstr(com, "map ");
	lev=(int)strstr(com, "spacelev ");

	if(gaby)
	{
		sscanf(com, "%s %d", trash, &mouse_grab);
	}	

	if(speed)
	{
		sscanf(com, "%s %f", trash, &walk_speed);
	}


	if(sens)
	{
		sscanf(com, "%s %f", trash, &sensitivity);
	}	

	if(exitt)
	{
		SDL_Quit();
		exit(0);
	}

	if(fps_status)
	{
		sscanf(com, "%s %d", trash, &draw_fpsc);
	}

	if(lev)
	{
		sscanf(com, "%s %f", trash, &space);
	}

	if(map_change)
	{
		sscanf(com, "%s %s", trash, next);
		load_map(map, next);
	}


	for(loopy=0; loopy<255; loopy++)
	{
		console.command[loopy]=(char) NULL;
	}

}



int loop=0;

void console_input(void)

{
	if(event.key.keysym.sym == SDLK_a)
	{	
		console.command[loop]='a';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_b)

	{	
		console.command[loop]='b';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_c)
	{	
		console.command[loop]='c';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_d)
	{	
		console.command[loop]='d';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_e)
	{	
		console.command[loop]='e';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_f)
	{	
		console.command[loop]='f';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_g)
	{	
		console.command[loop]='g';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_h)
	{	
		console.command[loop]='h';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_i)
	{
		console.command[loop]='i';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_j)
	{
		console.command[loop]='j';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_k)
	{
		console.command[loop]='k';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_l)
	{
		console.command[loop]='l';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_m)
	{
		console.command[loop]='m';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_n)
	{
		console.command[loop]='n';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_o)
	{
		console.command[loop]='o';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_p)
	{
		console.command[loop]='p';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_q)
	{
		console.command[loop]='q';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_r)
	{

		console.command[loop]='r';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_s)
	{
		console.command[loop]='s';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_t)
	{

		console.command[loop]='t';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_u)
	{
		console.command[loop]='u';
		pass_char(console.command[loop]);
		loop++;

	}

	if(event.key.keysym.sym == SDLK_v)
	{
		console.command[loop]='v';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_w)
	{
		console.command[loop]='w';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_x)
	{
		console.command[loop]='x';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_y)
	{
		console.command[loop]='y';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_z)
	{
		console.command[loop]='z';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_SPACE)
	{
		console.command[loop]=' ';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_1)
	{
		console.command[loop]='1';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_2)
	{
		console.command[loop]='2';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_3)
	{
		console.command[loop]='3';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_4)
	{
		console.command[loop]='4';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_5)
	{
		console.command[loop]='5';
		pass_char(console.command[loop]);
		loop++;

	}

	if(event.key.keysym.sym == SDLK_6)
	{
		console.command[loop]='6';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_7)
	{
		console.command[loop]='7';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_8)
	{

		console.command[loop]='8';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_9)

	{
		console.command[loop]='9';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_0)
	{
		console.command[loop]='0';
		pass_char(console.command[loop]);
		loop++;
	}

	if(event.key.keysym.sym == SDLK_BACKSPACE)
	{

		console.command[loop]=(char)NULL;
		pass_char(console.command[loop]);
		loop--;

	}

	if(event.key.keysym.sym == SDLK_RETURN)
	{
		console.command[loop]='\n';
		process_commands(console.command);
		loop=0;
	}
}

int main(int argc, char** argv)
{

	int done;
	int x, y;
	Uint8 *keys;

	if(SDL_Init( SDL_INIT_VIDEO ) < 0) 
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	fmt.freq = 44100;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 4096;        
	fmt.callback = mixaudio;
	fmt.userdata = NULL;



	//if( SDL_OpenAudio(&fmt, NULL) < 0 ) {

	//fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());

	//exit(1);

	//}



	if ( SDL_SetVideoMode(640, 480, 0, SDL_OPENGL) == NULL ) 
	{
		fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}

	SDL_WM_SetCaption("Silver Baloon 3D Engine",NULL);

	Init();

	done = 0;

	// SDL_PauseAudio(0);
	// PlaySound("data/splat.wav");

	while ( ! done ) 
	{

		if(mouse_grab)
		{
			SDL_WM_GrabInput(SDL_GRAB_ON);
			SDL_ShowCursor(0);

		} else 
		{
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			SDL_ShowCursor(1);

		}

		SDL_GetRelativeMouseState(&x, &y);

		xrot+=(float)x/100 * sensitivity;

		yrot+=(float)y/100 * sensitivity;

		keys = SDL_GetKeyState(NULL);

		if(!console.active)
		{
			if(keys[SDLK_w])
			{
				nextXpos-=sin(xrot*(3.1416/180))*walk_speed;
				nextZpos+=cos(xrot*(3.1416/180))*walk_speed;
			//	ypos-=sin((yrot+90)*(3.1416/180))*walk_speed;
				nextYpos+=sin((yrot)*(3.1416/180))*walk_speed;
			}

			if(keys[SDLK_s])

			{
				nextXpos+=sin(xrot*(3.1416/180))*walk_speed;
				nextZpos-=cos(xrot*(3.1416/180))*walk_speed;
			//	ypos+=sin((yrot+90)*(3.1416/180))*walk_speed;
				nextYpos-=sin((yrot)*(3.1416/180))*walk_speed;
			}

			if(keys[SDLK_a])
			{
				nextXpos+=sin((xrot+90)*(3.1416/180))*walk_speed;
			//	xpos+=sin((xrot)*(3.1416/180))*walk_speed;
				nextZpos-=cos((xrot+90)*(3.1416/180))*walk_speed;
			//	zpos-=cos((xrot)*(3.1416/180))*walk_speed;
			}

			if(keys[SDLK_d])
			{
				nextXpos-=sin((xrot+90)*(3.1416/180))*walk_speed;
			//	xpos-=sin((xrot)*(3.1416/180))*walk_speed;
				nextZpos+=cos((xrot+90)*(3.1416/180))*walk_speed;
			//	zpos+=cos((xrot)*(3.1416/180))*walk_speed;
			}

			if(keys[SDLK_SPACE])
			{
				nextYpos-=space;
			}

		}

		display();

		while ( SDL_PollEvent(&event) )
		{
			if ( event.type == SDL_QUIT ) 
			{

				done = 1;
			}

			if ( event.type == SDL_KEYDOWN ) 
			{

				if ( event.key.keysym.sym == SDLK_ESCAPE ) 
				{

					done = 1;

				}

				if(event.key.keysym.sym == SDLK_EQUALS)

				{

					if(!console.active)

					{

					console.active=1;

					} else {

						console.active=0;

					}

				}

				if(console.active)

				{

					console_input();

				}

			}
		}

	}
	SDL_Quit();

	return 1;

}

