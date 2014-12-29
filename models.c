#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "models.h"


FILE *fp;

#define SEARCH_STRING1 "*MATERIAL_AMBIENT"
#define SEARCH_STRING2 "*MATERIAL_DIFFUSE"
#define SEARCH_STRING3 "*MATERIAL_SPECULAR"
#define SEARCH_STRING4 "*MATERIAL_SHINE"
#define SEARCH_STRING5 "*MATERIAL_TRANSPARENCY"
#define SEARCH_STRING6 "*MESH_NUMVERTEX"
#define SEARCH_STRING7 "*MESH_NUMFACES"
#define SEARCH_STRING8 "*MESH_VERTEX"
#define SEARCH_STRING9 "*MESH_FACE"
#define SEARCH_STRING10 "*MESH_NUMTVERTEX"
#define SEARCH_STRING11 "*MESH_TVERT"
#define SEARCH_STRING12 "*BITMAP"

extern void CalculateNormals(float *vertex, int *vertin, int indexnum, float *normals);

int open_file(char *filename)
{
	fp=fopen(filename, "r");
	if(!fp)
	{
		printf("Model %s could not be opened for reading\n", filename);
		return 0;
	} else {
		return 1;
	}
}

void read_line(FILE *fp, char *s)
{
	do 
	{
		fgets(s, 255, fp);
	}while(s[0] == '\n');
}

void parse_file(MODEL *model)
{
	int status;
	int index, index2=0;
	char trash[255];
	char line[255];

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING1);
	}while(!status);
	sscanf(line, "%s %f %f %f", trash, &model->ambient[0], &model->ambient[1], &model->ambient[2]);

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING2);
	}while(!status);
	sscanf(line, "%s %f %f %f", trash, &model->diffuse[0], &model->diffuse[1], &model->diffuse[2]);

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING3);
	}while(!status);
	sscanf(line, "%s %f %f %f", trash, &model->specular[0], &model->specular[1], &model->specular[2]);

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING4);
	}while(!status);
	sscanf(line, "%s %f", trash, &model->shi);

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING5);
	}while(!status);
	sscanf(line, "%s %f", trash, &model->alpha);

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING12);
	}while(!status);
	sscanf(line, "%s %s", trash, model->textname);


//END OF MATERIALS!!!!

	
	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING6);
	}while(!status);
	sscanf(line, "%s %d", trash, &model->vNum);
model->vert=(float *)malloc(model->vNum*3*sizeof(float));

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING7);
	}while(!status);
	sscanf(line, "%s %d", trash, &model->fNum);
model->indx=(int *)malloc(model->fNum*3*sizeof(int));	
	
	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING8);
	}while(!status);


	for(index=0; index<model->vNum; index++)
	{
		read_line(fp, line);
		sscanf(line, "%s %s %f %f %f", trash, trash, &model->vert[index2], &model->vert[index2+1], &model->vert[index2+2]);
		index2+=3;

	}

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING9);
	}while(!status);
	
	index2=0;
	for(index=0; index<model->fNum; index++)
	{
		read_line(fp, line);
		sscanf(line, "%s %s %s %d %s %d %s %d %s", trash, trash, trash, &model->indx[index2], trash, &model->indx[index2+1], trash, &model->indx[index2+2], trash);
		index2+=3;

	}


	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING10);
	}while(!status);
	sscanf(line, "%s %d", trash, &model->tNum);
model->text=(float *)malloc(model->tNum*3*sizeof(float));

	do
	{
		read_line(fp, line);
		status=(int) strstr(line, SEARCH_STRING11);
	}while(!status);

	index2=0;
	for(index=0; index<model->tNum; index++)
	{
		read_line(fp, line);
		sscanf(line, "%s %s %f %f %f", trash, trash, &model->text[index2], &model->text[index2+1], &model->text[index2+2]);
		index2+=3;
	}

	
	
	
	CalculateNormals(model->vert, model->indx, model->vNum, model->norm);
}

void load_models(MODEL *model, char *filename)
{
	int status;
	status=open_file(filename);
	if(!status)
	{
		printf("Model %s not loaded\n", filename);
	} else {
		parse_file(model);
	}
	
}
