#include <stdio.h>

#include <string.h>

#include <stdlib.h>

//#include <GL/gl.h>

//#include <GL/glu.h>

//#include <GL/glext.h>

#include <SDL/SDL.h>

#include <SDL/SDL_opengl.h>



#include "map.h"

#include "utgatex.h"

#include "ctgatex.h"

#include "cmp.c"


void load_ctgatex(IMAGE_CTGA *img, char *filename);
void load_utgatex(IMAGE_UTGA *img, char *filename);
void CalculateNormals(float *vertex, int *vertin, int indexnum, float *normals);



FILE *fp;



static int texture[20];

static int lightmap[20];



IMAGE_CTGA texture_data[20];

IMAGE_CTGA lightmap_data[20];



void load_map_textures(MAP *map)

{

	int index;



	for(index=0; index<map[0].mat_count; index++)

	{

		load_ctgatex(&texture_data[index], map[index].textname1);



		glGenTextures(1, &texture[index]);

                glBindTexture(GL_TEXTURE_2D, texture[index]);

                gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture_data[index].width, texture_data[index].height, GL_RGB, GL_UNSIGNED_BYTE, texture_data[index].data);

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		free(texture_data[index].data);

	}



	for(index=0; index<map[0].mat_count; index++)

	{

		load_ctgatex(&lightmap_data[index], map[index].textname2);



		glGenTextures(1, &lightmap[index]);

                glBindTexture(GL_TEXTURE_2D, lightmap[index]);

                gluBuild2DMipmaps(GL_TEXTURE_2D, 3, lightmap_data[index].width, lightmap_data[index].height, GL_RGB, GL_UNSIGNED_BYTE, lightmap_data[index].data);

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		free(lightmap_data[index].data);

	}

}



void load_map(MAP *mapo, char *filename)

{

	int index;

	char cmp[30];

	char masp[30];

	

        sprintf(cmp, "data/%s.cmp", filename);



        sprintf(masp, "data/%s.s4w", filename);

	

	if(extract_cmp(cmp))

	{

		fp=fopen(masp, "rb");

	

		if(!fp)

		{

			printf("Could not open map %s for reading\n", masp);

		} else {



			fread(&mapo[0].mat_count, sizeof(int), 1, fp);

			for(index=0; index<mapo[0].mat_count; index++)

			{

				fread(&mapo[index].ambient[0], sizeof(float), 1, fp);

	                	fread(&mapo[index].ambient[1], sizeof(float), 1, fp);

	                	fread(&mapo[index].ambient[2], sizeof(float), 1, fp);



	                	fread(&mapo[index].diffuse[0], sizeof(float), 1, fp);

	                	fread(&mapo[index].diffuse[1], sizeof(float), 1, fp);

	                	fread(&mapo[index].diffuse[2], sizeof(float), 1, fp);



	                	fread(&mapo[index].specular[0], sizeof(float), 1, fp);

	                	fread(&mapo[index].specular[1], sizeof(float), 1, fp);

	                	fread(&mapo[index].specular[2], sizeof(float), 1, fp);



	                	fread(&mapo[index].alpha, sizeof(float), 1, fp);

																					

				fread(mapo[index].textname1, sizeof(mapo[index].textname1), 1, fp);

				fread(mapo[index].textname2, sizeof(mapo[index].textname2), 1, fp);

			}



			fread(&mapo[0].obj_count, sizeof(mapo[0].obj_count), 1, fp);



	

			for(index=0; index<mapo[0].obj_count; index++)

			{

				fread(&mapo[index].vNum, sizeof(int), 1, fp);

				fread(&mapo[index].fNum, sizeof(int), 1, fp);

				fread(&mapo[index].tNum, sizeof(int), 1, fp);

				fread(&mapo[index].tIndxNum, sizeof(int), 1, fp);

		

				mapo[index].vert=(float *)malloc(mapo[index].vNum*3*sizeof(float));

				mapo[index].norm=(float *)malloc(mapo[index].fNum*3*sizeof(float));

				mapo[index].indx=(int *)malloc(mapo[index].fNum*3*sizeof(int));

				mapo[index].text=(float *)malloc(mapo[index].tNum*3*sizeof(float));

				mapo[index].tindx=(int *)malloc(mapo[index].tIndxNum*3*sizeof(int));



				fread(mapo[index].vert, sizeof(float), mapo[index].vNum*3, fp);

				fread(mapo[index].indx, sizeof(int), mapo[index].fNum*3, fp);

				fread(mapo[index].text, sizeof(float), mapo[index].tNum*3, fp);

				fread(mapo[index].tindx, sizeof(int), mapo[index].tIndxNum*3, fp);



				fread(&mapo[index].mat_id, sizeof(int), 1, fp);



				CalculateNormals(mapo[index].vert, mapo[index].indx, mapo[index].fNum, mapo[index].norm);

			}

		

		}



	load_map_textures(mapo);

	remove_extracted();

	}


}



void render_map(MAP *map, int active)

{



	int cuenta;

	

	for(cuenta=0; cuenta<map[0].obj_count; cuenta++)

	{

		glEnableClientState(GL_VERTEX_ARRAY);

		glEnableClientState(GL_NORMAL_ARRAY);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);



		glMaterialfv(GL_FRONT, GL_AMBIENT, map[cuenta].ambient);

		glMaterialfv(GL_FRONT, GL_DIFFUSE, map[cuenta].diffuse);

		glMaterialfv(GL_FRONT, GL_SPECULAR, map[cuenta].specular);

		

		if(map[map[cuenta].mat_id].alpha < 1.0)

		{		

		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

		glColor4f(1.0, 1.0, 1.0, map[map[cuenta].mat_id].alpha);

		} else {

			glDisable(GL_BLEND);

			glColor3f(1.0, 1.0, 1.0);

		}



		glPushMatrix();

		glActiveTextureARB(GL_TEXTURE0_ARB);

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, texture[map[cuenta].mat_id]);

		

		glActiveTextureARB(GL_TEXTURE1_ARB);

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, lightmap[map[cuenta].mat_id]);

		

		glVertexPointer(3, GL_FLOAT, 0, map[cuenta].vert);

		glNormalPointer(GL_FLOAT, 0, map[cuenta].norm);



		glClientActiveTextureARB(GL_TEXTURE0_ARB);

		glTexCoordPointer(3, GL_FLOAT, 0, map[cuenta].text);



		glClientActiveTextureARB(GL_TEXTURE1_ARB);

		glTexCoordPointer(3, GL_FLOAT, 0, map[cuenta].text);



//		glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, map[cuenta].text); 

//		glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, map[cuenta].text);

				

		glDrawElements(GL_TRIANGLES, map[cuenta].fNum*3, GL_UNSIGNED_INT, map[cuenta].indx);

		glPopMatrix();

		

		glActiveTextureARB(GL_TEXTURE1_ARB);

		glDisable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE0_ARB);



	}

}

