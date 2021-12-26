/*********************************************************************************************************	
								 _    _    __  __           _      
								/ \  (_)_ _\ \/ /___  _ __ (_)_  __
							   / _ \ | | '__\  // _ \| '_ \| \ \/ /
							  / ___ \| | |  /  \ (_) | | | | |>  < 
							 /_/   \_\_|_| /_/\_\___/|_| |_|_/_/\_\

  Description:	This game was made by The Rad Squad for 3D Graphics Course - EELU (Fall 2021-22)

				In this 3D game, you take control of a spherical ball on a 2D board. Your goal is to fill
				a percentage of the board by enclosing parts of it with your drawing line.

				But beware! Those evil squares aren't going to let you off that easily! If they touch
				you or your line before you enclose an area, you'll get hit. Get hit enough times, and
				you lose the game!

  Developers:	Khalid Tarek
				Abdallah Kareem
				Mona Mohammed Saeed
				Ahmed Mohammed El-Sayed Moustafa
				Osama Sakr

  Project
  Structure:	main.cpp		& main.h	--------------> Program Entry Point + main logic
				glutHelper.cpp	& glutHelper.h -----------> All OpenGL rendering happens here in its 
															respective functions
				field.cpp --------------------------------> Field class + Cell inner class
				entity.cpp -------------------------------> Entity class + Player & Enemy Subclasses

  Rights:		All rights go back to the original designer(s) of the games
						Xonix (1984):		Ilan Rav Dani Katz
						AirXonix (2000):	AXYSOFT

				This is a simple reimplementation of the original games, hope you enjoy it!
*********************************************************************************************************/

#include "main.h"

//Current Game State
int gameState = PLAYING;

//The angles of view of the scene
double sceneRotateX = 30;
double sceneRotateY = 0;

//Game Objects used to represent the player, level and enemies
Player player;
Field level;
vector<Enemy> enemies;

//Periodically Checks for change in the gameState variable
void checkGameState(){
	if(gameState != PLAYING) return;

	if(player.lives < 0 || level.timeLeft <= 0){
		gameState = LOST;
		player.color[0] = 0;
		player.color[1] = 0;
		player.color[2] = 0;
		//TODO: "You Lost!" Pop up
	}
		
	if(level.currentlyFilled >= level.winCondition){
		gameState = WON;
		player.color[0] = 0;
		player.color[1] = 1;
		player.color[2] = 0;
		//TODO: "You Won!" Pop up
	}
		
}

//Main display function. Draws 1 frame at a time
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	addLighting();

	renderGUI(player.lives, level.currentlyFilled, level.winCondition, level.timeLeft);

	glTranslated(-5, 0, -20);
	glRotated(sceneRotateX, 1, 0, 0);
	glRotated(sceneRotateY, 0, 1, 0);

	//renderCoordinateSystem();

	renderField(level);

	renderPlayer(player);

	renderEnemies(enemies);
	
	glutSwapBuffers();
}

//Method used to stop the user from resizing the window
void lockResizing(int width, int height){
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	display();
}

//Arrows are used to rotate around the scene
void arrows(int key, int x, int y){
	switch(key){
		case GLUT_KEY_DOWN:
			sceneRotateX += 5;
			break;
		case GLUT_KEY_UP:
			sceneRotateX -= 5;
			break;
		case GLUT_KEY_LEFT:
			sceneRotateY += 5;
			break;
		case GLUT_KEY_RIGHT:
			sceneRotateY -= 5;
			break;
	}
	if(sceneRotateX >= 360)		sceneRotateX -= 360;
	if(sceneRotateX < 0)		sceneRotateX += 360;
	if(sceneRotateY >= 360)		sceneRotateY -= 360;
	if(sceneRotateY < 0)		sceneRotateY += 360;
	glutPostRedisplay();
}

//Defines the functionality of ESC and WASD
void keyboard(unsigned char key, int x, int y) {
	if(key == 27) exit(0);
	if(gameState != PLAYING) return;
	switch(key){
	case 'w': //Sets the player to move in forward direction (-Z direction)
		player.directions[0] = 0;
		player.directions[1] = -1;
		break;
	case 's': //Sets the player to move in backward direction (+Z direction)
		player.directions[0] = 0;
		player.directions[1] = 1;
		break;
	case 'a': //Sets the player to move in left direction (-X direction)
		player.directions[0] = -1;
		player.directions[1] = 0;
		break;
	case 'd': //Sets the player to move in right direction (+X direction)
		player.directions[0] = 1;
		player.directions[1] = 0;
		break;
	}
	
	//If the player isn't currently filling, move him, otherwise the timer function will handle the movement
	if(!player.isFilling)
		level.move(player);
		
	glutPostRedisplay();	//Notify the display function to fire again
}

//Timer function
void timer(int x){
	if(gameState != PLAYING) return;
	checkGameState();

	//Automatically moves the player if he's filling
	if(player.isFilling)
		level.move(player);

	//Handles enemy movement
	for(int i = 0; i < enemies.size(); i++)
			level.move(enemies[i]);

	//Remove a second every 60 times this method is called
	if(x >= 60){
		x = 0;
		level.timeLeft -= 1;
	}

	glutTimerFunc((1000.0/FPS)/SLOW_RATE, timer, x + 1);
	glutPostRedisplay();	//Notify the display function to fire again
}

//Program entry point
int main(int argc, char* argv[]) {
	
	//Game Logic Initialization
	int position[2] = {0, 0};
	player = Player(position);

	level = Field(player, 1);	//Change the second parameter to select a level

	enemies = level.enemies;

	//GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	//GLUT Window Initialization
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WINDOW_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2);
	glutCreateWindow("AirXonix");
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Callback Attatchment
	glutDisplayFunc(display);
	glutReshapeFunc(lockResizing);
	glutSpecialFunc(arrows);
	glutKeyboardFunc(keyboard);
	glutTimerFunc((1000.0/FPS)/SLOW_RATE, timer, 0);

	//Perspective Projection Setup
	glClearColor(BACKGROUND_COLOR);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, WINDOW_WIDTH / WINDOW_HEIGHT, 2.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	//Depth and Multisampling Setup
	glEnable(GL_DEPTH_TEST);

	//Lighting and Shadows
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);			//Enables Lighting: Use current lighting parameters to compute color
	glEnable(GL_LIGHT0);			//Enables Light0
	glEnable(GL_NORMALIZE);			//Normalizes color vectors to a unit length before lighting
	glShadeModel(GL_SMOOTH);

	//glutFullScreen();
	glutMainLoop();
}
