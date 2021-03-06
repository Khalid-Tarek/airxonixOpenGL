#include "glutHelper.h"

/**
 *  Adds some lighting to the scene.
 *	This method is called before the matrix is shifted in front of the perspective so the origin is at the camera
 *	Still needs some tweaking
 */
void addLighting(){
	glPushMatrix();
	GLfloat lightColor[] = {0.3, 0.3, 0.3, 1};

	GLfloat lightPosition0[] = {-5, 2, -10, 1};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glPopMatrix();
}

/**
 *	Renders the GUI of the game.
 *  This method is called before the matrix is shifted in front of the perspective so, we shift it by Z_DISTANCE
 */
void renderGUI(int lives, double percentageFilled, double goalFilled, int timeLeft){
	glPushMatrix();

	glTranslated(0, 0, -Z_DISTANCE);		//Move infront of the origin (-Z) to simulate a screen

	//Draw lives at top left
	renderChar(0,   0, (lives >= 0 ? lives + '0' : '-'));

	//Draw percentageFilled/goalFilled at top center
	percentageFilled *= 100;
	goalFilled *= 100;

	renderChar(8,   0, ((int)percentageFilled / 10) + '0');
	renderChar(9,   0, ((int)percentageFilled % 10) + '0');
	renderChar(10,  0, '/');
	renderChar(11,  0, ((int)goalFilled / 10) + '0');
	renderChar(12,  0, ((int)goalFilled % 10) + '0');

	//Draw timeLeft at top right
	renderChar(20, 0, ((int)timeLeft / 10) + '0');
	renderChar(21, 0, ((int)timeLeft % 10) + '0');

	glPopMatrix();
}

//Draws one character at the passed in coordinates. (the screen usually spans from 0 to Z_DISTANCE in both directions). Origin is top left
void renderChar(double x, double y, char c){

	//Shift input, since it actually goes from -Z_DISTANCE / 2 to Z_DISTANCE / 2
	x -= Z_DISTANCE / 2 + 1;
	y += Z_DISTANCE / 2;

	glPushMatrix();
	glTranslated(x, y, 0);
	glScaled(0.01, 0.01, 0.01);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, c);
	glPopMatrix();

}

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
void renderCell(Field::Cell cell, double raisedColor[3]) {
	//Get the 3D coordinate of the center of the block
	double x = cell.x * CELL_SIZE;
	double y = 0;
	double z = cell.z * CELL_SIZE;
	
	double hL = CELL_SIZE / 2;									//half Length
	double h = (cell.raised * CELL_SIZE * 3) + CELL_SIZE;		//Height
	double hB = CELL_SIZE / 2;									//half Breadth

	if(cell.cellFlag == QUEUED) h = CELL_SIZE * 4; 

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

	double color[3];
	color[0] = 1 - (cell.raised * (1 - raisedColor[0]));
	color[1] = 1 - (cell.raised * (1 - raisedColor[1]));
	color[2] = 1 - (cell.raised * (1 - raisedColor[2]));

	//If it is queued, draw it yellow
	if(cell.cellFlag == QUEUED){
		color[0] = 1;
		color[1] = 1;
		color[2] = 0;
	}

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

	/*
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
	*/
}

//Renders the field one cell at a time
void renderField(Field field){
	for(int i = 0; i < field.dimension; i++)
		for(int j = 0; j < field.dimension; j++)
			renderCell(*field.board[i][j], field.raisedColors);
}

//Renders the player as a sphere at the player's position
void renderPlayer(Player player){
	glPushMatrix();

	glTranslated(player.position[0] / 10.0, CELL_SIZE * 7.5, player.position[1] / 10.0);
	glColor3dv(player.color);
	glutSolidSphere(CELL_SIZE * 3, 20, 20);

	glPopMatrix();
}

//Renders the enemies as cubes at their respective positions
void renderEnemies(vector<Enemy> enemies){
	for(int i = 0; i < enemies.size(); i++){
		glPushMatrix();

		glTranslated(enemies[i].position[0] / 10.0, CELL_SIZE * 5, enemies[i].position[1] / 10.0);
		if(enemies[i].type == FILLED_ENEMY) //Raise the filled enemy a little bit higher than regular enemies
			glTranslated(0, CELL_SIZE * 2.5, 0);

		glColor3dv(enemies[i].color);

		glutSolidCube(CELL_SIZE * 4);

		glPopMatrix();
	}
}