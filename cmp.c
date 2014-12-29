#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
	char filename[80];
}FILES;

FILES files[50];

int numero;

int extract_cmp(char *file)
{
	FILE *fp;
	FILE *fp2;
	int number;
	int size;
	int index, index0;
	char c;
	char filename[80];
	char out[80];

	fp=fopen(file, "rb");
	if(fp)
	{
	fread(&number, sizeof(int), 1, fp);

	for(index0=0; index0<number; index0++)
	{
		fread(filename, sizeof(char), 20, fp);
		fread(&size, sizeof(int), 1, fp);
		sprintf(out, "data/%s", filename);
		sprintf(files[index0].filename, "%s", out);
		fp2=fopen(out, "wb");
		for(index=0; index<=size; index++)
		{
			c=getc(fp);
			fwrite(&c, sizeof(char), 1, fp2);
		}
		fclose(fp2);
	}
	fclose(fp);
	numero=index0;
	return 1;
	} else {
		printf("Could not open CMP file for reading\n");
		return 0;
	}
}

void remove_extracted(void)
{
	int index;
	
	for(index=0; index<numero; index++)
	{
		unlink(files[index].filename);
		printf("Removing temporary files: %s\n", files[index].filename);
	}
}
