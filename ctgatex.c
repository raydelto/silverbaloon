#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctgatex.h"

FILE *fp;

void load_ctgatex(IMAGE_CTGA *img, char *filename)
{
	int currentpixel=0;		
	int pixelcount=0;
	int bytee=0;
	int s;
	int length;
	int fu;
	GLubyte uheader_format[12]={0,0,10,0,0,0,0,0,0,0,0,0};
	GLubyte header_to_compare[12];
	GLubyte attr[6];
	GLubyte first=0;
	GLubyte temp[4];

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
			pixelcount = img->width * img->height;
			img->data=(GLubyte *)malloc(length);
			if(!img->data)
			{
				printf("Could not allocate memory for the TGA texture\n");
			} else {

				
				do {
					fread(&first, 1, sizeof(GLubyte), fp);
					if(first < 128)
					{
						first++;
						for(fu=0; fu<first; fu++)
						{
							fread(temp, 1, img->bpp, fp);
							img->data[bytee]=temp[2];
							img->data[bytee+1]=temp[1];
							img->data[bytee+2]=temp[0];

							if(img->bpp == 4)
							{
								img->data[bytee+3]=temp[3];
							}
							bytee+=img->bpp;
							currentpixel++;
						}
					} else {
						first-=127;
						fread(temp, 1, img->bpp, fp);
						
						for(fu=0; fu<first; fu++)
						{
							img->data[bytee]=temp[2];
							img->data[bytee+1]=temp[1];
							img->data[bytee+2]=temp[0];

							if(img->bpp == 4)
							{
								img->data[bytee+3]=temp[3];
							}

							bytee+=img->bpp;
							currentpixel++;
						}
					}
				}while(currentpixel < pixelcount);
			
			
			}

		}
	}

	fclose(fp);
	
}
