#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utgatex.h"

FILE *fp;

void load_utgatex(IMAGE_UTGA *img, char *filename)
{
	GLubyte uheader_format[12]={0,0,2,0,0,0,0,0,0,0,0,0};
	GLubyte header_to_compare[12];
	GLubyte attr[6];
	int s;
	unsigned int length;
	unsigned int temp;
	int fu;

	fp=fopen(filename, "rb");

	if(!fp)
	{
		printf("Could not open file %s\n", filename);
	} else {
		fread(header_to_compare, 1, sizeof(header_to_compare), fp);
		s=memcmp(uheader_format, header_to_compare, sizeof(uheader_format));

		if(s==0) 
		{
			fread(attr, 1, sizeof(attr), fp);
			img->width=attr[1] * 256 + attr[0];
			img->height=attr[3] * 256 + attr[2];
			img->bpp=attr[4]/8;
			length = img->width * img->height * img->bpp;
			img->data=(GLubyte *)malloc(length);
			if(!img->data)
			{
				printf("Could not allocate memory for the TGA texture\n");
			} else {
				fread(img->data, 1, length, fp); 
			}

			for(fu=0; fu<length; fu+=img->bpp)
			{
				temp=img->data[fu];
				img->data[fu]=img->data[fu + 2];
				img->data[fu +2]=temp;
			}	

		}
	}

	fclose(fp);
	
}
