#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *filep;
char line[255];
int number_of_objects;
int materials;

#define needle1 "*MATERIAL_COUNT "
#define needle2 "*GEOMOBJECT "
#define needle3 "*BITMAP "

typedef struct
{
	int vNum;
	int fNum;
	int tNum;
	int tIndxNum;
	
	float *vert;
	float *text;
	int   *indx;
	int   *tindx;
	
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float alpha;
	
	char text_name1[30];
	char text_name2[30];

	int id;
}OBJECT;

OBJECT obj[200];

int tNum[200];

read_line(FILE *fp, char *line)
{
	do
	{
		fgets(line, 255, fp);
	}while(line[0] == '\n');
}


void parse_file(char *filename)
{	
	int index, index2;
	int status, status2;
	int count=0;
	char trash[255];
	filep=fopen(filename, "r");
	if(filep)
	{
		while(!feof(filep))
		{
			read_line(filep, line);
			status=(int)strstr(line, needle2);
			if(status)
			{
				number_of_objects++;
			}
			status2=(int)strstr(line, needle1);
			if(status2)
			{
				sscanf(line, "%s %d", trash, &materials);
			}
		}
		fseek(filep, 0, SEEK_SET);
		printf("Number of Objects: %d\n", number_of_objects);
		printf("Number of Materials: %d\n", materials);

		
		for(index=0; index<materials; index++)
		{
			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MATERIAL_AMBIENT ");
			}while(!status);
			sscanf(line, "%s %f %f %f", trash, &obj[index].ambient[0], &obj[index].ambient[1], &obj[index].ambient[2]);
			
			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MATERIAL_DIFFUSE ");
			}while(!status);
			sscanf(line, "%s %f %f %f", trash, &obj[index].diffuse[0], &obj[index].diffuse[1], &obj[index].diffuse[2]);

			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MATERIAL_SPECULAR ");
			}while(!status);
			sscanf(line, "%s %f %f %f", trash, &obj[index].specular[0], &obj[index].specular[1], &obj[index].specular[2]);

			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MATERIAL_TRANSPARENCY ");
			}while(!status);
			sscanf(line, "%s %f", trash, &obj[index].alpha);
			
			do
			{
				read_line(filep, line);
				status=(int)strstr(line, needle3);
			}while(!status);
			sscanf(line, "%s %s", trash, obj[index].text_name1);

			do
			{
				read_line(filep, line);
				status=(int)strstr(line, needle3);
			}while(!status);
			sscanf(line, "%s %s", trash, obj[index].text_name2);
			
			printf("Name %d : %s\n", index, obj[index].text_name1);
			printf("Name %d : %s\n", index, obj[index].text_name2);
		}

		for(index=0; index<number_of_objects; index++)
		{
			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MESH_NUMVERTEX ");
			}while(!status);
			sscanf(line, "%s %d", trash, &obj[index].vNum);

			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MESH_NUMFACES ");
			}while(!status);
			sscanf(line, "%s %d", trash, &obj[index].fNum);

			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MESH_VERTEX ");
			}while(!status);

			obj[index].vert=(float *)malloc(obj[index].vNum*3*sizeof(float));
			
			count=0;

			for(index2=0; index2<obj[index].vNum; index2++)
			{
			// EDITED HERE!!!!!!!
			sscanf(line, "%s %s %f %f %f", trash, trash, &obj[index].vert[count], &obj[index].vert[count+2], &obj[index].vert[count+1]);
			obj[index].vert[count+2]=-obj[index].vert[count+2];
			read_line(filep, line);
			count+=3;
			}

			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MESH_FACE ");
			}while(!status);
			
			count=0;

			obj[index].indx=(int *)malloc(obj[index].fNum*3*sizeof(int));
			
			for(index2=0; index2<obj[index].fNum; index2++)
			{
			sscanf(line, "%s %s %s %d %s %d %s %d %s", trash, trash, trash, &obj[index].indx[count], trash, &obj[index].indx[count+1], trash, &obj[index].indx[count+2], trash);
			read_line(filep, line);
			count+=3;
			}

			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MESH_NUMTVERTEX ");
			}while(!status);
			sscanf(line, "%s %d", trash, &tNum[index]);

			obj[index].text=(float *)malloc(tNum[index]*3*sizeof(float));

			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MESH_TVERT ");
			}while(!status);

			
			
			count=0;
			for(index2=0; index2<tNum[index]; index2++)
			{
				sscanf(line, "%s %s %f %f %f", trash, trash, &obj[index].text[count], &obj[index].text[count+1], &obj[index].text[count+2]);
				read_line(filep, line);
				count+=3;
			}

			// NEW CODE
			
			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MESH_NUMTVFACES ");
			}while(!status);
			sscanf(line, "%s %d", trash, &obj[index].tIndxNum);
			obj[index].tindx=(int *)malloc(obj[index].tIndxNum*3*sizeof(int));
			
			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MESH_TFACE ");
			}while(!status);
			
			count=0;
			for(index2=0; index2<obj[index].tIndxNum; index2++)
			{
				sscanf(line, "%s %s %d %d %d", trash, trash, &obj[index].tindx[count], &obj[index].tindx[count+1], &obj[index].tindx[count+2]);
				read_line(filep, line);
				count+=3;
			}
			
			// NEW CODE
			
			do
			{
				read_line(filep, line);
				status=(int)strstr(line, "*MATERIAL_REF ");
			}while(!status);
			sscanf(line, "%s %d", trash, &obj[index].id);

		}

		fclose(filep);
		
	} else {
		printf("Could not read file %s\n", filename);
	}
}

void write_file(char *filename)
{
	int index=0;
	filep=fopen(filename, "wb");

	fwrite(&materials, sizeof(int), 1, filep);

	for(index=0; index<materials; index++)
	{
		fwrite(&obj[index].ambient[0], sizeof(float), 1, filep);
		fwrite(&obj[index].ambient[1], sizeof(float), 1, filep);
		fwrite(&obj[index].ambient[2], sizeof(float), 1, filep);

		fwrite(&obj[index].diffuse[0], sizeof(float), 1, filep);
		fwrite(&obj[index].diffuse[1], sizeof(float), 1, filep);
		fwrite(&obj[index].diffuse[2], sizeof(float), 1, filep);

		fwrite(&obj[index].specular[0], sizeof(float), 1, filep);
		fwrite(&obj[index].specular[1], sizeof(float), 1, filep);
		fwrite(&obj[index].specular[2], sizeof(float), 1, filep);

		fwrite(&obj[index].alpha, sizeof(float), 1, filep);

		fwrite(obj[index].text_name1, sizeof(obj[index].text_name1), 1, filep);
		fwrite(obj[index].text_name2, sizeof(obj[index].text_name2), 1, filep);
	}

	fwrite(&number_of_objects, sizeof(int), 1, filep);
	
	for(index=0; index<number_of_objects; index++)
	{
		fwrite(&obj[index].vNum, sizeof(int), 1, filep);
		fwrite(&obj[index].fNum, sizeof(int), 1, filep);
		fwrite(&tNum[index], sizeof(int), 1, filep);
		fwrite(&obj[index].tIndxNum, sizeof(int), 1, filep);

		fwrite(obj[index].vert, sizeof(float), obj[index].vNum*3, filep);
		fwrite(obj[index].indx, sizeof(int), obj[index].fNum*3, filep);
		fwrite(obj[index].text, sizeof(float), tNum[index]*3, filep);
		fwrite(obj[index].tindx, sizeof(int), obj[index].tIndxNum*3, filep);

		fwrite(&obj[index].id, sizeof(int), 1, filep);
		
	}
	
	fclose(filep);
}

main(int argc, char *argv[])
{
	if(argc < 2)
	{
	printf("You MUST specify an source and a destination filename\n");
	} else {
		parse_file(argv[1]);
		write_file(argv[2]);
	}	
}
