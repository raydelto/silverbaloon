#include <stdio.h>
#include <math.h>
#include "rawtex.h"


FILE *fp;

void load_rawtex(IMAGE_RAW *img, char *filename)
{
	int div;
	int out;
	fp=fopen(filename, "rb");	
	if(!fp) 
	{
		printf("Error, could not read input Texture %s\n", filename);
	} else {
	fseek(fp, 0L, SEEK_END);
	img->size=ftell(fp);
	div=img->size/3;
	out=sqrt(div);
	img->x=out;
	img->y=out;
	fseek(fp, 0L, SEEK_SET);
	fread(img->data, sizeof(GLubyte), img->size, fp);
	fclose(fp);
	}
}
