#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include "map.h"
#include "md2.h"

typedef struct
{
	double x, y, z;
}vector_t;


double dot(vector_t v1, vector_t v2)
{
	double resu;
	resu = ( (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z) );
	return resu;
}

vector_t substract_vectors(vector_t vect1, vector_t vect2)
{
	vector_t out;
	
	out.x = vect1.x - vect2.x;
	out.y = vect1.y - vect2.y;
	out.z = vect1.z - vect2.z;

	return out;
}

vector_t normalize(vector_t vect)
{
	double l;
	vector_t n;
	
	l = sqrt(((vect.x*vect.x) + (vect.y*vect.y) + (vect.z*vect.z)));
	
	n.x = vect.x / l;
	n.y = vect.y / l;
	n.z = vect.z / l;

	return n;
}

double length(vector_t v1)
{
	double length;
	length = sqrt( v1.x*v1.x + v1.y*v1.y + v1.z*v1.z );
	return length;
}

int check_angles(vector_t point1, vector_t point2, vector_t point3, vector_t currv)
{
	double angle;
	double dot1, dot2, dot3;
	
	vector_t v1;
	vector_t v2;
	vector_t v3;

	v1 = substract_vectors(point1, currv);
	v2 = substract_vectors(point2, currv);
	v3 = substract_vectors(point3, currv);

	v1 = normalize(v1);
	v2 = normalize(v2);
	v3 = normalize(v3);

	dot1 = dot(v1, v2);
	dot2 = dot(v2, v3);
	dot3 = dot(v3, v1);

	angle=acos(dot1)*180/3.1416 + acos(dot2)*180/3.1416 + acos(dot3)*180/3.1416;
	
//printf("----------------------------------------------\n");
//printf("%f %f %f\n", point1.x,point1.y, point1.z);
//printf("%f %f %f\n", point2.x,point2.y, point2.z);
//printf("%f %f %f\n", point3.x,point3.y, point3.z);
//printf("%f %f %f\n", currv.x, currv.y, currv.z);
//printf("Angle: %f\n", angle);
//printf("----------------------------------------------\n");
	
	if(angle < 359)
	{
		return 0;
	} else {
		return 1;
	}
	
}



int test_in_polygon(MAP *map, float currx, float curry, float currz)
{
	int index=0, index2=0, index3=0;
	double point1[3], point2[3], point3[3];
	double D, d, Distance;
	int yes;
	
	vector_t currv;
	vector_t p1;
	vector_t p2;
	vector_t p3;
	
	for(index=0; index<map[0].obj_count; index++)
	{
		index3=0;
		for(index2=0; index2<map[index].fNum; index2++)
		{
			point1[0]=map[index].vert[(map[index].indx[index3]*3)	];
			point1[1]=map[index].vert[(map[index].indx[index3]*3)+1	];
			point1[2]=map[index].vert[(map[index].indx[index3]*3)+2	];
		
			point2[0]=map[index].vert[(map[index].indx[index3+1]*3)	];
			point2[1]=map[index].vert[(map[index].indx[index3+1]*3)+1	];
			point2[2]=map[index].vert[(map[index].indx[index3+1]*3)+2	];
		
			point3[0]=map[index].vert[(map[index].indx[index3+2]*3)	];
			point3[1]=map[index].vert[(map[index].indx[index3+2]*3)+1	];
			point3[2]=map[index].vert[(map[index].indx[index3+2]*3)+2	];

			
			D = (point1[0]*map[index].norm[index3]) + (point1[1]*map[index].norm[index3+1]) + (point1[2]*map[index].norm[index3+2]);
			d = (currx*map[index].norm[index3]) + (curry*map[index].norm[index3+1]) + (currz*map[index].norm[index3+2]);	
			
			index3+=3;
			
			Distance = D-d;
			Distance=fabs(Distance);

			if(Distance <= 2)
			{
				currv.x=currx;
				currv.y=curry;
				currv.z=currz;
				
				p1.x=point1[0];
				p1.y=point1[1];
				p1.z=point1[2];
				
				p2.x=point2[0];
				p2.y=point2[1];
				p2.z=point2[2];
		
				p3.x=point3[0];
				p3.y=point3[1];
				p3.z=point3[2];
				yes = check_angles(p1, p2, p3, currv);
			
				if(yes)
				{
					return 1;
				} 
			
			
			} 

	
		}
	}
	return 0;
}


int test_sphere_colision(modelData *m, float xpos, float ypos, float zpos, float radius)
{
	float distance;

	distance=sqrt(	((m->x-xpos)*(m->x-xpos)) +	((m->y-ypos)*(m->y-ypos)) + ((m->z-zpos)*(m->z-zpos))	);
	
	if(distance <= (m->radius+radius))
	{
		return 1;
	} else {
		return 0;	
	}

}
