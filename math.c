#include <math.h>
#include <SDL/SDL_opengl.h>
#include "md2.h"


void CalculateNormals(float *vertex, int *vertin, int indexnum, float *normals)
{
float x1, x2, x3, y1, y2, y3, z1, z2, z3;
float xx1, xx2, yy1, yy2, zz1, zz2;
float Length;
int count, count2;
count2=0;
for(count=0; count < (indexnum); count++) {

x1=vertex[  vertin[count2]*3];
y1=vertex[( vertin[count2]*3)+1];
z1=vertex[(vertin[count2]*3)+2];

x2=vertex[ vertin[count2+1]*3];
y2=vertex[(vertin[count2+1]*3)+1];
z2=vertex[(vertin[count2+1]*3)+2];

x3=vertex[ vertin[count2+2]*3];
y3=vertex[(vertin[count2+2]*3)+1];
z3=vertex[(vertin[count2+2]*3)+2];

xx1=x1-x2;
yy1=y1-y2;
zz1=z1-z2;

xx2=x1-x3;
yy2=y1-y3;
zz2=z1-z3;

normals[count2+0]=(yy1*zz2)-(yy2*zz1);
normals[count2+1]=(zz1*xx2)-(xx1*zz2);
normals[count2+2]=(xx1*yy2)-(yy1*xx2);


Length = sqrt((normals[count2]*normals[count2]) + (normals[count2+1]*normals[count2+1]) + (normals[count2+2]*normals[count2+2]));

normals[count2]= normals[count2] / Length;
normals[count2+1]=normals[count2+1] / Length;
normals[count2+2]=normals[count2+2] / Length;

count2=count2+3;
}

}


void Normal(float *p1, float *p2, float *p3)
{
	float a[3], b[3], result[3];
	float length;

	a[0] = p1[0] - p2[0];
	a[1] = p1[1] - p2[1];
	a[2] = p1[2] - p2[2];

	b[0] = p1[0] - p3[0];
	b[1] = p1[1] - p3[1];
	b[2] = p1[2] - p3[2];

	result[0] = a[1] * b[2] - b[1] * a[2];
	result[1] = b[0] * a[2] - a[0] * b[2];
	result[2] = a[0] * b[1] - b[0] * a[1];

	length = sqrt(result[0]*result[0] + result[1]*result[1] + result[2]*result[2]);
					 
	glNormal3f(result[0]/length, result[1]/length, result[2]/length);
}

float distPlayerToEnemy(modelData *m, float x, float y, float z)
{
	float distance;
	distance=sqrt( (m->x-x) * (m->x-x) + (m->y-y) * (m->y-y) + (m->z-z) * (m->z-z) );
	return distance;
}
