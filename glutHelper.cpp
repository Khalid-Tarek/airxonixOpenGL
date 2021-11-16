#include "glutHelper.h"

/**
 *	Renders 3 simple lines signifying the 3 positive axis. Only used for debugging purposes
 */
void renderCoordinateSystem(){
	double origin[3] = {0,0,0};
	double x[3] = {100,0,0};
	double y[3] = {0,100,0};
	double z[3] = {0,0,100};

	glColor3d(0.5,0.5,0.5);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex3dv(origin);		
		glVertex3dv(x);

		glVertex3dv(origin);		
		glVertex3dv(y);

		glVertex3dv(origin);		
		glVertex3dv(z);
	glEnd();
}

/**
 *	Similar to the (renderCuboid function we used in bloxrozOpenGL) 
 *	Only difference here is the center position is not the absolute center, but bottom face's center
 */
void renderCell(Field::Cell cell, double color[3]) {
	//Get the 3D coordinate of the center of the block
	double x = cell.x;
	double y = 0;
	double z = cell.z;
	
	double hL = CELL_SIZE / 2;			//half Length
	double h = cell.raised? 0.5 : 0.1;	//Height
	double hB = CELL_SIZE / 2;			//half Breadth

	//Create an array of all the faces, each face represented by 4 points, and each point having 3 coordinates xyz
	//All faces go from top left -> top right -> bottom right -> bottom left (To make the front of the face on the outside of the cuboid)
	double faces[6][4][3] = {
		{{x - hL, y + h, z + hB}, {x + hL, y + h, z + hB}, {x + hL, y, z + hB}, {x - hL, y, z + hB}},			//front
		{{x + hL, y + h, z + hB}, {x + hL, y + h, z - hB}, {x + hL, y, z - hB}, {x + hL, y, z + hB}},			//right
		{{x + hL, y + h, z - hB}, {x - hL, y + h, z - hB}, {x - hL, y, z - hB}, {x + hL, y, z - hB}},			//back
		{{x - hL, y + h, z - hB}, {x - hL, y + h, z + hB}, {x - hL, y, z + hB}, {x - hL, y, z - hB}},			//left
		{{x - hL, y + h, z - hB}, {x + hL, y + h, z - hB}, {x + hL, y + h, z + hB}, {x - hL, y + h, z + hB}},	//top
		{{x - hL, y, z + hB}, {x + hL, y, z + hB}, {x + hL, y, z - hB}, {x - hL, y, z - hB}},					//bottom
	};

	//render the cuboid
	glColor3dv(color);				//Set the current rendering color to the block's color
	glLineWidth(1);					//Set the line width to the default: 1
	glBegin(GL_QUADS);
		for(int i = 0; i < 6; i++){
			for(int j = 0; j < 4; j++){
				glVertex3dv(faces[i][j]);
			}
		}
	glEnd();

	//render an outline line loop around the cuboid to distinguish faces (MIGHT REMOVE THIS IF WE IMPLEMENT SHADOWS)
	glColor4d(0, 0, 0, 1);			//Set the current rendering color to black
	glLineWidth(1.5);				//Set the line width to: 1.5
	for(int i = 0; i < 6; i++){
		glBegin(GL_LINE_LOOP);
			for(int j = 0; j < 4; j++){
				glVertex3dv(faces[i][j]);
			}
		glEnd();
	}
}

void renderField(Field f){
	
	glColor3d(1, 1, 1);
}