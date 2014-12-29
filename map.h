typedef struct
{
	int mat_count;
	int obj_count;
	int mat_id;
	int vNum;
	int fNum;
	int tNum;
	int tIndxNum;
	
	float *vert;
	float *norm;
	float *text;
	int   *indx;
	int   *tindx;
	
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shi;
	float alpha;

	char textname1[30];
	char textname2[30];
}MAP;
