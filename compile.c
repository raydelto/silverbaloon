#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp1;
FILE *fp2;

main(int argc, char *argv[])
{
	int file;
	int files_to_compile;
	char c, new;
	int size;

	fp2=fopen("out.cmp", "wb");
	for(file=1; file<argc; file++)
	{
		fp1=fopen(argv[file], "rb");
		if(file==1)
		{
			printf("Number of files to read: %d\n", (argc-1));
			files_to_compile=argc-1;
			fwrite(&files_to_compile, sizeof(int), 1, fp2);
		}
		fseek(fp1, 0, SEEK_END);
		size=ftell(fp1);
		fwrite(argv[file], sizeof(char), 20, fp2);
		fwrite(&size, sizeof(int), 1, fp2);
		fseek(fp1, 0, SEEK_SET);
		while(!feof(fp1))
		{
			c=getc(fp1);
			fwrite(&c, sizeof(char), 1, fp2);
		}
		fclose(fp1);
	}
		fclose(fp2);
}
