#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include "md2.h"

extern void Normal( float *p1, float *p2, float *p3 );

modelData *load_md2(char *filename, char *texturename)
{
FILE *fp;
int length;
int yahoo;
int google;

char *buffer;

modelData *model;
header *head;
textindx *stPtr;

frame *fra;
vector *pntlst;
mesh *triIndex, *bufIndexPtr;

fp=fopen(filename, "rb");
fseek(fp, 0, SEEK_END);
length=ftell(fp);
fseek(fp, 0, SEEK_SET);

buffer=(char*)malloc(length+1);
fread(buffer, sizeof(char), length, fp);

head=(header*)buffer;
model=(modelData*)malloc(sizeof(modelData));

model->pointList=(vector*)malloc(sizeof(vector)*head->vNum*head->Number_Of_Frames);
model->numPoints=head->vNum;
model->numFrames=head->Number_Of_Frames;
model->frameSize=head->framesize;

for(yahoo=0; yahoo<head->Number_Of_Frames; yahoo++)
{
	fra=(frame *)&buffer[head->offsetFrames + head->framesize * yahoo];
	pntlst=(vector *)&model->pointList[head->vNum * yahoo];
	for(google=0; google<head->vNum; google++)
	{
		pntlst[google].point[0]=fra->scale[0] * fra->fp[google].v[0] + fra->translate[0];
		pntlst[google].point[1]=fra->scale[1] * fra->fp[google].v[1] + fra->translate[1];
		pntlst[google].point[2]=fra->scale[2] * fra->fp[google].v[2] + fra->translate[2];
	}
}

model->st=(textcoord *)malloc(sizeof(textcoord)*head->tNum);
model->numST=head->tNum;
stPtr=(textindx *)&buffer[head->offsetTCoord];

for(yahoo=0; yahoo<head->tNum; yahoo++)
{
	model->st[yahoo].s=(float)stPtr[yahoo].s / (float)head->twidth; 
	model->st[yahoo].t=(float)stPtr[yahoo].t / (float)head->theight;
}

triIndex=(mesh *)malloc(sizeof(mesh) * head->fNum);
model->triIndx=triIndex;
model->numTriangles=head->fNum;
bufIndexPtr=(mesh*)&buffer[head->offsetIndx];

for(yahoo=0; yahoo<head->Number_Of_Frames; yahoo++)
{
	for(google=0; google<head->fNum; google++)
	{
		triIndex[google].meshIndex[0]=bufIndexPtr[google].meshIndex[0];
		triIndex[google].meshIndex[1]=bufIndexPtr[google].meshIndex[1];
		triIndex[google].meshIndex[2]=bufIndexPtr[google].meshIndex[2];

		triIndex[google].stIndex[0]=bufIndexPtr[google].stIndex[0];
		triIndex[google].stIndex[1]=bufIndexPtr[google].stIndex[1];
		triIndex[google].stIndex[2]=bufIndexPtr[google].stIndex[2];
		
	}
}

model->currentFrame = 0;
model->nextFrame = 1;
model->interpol = 0.0;

fclose(fp);
return model;
}

float run;
void render_md2_interpol(modelData *model, int startFrame, int endFrame, int t)
{
	int index;
	vector *stFrame;
	vector *cdFrame;
	vector vector[3];

	float x1, y1, z1;
	float x2, y2, z2;

	if(model->interpol==0.0)
	{
		model->currentFrame = startFrame;
	}
	
	if (model->interpol >= 1.0)
        {
		model->interpol = 0.0f;
		model->currentFrame++;
		
		if (model->currentFrame >= endFrame)
		{
 			model->currentFrame = startFrame; 
		}
		
		model->nextFrame = model->currentFrame + 1;
		
		
		if (model->nextFrame >= endFrame)
		{
			model->nextFrame = startFrame;
	        }
		
	}


		stFrame = &model->pointList[model->numPoints*model->currentFrame];
		cdFrame = &model->pointList[model->numPoints*model->nextFrame];

		glBindTexture(GL_TEXTURE_2D, t);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glBegin(GL_TRIANGLES);
		for(index=0; index<model->numTriangles; index++)
		{
		x1 = stFrame[model->triIndx[index].meshIndex[0]].point[0];
		y1 = stFrame[model->triIndx[index].meshIndex[0]].point[1];
		z1 = stFrame[model->triIndx[index].meshIndex[0]].point[2];
		
		x2 = cdFrame[model->triIndx[index].meshIndex[0]].point[0];
		y2 = cdFrame[model->triIndx[index].meshIndex[0]].point[1];
		z2 = cdFrame[model->triIndx[index].meshIndex[0]].point[2];

		vector[0].point[0] = x1 + model->interpol * (x2 - x1);
		vector[0].point[1] = y1 + model->interpol * (y2 - y1);
		vector[0].point[2] = z1 + model->interpol * (z2 - z1);

		x1 = stFrame[model->triIndx[index].meshIndex[2]].point[0];
		y1 = stFrame[model->triIndx[index].meshIndex[2]].point[1];
		z1 = stFrame[model->triIndx[index].meshIndex[2]].point[2];
		
		x2 = cdFrame[model->triIndx[index].meshIndex[2]].point[0];
		y2 = cdFrame[model->triIndx[index].meshIndex[2]].point[1];
		z2 = cdFrame[model->triIndx[index].meshIndex[2]].point[2];

		vector[2].point[0] = x1 + model->interpol * (x2 - x1);
		vector[2].point[1] = y1 + model->interpol * (y2 - y1);
		vector[2].point[2] = z1 + model->interpol * (z2 - z1);
		
		x1 = stFrame[model->triIndx[index].meshIndex[1]].point[0];
		y1 = stFrame[model->triIndx[index].meshIndex[1]].point[1];
		z1 = stFrame[model->triIndx[index].meshIndex[1]].point[2];
		
		x2 = cdFrame[model->triIndx[index].meshIndex[1]].point[0];
		y2 = cdFrame[model->triIndx[index].meshIndex[1]].point[1];
		z2 = cdFrame[model->triIndx[index].meshIndex[1]].point[2];

		vector[1].point[0] = x1 + model->interpol * (x2 - x1);
		vector[1].point[1] = y1 + model->interpol * (y2 - y1);
		vector[1].point[2] = z1 + model->interpol * (z2 - z1);
		
		Normal(vector[0].point, vector[2].point, vector[1].point);
		
		glTexCoord2f(model->st[model->triIndx[index].stIndex[0]].s, model->st[model->triIndx[index].stIndex[0]].t);
		glVertex3fv(vector[0].point);
		
		glTexCoord2f(model->st[model->triIndx[index].stIndex[2]].s, model->st[model->triIndx[index].stIndex[2]].t);
		glVertex3fv(vector[2].point);
		
		glTexCoord2f(model->st[model->triIndx[index].stIndex[1]].s, model->st[model->triIndx[index].stIndex[1]].t);
		glVertex3fv(vector[1].point);

		}
		glEnd();
		model->interpol+=0.1;
}

