typedef struct
{
	int vNum;
	int fNum;
	int tNum;
	
	float *vert;
	float norm[99999];
	float *text;
	int   *indx;
	
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shi;
	float alpha;

	char textname[100];
}MODEL;
