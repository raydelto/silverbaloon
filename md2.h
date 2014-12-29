#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AI_IDLE1	0
#define AI_IDLE2	1
#define AI_IDLE3	2
#define AI_IDLE4	3
#define AI_RUNNING	4
#define AI_WALKING	5
#define AI_DEAD		6
#define AI_SCARED	7


typedef struct
{
	int id;
	int version;
	int twidth;
	int theight;
	int framesize;
	int textures;
	int vNum;
	int tNum;
	int fNum;
	int numGLcmds;
	int Number_Of_Frames;
	int offsetSkins;
	int offsetTCoord;
	int offsetIndx;
	int offsetFrames;
	int offsetGLcmds;
	int offsetEnd;
}header;

typedef struct
{
	float s;
	float t;
}textcoord;

typedef struct
{
	short s;
	short t;
}textindx;

typedef struct
{
	unsigned char v[3];
	unsigned char normalIndex;
}framePoint_t;

typedef struct
{
	float scale[3];
	float translate[3];
	char name[16];
	framePoint_t fp[1];
}frame;

typedef struct
{
	unsigned short meshIndex[3];
	unsigned short stIndex[3];
}mesh;

typedef struct
{
	float point[3];
}vector;

typedef struct
{
	int numFrames;
	int numPoints;
	int numTriangles;
	int numST;
	int frameSize;
	int twidth;
    	int theight;
	int currentFrame;
	int nextFrame;
	float interpol;
	mesh *triIndx;
	textcoord *st;
	vector *pointList;
	float x, y, z;
	float nextX, nextY, nextZ;
	float radius;
	float dist_to_player;
	int state;
	float speed;
}modelData;
